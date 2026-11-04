#ifndef FX_182_H
#define FX_182_H

/* Effect module fx_182 (dir 13 file 0x182): Countdown (Elan). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_182 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COUNTDOWN_BACKDROP    0x1 /* table: background layers for btlfx_drawBackdrop (byte-identical to fx_181 entry 0x1) */
#define FX_COUNTDOWN_GOLD_SHARDS 0x3 /* emitter: golden shard flashes, a dark teal sprite and white-blue gouraud rings (renders as fx_181 entry 0x2) */
#define FX_COUNTDOWN_LENS_FLASH  0x6 /* emitter: pink lens streak, golden shard flashes, rings (ring texture not resolved; renders as fx_181 entry 0x3) */

extern Unk800F78A4 *fx_182_env;

#endif
