#ifndef FX_1F8_H
#define FX_1F8_H

/* Effect module fx_1f8 (dir 13 file 0x1f8): Carbuncle (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_1f8_env;
extern SVECTOR D_801E7B6C;
extern s32 D_801E7B74[1]; /* countdown */
extern s32 D_801E7B84[4]; /* models */
extern SVECTOR D_801E7B94; /* model rotation */
extern VECTOR D_801E7BA4; /* model scale */
extern VECTOR D_801E7BB4[4]; /* model positions */
extern SVECTOR D_801E7BF4[4]; /* unit body positions */
extern BtlModel fx_1f8_effModels[];

#endif
