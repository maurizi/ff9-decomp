#ifndef FX_0BD_H
#define FX_0BD_H

/* Effect module fx_0bd (dir 13 file 0xBD): Minus Strike. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0bd effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MINUS_STRIKE_SLASH_ARC 0x4 /* emitter: blue slash arc, blue sparkle sprites, pale yellow ring, fan */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx0bdWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 position */
} Fx0bdWork81;

extern Unk800F78A4 *fx_0bd_env;

#endif
