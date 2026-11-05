#ifndef FX_1B9_H
#define FX_1B9_H

/* Effect module fx_1b9 (dir 13 file 0x1b9): enemy Appearance (Gizamaluke). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_APPEARANCE_RIPPLES     0x1 /* emitter: green ripple-circle particles + grey widening ring (byte-identical to fx_1b8 entry 0x7) */
#define FX_APPEARANCE_TEAL_GLOWS  0x2 /* emitter: anim quads + sprites, soft teal-white glows (byte-identical to fx_1b8 entry 0x1); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_APPEARANCE_WATER_WAVE  0x6 /* emitter: green-teal water wave particles, 16 frames */
#define FX_APPEARANCE_WATER_SPOUT 0x7 /* emitter: green / blue water spout, 10 frames (byte-identical to fx_1b8 entry 0xE) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20;
} Fx1b9Work;

extern Unk800F78A4 *fx_1b9_env;

#endif
