#ifndef FX_07E_H
#define FX_07E_H

/* Effect module fx_07e (dir 13 file 0x7E): Slow. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLOW_CLOCK_FACE 0x1 /* particles: pink clock dial with Roman numerals I-XII on the target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_SLOW_GREEN_RING 0x2 /* rings: white-green gouraud rings */
#define FX_SLOW_BLUE_WAVE  0x3 /* particles: blue chevron wave frames */
#define FX_SLOW_BLUE_GLOWS 0x5 /* particles: soft blue round glows growing and fading around the clock; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_SLOW_BACKDROP   0xA /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SLOW_CRESCENT   0xC /* model: one quad textured with a blue-white crescent */

/* fx_07e choreography (docs/fxdata.md, "Choreography literals"): op80's crescent ring (pool particles) and its
 * states; op81's clock face and twisted screen. */
#define FX_SLOW_CRESCENTS        11    /* op80's pool: FX_SLOW_CRESCENT copies round the target, one per pool entry */
#define FX_SLOW_STATE_SPAWN      0     /* op80: one crescent spawned per frame */
#define FX_SLOW_STATE_SHRINK     1     /* the ring closes in */
#define FX_SLOW_STATE_FADE       2     /* the crescents fade, then the op ends */
#define FX_SLOW_SHRINK_FRAMES    22    /* FX_SLOW_STATE_SHRINK's length (the (frame << 12) / N ramp) */
#define FX_SLOW_FADE_FRAMES      17    /* FX_SLOW_STATE_FADE's length */
#define FX_SLOW_RING_RADIUS      0x2EE /* the ring's start radius (and the radius at scale 1) */
#define FX_SLOW_RING_RADIUS_END  0x20  /* and end radius */
#define FX_SLOW_CRESCENT_STATE_APPEAR 0 /* a crescent brightens in */
#define FX_SLOW_CRESCENT_STATE_HOLD   1 /* until the ring has closed (shrinkDone24) */
#define FX_SLOW_CRESCENT_STATE_FADE   2 /* then fades out and the particle ends */
#define FX_SLOW_CRESCENT_APPEAR_FRAMES 8  /* FX_SLOW_CRESCENT_STATE_APPEAR's length (the ramp is frame << 9) */
#define FX_SLOW_CRESCENT_FADE_FRAMES  16  /* FX_SLOW_CRESCENT_STATE_FADE's length (the ramp is frame << 8) */
#define FX_SLOW_MODEL_COUNT      1     /* btlfx_initEffModels slots: FX_SLOW_CRESCENT */
#define FX_SLOW_CRESCENT_TPAGE   0xBB   /* 8-bit page (0x2C0, 0x100), abr 1 */
#define FX_SLOW_CRESCENT_CLUT    0x3E40 /* CLUT (0, 0xF9) */
#define FX_SLOW_CRESCENT_V       0x80
#define FX_SLOW_OP81_STATE_SHOW  0     /* op81: the clock face and ring fade in */
#define FX_SLOW_OP81_STATE_TWIST 1     /* the screen round the target twists (fxm_drawRadialScreenGrid) */
#define FX_SLOW_OP81_STATE_FADE  2     /* everything untwists and fades, then the op ends */
#define FX_SLOW_SHOW_FRAMES      12    /* FX_SLOW_OP81_STATE_SHOW's length (the (frame << 12) / N ramp) */
#define FX_SLOW_TWIST_FRAMES     16    /* FX_SLOW_OP81_STATE_TWIST's length (the ramp is frame << 8) */
#define FX_SLOW_UNTWIST_FRAMES   12    /* FX_SLOW_OP81_STATE_FADE's length (the (frame << 12) / N ramp) */
#define FX_SLOW_GRAB_SIZE        0x7A  /* the screen square copied to VRAM_BTLFX_SCRATCH (+ 0x80 down) and drawn twisted */
#define FX_SLOW_GRID_SEGMENTS    0x10  /* the twisted grid's segments round */
#define FX_SLOW_GRID_RINGS       6     /* its rings */
#define FX_SLOW_GRID_RING_STEP   0xA   /* the radius step between rings */

typedef struct {
    s32 state0; /* state */
    s32 spawnCount4; /* spawned */
    s32 frame8; /* frame */
    SVECTOR posC; /* caster body position */
    SVECTOR ringRot14; /* ring rotation */
    s32 radius1C; /* radius */
    s32 scale20; /* scale */
    s32 shrinkDone24; /* shrink done */
    s32 model28; /* model */
} Fx07eWork80; /* 0x2C */

typedef struct {
    s32 angle0; /* angle offset */
    s32 state4; /* state */
    s32 index8; /* index */
} Fx07ePart; /* 0xC */

extern Unk800F78A4 *fx_07e_env;
extern BtlModelBuf fx_07e_effModels[];
extern s32 fx_07e_twist;

static s32 fx_07e_op80PoolTask(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx07eWork80 *work);
static void fx_07e_op81GridVertex(s32 radius, s32 unused1, s32 ang, s32 unused3, s16 *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 otZ8; /* blend */
    SVECTOR posC; /* caster body position */
} Fx07eWork81; /* 0x14 */

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
