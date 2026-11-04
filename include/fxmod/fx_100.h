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

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* caster position */
    SVECTOR pos1C; /* target position */
    SVECTOR pos24;
} Fx100Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target position */
    SVECTOR pos14;
} Fx100Work81;

extern Unk800F78A4 *fx_100_env;
extern s32 fx_100_op80Trails[];
extern s32 fx_100_op81Trails[];

static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

#endif
