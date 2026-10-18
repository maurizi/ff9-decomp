#ifndef FX_1EF_H
#define FX_1EF_H

/* Effect module fx_1ef (dir 13 file 0x1ef). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
} Fx1efSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1efSummonData *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx1efSummon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[21];
} Fx1efTrail; /* 0xAC */

extern Unk800F78A4 *D_801E8B9C;
extern SVECTOR D_801E8D14; /* summon rotation */
extern VECTOR D_801E8D1C; /* summon position */
extern VECTOR D_801E8D2C; /* summon scale */
extern SVECTOR D_801E8D3C;
extern s32 D_801E8D4C[8]; /* timers */
extern Pool6CEntry *D_801E8D6C;
extern SVECTOR D_801E8D74;
extern Pool6CEntry *D_801E8D7C;
extern SVECTOR D_801E8D84;
extern Pool6CEntry *D_801E8D8C;
extern SVECTOR D_801E8D94;
extern s32 D_801E8D9C[];
extern Pool6CEntry *D_801EA87C;
extern SVECTOR D_801EA884;
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
extern Pool6CEntry *D_801EA8F4;
extern SVECTOR D_801EA8FC;
extern Fx1efTrail D_801EA90C[2];

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1efSummon *)(env)->unk4)
extern s32 D_801EAA64[2]; /* fade timers */
extern SVECTOR D_801EAA6C[4]; /* unit body positions */
extern s32 D_801EAA8C[4][0x1FE];
extern s32 D_801ECA6C[4][0x1FE];

/* copied */
extern Unk800F78A4 *D_801E8B9C;
/* end copied */

/* propagated */

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx1efTrack; /* 0x14 */

extern s32 D_801E8BA0; /* frame */
extern Fx1efTrack D_801E8BAC[];
void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR D_801E8CEC[];
void func_801E7788(void);

#endif
