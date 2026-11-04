#ifndef FX_192_H
#define FX_192_H

/* Effect module fx_192 (dir 13 file 0x192): enemy Death. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_192 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_GREEN_SMOKE 0x1 /* emitter: grey-green smoke puffs in a spiral, 24 frames */

extern Unk800F78A4 *fx_192_env;

#endif
