#ifndef FX_04F_H
#define FX_04F_H

/* Effect module fx_04f (dir 13 file 0x4F: Reflect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_04f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REFLECT_HALO_RINGS 0x4 /* emitter: white-pink gouraud rings, golden glow strip, leaf-shaped halo, white-green / white-pink rings */
#define FX_REFLECT_STAR       0x6 /* model: four-pointed star, sketch texture */
#define FX_REFLECT_BACKDROP   0xA /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */
#define FX_REFLECT_GLOW_BAND  0xB /* gouraud model: wide ring band */
#define FX_REFLECT_PANELS     0xC /* model: three curved panels, black / white texture */
#define FX_REFLECT_BACKDROP_2 0xD /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 unk8; /* unused (runtime watch, round 33) */
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* in front of the target */
    SVECTOR pos1C;
    SVECTOR pos24;
    s32 pos2C[3]; /* model position (a VECTOR without its pad word) */
    SVECTOR rot38; /* model rotation (target facing) */
    s32 radius40; /* target radius */
    Pool6CEntry *emit44;
    s32 frame48; /* frame */
    s32 scaleX4C; /* model scale x */
    s32 scaleZ50; /* model scale z */
} Fx04fWork; /* 0x54 */

extern Unk800F78A4 *fx_04f_env;
extern BtlModelBuf fx_04f_effModels[];

static s32 fx_04f_op80PoolTask(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work);

#endif
