#ifndef FX_1BF_1_H
#define FX_1BF_1_H

/* Effect module fx_1bf_1 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* frame */
    u8 pad4[0xC];
    s16 *unk10; /* pool */
} Fx1bf1Inst;

typedef struct {
    s32 *unk0; /* noise */
    s32 unk4; /* spawn count */
    s32 unk8; /* kind */
} Fx1bf1Work83;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx1bf1Trail;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx1bf1Trail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[5];
} Fx1bf1Trail5; /* 0x2C */

typedef struct {
    SVECTOR unk0; /* to */
    SVECTOR unk8; /* from */
    u8 unk10; /* trail index */
    u8 unk11; /* kind */
    u8 unk12;
    u8 unk13;
    u8 unk14; /* angle */
    u8 unk15;
    u8 pad16[2];
    s32 *unk18; /* noise */
} Fx1bf1Bolt;

extern Fx1bf1Trail17 D_801EFDF8[];
extern Fx1bf1Trail5 D_801F00B8[];

void func_801EE968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *path, Fx1bf1Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad);

/* copied */
extern Unk800F78A4 *D_801EF2D8;
extern s32 D_801EFDD8; /* radius */
void func_801EE6BC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
s32 func_801EEA80(s32 mode, Fx1bf1Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
extern s32 D_801F0198[64][3]; /* noise */
extern SVECTOR D_801EFDE0;
extern SVECTOR D_801EFDE8;
/* end copied */

#endif
