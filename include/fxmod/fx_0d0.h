#ifndef FX_0D0_H
#define FX_0D0_H

/* Effect module fx_0d0 (dir 13 file 0xD0): Power Break. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR pos18; /* caster body position */
} Fx0d0Work;

extern Unk800F78A4 *fx_0d0_env;
extern s32 fx_0d0_trails[];

#endif
