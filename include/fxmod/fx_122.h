#ifndef FX_122_H
#define FX_122_H

/* Effect module fx_122 (dir 13 file 0x122): item Tent. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    s16 color2C; /* model brightness */
    s16 height2E; /* model height */
    SVECTOR pos30; /* target position */
    SVECTOR rot38; /* target rotation */
    SVECTOR pos40;
    s16 timer48; /* fade-in countdown */
    s16 timer4A; /* fade-out countdown */
    u16 model4C; /* model */
} Fx122Work;

typedef struct {
    s16 height0; /* unit id */
    s16 radius2; /* unit radius */
    u16 offset4; /* height offset; bit 15: forward offset instead */
} Fx122Offset;

extern Fx122Offset fx_122_offsets[0x11];
extern Unk800F78A4 *fx_122_env;
extern BtlModelBuf fx_122_effModels[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
