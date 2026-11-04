#ifndef FX_18F_H
#define FX_18F_H

/* Effect module fx_18f (dir 13 file 0x18f): What's That!?. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WHATS_THAT_LIGHT_STREAKS 0x1 /* emitter: navy / pale-blue / grey gouraud rings + three white light-streak fans */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx18fWork;

extern Unk800F78A4 *fx_18f_env;

#endif
