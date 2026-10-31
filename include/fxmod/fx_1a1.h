#ifndef FX_1A1_H
#define FX_1A1_H

/* Effect module fx_1a1 (dir 13 file 0x1a1): Climhazzard (Seiken). Evidence: the static sweep of the ability, item
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
} Fx1a1Work;

extern Unk800F78A4 *fx_1a1_env;

#endif
