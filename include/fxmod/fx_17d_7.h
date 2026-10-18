#ifndef FX_17D_7_H
#define FX_17D_7_H

/* Effect module fx_17d_7 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx17d7Trail;

extern Unk800F78A4 *D_801EF2DC;
extern s32 D_801EFDDC; /* radius */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx17d7Trail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[5];
} Fx17d7Trail5; /* 0x2C */

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
} Fx17d7Bolt;

extern Fx17d7Trail17 D_801EFDFC[];
extern Fx17d7Trail5 D_801F00BC[];

typedef struct {
    s32 unk0; /* frame */
    u8 pad4[0xC];
    s16 *unk10; /* pool */
} Fx17d7Inst;

typedef struct {
    s32 *unk0; /* noise */
    s32 unk4; /* spawn count */
    s32 unk8; /* kind */
} Fx17d7Work83;

extern SVECTOR D_801EFDE4;
extern SVECTOR D_801EFDEC;
extern s32 D_801F019C[64][3]; /* noise */

s32 func_801EEAA8(s32 mode, Fx17d7Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

void func_801EE6E4(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d7Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
void func_801EE990(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d7Trail *path, Fx17d7Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad);

/* copied */
void func_801EE6E4(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d7Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
/* end copied */

#endif
