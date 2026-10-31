#ifndef FX_0A3_H
#define FX_0A3_H

/* Effect module fx_0a3 (dir 13 file 0xA3): enemy Electrocute (Torama and others). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 body position, moved 0x190 along rot18 */
    SVECTOR pos10; /* unit 0x10 position, moved 0xC8 along rot18 */
    SVECTOR rot18; /* unit 0x10 rotation */
} Fx0a3Work;

extern Unk800F78A4 *fx_0a3_env;
extern s32 fx_0a3_trails[];
extern s32 fx_0a3_trails2[];

#endif
