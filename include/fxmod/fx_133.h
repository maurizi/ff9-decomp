#ifndef FX_133_H
#define FX_133_H

/* Effect module fx_133 (dir 13 file 0x133): enemy Disappear, Disappear2, Close Book (Tantarian, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_133 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DISAPPEAR_DUST_PUFF 0x1 /* emitter: pale smoke puffs (file 0x133; file 0x1B5 has coloured puffs and a "Dummy" letter sprite here) */

/* fx_133 choreography (docs/fxdata.md, "Choreography literals"): op80's length (DUST_PUFF starts at init, at the
 * target's fxArgs[1] bone raised by 4 * the signed byte fxArgs[0]). */
#define FX_DISAPPEAR_FRAMES 0x30 /* op80's length */

extern Unk800F78A4 *fx_133_env;

#endif
