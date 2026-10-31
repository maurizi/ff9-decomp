#ifndef FX_0AE_H
#define FX_0AE_H

/* Effect module fx_0ae (dir 13 file 0xAE): enemy Venom Breath (Chimera and others). Emitters only. Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 bone position */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR aim20; /* angles from pos8 to pos10 */
} Fx0aeWork;

extern Unk800F78A4 *fx_0ae_env;
extern s32 fx_0ae_trails[];

#endif
