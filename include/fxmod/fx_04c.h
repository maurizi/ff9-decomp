#ifndef FX_04C_H
#define FX_04C_H

/* Effect module fx_04c (dir 13 file 0x4C). */
#include "btlfx_mod.h"

/* fx_04c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLOAT_CLOUD      0x1 /* emitter: white puffy cloud particles bursting under the target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_FLOAT_CLOUD_2    0x2 /* emitter: white cloud puffs (the second cloud emitter, same cloud texture); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_FLOAT_STAR_FLASH 0x3 /* emitter: speckled ring, blue star flashes, two fans (per target) */
#define FX_FLOAT_SPECK_RING 0x4 /* emitter: ring textured with brown specks (per target) */
#define FX_FLOAT_CLOUD_TEXTURE 0x5 /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x200,0x180) 128x128, a white cloud sampled by the cloud burst; runtime (docs/runtime-findings.md effect-data pass) */

/* fx_04c choreography (docs/fxdata.md, "Choreography literals"): op80's length; op81 / op82's per-target states. */
#define FX_FLOAT_FRAMES        0x3C /* op80: the task ends at this frame (FX_FLOAT_CLOUD_TEXTURE is rendered until then) */
#define FX_FLOAT_TARGET_SLOTS  4    /* Fx04cWork81 target slots */
#define FX_FLOAT_STATE_START   0    /* op81 / op82: start the emitter (STAR_FLASH / SPECK_RING) at the current target */
#define FX_FLOAT_STATE_NEXT    1    /* a frame later, go to the next target (or END) */
#define FX_FLOAT_STATE_END     2    /* wait END_FRAMES, the op ends (op82 sets signal 2 first) */
#define FX_FLOAT_END_FRAMES    0xA

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4;
} Fx04cWork80;

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4[FX_FLOAT_TARGET_SLOTS]; /* target positions */
    Pool6CEntry *emit24[FX_FLOAT_TARGET_SLOTS];
    s32 count34; /* target count */
    s32 target38; /* current target */
} Fx04cWork81; /* 0x3C */

extern Unk800F78A4 *fx_04c_env;

#endif
