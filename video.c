#include "video.h"

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

#include <stdlib.h>
#include <string.h>

#define VID_IOBUFFER   32768
/* Deliberately not a round multiple of a decoder's frame size: a
   queue whose length divides evenly into the chunks written to it
   steps over its own end exactly and the wrap is taken only under
   rare alignments, which is how a wrap bug survives being used. */
#define VID_PCMBUFFER  (257 * 1024)

struct sVideoStream
{
    AVFormatContext* format;
    AVIOContext*     io;
    unsigned char*   ioBuffer;

    AVCodecContext*  videoDec;
    int              videoIdx;
    AVFrame*         frame;          /* the frame in hand */
    AVFrame*         scratch;        /* what the decoder writes into */
    AVPacket*        packet;
    struct SwsContext* scaler;
    int              scalerFormat;   /* the pixel format the scaler was built for */
    int              scalerAlpha;

    AVCodecContext*  soundDec;
    int              soundIdx;
    SwrContext*      resampler;
    int              soundRate;
    int              soundChannels;

    /* decoded PCM waiting for the sink, as a ring */
    uint8_t*         pcm;
    int              pcmHead;
    int              pcmTail;

    int              width;
    int              height;
    int              frameCount;
    int              frameNum;
    int              haveFrame;
    int              ended;

    /* the clock: sound frames the sink reports, or wall time */
    int64_t          soundPlayed;
    int              soundIsClock;
    int64_t          wallStart;
    double           frameSeconds;
    double           frameTime;      /* when the frame in hand is due */

    tVideoReadFunc   read;
    void*            readUser;
};

/* ------------------------------------------------------------- the clock */

static int64_t VID_l_Now(void)
{
    return (int64_t)av_gettime_relative();
}

/* Where the stream is, in seconds: what the sink has played when
   there is one, otherwise how long since the first frame went out. */
static double VID_d_Clock(const tVideoStream* video)
{
    if (video->soundIsClock && video->soundRate > 0)
        return (double)video->soundPlayed / (double)video->soundRate;
    if (!video->wallStart)
        return 0.0;
    return (double)(VID_l_Now() - video->wallStart) / 1000000.0;
}

/* ------------------------------------------------------------------- I/O */

static int VID_i_ReadPacket(void* opaque, uint8_t* buf, int size)
{
    tVideoStream* video = (tVideoStream*)opaque;
    int got;

    got = video->read(video->readUser, buf, size);
    if (got <= 0)
        return got == 0 ? AVERROR_EOF : AVERROR(EIO);
    return got;
}

/* ------------------------------------------------------------- the ring */

static int VID_i_PcmUsed(const tVideoStream* video)
{
    int used = video->pcmHead - video->pcmTail;
    if (used < 0)
        used += VID_PCMBUFFER;
    return used;
}

static void VID_PcmWrite(tVideoStream* video, const uint8_t* src, int bytes)
{
    int room;
    int chunk;

    /* Never let the write run over the read: the sink is behind, and
       dropping the newest is better than corrupting what is queued. */
    room = VID_PCMBUFFER - 1 - VID_i_PcmUsed(video);
    if (bytes > room)
        bytes = room;
    while (bytes > 0)
    {
        chunk = VID_PCMBUFFER - video->pcmHead;
        if (chunk > bytes)
            chunk = bytes;
        memcpy(video->pcm + video->pcmHead, src, (size_t)chunk);
        video->pcmHead = (video->pcmHead + chunk) % VID_PCMBUFFER;
        src += chunk;
        bytes -= chunk;
    }
}

int VID_i_SoundRead(tVideoStream* video, void* dest, int bytes)
{
    uint8_t* out = (uint8_t*)dest;
    int used;
    int done = 0;
    int chunk;

    if (!video || !video->pcm)
        return 0;
    used = VID_i_PcmUsed(video);
    if (bytes > used)
        bytes = used;
    /* Whole sample frames only. A sink handed half of one would put
       the channels the wrong way round for the rest of the stream,
       and a caller asking in round numbers of bytes rather than of
       frames is the normal case. */
    bytes -= bytes % (video->soundChannels * 2);
    while (bytes > 0)
    {
        chunk = VID_PCMBUFFER - video->pcmTail;
        if (chunk > bytes)
            chunk = bytes;
        memcpy(out, video->pcm + video->pcmTail, (size_t)chunk);
        video->pcmTail = (video->pcmTail + chunk) % VID_PCMBUFFER;
        out += chunk;
        bytes -= chunk;
        done += chunk;
    }
    return done;
}

