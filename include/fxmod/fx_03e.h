#ifndef FX_03E_H
#define FX_03E_H

/* Effect module fx_03e (dir 13 file 0x3E). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
    s32 frame24; /* frame */
} Fx03eWork; /* 0x28 */

extern Unk800F78A4 *fx_03e_env;
extern BtlModel fx_03e_effModels[];
extern s32 fx_03e_trails[];

#endif
