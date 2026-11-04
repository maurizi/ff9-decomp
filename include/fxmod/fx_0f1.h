#ifndef FX_0F1_H
#define FX_0F1_H

/* Effect module fx_0f1 (dir 13 file 0x0F1): Free Energy. One emitter at the target body. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FREE_ENERGY_RUNE_BURST 0x2 /* emitter: white rune glyphs with blue-white rings, then a fire burst */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx0f1Work;

extern Unk800F78A4 *fx_0f1_env;

#endif
