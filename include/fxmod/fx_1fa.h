#ifndef FX_1FA_H
#define FX_1FA_H

/* Effect module fx_1fa (dir 13 file 0x1fa): Carbuncle (2) (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 delay0; /* delay */
    s32 timer4; /* countdown */
    u8 *sprite8; /* effect data */
    s32 angleC; /* angle */
    s32 radius10; /* radius */
} Fx1faRing; /* 0x14 */

extern Unk800F78A4 *fx_1fa_env;
extern SVECTOR fx_1fa_emitPos0;
extern s32 fx_1fa_timer[]; /* countdown; an array (in-struct for alias analysis) */
extern SVECTOR fx_1fa_emitPos1;
extern SVECTOR fx_1fa_emitPos2[];
extern Fx1faRing fx_1fa_spiralSprites[12][4];

#endif
