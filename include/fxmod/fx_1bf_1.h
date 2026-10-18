#ifndef FX_1BF_1_H
#define FX_1BF_1_H

/* Effect module fx_1bf_1 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx1bf1Trail;

/* copied */
extern Unk800F78A4 *D_801EF2D8;
extern s32 D_801EFDD8; /* radius */
void func_801EE6BC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);
/* end copied */

#endif
