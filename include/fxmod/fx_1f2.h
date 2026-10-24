#ifndef FX_1F2_H
#define FX_1F2_H

/* Effect module fx_1f2 (dir 13 file 0x1f2). */
#include "btlfx_mod.h"

/* copied */
extern Unk800F78A4 *D_801EA260;
/* end copied */

typedef struct {
    s32 unk0; /* models drawn */
    s32 unk4; /* flash model timer */
    s32 unk8; /* twin model timer */
    s32 unkC; /* tint timers */
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20; /* screen ripple timer */
    s32 unk24;
    s32 unk28;
    s32 unk2C; /* spin speed */
    s32 unk30;
    s32 unk34; /* sparks timer */
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 pad5C[(0x80 - 0x5C) / 4];
    Pool6CEntry *unk80;
    Pool6CEntry *unk84;
    Pool6CEntry *unk88;
    Pool6CEntry *unk8C;
    s32 pad90[(0x100 - 0x90) / 4];
    SVECTOR unk100;
    SVECTOR unk108;
    SVECTOR unk110[2]; /* bone 5 and bone 9 positions */
    SVECTOR unk120; /* midpoint */
} Fx1f2Work; /* 0x128 */

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* radius */
    SVECTOR unk8; /* rotation */
    VECTOR unk10; /* position */
    VECTOR unk20; /* scale */
} Fx1f2Model; /* 0x30 */

typedef struct {
    s32 unk0; /* models */
    s32 unk4;
    s32 unk8;
    s32 unkC; /* radius */
    SVECTOR unk10; /* rotation */
    VECTOR unk18; /* position */
    VECTOR unk28; /* scale */
} Fx1f2Ring; /* 0x38 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9];
    s32 unk4C; /* timer */
    SVECTOR unk50[16]; /* points (pad = next) */
    SVECTOR unkD0[16]; /* velocities */
} Fx1f2Trail; /* 0x150 */

extern Fx1f2Model D_801EA270;
extern Fx1f2Model D_801EA2A0;
extern Fx1f2Model D_801EA2D0;
extern Fx1f2Ring D_801EA300[4];
extern Fx1f2Model D_801EA3E0;
extern Fx1f2Model D_801EA410[2];
extern Fx1f2Model D_801EA470;
extern Fx1f2Model D_801EA4A0[2];
extern Fx1f2Model D_801EA500;
extern Fx1f2Model D_801EA530[4];
extern SVECTOR D_801EA568;
extern Fx1f2Model D_801EA5F0;
extern VECTOR D_801EA600;
extern Fx1f2Model D_801EA620;
extern SVECTOR D_801EA650; /* target */
extern SVECTOR D_801EA658; /* angles to the target */
extern SVECTOR D_801EA660;
extern SVECTOR D_801EA670[4]; /* unit positions */
extern Fx1f2Trail D_801EA690[16];
extern Fx1f2Trail D_801EBB90[4];
extern BtlModel D_801ED090[];

s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
