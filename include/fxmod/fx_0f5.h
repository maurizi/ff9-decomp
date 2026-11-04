#ifndef FX_0F5_H
#define FX_0F5_H

/* Effect module fx_0f5 (dir 13 file 0x0F5): Stellar Circle 5. Textured cylinders (fxm_drawTexCylinder) around a
 * model at the target centre, with scrolling textures (btlfx_drawScrollMove). Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_STELLAR_CIRCLE_5_SMOKE 0x7 /* emitter: grey smoke cloud, 20 animated frames */
#define FX_STELLAR_CIRCLE_5_MODEL 0x9 /* 'so' model (btlfx_registerEffectModel) (look pending) */

extern Unk800F78A4 *fx_0f5_env;
extern s32 fx_0f5_flickerPhase; /* flicker phase */
extern u8 fx_0f5_flickerGradient[];

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR center8; /* base position */
    SVECTOR pos10; /* position */
    SVECTOR rot18; /* rotation */
    s32 model20; /* model */
} Fx0f5Work80; /* 0x24 */

extern BtlModelBuf fx_0f5_effModels[];

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 bright, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 dr, s32 ang, s32 dang, s32 dseg, s32 abr, s32 flicker);

#endif
