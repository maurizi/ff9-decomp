#ifndef FX_1AF_H
#define FX_1AF_H

/* Effect module fx_1af (dir 13 file 0x1af): enemy Death (Valia Pira). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    s32 pos8[3]; /* position */
    SVECTOR rot14; /* rotation */
    s32 scale1C[3]; /* scale */
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    s32 model40[5]; /* models */
} Fx1afWork; /* 0x54 */

extern s16 fx_1af_modelFileIds[5]; /* model files */
extern Unk800F78A4 *fx_1af_env;
extern Unk80100C78Entry *fx_1af_task;
extern BtlModelBuf fx_1af_effModels[];
extern s32 fx_1af_op80Trails[];

#endif
