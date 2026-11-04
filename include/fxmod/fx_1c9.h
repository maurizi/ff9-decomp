#ifndef FX_1C9_H
#define FX_1C9_H

/* Effect module fx_1c9 (dir 13 file 0x1c9): enemy Thunder Slash (Beatrix). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1c9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDER_SLASH_SPARKS    0x1 /* emitter: cone of blue-green electric wisps, orange sparks and white glints (byte-identical to fx_0bf entry 0x1) */
#define FX_THUNDER_SLASH_LIGHTNING 0x2 /* sprite: blue / red lightning streak, 12 frames (drawn directly) (byte-identical to fx_0bf entry 0x2) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 colorC;
} Fx1c9Work;

extern Unk800F78A4 *fx_1c9_env;

#endif
