#ifndef FX_124_H
#define FX_124_H

/* Effect module fx_124 (dir 13 file 0x124): items Amethyst, Aquamarine, Diamond, Emerald, Garnet, Lapis Lazuli, Moonstone, Opal and others. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_124 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AMETHYST_YELLOW_RINGS 0x1 /* emitter: white-yellow to navy gouraud rings spiralling + sprites (render black); the same file plays every gem item */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx124Work;

extern Unk800F78A4 *fx_124_env;

#endif
