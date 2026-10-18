#ifndef FX_040_H
#define FX_040_H

/* Effect module fx_040 (dir 13 file 0x40). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0[4];
    u8 pad10[8];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    SVECTOR unk28; /* target position */
    SVECTOR unk30; /* left of the target */
    SVECTOR unk38; /* right of the target */
    SVECTOR unk40;
    u8 pad48[8];
} Fx040Work; /* 0x50 */

extern u16 D_801E7E98[4]; /* file data indices */
extern s32 D_801E7EA0[4]; /* particle callbacks */
extern Unk800F78A4 *D_801E7EF8;
extern SVECTOR D_801E7F00; /* particle origin */
extern SVECTOR D_801E7F08; /* above the target */
extern SVECTOR D_801E7F18[4];

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n);
s32 func_801E775C(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n);
s32 func_801E77B4(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n);
s32 func_801E780C(VECTOR *out, s32 arg1, SVECTOR *arg2, s32 arg3, s32 t, s32 n);
s32 func_801E7864(VECTOR *out, SVECTOR *pos, SVECTOR *target, s32 t, s32 n);

#endif
