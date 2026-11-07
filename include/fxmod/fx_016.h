#ifndef FX_016_H
#define FX_016_H

/* Effect module fx_016 (dir 13 file 0x16). */
#include "btlfx_mod.h"

/* fx_016 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATER_SPLASH_DROPS  0x1 /* emitter: water droplet clusters, blue glow, small bubbles */
#define FX_WATER_GLOBE_1       0x2 /* model: flared water bowl, second morph stage */
#define FX_WATER_GLOBE_2       0x3 /* model: water vase shape, third morph stage */
#define FX_WATER_GLOBE_3       0x4 /* model: bulging water bowl, fourth morph stage */
#define FX_WATER_GLOBE_0       0x5 /* model: flat water dish, first morph stage of the growing water globe */
#define FX_WATER_SPLASH_FUNNEL 0x7 /* model: flared funnel, misty water texture */
#define FX_WATER_SPLASH_COLUMN 0x8 /* model: open cylinder, misty water texture; morphs to the funnel as the globe bursts */
#define FX_WATER_BASE_DISC     0x9 /* model: flat disc (water texture) drawn under the globe */
#define FX_WATER_BLUE_RING     0xA /* emitter: white-blue gouraud ring */
#define FX_WATER_GLOBE         0xE /* model: full water sphere, last morph stage, then fades */

/* fx_016 choreography (docs/fxdata.md, "Choreography literals"): op80's two phases and keyframes, the models' textures and the
 * globe's scrolling water; op81's lift. */
#define FX_WATER_GROW_FRAMES       24 /* op80: FX_WATER_GLOBE_0 morphs through GLOBE_1..3 into FX_WATER_GLOBE over this many frames, then */
#define FX_WATER_BURST_FRAMES      26 /* the globe fades out while the splash column morphs into the funnel, over this many; the task ends */
#define FX_WATER_BLUE_RING_FRAME   1  /* op80: FX_WATER_BLUE_RING starts */
#define FX_WATER_SPLASH_DROPS_FRAME 16 /* op80: FX_WATER_SPLASH_DROPS start */
#define FX_WATER_MODEL_COUNT       8  /* btlfx_initEffModels slots: GLOBE_0..3, GLOBE, SPLASH_COLUMN, SPLASH_FUNNEL, BASE_DISC */
/* The globe models and the base disc: 4-bit page (0x240, 0x100), abr 1, clut (0, 0xF4), v 0x80 = VRAM (0x240, 0x180);
 * each op80 frame btlfx_drawScrollMove copies one of the GLOBE_ANIM_FRAMES source rects of fx_016_scrollDst
 * (cycled from entry 1) into that rect (VRAM_X/Y/W/H). */
#define FX_WATER_GLOBE_TPAGE       0x39
#define FX_WATER_GLOBE_CLUT        0x3D00
#define FX_WATER_GLOBE_V           0x80
#define FX_WATER_GLOBE_VRAM_X      0x240
#define FX_WATER_GLOBE_VRAM_Y      0x180
#define FX_WATER_GLOBE_VRAM_W      0x10
#define FX_WATER_GLOBE_VRAM_H      0x40
#define FX_WATER_GLOBE_ANIM_FRAMES 23
/* The splash column / funnel: 4-bit page (0x2C0, 0x100), abr 1, clut (0x10, 0xF4), v 0x80. */
#define FX_WATER_SPLASH_TPAGE      0x3B
#define FX_WATER_SPLASH_CLUT       0x3D01
#define FX_WATER_SPLASH_V          0x80
#define FX_WATER_LIFT_FRAMES       0x20 /* op81: the targets bob up (jump44 * sin) over this many frames, then */
#define FX_WATER_DROP_FRAMES       4    /* settle back over this many (t = frame << 10); then they are put home and the op ends */

typedef struct {
    SVECTOR home0[8]; /* target home positions */
    s32 count40; /* target count */
    s32 jump44; /* jump height */
    s32 height48[8]; /* target heights */
} Fx016Work81; /* 0x68 */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body */
    SVECTOR posC; /* target */
    s32 model14; /* models */
    s32 model18;
    s32 model1C;
    s32 model20;
    s32 model24;
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 scale34; /* scale */
} Fx016Work80; /* 0x38 */

typedef struct {
    u16 x0;
    u16 y2;
} Fx016Pair;

extern BtlModelBuf fx_016_effModels[];
extern Fx016Pair fx_016_scrollDst[];
extern Unk800F78A4 *fx_016_env;
extern s32 fx_016_radiusScale; /* jump scale */

#endif
