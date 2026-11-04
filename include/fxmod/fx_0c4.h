#ifndef FX_0C4_H
#define FX_0C4_H

/* Effect module fx_0c4 (dir 13 file 0xC4): Curse (Elan). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0c4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURSE_FLAME_PILLAR 0x6 /* emitter: orange-green flame sprite and grey smoke wisps (same call site as fx_0c3 entry 0x8) */
#define FX_CURSE_SPIRIT_WISPS 0x8 /* emitter: blue and magenta wisps (+ two rings, texture not in the file) (same call site as fx_0c3 entry 0x6) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
} Fx0c4Work;

extern Unk800F78A4 *fx_0c4_env;

#endif
