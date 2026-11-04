#ifndef FX_1AB_H
#define FX_1AB_H

/* Effect module fx_1ab (dir 13 file 0x1ab): enemy Waterga (Kraken, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ab effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATERGA_WATER_RING     0x1 /* 'so' model: flared ring, blue water texture */
#define FX_WATERGA_WATER_SPHERE   0x2 /* 'so' model: sphere, blue water-caustic texture */
#define FX_WATERGA_WATER_SPLASH   0x3 /* emitter: cone of dark-blue water splash sprites, 24 frames */
#define FX_WATERGA_RINGS          0x4 /* emitter: cone of textured rings (texture not resolved) (look pending) */
#define FX_WATERGA_BACKDROP       0x5 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_WATERGA_BACKDROP_2     0x6 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_WATERGA_BACKDROP_3     0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_WATERGA_WATER_SPLASH_2 0x8 /* emitter: sphere of dark-blue water splash sprites, 24 frames */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model 1 rotation */
    VECTOR pos14; /* model 1 position */
    VECTOR scale24; /* model 1 scale */
    s32 color34; /* brightness */
    SVECTOR rot38; /* model 2 rotation */
    VECTOR pos40; /* model 2 position */
    VECTOR scale50; /* model 2 scale */
    VECTOR unk60; /* write-only (cleared at init) */
    SVECTOR pos70; /* target position */
    SVECTOR pos78;
    s16 timer80; /* fade-in countdown */
    s16 timer82; /* fade-out countdown */
    u16 model84; /* models */
    u16 model86;
} Fx1abWork; /* 0x88 */

extern Unk800F78A4 *fx_1ab_env;
extern BtlModelBuf fx_1ab_effModels[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
