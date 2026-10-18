#ifndef FX_0E3_0_H
#define FX_0E3_0_H

/* Effect module fx_0e3_0 (dir 13 file 0x0E3). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[49];
} Fx0e30Bolt;

typedef struct {
    SVECTOR unk0; /* position */
    s32 unk8; /* phase */
    s16 unkC; /* size */
    s16 unkE; /* speed */
} Fx0e30Puff;

extern Unk800F78A4 *D_801EA868;
extern s32 D_801EB370;
extern s32 D_801EB374;
extern s32 D_801EB378;

void func_801E7B40(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
void func_801E7D40(Unk80100C78Entry *entry, s32 arg1, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
s32 func_801E7F48(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry);

#endif
