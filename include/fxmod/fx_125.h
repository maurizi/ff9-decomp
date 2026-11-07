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

/* fx_125 choreography (docs/fxdata.md, "Choreography literals"): op80's emitter keys, the crystal's spin and shrink,
 * its texture. */
#define FX_POLY_MODEL_SLOTS       3    /* btlfx_initEffModels slots: GREEN_CRYSTAL and GREEN_CRYSTAL_2 twice */
#define FX_POLY_CYAN_RINGS_FRAME  0x10 /* CYAN_RINGS and DOTS start */
#define FX_POLY_SPIKES_FRAME      0x13 /* DOTS_2 and COLOR_SPIKES start */
#define FX_POLY_CRYSTAL_FRAME     0x14 /* the crystal draws, spinning, from this frame */
#define FX_POLY_STAR_GLOWS_FRAME  0x1A /* STAR_GLOWS start */
#define FX_POLY_STAR_BURST_FRAME  0x28 /* STAR_BURST and GREEN_SPECKS start */
#define FX_POLY_SHRINK_FRAME      0x3C /* the crystal shrinks from this frame */
#define FX_POLY_SHRINK_FRAMES     10   /* over this many (the (frame << 12) / N ramp) */
#define FX_POLY_FRAMES            (FX_POLY_SHRINK_FRAME + FX_POLY_SHRINK_FRAMES) /* 0x46: op80's length (the op ends) */
#define FX_POLY_GREEN_CRYSTAL_TPAGE 0xB3   /* Hi_RegisterTexEffModel texture: 8-bit page (0xC0, 0x100), abr 1 */
#define FX_POLY_GREEN_CRYSTAL_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_POLY_GREEN_CRYSTAL_U     0      /* texture origin (u, v) in the page */
#define FX_POLY_GREEN_CRYSTAL_V     0

typedef struct {
    s32 hold0; /* dead early-out flag, cleared at init and never set (see Fx046Work.hold0) */
    SVECTOR pos4; /* target position */
    SVECTOR posC;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C;
    s32 model24; /* models */
    s32 model28[2];
} Fx125Work; /* 0x30 */

extern Unk800F78A4 *fx_125_env;
extern BtlModelBuf fx_125_effModels[];

#endif
