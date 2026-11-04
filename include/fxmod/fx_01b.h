#ifndef FX_01B_H
#define FX_01B_H

/* Effect module fx_01b (dir 13 file 0x1B: Float, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_01b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLOAT_CLOUD      0x1 /* emitter: white puffy cloud particles bursting under the target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_FLOAT_LIFT_RINGS 0x2 /* emitter: speckled ring, white-lilac gouraud rings, two fans */
#define FX_FLOAT_CLOUD_2    0x3 /* emitter: white cloud puffs (the second cloud emitter, same cloud texture); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_FLOAT_CLOUD_TEXTURE 0x4 /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x200,0x180) 128x128, a white cloud sampled by the cloud burst; runtime (docs/runtime-findings.md effect-data pass) */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
} Fx01bWork;

extern Unk800F78A4 *fx_01b_env;

#endif
