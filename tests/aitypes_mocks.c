/* What ai/ai_types.c needs and this check does not drive.
 *
 * Only the object copier reaches outside the file at all, for the two
 * main actors and the handle layer. The layer is the real one. */

#include <stdlib.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../ai.h"

/* gameobject.h keeps the array as a tentative definition and the
   pointer as an extern, so only the pointer needs a home here. */
static struct sGameObjectMain* aitypes_actors[4];
struct sGameObjectMain** AI_MainActor = &aitypes_actors[1];

void AITYPES_MockActors(void)
{
    static int first, second;

    AI_MainActor[0] = (struct sGameObjectMain*)&first;
    AI_MainActor[1] = (struct sGameObjectMain*)&second;
}
