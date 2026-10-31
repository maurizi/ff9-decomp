#ifndef FX_0B0_H
#define FX_0B0_H

/* Effect module fx_0b0 (dir 13 file 0xB0). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* unk14 raised by 0xF0 */
    SVECTOR pos14; /* position: env->targets->centerX */
    SVECTOR pos1C; /* unit 0x10 bone D_801E7C8C[0] */
    SVECTOR pos24; /* unit 0x10 bone D_801E7C8C[1] */
    SVECTOR pos2C; /* unkC, 0x300 to one side */
    SVECTOR pos34; /* unkC, 0x300 to the other side */
    SVECTOR pos3C;
    SVECTOR pos44;
} Fx0b0Work;

extern u16 D_801E7C8C[2]; /* bones */
extern Unk800F78A4 *fx_0b0_env;
extern s32 D_801E7CE8[];
extern s32 D_801E88A8[];
extern s32 D_801E9468[];
extern s32 D_801E9B28[];
extern s32 D_801E9C98[];
extern s32 D_801E9E08[];
extern s32 D_801E9F78[];

#endif
