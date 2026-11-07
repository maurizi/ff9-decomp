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
#define FX_STELLAR_CIRCLE_5_CLOUD_DOME 0x9 /* 'so' model: shallow dome with a grey-violet cloud texture */

/* fx_0f5 choreography (docs/fxdata.md, "Choreography literals"): op80's four states (two textured tubes and the
 * cloud dome open, widen, swell with the smoke, fade), the tube helper's mesh and the scrolled VRAM rects. */
#define FX_STELLAR_CIRCLE_5_STATE_OPEN    0    /* op80 state: the tubes and dome grow in */
#define FX_STELLAR_CIRCLE_5_STATE_WIDEN   1    /* the inner tube widens */
#define FX_STELLAR_CIRCLE_5_STATE_SWELL   2    /* SMOKE runs, everything swells */
#define FX_STELLAR_CIRCLE_5_STATE_FADE    3    /* tubes and dome fade (op80 ends) */
#define FX_STELLAR_CIRCLE_5_OPEN_FRAMES   12   /* OPEN lasts this many frames (the (frame << 12) / N ramp) */
#define FX_STELLAR_CIRCLE_5_WIDEN_FRAMES  12   /* WIDEN likewise; SMOKE starts at its end */
#define FX_STELLAR_CIRCLE_5_SWELL_FRAMES  0x20 /* SWELL */
#define FX_STELLAR_CIRCLE_5_FADE_FRAMES   0x10 /* FADE */
#define FX_STELLAR_CIRCLE_5_FRAMES        (FX_STELLAR_CIRCLE_5_OPEN_FRAMES + FX_STELLAR_CIRCLE_5_WIDEN_FRAMES + FX_STELLAR_CIRCLE_5_SWELL_FRAMES + FX_STELLAR_CIRCLE_5_FADE_FRAMES) /* 72: the whole op; frame4 / N drives the centre's drift */
#define FX_STELLAR_CIRCLE_5_CYL_RINGS     8    /* fxm_drawTexCylinder: rings along the tube (the first and last gouraud-faded) */
#define FX_STELLAR_CIRCLE_5_CYL_SEGMENTS  16   /* quads around a ring */
#define FX_STELLAR_CIRCLE_5_CYL_RING_DEPTH 0x90 /* z length of one ring */
#define FX_STELLAR_CIRCLE_5_CYL_TILE      0x20 /* texels per quad: the texture repeats every 4 x 4 quads */
#define FX_STELLAR_CIRCLE_5_CYL_TWIST     300  /* angle step between rings (the tube's spiral) */
#define FX_STELLAR_CIRCLE_5_TUBE_END_RADIUS   0x180 /* the inner tube's radius at its far end */
#define FX_STELLAR_CIRCLE_5_TUBE_2_END_RADIUS 0x280 /* the flickering outer tube's */
#define FX_STELLAR_CIRCLE_5_TUBE_TPAGE    0x97   /* inner tube texture: 8-bit page (0x1C0, 0x100), abr 0 (the helper adds abr 1) */
#define FX_STELLAR_CIRCLE_5_TUBE_CLUT     0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_STELLAR_CIRCLE_5_TUBE_U        0      /* texture origin (u, v) in the page */
#define FX_STELLAR_CIRCLE_5_TUBE_V        0
#define FX_STELLAR_CIRCLE_5_TUBE_2_TPAGE  0x17   /* outer tube texture: 4-bit page (0x1C0, 0x100), abr 0 */
#define FX_STELLAR_CIRCLE_5_TUBE_2_CLUT   0x3D00 /* CLUT at (0x0, 0xF4) */
#define FX_STELLAR_CIRCLE_5_TUBE_2_U      0
#define FX_STELLAR_CIRCLE_5_TUBE_2_V      0x80
#define FX_STELLAR_CIRCLE_5_TUBE_VRAM_X   0x1C0 /* btlfx_drawScrollMove rect scrolled under the inner tube: its 0x80 x 0x80 8-bit texels */
#define FX_STELLAR_CIRCLE_5_TUBE_VRAM_Y   0x100
#define FX_STELLAR_CIRCLE_5_TUBE_VRAM_W   0x40
#define FX_STELLAR_CIRCLE_5_TUBE_VRAM_H   0x80
#define FX_STELLAR_CIRCLE_5_TUBE_SRC_X    0x200 /* from this source */
#define FX_STELLAR_CIRCLE_5_TUBE_SRC_Y    0x100
#define FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_X 0x1C0 /* the outer tube's 0x80 x 0x80 4-bit texels (v 0x80) */
#define FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_Y 0x180
#define FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_W 0x20
#define FX_STELLAR_CIRCLE_5_TUBE_2_VRAM_H 0x80
#define FX_STELLAR_CIRCLE_5_TUBE_2_SRC_X  0x1E0
#define FX_STELLAR_CIRCLE_5_TUBE_2_SRC_Y  0x180
#define FX_STELLAR_CIRCLE_5_DOME_VRAM_X   0x280 /* third rect, presumably CLOUD_DOME's texture (not checked against the model) */
#define FX_STELLAR_CIRCLE_5_DOME_VRAM_Y   0x100
#define FX_STELLAR_CIRCLE_5_DOME_VRAM_W   0x20
#define FX_STELLAR_CIRCLE_5_DOME_VRAM_H   0x80
#define FX_STELLAR_CIRCLE_5_DOME_SRC_X    0x2A0
#define FX_STELLAR_CIRCLE_5_DOME_SRC_Y    0x100

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
