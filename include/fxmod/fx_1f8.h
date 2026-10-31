#ifndef FX_1F8_H
#define FX_1F8_H

/* Effect module fx_1f8 (dir 13 file 0x1f8): Carbuncle (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_1f8_env;
extern SVECTOR fx_1f8_emitPos0;
extern s32 fx_1f8_timer[1]; /* countdown */
extern s32 fx_1f8_models[4]; /* models */
extern SVECTOR fx_1f8_modelRot; /* model rotation */
extern VECTOR fx_1f8_modelScale; /* model scale */
extern VECTOR fx_1f8_modelPos[4]; /* model positions */
extern SVECTOR fx_1f8_emitPos1[4]; /* unit body positions */
extern BtlModelBuf fx_1f8_effModels[];

#endif
