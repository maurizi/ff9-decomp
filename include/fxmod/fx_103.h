#ifndef FX_103_H
#define FX_103_H

/* Effect module fx_103 (dir 13 file 0x103): enemy Mist (Mistodon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_103 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIST_SWIRL 0x2 /* emitter: blue-grey mist puffs orbiting */
#define FX_MIST_CLOUD 0x3 /* emitter: blue-grey mist puffs */

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR rot20;
    SVECTOR rot28;
    SVECTOR pos30;
    Pool6CEntry *emit38;
    Pool6CEntry *emit3C;
} Fx103Work;

extern Unk800F78A4 *fx_103_env;

#endif
