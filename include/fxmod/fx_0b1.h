#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1): Carbuncle, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
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
extern BtlModel fx_0b1_effModels[];

static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
static void fxm_camKeyUpdate(void);
extern Unk800F78A4 *fx_0b1_env;
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *motion180;
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

#define SUMMON0B1(env) ((Fx0b1Summon *)(env)->loader)

extern SVECTOR D_801E8D04;
extern VECTOR D_801E8D0C;
extern VECTOR D_801E8D1C;
extern SVECTOR D_801E8D2C;
extern s32 D_801E8D3C[8];
extern Pool6CEntry *fx_0b1_emitter0;
extern SVECTOR D_801E8D64;
extern Pool6CEntry *fx_0b1_emitter1;
extern SVECTOR D_801E8D74;
extern Pool6CEntry *fx_0b1_emitter2;
extern SVECTOR D_801E8D84;
extern s32 D_801E8D8C[];
extern Pool6CEntry *fx_0b1_emitter3;
extern SVECTOR D_801EA874;
extern SVECTOR D_801EA87C;
extern Pool6CEntry *fx_0b1_emitter4;
extern SVECTOR D_801EA88C;
extern Pool6CEntry *fx_0b1_emitter5;
extern SVECTOR D_801EA89C;
extern Pool6CEntry *fx_0b1_emitter6;
extern SVECTOR D_801EA8AC;
extern Pool6CEntry *fx_0b1_emitter7;
extern SVECTOR D_801EA8BC;
extern Pool6CEntry *fx_0b1_emitter8;
extern SVECTOR D_801EA8CC;
extern Pool6CEntry *fx_0b1_emitter9;
extern SVECTOR D_801EA8DC;
extern Pool6CEntry *fx_0b1_emitter10;
extern SVECTOR D_801EA8EC;
extern Fx0b1Trail D_801EA8FC[2];

#endif
