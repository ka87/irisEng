#pragma once

/* Where an instance's variable values go.
 *
 * A scripting container names two blocks: the model, which carries the
 * declaration list and the default value of every variable, and the
 * instance block, which carries the values set per placed object. The
 * second is applied over the first by matching variable names, and the
 * two blocks use completely different offsets for the same variable --
 * the instance block packs only the variables it overrides, densely,
 * while the model's offsets are those the compiled behaviours read.
 *
 * So every value copied has to cross from one offset space to the
 * other, and a value that lands on the wrong variable is invisible
 * afterwards: the behaviour reads a plausible number of the right type
 * and simply acts on the wrong one. This prints each matched pair --
 * both names, both offsets, both types, and the bytes that actually
 * moved -- so a misdirected write can be seen rather than inferred.
 *
 * Set AIVARCHK_ENABLED to 0 to compile it all away. */
#define AIVARCHK_ENABLED 1

/* Every placed object with an instance block goes through this, so
 * reporting all of them buries the one being chased. An instance is
 * reported only when its model declares one of these names, and then
 * every watched variable of that instance is printed. Set
 * AIVARCHK_WATCH to { 0 } to report every instance instead. */
#define AIVARCHK_WATCH { "des_f_agit_distmax",      \
                         "des_i_auto",              \
                         "des_i_action_type_peyj",  \
                         "des_i_action_type_mili",  \
                         "SF_Init_Capa_Swich_Vrai", \
                         "o_vala_visee",            \
                         0 }

/* A hard ceiling, in case a watched name turns out to be common. */
#define AIVARCHK_MAX_INSTANCES 64

#if AIVARCHK_ENABLED

struct sAI;
struct sGameObjectMain;

/* Called with the object an instance belongs to, before its values are
   applied, so every line below can be tied to a placed object rather
   than to an index. */
void AIVARCHK_Object(struct sGameObjectMain* gao, struct sAI* ai);

/* Called once the values have been copied forward into the block the
   behaviours actually read, which is a separate buffer from the one
   the values are applied to. */
void AIVARCHK_Current(struct sAI* ai);

/* Called once per instance, after the model defaults have been copied
   and the instance block resolved, before any value is applied. */
void AIVARCHK_Begin(struct sAI* ai);

/* Called for each matched pair, after its bytes have been copied.
   modelIdx and instIdx are the indices into the two declaration lists;
   bytes is the length the caller passed to memcpy. */
void AIVARCHK_Pair(struct sAI* ai, unsigned int modelIdx,
                   unsigned int instIdx, unsigned int modelOffset,
                   unsigned int instOffset, int typeUsed,
                   unsigned int dims, unsigned int bytes);

/* Called once the loop has finished. Reports any instance variable the
   loop never matched to a model variable, which is the other way a
   value goes missing. */
void AIVARCHK_End(struct sAI* ai, int processedVarNum);

#else

#define AIVARCHK_Object(gao, ai)                                    ((void)0)
#define AIVARCHK_Current(ai)                                        ((void)0)
#define AIVARCHK_Begin(ai)                                          ((void)0)
#define AIVARCHK_Pair(ai, mi, ii, mo, io, t, d, b)                  ((void)0)
#define AIVARCHK_End(ai, n)                                         ((void)0)

#endif
