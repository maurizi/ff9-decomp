#ifndef FX_19B_H
#define FX_19B_H

/* Effect module fx_19b (dir 13 file 0x19b): Iai Strike (alt). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_19b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_IAI_STRIKE_SLASH_ARCS    0x4 /* emitter: blue and pink slash arcs (byte-identical to fx_0be entry 0x4) */
#define FX_IAI_STRIKE_SPARKLE_BURST 0x8 /* emitter: violet / blue ring, blue and gold sparkle sprites, fan (byte-identical to fx_0be entry 0x8) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8;
} Fx19bWork;

extern Unk800F78A4 *fx_19b_env;

#endif
