#ifndef FX_1A6_H
#define FX_1A6_H

/* Effect module fx_1a6 (dir 13 file 0x1a6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* caster body position */
    SVECTOR unk18; /* target body position */
    SVECTOR unk20; /* bone position */
    SVECTOR unk28;
    u8 pad30[0x38 - 0x30];
    SVECTOR unk38; /* angles */
    SVECTOR unk40;
    s32 unk48; /* index */
} Fx1a6Work80; /* 0x4C */

extern u16 D_801E823C[3]; /* files */
extern s32 (*D_801E8244[3])();
extern Unk800F78A4 *D_801E8298;
extern s32 D_801E82A8[3][0x5A];
extern s32 D_801E86E8[3][0x1D0];
extern s32 D_801E9CA8[3][0x560];
extern SVECTOR D_801EDD28[3]; /* from */
extern SVECTOR D_801EDD48[3]; /* to */
extern SVECTOR D_801EDD68[3];

s32 func_801E77C4(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames);
void func_801E80B4(s32 unit, s32 motion);
s32 func_801E8120(s32 unit);
void func_801E81A4(s32 unit);

#endif
