#ifndef FX_094_H
#define FX_094_H

/* Effect module fx_094 (dir 13 file 0x94): enemy Delta Attack (Mover). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR aim18; /* angles from pos0 to pos8 */
    Pool6CEntry *emit20;
} Fx094Work;

extern Unk800F78A4 *fx_094_env;

#endif
