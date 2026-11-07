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

/* fx_194 choreography (docs/fxdata.md, "Choreography literals"): op80's blast (camera pull and shake with white
 * flashes), then the screen-grid ripple; op81's glow; the states and lengths. */
#define FX_SHOCKWAVE_STATE_BLAST 0    /* op80 work->state0: flashes and camera shake */
#define FX_SHOCKWAVE_STATE_WAVE  1    /* the screen grid ripples and fades */
#define FX_SHOCKWAVE_PULL_FRAME  2    /* fx_194_shakeCamPoint: the camera point pulls back once */
#define FX_SHOCKWAVE_FLASH_FRAME 0x13 /* the flashes (fx_194_flashLevels, percent of white) and the shake start */
#define FX_SHOCKWAVE_FLASH_FRAMES 6   /* one level / shake offset per frame */
#define FX_SHOCKWAVE_SHAKE       0x300 /* fx_194_shakeCamPoint amplitude (offsets * amp / 0x80) */
#define FX_SHOCKWAVE_WHITE_FADE_FRAMES 4 /* STATE_WAVE: the additive white fades out over this many frames */
#define FX_SHOCKWAVE_WAVE_FRAMES 0x20 /* STATE_WAVE's length (the op ends) */
#define FX_SHOCKWAVE_GLOW_FRAMES 0x20 /* op81 (WHITE_GLOW at the caster): its length */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* caster position */
    SVECTOR camPos14;
} Fx194Work; /* 0x1C */

extern SVECTOR fx_194_shakeOffsets[FX_SHOCKWAVE_FLASH_FRAMES];
extern s16 fx_194_flashLevels[FX_SHOCKWAVE_FLASH_FRAMES];
extern Unk800F78A4 *fx_194_env;
extern s32 fx_194_waveAmp; /* wave amplitude */
extern s32 fx_194_wavePhase; /* wave phase */

static void fx_194_op80GridVertex(SVECTOR *v);
void fx_194_shakeCamPoint(s32 frame, s32 amp, SVECTOR *point);

#endif
