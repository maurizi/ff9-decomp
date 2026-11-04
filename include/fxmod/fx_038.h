#ifndef FX_038_H
#define FX_038_H

/* Effect module fx_038 (dir 13 file 0x38). */
#include "btlfx_mod.h"

/* fx_038 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COMET_BLUE_GLOW    0x1 /* particles: blue glow with a white core, drawn on each falling rock */
#define FX_COMET_ROCK         0x2 /* model: rock (grey stone texture) */
#define FX_COMET_ROCK_DEBRIS  0x3 /* emitter: orange / white striped debris sprites */
#define FX_COMET_TAIL         0x4 /* trail: violet-white strip behind each rock */
#define FX_COMET_FIRE_GLOW    0x5 /* particles: orange-red fiery glow */
#define FX_COMET_IMPACT_SMOKE 0x9 /* emitter: white-pink smoke burst at the impact */
#define FX_COMET_GLOW_DISH    0xA /* gouraud model: wide shallow dish */
#define FX_COMET_SHOCK_RING   0xB /* model: flat ring, white texture */
#define FX_COMET_HAZE_TEXTURE 0xC /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x200,0x180) 64x64, a dim violet haze band; skipping it changes nothing on screen; runtime (docs/runtime-findings.md effect-data pass, low) (look pending: low) */
#define FX_COMET_BLUE_RING    0xD /* rings: dark blue / blue gouraud rings at the impact */

/* falling rock, a pool item */
typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* rotation */
    Pool6CEntry *emit14;
    SVECTOR pos18; /* landing position */
    s32 strip20; /* index */
} Fx038Item; /* 0x24 */

/* trail strip for btlfx_drawTrail */
typedef struct {
    u8 pad0[0x8C];
} Fx038Strip;

extern Fx038Strip fx_038_strips[];

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
    s32 countC; /* items spawned */
    s32 model10; /* models */
    s32 model14;
    s32 model18;
} Fx038Work; /* 0x1C */

extern u8 fx_038_spawnFrames[0x18]; /* spawn frames */
extern Unk800F78A4 *fx_038_env;
extern BtlModelBuf fx_038_effModels[];

static s32 fx_038_op80PoolTask(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work);

#endif
