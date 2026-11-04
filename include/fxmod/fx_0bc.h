#ifndef FX_0BC_H
#define FX_0BC_H

/* Effect module fx_0bc (dir 13 file 0xBC): Darkside. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0bc effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DARKSIDE_SPARKLE_BURST 0x1 /* emitter: yellow / blue ring, blue sparkle sprites, fan */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx0bcWork80;

typedef struct {
    SVECTOR pos0; /* unit 0x10 position */
} Fx0bcWork81;

extern Unk800F78A4 *fx_0bc_env;

#endif
