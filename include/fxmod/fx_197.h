#ifndef FX_197_H
#define FX_197_H

/* Effect module fx_197 (dir 13 file 0x197). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* pad: already split off */
    SVECTOR unk8;
    SVECTOR unk10;
} Fx197Tri; /* 0x18 */

typedef struct {
    s16 unk0;
    s16 unk2; /* triangle count */
    s32 unk4; /* triangles of the last model */
    Fx197Tri *unk8;
    Fx197Tri *unkC; /* next free triangle */
    Fx197Tri *unk10; /* first triangle of the last model */
} Fx197Shatter; /* 0x14 */

typedef struct {
    s32 unk0; /* position (fixed 20.12) */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* spin */
    s16 unk16;
    s16 unk18; /* frame, -1 = free */
    s16 unk1A; /* triangle */
} Fx197Shard; /* 0x1C */

typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx197Track;

/* copied */
extern Unk800F78A4 *D_801E9660;
extern Fx197Shatter *D_801EAF80[8];
extern Fx197Shard *D_801EAFA0[8];
extern s32 D_801EAFBC; /* shatter line */
Fx197Shard *func_801E7BBC(s32 i);
/* end copied */

#endif
