#ifndef FX_12F_H
#define FX_12F_H

/* Effect module fx_12f (dir 13 file 0x12F): enemy Paper Storm (Agares/Gargoyle, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_12f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PAPER_STORM_PAPER_SHEETS 0x1 /* emitter: cone of fluttering paper sheets */
#define FX_PAPER_STORM_ORANGE_BURST 0x3 /* emitter: orange speckle burst + red flat rings at each target */
#define FX_PAPER_STORM_PAPER_SWIRL  0x4 /* emitter: paper sheets spiralling up */
#define FX_PAPER_STORM_GREEN_BURST  0x8 /* emitter: green spiky burst, sprites, yellow and red rings at each target */

typedef struct {
    SVECTOR rot0;
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20[4];
    Pool6CEntry *emit40;
} Fx12fWork;

extern Unk800F78A4 *fx_12f_env;

#endif
