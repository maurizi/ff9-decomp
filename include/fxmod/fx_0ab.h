#ifndef FX_0AB_H
#define FX_0AB_H

/* Effect module fx_0ab (dir 13 file 0xAB): enemy Maelstrom (Feather Circle/Garuda and others). Emitters only.
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* pos10 raised by 0xC8 */
    SVECTOR pos20; /* pos10 raised by 0x190 */
} Fx0abWork;

extern Unk800F78A4 *fx_0ab_env;
extern s32 fx_0ab_trails[];

#endif
