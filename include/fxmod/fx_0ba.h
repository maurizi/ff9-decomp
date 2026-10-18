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
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* trail index */
    s16 unk16; /* point index */
    u8 *unk18; /* strip point */
} Fx0baPart; /* 0x1C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[17];
} Fx0baTrail; /* 0x8C */

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* rotation */
} Fx0baWork85; /* 0x10 */

extern Fx0baTrail D_801EED20[5];
extern s32 D_801EC54C;
extern SVECTOR D_801ECAB0;

typedef struct {
    SVECTOR unk0[17];
    s32 unk88;
} Fx0baPath; /* 0x8C */

extern Unk800F78A4 *D_801EC5F0;
extern Fx0baPath D_801ED424[32];
extern s32 D_801EEFDC;

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b);
void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad);
void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave);
void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad);
s32 func_801EB1CC(SVECTOR *v);
s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801EC124(void);
void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to);

#endif
