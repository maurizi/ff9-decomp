#ifndef FX_1BF_1_H
#define FX_1BF_1_H

/* Effect module fx_1bf_1 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* sub-state */
    s16 unk4; /* velocity */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* scale */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    s16 unk20; /* model index */
    s16 unk22;
} Fx1bf1Rock;

typedef struct {
    u8 pad0[0x84];
    s32 unk84[1]; /* models */
} Fx1bf1RockInst;

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
extern s32 D_801EF25C;
/* end copied */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10;
    SVECTOR unk18; /* camera */
    u8 pad20[8];
    s32 unk28[3]; /* model position */
    SVECTOR unk34;
    SVECTOR unk3C;
    s32 unk44[3]; /* model scale */
    s32 unk50[3];
    SVECTOR unk5C;
    Pool6CEntry *unk64;
    Pool6CEntry *unk68;
    Pool6CEntry *unk6C;
    s32 unk70; /* models */
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
} Fx1bf1Work80; /* 0xA4 */

extern BtlModel D_801EF2E8[];
extern SVECTOR D_801EF254;
extern s16 D_801EF260[];
s32 func_801EC704(SVECTOR *v, SVECTOR *rot);
s32 func_801EC7C8(s32 mode, Fx1bf1Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1bf1RockInst *inst);

#endif
