#ifndef FX_1F0_H
#define FX_1F0_H

/* Effect module fx_1f0 (dir 13 file 0x1f0): enemy Appearance (Dummy/Necron). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_APPEARANCE_GREY_RINGS         0x1 /* emitter: grey / white / blue gouraud rings */
#define FX_APPEARANCE_BLUE_SMOKE_COLUMN  0x3 /* 'so' model: open cylinder, blue smoke texture */
#define FX_APPEARANCE_WHITE_SPECKS       0x4 /* emitter: sprites, tiny white specks at run time (ablation changes < 470 px); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_APPEARANCE_BLUE_SMOKE_TWIST   0x5 /* 'so' model: twisted cylinder, blue smoke texture */
#define FX_APPEARANCE_BROWN_SMOKE_COLUMN 0x6 /* 'so' model: open cylinder, brown smoke texture */
#define FX_APPEARANCE_BROWN_SMOKE_TWIST  0x7 /* 'so' model: twisted cylinder, brown smoke texture */

/* fx_1f0 choreography (docs/fxdata.md, "Choreography literals"): op80's timeline (the targets wait high above the
 * field, fly down to their home positions one after another, then the smoke columns twist and the screen flashes),
 * its countdown slots (work->timer0 indices) and the smoke textures' scrolled rects. */
#define FX_APPEARANCE_TIMERS        16   /* work->timer0[]: countdowns (cleared at init): */
#define FX_APPEARANCE_T_HOLD        0    /*   the targets are held at their start positions (0x3000 below, 0x1000 behind home) */
#define FX_APPEARANCE_T_FLIGHT      1    /*   the targets' flights run (each after its fx_1f0_unitDelays delay) */
#define FX_APPEARANCE_T_SMOKE_4     4    /*   (never set: with T_SMOKE they gate the texture scroll) */
#define FX_APPEARANCE_T_SMOKE_5     5
#define FX_APPEARANCE_T_SMOKE       6    /*   the smoke twists morph into the columns (both pairs drawn) */
#define FX_APPEARANCE_T_SMOKE_GROW  7    /*   the smoke models grow */
#define FX_APPEARANCE_T_SMOKE_SPIN  8    /*   their spin speeds up */
#define FX_APPEARANCE_T_FLASH       9    /*   the additive screen flash (btlfx_drawScreenGrid) fades */
#define FX_APPEARANCE_MODEL_SLOTS   4    /* btlfx_initEffModels slots (the four smoke models) */
#define FX_APPEARANCE_UNITS         4    /* fx_1f0_units */
#define FX_APPEARANCE_HOLD_FRAMES   0x32 /* T_HOLD from frame 0, */
#define FX_APPEARANCE_FLIGHT_FRAME  FX_APPEARANCE_HOLD_FRAMES /* then T_FLIGHT */
#define FX_APPEARANCE_FLIGHT_FRAMES 0xA0 /*   for this many frames */
#define FX_APPEARANCE_RINGS_LEAD    8    /* a target's GREY_RINGS and WHITE_SPECKS start this many frames before its flight, fading in */
#define FX_APPEARANCE_UNIT_FRAMES   0x80 /* a target flies home along a 4-point curve (fxm_easeBlend4) in this many frames, */
#define FX_APPEARANCE_UNIT_FADE_FRAMES 0x10 /*   its rings fading over the last this many */
#define FX_APPEARANCE_SMOKE_FRAME   0xC0 /* T_SMOKE, T_SMOKE_GROW and T_SMOKE_SPIN start, */
#define FX_APPEARANCE_SMOKE_FRAMES  0x18 /*   each this long (the easing length too) */
#define FX_APPEARANCE_SMOKE_FADE_FRAMES 8 /* the smoke fades in over the first and out over the last this many frames */
#define FX_APPEARANCE_FLASH_FRAME   0xC8 /* T_FLASH starts, */
#define FX_APPEARANCE_FLASH_FRAMES  0x20 /*   this long */
#define FX_APPEARANCE_FRAMES        0x108 /* the task ends at this frame */
#define FX_APPEARANCE_BLUE_SMOKE_SCALE  0x19B0 /* the blue smoke pair's scale (scale80) before it grows */
#define FX_APPEARANCE_BROWN_SMOKE_SCALE 0x1400 /* the brown pair's (scale90) */
#define FX_APPEARANCE_BLUE_SMOKE_VRAM_X 0x200 /* BLUE_SMOKE_*'s texture (tpage 0x98: 8-bit page (0x200, 0x100), u, v 0 .. 0x7F): */
#define FX_APPEARANCE_BLUE_SMOKE_VRAM_Y 0x100 /*   btlfx_drawScrollMove scrolls it 1 texel a frame */
#define FX_APPEARANCE_BLUE_SMOKE_VRAM_W 0x40
#define FX_APPEARANCE_BLUE_SMOKE_VRAM_H 0x80
#define FX_APPEARANCE_BLUE_SMOKE_SRC_X  0x200 /*   from the strip below it */
#define FX_APPEARANCE_BLUE_SMOKE_SRC_Y  0x180
#define FX_APPEARANCE_BROWN_SMOKE_VRAM_X 0x1C0 /* BROWN_SMOKE_*'s (tpage 0x97: 8-bit page (0x1C0, 0x100), u 0 .. 0x6F, v 0x20 .. 0x8F), */
#define FX_APPEARANCE_BROWN_SMOKE_VRAM_Y 0x120 /*   2 texels a frame */
#define FX_APPEARANCE_BROWN_SMOKE_VRAM_W 0x38
#define FX_APPEARANCE_BROWN_SMOKE_VRAM_H 0x70
#define FX_APPEARANCE_BROWN_SMOKE_SRC_X  0x1C0
#define FX_APPEARANCE_BROWN_SMOKE_SRC_Y  0x190

typedef struct {
    s32 delay0; /* start delay */
    s32 timer4; /* flight countdown */
    SVECTOR startPos8; /* start position */
    SVECTOR homePos10; /* home position */
    SVECTOR emitPos18;
    SVECTOR pos20; /* current position */
    Pool6CEntry *emit28;
} Fx1f0Unit; /* 0x2C */

typedef struct {
    s32 timer0[FX_APPEARANCE_TIMERS]; /* timers */
    SVECTOR ctrlPts40[4]; /* curve control points */
    s32 model60; /* models */
    s32 model64;
    s32 model68;
    s32 model6C;
    VECTOR pos70; /* model position */
    VECTOR scale80; /* model scale */
    VECTOR scale90;
    SVECTOR rotA0; /* model rotation */
    SVECTOR rotA8;
    s32 spinB0;
    s32 spinB4;
} Fx1f0Work; /* 0xB8 */

extern s32 fx_1f0_unitDelays[];
extern Unk800F78A4 *fx_1f0_env;
extern Fx1f0Unit fx_1f0_units[FX_APPEARANCE_UNITS];
extern BtlModelBuf fx_1f0_effModels[];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

/* propagated */
static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out);

#endif
