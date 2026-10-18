#ifndef FX_17D_6_H
#define FX_17D_6_H

/* Effect module fx_17d_6 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx17d6Trail;

extern Unk800F78A4 *D_801E93B4;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx17d6Trail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[5];
} Fx17d6Trail5; /* 0x2C */

typedef struct {
    SVECTOR unk0; /* to */
    SVECTOR unk8; /* from */
    u8 unk10; /* trail index */
    u8 unk11;
    u8 unk12;
    u8 pad13;
    u8 unk14; /* angle */
    u8 unk15;
    u8 pad16[2];
    s32 *unk18; /* noise */
} Fx17d6Bolt;

extern Fx17d6Trail17 D_801EB124[];
extern Fx17d6Trail5 D_801EB474[];

void func_801E8B10(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);

#endif
