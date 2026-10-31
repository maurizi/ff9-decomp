#ifndef FX_05F_H
#define FX_05F_H

/* Effect module fx_05f (dir 13 file 0x5F). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[8]; /* target home positions */
    s32 unk40; /* target count */
    s32 unk44; /* jump height */
    s32 unk48[8]; /* target heights */
} Fx05fWork81; /* 0x68 */

typedef struct {
    s32 unk0;
    SVECTOR pos4; /* target body */
    SVECTOR unkC; /* target */
    s32 model14; /* models */
    s32 model18;
    s32 model1C;
    s32 model20;
    s32 model24;
    s32 model28;
    s32 model2C;
    s32 model30;
} Fx05fWork80; /* 0x34 */

typedef struct {
    u16 unk0;
    u16 unk2;
} Fx05fPair;

extern BtlModel fx_05f_effModels[];
extern Fx05fPair D_801E8278[];
extern Unk800F78A4 *fx_05f_env;

#endif
