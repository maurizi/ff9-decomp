#ifndef FX_100_H
#define FX_100_H

/* Effect module fx_100 (dir 13 file 0x100): enemy Solution (Blazer Beetle/Troll, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_100 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SOLUTION_DUST_PUFF    0x1 /* emitter: olive-tan dust puff particles */
#define FX_SOLUTION_SPIRAL_TRAIL 0x2 /* emitter: 3D trail spiralling in a tube, grey jagged strip */
#define FX_SOLUTION_OLIVE_RINGS  0x3 /* emitter: olive-grey gouraud rings, 8 frames */
#define FX_SOLUTION_DUST_WISPS   0x4 /* emitter: brown dust wisps in a cone */
#define FX_SOLUTION_PINK_STREAK_ORBIT 0x5 /* emitter: orbiting fan of a pink streak with red dots, white dot specks */
#define FX_SOLUTION_SPECK_TRAIL  0x6 /* emitter: 3D trail (white blob strip) with white dot specks */
#define FX_SOLUTION_VIOLET_SMOKE 0x7 /* emitter: violet smoke clouds */
#define FX_SOLUTION_VIOLET_RINGS 0x8 /* emitter: violet gouraud rings, 24 frames */

/* fx_100 choreography (docs/fxdata.md, "Choreography literals"): the two ops' keys and trail buffers. */
#define FX_SOLUTION_DUST_PUFF_FRAME      4    /* op80: DUST_PUFF starts at the caster's bone 0x22, on the ground */
#define FX_SOLUTION_SPIRAL_FRAME         0xC  /* SPIRAL_TRAIL starts at the target */
#define FX_SOLUTION_HIT_FRAME            0xE  /* the target's hit motion, OLIVE_RINGS, DUST_WISPS, PINK_STREAK_ORBIT; from here the target returns to its idle motion */
#define FX_SOLUTION_SPIRAL_TRAIL_SLOTS   9    /* SPIRAL_TRAIL's trail buffer (fx_100_op80Trails): slots */
#define FX_SOLUTION_SPIRAL_TRAIL_STRIDE  0x90 /* bytes per slot */
#define FX_SOLUTION_HIT_OP_FRAMES        0x28 /* op80's length */
#define FX_SOLUTION_SPECK_TRAIL_SLOTS    0x1A /* op81: SPECK_TRAIL (frame 0, at the caster's bone 0x11) trail buffer (fx_100_op81Trails): slots */
#define FX_SOLUTION_SPECK_TRAIL_STRIDE   0x60 /* bytes per slot */
#define FX_SOLUTION_SMOKE_FRAME          4    /* VIOLET_SMOKE and VIOLET_RINGS start at the target */
#define FX_SOLUTION_SMOKE_OP_FRAMES      0x28 /* op81's length */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* caster position */
    SVECTOR pos1C; /* target position */
    SVECTOR pos24;
    u8 unused2C[8]; /* op80's mode 0 asks for 0x34 */
} Fx100Work80; /* 0x34 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx100Work81; /* 0x1C */

extern Unk800F78A4 *fx_100_env;
extern s32 fx_100_op80Trails[];
extern s32 fx_100_op81Trails[];

static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

#endif
