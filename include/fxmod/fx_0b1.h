#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 D_801E8B90; /* frame */
extern Fx0b1Track D_801E8B9C[16];
extern SVECTOR D_801E8CDC[];
extern s32 D_801EAA54; /* fade timer */
extern s32 D_801EAA5C[4]; /* models */
extern SVECTOR D_801EAA6C; /* model rotation */
extern VECTOR D_801EAA7C; /* model scale */
extern VECTOR D_801EAA8C[4]; /* model positions */
extern SVECTOR D_801EAACC[4]; /* unit body positions */
extern BtlModel D_801EAAEC[];

void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);
void func_801E7788(void);
extern Unk800F78A4 *D_801E8B8C;
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
} Fx0b1SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0b1SummonData *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0b1Summon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[21];
} Fx0b1Trail; /* 0xAC */

#define SUMMON0B1(env) ((Fx0b1Summon *)(env)->unk4)

extern SVECTOR D_801E8D04;
extern VECTOR D_801E8D0C;
extern VECTOR D_801E8D1C;
extern SVECTOR D_801E8D2C;
extern s32 D_801E8D3C[8];
extern Pool6CEntry *D_801E8D5C;
extern SVECTOR D_801E8D64;
extern Pool6CEntry *D_801E8D6C;
extern SVECTOR D_801E8D74;
extern Pool6CEntry *D_801E8D7C;
extern SVECTOR D_801E8D84;
extern s32 D_801E8D8C[];
extern Pool6CEntry *D_801EA86C;
extern SVECTOR D_801EA874;
extern SVECTOR D_801EA87C;
extern Pool6CEntry *D_801EA884;
extern SVECTOR D_801EA88C;
extern Pool6CEntry *D_801EA894;
extern SVECTOR D_801EA89C;
extern Pool6CEntry *D_801EA8A4;
extern SVECTOR D_801EA8AC;
extern Pool6CEntry *D_801EA8B4;
extern SVECTOR D_801EA8BC;
extern Pool6CEntry *D_801EA8C4;
extern SVECTOR D_801EA8CC;
extern Pool6CEntry *D_801EA8D4;
extern SVECTOR D_801EA8DC;
extern Pool6CEntry *D_801EA8E4;
extern SVECTOR D_801EA8EC;
extern Fx0b1Trail D_801EA8FC[2];

#endif
