#ifndef FX_00E_H
#define FX_00E_H

/* Effect module fx_00e (dir 13 file 0x0E: Berserk, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_00e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BERSERK_FLAME_TEXTURE 0x3 /* layer table: render-to-texture (btlfx_drawLayerTexture): 5 layers into VRAM (0x200,0x180) 64x128, rising flames sampled by the red fire round the target; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BERSERK_CONE       0x4 /* model: open cone (frustum), plain white texture */
#define FX_BERSERK_FIRE_RINGS 0xA /* emitter: dark red-orange-yellow gouraud rings */
#define FX_BERSERK_RED_FLASH  0xB /* emitter: white star flash, red star flash, violet circle */

/* fx_00e choreography (docs/fxdata.md, "Choreography literals"): op80's length and the cone's texture. */
#define FX_BERSERK_FRAMES      0x20 /* op80: the cone is drawn (growing) until this frame, then the task ends */
#define FX_BERSERK_MODEL_COUNT 2    /* btlfx_initEffModels slots (only FX_BERSERK_CONE is registered) */
/* FX_BERSERK_CONE samples the flames FX_BERSERK_FLAME_TEXTURE renders each frame: 15-bit page (0x200, 0x100),
 * abr 1, no clut, v 0x80 = VRAM (0x200, 0x180). */
#define FX_BERSERK_CONE_TPAGE  0x138
#define FX_BERSERK_CONE_CLUT   0xFFFF
#define FX_BERSERK_CONE_V      0x80

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    s32 frame4; /* background frame */
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
    s32 pos18[3]; /* model position (a VECTOR without its pad word) */
    s32 model24; /* model */
} Fx00eWork; /* 0x28 */

extern Unk800F78A4 *fx_00e_env;
extern BtlModelBuf fx_00e_effModels[];

#endif
