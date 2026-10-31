#ifndef FX_088_H
#define FX_088_H

/* Effect module fx_088 (dir 13 file 0x88): Spare Change (Elan). Emitters and a turn of unit 0x10
 * (fxm_turnUnitBy). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 body position */
    SVECTOR pos10; /* btlfx_getUnitBonePos(0x10, 0x17) */
    SVECTOR aim18; /* angles from pos8 to env->targets->centerX */
} Fx088Work80;

typedef struct {
    SVECTOR turn0; /* rotation step total */
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx088Work81;

extern Unk800F78A4 *fx_088_env;
extern s32 fx_088_trails[];

static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);

#endif
