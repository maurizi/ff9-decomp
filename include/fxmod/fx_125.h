#ifndef FX_125_H
#define FX_125_H

/* Effect module fx_125 (dir 13 file 0x125): Poly; enemy Poly (Benero/King Leo/Zenero). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_125 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POLY_DOTS           0x1 /* emitter: tiny yellow / violet dots */
#define FX_POLY_DOTS_2         0x2 /* emitter: tiny yellow / green / blue dots */
#define FX_POLY_GREEN_SPECKS   0x3 /* emitter: small green and cyan bow-tie specks */
#define FX_POLY_COLOR_SPIKES   0x4 /* emitter: two fans of blue and green light spikes, multicoloured dot sparkles */
#define FX_POLY_STAR_BURST     0x5 /* emitter: orange-to-green glow, white-green gouraud rings, violet star sprites */
#define FX_POLY_STAR_GLOWS     0x6 /* emitter: orange-to-green glows and violet star sprites */
#define FX_POLY_GREEN_SPECKS_2 0x7 /* emitter: small green and cyan bow-tie specks */
#define FX_POLY_CYAN_RINGS     0x8 /* emitter: white-cyan gouraud rings, 24 frames */
#define FX_POLY_GREEN_CRYSTAL  0xD /* model: faceted polyhedron, a green crystalline texture at run time: part of the green crystal over the target (byte-identical to entry 0xE); runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_POLY_GREEN_CRYSTAL_2 0xE /* model: faceted polyhedron, registered with a tpage the C computes: also part of the green crystal over the target (byte-identical to entry 0xD); runtime (docs/runtime-findings.md effect-data pass 2, medium) */

typedef struct {
    s32 hold0; /* dead early-out flag, cleared at init and never set (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
    SVECTOR posC;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C;
    s32 model24; /* models */
    s32 model28[2];
} Fx125Work;

extern Unk800F78A4 *fx_125_env;
extern BtlModelBuf fx_125_effModels[];

#endif
