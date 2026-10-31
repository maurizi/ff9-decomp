#ifndef FX_0F0_H
#define FX_0F0_H

/* Effect module fx_0f0 (dir 13 file 0x0F0): enemy Zombie Powder (Whale Zombie). Emitters from a caster bone with
 * trail buffers, one homing on the target. Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx0f0Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx0f0Work81;

extern Unk800F78A4 *fx_0f0_env;
extern s32 fx_0f0_op80Trails0[];
extern s32 fx_0f0_op80Trails1[];
extern s32 fx_0f0_op80Trails2[];

#endif
