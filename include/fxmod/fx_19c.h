#ifndef FX_19C_H
#define FX_19C_H

/* Effect module fx_19c (dir 13 file 0x19c): Thunder Slash (Seiken). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_19c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_THUNDER_SLASH_SPARKS    0x1 /* emitter: cone of blue-green electric wisps, orange sparks and white glints (byte-identical to fx_0bf entry 0x1) */
#define FX_THUNDER_SLASH_LIGHTNING 0x2 /* sprite: blue / red lightning streak, 12 frames (drawn directly; byte-identical to fx_0bf entry 0x2) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 colorC;
} Fx19cWork;

extern Unk800F78A4 *fx_19c_env;

#endif
