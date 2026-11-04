#ifndef FX_132_H
#define FX_132_H

/* Effect module fx_132 (dir 13 file 0x132): enemy Appearance, Hide (Cactuar). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_132 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_APPEARANCE_SMOKE_PUFF    0x1 /* emitter: pale yellow-green smoke puffs */
#define FX_APPEARANCE_SPARKLE_HELIX 0x2 /* emitter: helix of white and yellow dot sparkles */

extern Unk800F78A4 *fx_132_env;

#endif
