#ifndef FX_133_H
#define FX_133_H

/* Effect module fx_133 (dir 13 file 0x133): enemy Disappear, Disappear2, Close Book (Tantarian, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_133 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DISAPPEAR_SMOKE_PUFF 0x1 /* emitter: pale smoke puffs (file 0x133; file 0x1B5 has coloured puffs and a "Dummy" letter sprite here) */

extern Unk800F78A4 *fx_133_env;

#endif
