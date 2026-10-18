#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d9Work84;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0d9Strip; /* 0x7C */
extern u16 D_801E9E40[];
void func_801E8188(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9E48[];
void func_801E83F0(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E8674(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9EA0;
extern SVECTOR D_801EA04C[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_0D9_STRIPS ((Fx0d9Strip *)((u8 *)D_801EA04C - 4))
s32 func_801E8B10(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