void VID_SoundPlayed(tVideoStream* video, int64_t frames)
{
    if (!video)
        return;
    video->soundPlayed = frames;
    video->soundIsClock = 1;
}

/* --------------------------------------------------------------- opening */

static int VID_i_OpenCodecs(tVideoStream* video, int wantSound)
{
    const AVCodec* codec;
    AVStream* stream;

    video->videoIdx = av_find_best_stream(video->format, AVMEDIA_TYPE_VIDEO,
                                          -1, -1, &codec, 0);
    if (video->videoIdx < 0 || !codec)
        return 0;
    stream = video->format->streams[video->videoIdx];
    video->videoDec = avcodec_alloc_context3(codec);
    if (!video->videoDec)
        return 0;
    if (avcodec_parameters_to_context(video->videoDec, stream->codecpar) < 0)
        return 0;
    if (avcodec_open2(video->videoDec, codec, NULL) < 0)
        return 0;

    video->width = video->videoDec->width;
    video->height = video->videoDec->height;
    if (video->width <= 0 || video->height <= 0)
        return 0;
    video->frameCount = stream->nb_frames > 0 ? (int)stream->nb_frames : 0;

    /* How long a frame lasts. The stream's average rate is what Bink
       reported as FrameRate/FrameRateDiv; a file that gives neither
       is played at 30, which is what the engine assumed anyway. */
    if (stream->avg_frame_rate.num > 0 && stream->avg_frame_rate.den > 0)
        video->frameSeconds = (double)stream->avg_frame_rate.den
                            / (double)stream->avg_frame_rate.num;
    else
        video->frameSeconds = 1.0 / 30.0;

    if (!wantSound)
        return 1;

    video->soundIdx = av_find_best_stream(video->format, AVMEDIA_TYPE_AUDIO,
                                          -1, -1, &codec, 0);
    if (video->soundIdx < 0 || !codec)
    {
        video->soundIdx = -1;
        return 1;      /* a silent file is not a failure */
    }
    stream = video->format->streams[video->soundIdx];
    video->soundDec = avcodec_alloc_context3(codec);
    if (!video->soundDec
        || avcodec_parameters_to_context(video->soundDec, stream->codecpar) < 0
        || avcodec_open2(video->soundDec, codec, NULL) < 0)
    {
        /* the picture is still worth having */
        if (video->soundDec)
            avcodec_free_context(&video->soundDec);
        video->soundIdx = -1;
        return 1;
    }

    video->soundRate = video->soundDec->sample_rate;
    video->soundChannels = video->soundDec->ch_layout.nb_channels;
    if (video->soundChannels > 2)
        video->soundChannels = 2;      /* the sink is stereo at most */
    if (video->soundRate <= 0 || video->soundChannels <= 0)
    {
        avcodec_free_context(&video->soundDec);
        video->soundIdx = -1;
        return 1;
    }

    {
        AVChannelLayout out;
        av_channel_layout_default(&out, video->soundChannels);
        if (swr_alloc_set_opts2(&video->resampler, &out, AV_SAMPLE_FMT_S16,
                                video->soundRate, &video->soundDec->ch_layout,
                                video->soundDec->sample_fmt,
                                video->soundDec->sample_rate, 0, NULL) < 0
            || swr_init(video->resampler) < 0)
        {
            av_channel_layout_uninit(&out);
            swr_free(&video->resampler);
            avcodec_free_context(&video->soundDec);
            video->soundIdx = -1;
            return 1;
        }
        av_channel_layout_uninit(&out);
    }

    video->pcm = (uint8_t*)av_malloc(VID_PCMBUFFER);
    if (!video->pcm)
    {
        swr_free(&video->resampler);
        avcodec_free_context(&video->soundDec);
        video->soundIdx = -1;
    }
    return 1;
}

static tVideoStream* VID_pst_Finish(tVideoStream* video, int wantSound)
{
    if (avformat_find_stream_info(video->format, NULL) < 0)
    {
        VID_Close(video);
        return NULL;
    }
    video->frame = av_frame_alloc();
    video->scratch = av_frame_alloc();
    video->packet = av_packet_alloc();
    if (!video->frame || !video->scratch || !video->packet
        || !VID_i_OpenCodecs(video, wantSound))
    {
        VID_Close(video);
        return NULL;
    }
    return video;
}

static tVideoStream* VID_pst_Alloc(void)
{
    tVideoStream* video = (tVideoStream*)av_mallocz(sizeof(tVideoStream));
    if (!video)
        return NULL;
    video->videoIdx = -1;
    video->soundIdx = -1;
    video->scalerFormat = AV_PIX_FMT_NONE;
    return video;
}

