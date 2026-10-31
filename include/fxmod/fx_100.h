#ifndef FX_100_H
#define FX_100_H

/* Effect module fx_100 (dir 13 file 0x100): enemy Solution (Blazer Beetle/Troll, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

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
