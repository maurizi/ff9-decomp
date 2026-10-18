#ifndef FX_18B_H
#define FX_18B_H

/* Effect module fx_18b (dir 13 file 0x18b). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    s32 unk8;
    SVECTOR unkC; /* target body position */
    SVECTOR unk14; /* caster body position */
} Fx18bWork; /* 0x1C */

extern Unk800F78A4 *D_801E827C;
extern s32 D_801E8280; /* wave phase */
extern s32 D_801E8284; /* wave amplitude */

void func_801E7EB8(s32 r, s32 i, s32 ang, s32 arg3, s16 *out);

/* propagated */
void func_801E7704(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
