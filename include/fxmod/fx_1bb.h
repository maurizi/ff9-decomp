#ifndef FX_1BB_H
#define FX_1BB_H

/* Effect module fx_1bb (dir 13 file 0x1bb): enemy Climhazzard (Beatrix). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1bb effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CLIMHAZZARD_FLAME_RINGS 0x1 /* emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames (byte-identical to fx_18d entry 0x1) */

extern Unk800F78A4 *fx_1bb_env;

#endif
