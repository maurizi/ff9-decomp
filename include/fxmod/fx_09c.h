#ifndef FX_09C_H
#define FX_09C_H

/* Effect module fx_09c (dir 13 file 0x9C): enemy Gnoll Attack (Dummy/Gnoll). Emitters only. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* write-only: pos10 + random */
    SVECTOR unk8; /* write-only: pos10 + random */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster body position */
    Pool6CEntry *emit20;
    u8 pad24[0x2C - 0x24];
    SVECTOR pos2C;
    SVECTOR aim34; /* angles */
    Pool6CEntry *emit3C;
} Fx09cWork;

extern Unk800F78A4 *fx_09c_env;

#endif
