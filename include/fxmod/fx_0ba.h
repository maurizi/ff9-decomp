#ifndef FX_0BA_H
#define FX_0BA_H

/* Effect module fx_0ba (dir 13 file 0xBA). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1];
} Fx0baStrip;

typedef struct {
    u8 pad0[0x12];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 *unk18; /* strip point */
} Fx0baPart;

typedef struct {
    SVECTOR unk0[17];
    s32 unk88;
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *D_801EC5F0;
extern Fx0baPath D_801ED424[32];
extern s32 D_801EEFDC;

void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave);
void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad);
s32 func_801EB1CC(SVECTOR *v);
s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801EC124(void);

#endif
