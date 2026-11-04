#ifndef FX_18E_H
#define FX_18E_H

/* Effect module fx_18e (dir 13 file 0x18e): Charge!. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHARGE_BLUE_GLOW 0x1 /* emitter: orbiting blue / cyan glow fans + white star glints + white rings */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    SVECTOR posC;
    SVECTOR pos14;
} Fx18eWork;

extern Unk800F78A4 *fx_18e_env;

#endif
