#ifndef FX_11B_H
#define FX_11B_H

/* Effect module fx_11b (dir 13 file 0x11B): item Annoyntment. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ANNOYNTMENT_SPARKLES 0x1 /* emitter: blue-white puff, pale sparkles, white-cyan gouraud ring, green glow */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx11bWork;

extern Unk800F78A4 *fx_11b_env;

#endif
