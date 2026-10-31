#ifndef FX_18D_H
#define FX_18D_H

/* Effect module fx_18d (dir 13 file 0x18d): Climhazzard. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* steps left */
    s32 unk4; /* steps */
    SVECTOR pos8;
    SVECTOR pos10; /* from */
    SVECTOR unk18; /* to */
    SVECTOR pos20; /* current */
    SVECTOR unk28; /* angles */
} Fx18dWork;

extern Unk800F78A4 *fx_18d_env;

#endif
