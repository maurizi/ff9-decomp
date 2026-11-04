#ifndef FX_1A1_H
#define FX_1A1_H

/* Effect module fx_1a1 (dir 13 file 0x1a1): Climhazzard (Seiken). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CLIMHAZZARD_FLAME_RINGS 0x1 /* emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames (byte-identical to fx_18d entry 0x1) */

typedef struct {
    s32 timer0; /* steps left */
    s32 count4; /* steps */
    SVECTOR pos8;
    SVECTOR pos10; /* from */
    SVECTOR pos18; /* to */
    SVECTOR pos20; /* current */
    SVECTOR angle28; /* angles */
} Fx1a1Work;

extern Unk800F78A4 *fx_1a1_env;

#endif
