#ifndef FX_1EE_H
#define FX_1EE_H

/* Effect module fx_1ee (dir 13 file 0x1ee). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x180];
    BtlUnit9CC *unk180;
} Fx1eeSummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1eeSummonData *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx1eeSummon;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[21];
} Fx1eeTrail; /* 0xAC */

extern Unk800F78A4 *D_801E9104;
extern SVECTOR D_801E927C; /* summon rotation */
extern VECTOR D_801E9284; /* summon position */
extern VECTOR D_801E9294; /* summon scale */
extern SVECTOR D_801E92A4;
extern s32 D_801E92B4[8]; /* timers */
extern Pool6CEntry *D_801E92D4;
extern SVECTOR D_801E92DC;
extern Pool6CEntry *D_801E92E4;
extern SVECTOR D_801E92EC;
extern Pool6CEntry *D_801E92F4;
extern SVECTOR D_801E92FC;
extern s32 D_801E9304[];
extern Pool6CEntry *D_801EADE4;
extern SVECTOR D_801EADEC;
extern SVECTOR D_801EADF4;
extern Pool6CEntry *D_801EADFC;
extern SVECTOR D_801EAE04;
extern Pool6CEntry *D_801EAE0C;
extern SVECTOR D_801EAE14;
extern Pool6CEntry *D_801EAE1C;
extern SVECTOR D_801EAE24;
extern Pool6CEntry *D_801EAE2C;
extern SVECTOR D_801EAE34;
extern Pool6CEntry *D_801EAE3C;
extern SVECTOR D_801EAE44;
extern Pool6CEntry *D_801EAE4C;
extern SVECTOR D_801EAE54;
extern Pool6CEntry *D_801EAE5C;
extern SVECTOR D_801EAE64;
extern Fx1eeTrail D_801EAE74[2];

void func_801E7704(void);
void func_801E7738(s32 arg0, s32 arg1, s32 arg2, s32 data);
void func_801E7788(void);
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

#define SUMMON(env) ((Fx1eeSummon *)(env)->unk4)

/* copied */
extern Unk800F78A4 *D_801E9104;
/* end copied */

#endif
