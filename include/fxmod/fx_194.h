#ifndef FX_194_H
#define FX_194_H

/* Effect module fx_194 (dir 13 file 0x194): enemy Shockwave (Nova Dragon, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_194 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHOCKWAVE_DUST_WAVE         0x1 /* emitter: rings textured with grey dust streaks + white glow flash; sub: white rock-debris ring */
#define FX_SHOCKWAVE_VIOLET_STREAKS    0x2 /* emitter: violet vertical streak sprite (three more sprites render black) */
#define FX_SHOCKWAVE_GREY_STREAK_RINGS 0x3 /* emitter: rings textured with grey vertical streaks */
#define FX_SHOCKWAVE_WHITE_GLOW        0x6 /* emitter: white glow particles fading in, 21 frames */

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
void fx_194_shakeCamPoint(s32 frame, s32 amp, SVECTOR *point);

#endif
