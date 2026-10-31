#ifndef FX_0E8_H
#define FX_0E8_H

/* Effect module fx_0e8 (dir 13 file 0x0E8): enemy Escape (Magic Vice, ...). The caster stretches, shrinks and fades
 * out (btlfx_setUnitScale, btlfx_fadeUnitById) among emitters. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    u8 pad1C[0x2C - 0x1C];
    VECTOR scale2C; /* unit scale */
    SVECTOR pos3C;
} Fx0e8Work;

extern Unk800F78A4 *fx_0e8_env;
extern s32 fx_0e8_op80Trails[];

#endif
