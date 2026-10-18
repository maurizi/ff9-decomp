#ifndef FX_1A2_H
#define FX_1A2_H

/* Effect module fx_1a2 (dir 13 file 0x1a2). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx1a2Seg; /* 0x10 */

typedef struct {
    SVECTOR unk0[3];
} Fx1a2Tri; /* 0x18 */

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fx1a2Piece; /* 0x1C */

extern Fx1a2Tri D_801E9854[130];
extern Fx1a2Piece D_801EA484[130];

/* copied */
extern Unk800F78A4 *D_801E9844;
/* end copied */

#endif
