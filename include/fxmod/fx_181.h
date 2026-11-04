#ifndef FX_181_H
#define FX_181_H

/* Effect module fx_181 (dir 13 file 0x181): Countdown. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_181 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COUNTDOWN_BACKDROP    0x1 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_COUNTDOWN_GOLD_SHARDS 0x2 /* emitter: golden shard flashes, a dark teal sprite and white-blue gouraud rings */
#define FX_COUNTDOWN_LENS_FLASH  0x3 /* emitter: pink lens streak, golden shard flashes, rings (ring texture not resolved) */

extern Unk800F78A4 *fx_181_env;

#endif
