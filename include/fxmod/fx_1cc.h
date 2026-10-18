#ifndef FX_1CC_H
#define FX_1CC_H

/* Effect module fx_1cc (dir 13 file 0x1cc). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E9C34;
extern s16 D_801E9BD0[];

typedef struct Fx1ccObj {
    s16 unk0; /* state / piece index */
    s16 unk2; /* 0: main object, 1: piece */
    s32 unk4; /* direction index */
    s32 unk8; /* frame */
    s32 unkC; /* position, 28.4 (low halves used by the main object) */
    s32 unk10;
    s32 unk14;
    s16 unk18; /* velocity (main: SVECTOR position) */
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* model */
    s32 unk30; /* model */
    Pool6CEntry *unk34; /* particle */
} Fx1ccObj;

typedef struct {
    u8 pad0[0x20];
    s32 unk20[14]; /* models */
    u8 pad58[0x78 - 0x58];
    s16 unk78; /* burst */
} Fx1ccInst;

extern SVECTOR D_801E9B68[];
void func_801E8284(s16 *from, s16 *to, s32 i, s16 *out);

#endif
