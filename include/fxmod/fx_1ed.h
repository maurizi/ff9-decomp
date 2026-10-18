#ifndef FX_1ED_H
#define FX_1ED_H

/* Effect module fx_1ed (dir 13 file 0x1ed). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
} Fx1edSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1edSummonData *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx1edSummon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[21];
} Fx1edTrail; /* 0xAC */

extern Unk800F78A4 *D_801E8910;
extern SVECTOR D_801E8A88; /* summon rotation */
extern VECTOR D_801E8A90; /* summon position */
extern VECTOR D_801E8AA0; /* summon scale */
extern SVECTOR D_801E8AB0;
extern s32 D_801E8AC0[8]; /* timers */
extern Pool6CEntry *D_801E8AE0;
extern SVECTOR D_801E8AE8;
extern Pool6CEntry *D_801E8AF0;
extern SVECTOR D_801E8AF8;
extern Pool6CEntry *D_801E8B00;
extern SVECTOR D_801E8B08;
extern s32 D_801E8B10[];
extern Pool6CEntry *D_801EA5F0;
extern SVECTOR D_801EA5F8;
extern SVECTOR D_801EA600;
extern Pool6CEntry *D_801EA608;
extern SVECTOR D_801EA610;
extern Pool6CEntry *D_801EA618;
extern SVECTOR D_801EA620;
extern Pool6CEntry *D_801EA628;
extern SVECTOR D_801EA630;
extern Pool6CEntry *D_801EA638;
extern SVECTOR D_801EA640;
extern Pool6CEntry *D_801EA648;
extern SVECTOR D_801EA650;
extern Pool6CEntry *D_801EA658;
extern SVECTOR D_801EA660;
extern Pool6CEntry *D_801EA668;
extern SVECTOR D_801EA670;
extern Fx1edTrail D_801EA680[2];

void func_801E7788(void);
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1edSummon *)(env)->unk4)

/* copied */
extern Unk800F78A4 *D_801E8910;
extern SVECTOR D_801EA7E0[];
/* end copied */

/* propagated */

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx1edTrack; /* 0x14 */

extern s32 D_801E8914; /* frame */
extern Fx1edTrack D_801E8920[];
void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);

#endif
