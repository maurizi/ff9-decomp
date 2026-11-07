#ifndef FX_095_H
#define FX_095_H

/* Effect module fx_095 (dir 13 file 0x95): Stop. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_095 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STOP_CLOCK_RING 0x20 /* sprite: silver clock ring with spikes */
#define FX_STOP_FLARE      0x2C /* sprite: blue-red flare, 6 frames */

/* fx_095 choreography (docs/fxdata.md, "Choreography literals"): op80's clock marks (pool particles) and their
 * states; op81's grey screen disc. */
#define FX_STOP_MARKS          12 /* op80's pool: the clock-face marks round the unit, one per pool entry */
#define FX_STOP_MARKS_PER_FRAME 2 /* FX_STOP_STATE_SPAWN spawns this many marks per frame */
#define FX_STOP_MARK_RADIUS    62 /* the marks' circle radius on screen (scaled up while they arrive) */
#define FX_STOP_STATE_SPAWN  0 /* op80: the marks are spawned */
#define FX_STOP_STATE_HOLD   1 /* op80: wait, then release the marks (ctl->releaseC) */
#define FX_STOP_STATE_FREEZE 2
#define FX_STOP_STATE_FADE   3
#define FX_STOP_MARK_STATE_ARRIVE 0 /* a mark spins in from a larger circle */
#define FX_STOP_MARK_STATE_HOLD   1 /* it glows (FX_STOP_FLARE) briefly, until released */
#define FX_STOP_MARK_STATE_FREEZE 2 /* drawn opaque from fx_095_markBlocks2 */
#define FX_STOP_MARK_STATE_FADE   3 /* fades out, then the particle ends */
#define FX_STOP_MARK_ARRIVE_FRAMES 8  /* FX_STOP_MARK_STATE_ARRIVE's length (the ramp is frame << 9) */
#define FX_STOP_MARK_GLOW_FRAMES   5  /* FX_STOP_MARK_STATE_HOLD: the glow and flare show this many frames */
#define FX_STOP_HOLD_FRAMES        16 /* FX_STOP_STATE_HOLD's length */
#define FX_STOP_FREEZE_FRAMES      16 /* FX_STOP_STATE_FREEZE's and FX_STOP_MARK_STATE_FREEZE's length */
#define FX_STOP_FADE_FRAMES        8  /* FX_STOP_STATE_FADE's and FX_STOP_MARK_STATE_FADE's length (the fade ramp is frame << 9) */
#define FX_STOP_OP81_STATE_GREY 0 /* op81: the screen round the unit is copied to VRAM_BTLFX_SCRATCH and greyed, band by band */
#define FX_STOP_OP81_STATE_DISC 1 /* the grey copy is drawn as a disc with the clock ring */
#define FX_STOP_OP81_STATE_FADE 2 /* both fade out, then the op ends */
#define FX_STOP_GREY_BANDS   3  /* fx_095_greyScreenCopy stores / greys / loads the rect in this many bands (pixels8 holds one) */
#define FX_STOP_GREY_FRAMES  (2 * FX_STOP_GREY_BANDS) /* FX_STOP_OP81_STATE_GREY's length (a store and a load per band, after the move) */
#define FX_STOP_DISC_FRAMES  16 /* FX_STOP_OP81_STATE_DISC's length */
#define FX_STOP_DISC_FADE_FRAMES 8 /* FX_STOP_OP81_STATE_FADE's length (the fade ramp is frame << 9) */
#define FX_STOP_DISC_VRAM_W  0xC0 /* the scratch rect at VRAM_BTLFX_SCRATCH_X/Y holding the grey copy */
#define FX_STOP_DISC_VRAM_H  0xC0
#define FX_STOP_DISC_RADIUS  0x60 /* the disc's outer radius on screen */
#define FX_STOP_DISC_RING_WIDTH 0x18 /* its textured ring's width (the inner radius is the outer one less this) */
#define FX_STOP_DISC_SEGMENTS 16 /* fx_095_drawTexDisc's segments (each a POLY_GT4 + POLY_FT3, and a darkening POLY_G4 + POLY_F3) */
#define FX_STOP_DARKEN_RGB 0x9B, 0xFF, 0    /* subtractive screen tint as the grey copy is done */
#define FX_STOP_FLASH_RGB  0x80, 0x80, 0xFF /* additive screen tint as the disc appears */

typedef struct {
    s32 state0; /* state */
    s32 spawnCount4; /* spawned */
    DVECTOR screenXY8; /* caster screen position */
    s32 releaseC; /* release */
} Fx095Work80; /* 0x10 */

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only (cleared at init) */
    u16 pixels8[FX_STOP_DISC_VRAM_W * FX_STOP_DISC_VRAM_H / FX_STOP_GREY_BANDS]; /* captured screen strip */
} Fx095Work81; /* 0x6008 */

typedef struct {
    s32 angle0; /* angle */
    s32 state4; /* state */
    s32 index8; /* index */
} Fx095Part; /* 0xC */

typedef struct {
    u8 pad0[8];
    s16 screenX8; /* screen x */
    s16 screenYA; /* screen y */
    s32 releaseC; /* release */
} Fx095Ctl;

extern Unk800F78A4 *fx_095_env;
extern SVECTOR fx_095_bodyPos;

static s32 fx_095_op80PoolTask(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool, Fx095Ctl *ctl);
void fx_095_greyScreenCopy(s32 step, s32 x, s32 y, u16 *buf, RECT *area);
void fx_095_drawTexDisc(s32 ot, s32 x, s32 y, s32 outerR, s32 bright, RECT *rect);

extern s32 fx_095_markBlocks[]; /* file ids */
extern s32 fx_095_glowBlocks[]; /* file ids */
extern s32 fx_095_markBlocks2[]; /* file ids */

#endif
