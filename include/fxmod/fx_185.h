#ifndef FX_185_H
#define FX_185_H

/* Effect module fx_185 (dir 13 file 0x185): Spear (2nd). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    SVECTOR rot10; /* model rotation */
    VECTOR pos18; /* model position */
    VECTOR scale28; /* model scale */
    s32 index38; /* index */
    u16 model3C; /* model */
} Fx185Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx185Work81;

extern Unk800F78A4 *fx_185_env;
extern BtlModel fx_185_effModels[];
extern SVECTOR fx_185_targetPos;

#endif
