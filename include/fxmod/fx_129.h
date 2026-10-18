#ifndef FX_129_H
#define FX_129_H

/* Effect module fx_129 (dir 13 file 0x129). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx, vy, vz;
} Fx129Vec3;

typedef struct {
    s32 unk0; /* model */
    VECTOR unk4; /* model position */
    SVECTOR unk14; /* model rotation */
    VECTOR unk1C; /* model scale */
    SVECTOR unk2C; /* target body position */
    s32 unk34; /* bounce phase */
    s32 unk38; /* bounce height */
    s32 unk3C;
    s32 unk40; /* height offset */
    s32 unk44; /* phase time */
    s32 unk48;
    s32 unk4C; /* fade time */
    s32 unk50; /* brightness */
    s32 unk54; /* squash countdown */
    s32 unk58; /* landings */
    s32 unk5C;
    s32 unk60; /* shake countdown */
    Fx129Vec3 unk64; /* target scale */
    Fx129Vec3 unk70;
} Fx129Work;

extern Unk800F78A4 *D_801E7C04;
extern BtlModel D_801E7C14[];

#endif
