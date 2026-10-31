#ifndef FX_0CD_H
#define FX_0CD_H

/* Effect module fx_0cd (dir 13 file 0xCD). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* model */
    SVECTOR unk4; /* rotation */
    VECTOR unkC; /* position */
    VECTOR unk1C; /* scale */
} Fx0cdModel; /* 0x2C */

typedef struct {
    s32 unk0; /* model C countdown */
    s32 unk4; /* model A countdown */
    s32 unk8; /* model B countdown */
    s32 unkC[5];
    Fx0cdModel unk20; /* A */
    Fx0cdModel unk4C; /* B */
    Fx0cdModel unk78; /* C */
    SVECTOR posA4; /* caster position */
    SVECTOR posAC; /* caster body position */
} Fx0cdWork;

extern Unk800F78A4 *fx_0cd_env;
extern BtlModel fx_0cd_effModels[];
extern s32 D_801E8044[];
extern s32 D_801E92B4[];

#endif
