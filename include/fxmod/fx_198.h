#ifndef FX_198_H
#define FX_198_H

/* Effect module fx_198 (dir 13 file 0x198): enemy Tsunami (Black Waltz 1/Sealion). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_198 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TSUNAMI_WATER_SPLASH    0x6 /* anim quads: blue-white water bubbles and foam, 30 frames */
#define FX_TSUNAMI_BOWL            0x15 /* model: hemisphere bowl, plain white texture */
#define FX_TSUNAMI_BOWL_KEYS       0x16 /* table: per-frame rotation keys of the bowl model */
#define FX_TSUNAMI_BACKDROP        0x17 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_TSUNAMI_SPLASH_ROW_KEYS 0x18 /* table: per-frame position keys of the row of water splashes */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 scale8; /* scale */
    SVECTOR posC; /* caster position */
    s16 unk14; /* write-only (cleared at init) */
    s16 unk16; /* write-only (cleared at init) */
    s16 unk18; /* write-only (cleared at init) */
    s32 model1C; /* model */
    s32 unk20; /* unused */
} Fx198Work; /* 0x24 */

typedef struct {
    s16 x0; /* x (block 0x16: rotation y - 0x800) */
    s16 y2; /* y (block 0x16: rotation x - 0x400) */
    s16 z4; /* z (block 0x16: rotation z) */
} Fx198Key;

extern s16 fx_198_quadPathFileIds[0x11]; /* file ids */
extern Unk800F78A4 *fx_198_env;
extern BtlModelBuf fx_198_effModels[];

#endif
