#ifndef FX_17E_H
#define FX_17E_H

/* Effect module fx_17e (dir 13 file 0x17e): enemy Chestnut (Mandragora). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 model0; /* model */
    s32 fade4; /* brightness */
    VECTOR scale8; /* model scale */
    VECTOR pos18; /* model position */
    SVECTOR rot28; /* model rotation */
    SVECTOR pos30;
    SVECTOR pos38; /* target body position */
    s32 timer40[4]; /* countdowns */
    s32 timer50;
} Fx17eWork;

extern Unk800F78A4 *fx_17e_env;
extern BtlModelBuf fx_17e_effModels[];

#endif
