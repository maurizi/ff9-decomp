#ifndef FX_194_H
#define FX_194_H

/* Effect module fx_194 (dir 13 file 0x194): enemy Shockwave (Nova Dragon, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* caster position */
    SVECTOR camPos14;
} Fx194Work;

extern SVECTOR fx_194_shakeOffsets[6];
extern s16 fx_194_flashLevels[6];
extern Unk800F78A4 *fx_194_env;
extern s32 fx_194_waveAmp; /* wave amplitude */
extern s32 fx_194_wavePhase; /* wave phase */

static void fx_194_op80GridVertex(SVECTOR *v);
void fx_194_shakeCamPoint(s32 frame, s32 amp, SVECTOR *v);

#endif
