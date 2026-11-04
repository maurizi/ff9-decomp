#ifndef FX_18D_H
#define FX_18D_H

/* Effect module fx_18d (dir 13 file 0x18d): Climhazzard. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CLIMHAZZARD_FLAME_RINGS 0x1 /* emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames */

typedef struct {
    s32 timer0; /* steps left */
    s32 duration4; /* steps */
    SVECTOR pos8;
    SVECTOR pos10; /* from */
    SVECTOR pos18; /* to */
    SVECTOR pos20; /* current */
    SVECTOR rot28; /* angles */
} Fx18dWork;

extern Unk800F78A4 *fx_18d_env;

#endif
