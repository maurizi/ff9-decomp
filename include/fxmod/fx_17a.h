#ifndef FX_17A_H
#define FX_17A_H

/* Effect module fx_17a (dir 13 file 0x17A). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* state */
    s16 unk2;
    s16 unk4; /* size */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* drift */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
    s32 unk24;
} Fx17aPart;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17aVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    u8 pad8[4];
    SVECTOR unkC; /* position */
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24;
    u8 pad2C[8];
    Fx17aVec3 unk34; /* model position */
    Fx17aVec3 unk40; /* model scale */
    SVECTOR unk4C; /* model rotation */
    u8 pad54[0xC];
    Fx17aVec3 unk60;
    SVECTOR unk6C;
    u8 pad74[0xC];
    s32 unk80; /* models */
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    SVECTOR unk90[4];
    Pool6CEntry *unkB0;
} Fx17aWork80; /* 0xB4 */

typedef struct {
    u8 pad0[4];
    SVECTOR unk4[0x40]; /* points; vy decays */
    u8 pad204[8];
} Fx17aRing; /* 0x20C */

extern SVECTOR D_801E9AC4;
extern u8 D_801E9ACC[];
extern Fx17aRing D_801E9B44[2];
extern BtlModel D_801E9F64[];

s32 func_801E7BC0(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* propagated */

extern Unk800F78A4 *D_801E9F5C;
void func_801E7718(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);

#endif