tVideoStream* VID_pst_OpenFile(const char* path, int wantSound)
{
    tVideoStream* video;

    if (!path || !path[0])
        return NULL;
    video = VID_pst_Alloc();
    if (!video)
        return NULL;
    if (avformat_open_input(&video->format, path, NULL, NULL) < 0)
    {
        av_free(video);
        return NULL;
    }
    return VID_pst_Finish(video, wantSound);
}

tVideoStream* VID_pst_OpenReader(tVideoReadFunc read, void* user, int wantSound)
{
    tVideoStream* video;

    if (!read)
        return NULL;
    video = VID_pst_Alloc();
    if (!video)
        return NULL;
    video->read = read;
    video->readUser = user;

    video->ioBuffer = (unsigned char*)av_malloc(VID_IOBUFFER);
    if (!video->ioBuffer)
    {
        av_free(video);
        return NULL;
    }
    /* Not seekable: the caller hands over bytes in order, which is
       all the .bf reader can do. */
    video->io = avio_alloc_context(video->ioBuffer, VID_IOBUFFER, 0, video,
                                   VID_i_ReadPacket, NULL, NULL);
    if (!video->io)
    {
        av_free(video->ioBuffer);
        av_free(video);
        return NULL;
    }
    video->format = avformat_alloc_context();
    if (!video->format)
    {
        avio_context_free(&video->io);
        av_free(video->ioBuffer);
        av_free(video);
        return NULL;
    }
    video->format->pb = video->io;
    video->format->flags |= AVFMT_FLAG_CUSTOM_IO;
    if (avformat_open_input(&video->format, NULL, NULL, NULL) < 0)
    {
        /* open_input frees the context it was given on failure, but
           not the AVIOContext behind it */
        video->format = NULL;
        VID_Close(video);
        return NULL;
    }
    return VID_pst_Finish(video, wantSound);
}

void VID_Close(tVideoStream* video)
{
    if (!video)
        return;
    if (video->scaler)
        sws_freeContext(video->scaler);
    if (video->resampler)
        swr_free(&video->resampler);
    if (video->videoDec)
        avcodec_free_context(&video->videoDec);
    if (video->soundDec)
        avcodec_free_context(&video->soundDec);
    if (video->frame)
        av_frame_free(&video->frame);
    if (video->scratch)
        av_frame_free(&video->scratch);
    if (video->packet)
        av_packet_free(&video->packet);
    if (video->format)
        avformat_close_input(&video->format);
    if (video->io)
    {
        /* the context owns whatever buffer it is holding now, which
           is not necessarily the one it was given */
        av_free(video->io->buffer);
        video->ioBuffer = NULL;
        avio_context_free(&video->io);
    }
    if (video->ioBuffer)
        av_free(video->ioBuffer);
    if (video->pcm)
        av_free(video->pcm);
    av_free(video);
}

/* ------------------------------------------------------------- accessors */

int VID_i_Width(const tVideoStream* video) { return video ? video->width : 0; }
int VID_i_Height(const tVideoStream* video) { return video ? video->height : 0; }
int VID_i_FrameNum(const tVideoStream* video) { return video ? video->frameNum : 0; }
int VID_i_FrameCount(const tVideoStream* video) { return video ? video->frameCount : 0; }
int VID_b_Ended(const tVideoStream* video) { return video ? video->ended : 1; }

int VID_b_HasSound(const tVideoStream* video)
{
    return video && video->soundIdx >= 0 && video->pcm != NULL;
}

int VID_i_SoundRate(const tVideoStream* video)
{
    return video ? video->soundRate : 0;
}

int VID_i_SoundChannels(const tVideoStream* video)
{
    return video ? video->soundChannels : 0;
}

/* -------------------------------------------------------------- decoding */

static void VID_DrainSound(tVideoStream* video, AVFrame* frame)
{
    uint8_t* out = NULL;
    int outSamples;
    int got;
    int bytes;

    outSamples = (int)av_rescale_rnd(
        swr_get_delay(video->resampler, video->soundRate) + frame->nb_samples,
        video->soundRate, video->soundDec->sample_rate, AV_ROUND_UP);
    if (outSamples <= 0)
        return;
    if (av_samples_alloc(&out, NULL, video->soundChannels, outSamples,
                         AV_SAMPLE_FMT_S16, 0) < 0)
    {
        return;
    }
    got = swr_convert(video->resampler, &out, outSamples,
                      (const uint8_t**)frame->extended_data, frame->nb_samples);
    if (got > 0)
    {
        bytes = got * video->soundChannels * 2;
        VID_PcmWrite(video, out, bytes);
    }
    av_freep(&out);
}

