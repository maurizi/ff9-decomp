#ifndef FX_118_H
#define FX_118_H

/* Effect module fx_118 (dir 13 file 0x118): item Eye Drops. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_118 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EYE_DROPS_SPARKLES 0x4 /* emitter: white-lilac gouraud rings, pale sparkles, blue and green glows */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx118Work;

extern Unk800F78A4 *fx_118_env;

#endif
