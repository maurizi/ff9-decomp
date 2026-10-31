#ifndef FX_006_H
#define FX_006_H

/* Effect module fx_006 (dir 13 file 0x06). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR unk14; /* target body position */
    SVECTOR unk1C; /* target position */
    s32 unk24; /* frame */
} Fx006Work; /* 0x28 */

extern Unk800F78A4 *fx_006_env;
extern BtlModel fx_006_effModels[];
extern s32 D_801E829C[];

#endif