/* Reads packets until the video decoder hands over a frame. */
int VID_b_DecodeFrame(tVideoStream* video)
{
    int ret;

    if (!video || video->ended)
        return 0;
    if (video->haveFrame)
        return 1;

    for (;;)
    {
        ret = avcodec_receive_frame(video->videoDec, video->scratch);
        if (ret == 0)
        {
            av_frame_unref(video->frame);
            av_frame_move_ref(video->frame, video->scratch);
            video->haveFrame = 1;
            if (!video->wallStart)
                video->wallStart = VID_l_Now();
            return 1;
        }
        if (ret != AVERROR(EAGAIN))
        {
            video->ended = 1;
            return 0;
        }

        ret = av_read_frame(video->format, video->packet);
        if (ret < 0)
        {
            /* no more input: flush whatever the decoder still holds */
            avcodec_send_packet(video->videoDec, NULL);
            ret = avcodec_receive_frame(video->videoDec, video->scratch);
            if (ret == 0)
            {
                av_frame_unref(video->frame);
                av_frame_move_ref(video->frame, video->scratch);
                video->haveFrame = 1;
                return 1;
            }
            video->ended = 1;
            return 0;
        }

        if (video->packet->stream_index == video->videoIdx)
        {
            avcodec_send_packet(video->videoDec, video->packet);
        }
        else if (video->packet->stream_index == video->soundIdx && video->pcm)
        {
            if (avcodec_send_packet(video->soundDec, video->packet) == 0)
            {
                while (avcodec_receive_frame(video->soundDec, video->scratch) == 0)
                {
                    VID_DrainSound(video, video->scratch);
                    av_frame_unref(video->scratch);
                }
            }
        }
        av_packet_unref(video->packet);
    }
}

void VID_NextFrame(tVideoStream* video)
{
    if (!video || !video->haveFrame)
        return;
    av_frame_unref(video->frame);
    video->haveFrame = 0;
    video->frameNum++;
    video->frameTime += video->frameSeconds;
}

int VID_b_Waiting(tVideoStream* video)
{
    if (!video || video->ended)
        return 0;
    if (!video->frameNum && !video->wallStart)
        return 0;        /* the first frame is always due */
    return VID_d_Clock(video) < video->frameTime;
}

/* --------------------------------------------------------------- scaling */

void VID_CopyToBuffer(tVideoStream* video, void* dest, int pitch,
                      int destHeight, int withAlpha)
{
    uint8_t* planes[4];
    int strides[4];
    int rows;
    int row;
    int col;

    if (!video || !video->haveFrame || !dest || pitch <= 0)
        return;

    /* Always BGRA, which is what the engine's 32-bit surfaces are.
       BGR0 would look like the cheaper choice for the no-alpha case,
       but libav documents its fourth byte as undefined, so the
       opaque byte is written here instead where it can be relied
       on. A file with no alpha plane of its own already comes out
       opaque; one that has a plane has it overwritten below. */
    if (!video->scaler || video->scalerFormat != video->frame->format)
    {
        if (video->scaler)
            sws_freeContext(video->scaler);
        video->scaler = sws_getContext(video->width, video->height,
                                       (enum AVPixelFormat)video->frame->format,
                                       video->width, video->height,
                                       AV_PIX_FMT_BGRA,
                                       SWS_BILINEAR, NULL, NULL, NULL);
        video->scalerFormat = video->frame->format;
        if (!video->scaler)
            return;
    }
    video->scalerAlpha = withAlpha;

    rows = video->height;
    if (destHeight > 0 && destHeight < rows)
        rows = destHeight;

    planes[0] = (uint8_t*)dest;
    planes[1] = NULL;
    planes[2] = NULL;
    planes[3] = NULL;
    strides[0] = pitch;
    strides[1] = 0;
    strides[2] = 0;
    strides[3] = 0;

    sws_scale(video->scaler, (const uint8_t* const*)video->frame->data,
              video->frame->linesize, 0, rows, planes, strides);

    if (!withAlpha)
    {
        /* every pixel opaque, a word at a time */
        for (row = 0; row < rows; row++)
        {
            uint32_t* line = (uint32_t*)((uint8_t*)dest + (size_t)row * pitch);
            for (col = 0; col < video->width; col++)
                line[col] |= 0xFF000000u;
        }
    }
}
