#ifndef FX_093_H
#define FX_093_H

/* Effect module fx_093 (dir 13 file 0x93): enemy Cannon (Armstrong and others). Emitters only. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos8; /* unit 0x10 bone position */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR aim20; /* angles from pos0 to pos8 */
    Pool6CEntry *emit28;
} Fx093Work;

extern Unk800F78A4 *fx_093_env;

#endif
