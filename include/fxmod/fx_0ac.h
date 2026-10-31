#ifndef FX_0AC_H
#define FX_0AC_H

/* Effect module fx_0ac (dir 13 file 0xAC): enemy Sleeping Juice (Cave Imp/Flan). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR pos20; /* unit 0x10 bone position, raised by 0x96 */
    SVECTOR aim28; /* angles from pos20 to pos10 */
} Fx0acWork;

extern Unk800F78A4 *fx_0ac_env;
extern s32 fx_0ac_trails[];

#endif
