#ifndef FX_119_H
#define FX_119_H

/* Effect module fx_119 (dir 13 file 0x119): item Magic Tag. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_119 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MAGIC_TAG_GLOW 0x4 /* emitter: blue-white glow sprite + a fan element */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx119Work;

extern Unk800F78A4 *fx_119_env;

#endif
