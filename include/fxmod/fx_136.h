#ifndef FX_136_H
#define FX_136_H

/* Effect module fx_136 (dir 13 file 0x136): enemy Absorb (Malboro). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_136 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ABSORB_SPARKLE     0x1 /* emitter: cyan swirl collapsing into a star sparkle */
#define FX_ABSORB_SMOKE_SWIRL 0x3 /* emitter: curling white smoke */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
} Fx136Work;

extern Unk800F78A4 *fx_136_env;

#endif
