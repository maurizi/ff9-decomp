#ifndef FX_085_H
#define FX_085_H

/* Effect module fx_085 (dir 13 file 0x85). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx085Strip; /* 0x7C */
extern u16 D_801E9D94[];
void func_801E82A0(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9D9C[];
void func_801E8508(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E878C(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9DF4;
extern SVECTOR D_801E9FA0[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)D_801E9FA0 - 4))
s32 func_801E8C28(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
