#ifndef FX_1B6_H
#define FX_1B6_H

/* Effect module fx_1b6 (dir 13 file 0x1b6). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    SVECTOR unkC; /* velocity (vx, vy, vz), unk12: scale */
    s16 unk14;
    s16 unk16;
    s16 unk18; /* age, -1 = free */
    s16 unk1A; /* face */
} Fx1b6Piece; /* 0x1C */

typedef struct {
    s16 unk0; /* model count */
    s16 unk2; /* face count */
    s32 unk4[7]; /* faces per model */
    SVECTOR (*unk20)[3]; /* faces */
    SVECTOR (*unk24)[3]; /* next free face */
    SVECTOR (*unk28[7])[3]; /* faces of each model */
    Fx1b6Piece unk44[0x708];
} Fx1b6Split;

extern Unk800F78A4 *D_801E85B4;
extern Fx1b6Split *D_801E9744;
extern Fx1b6Piece *D_801E9748;
extern s32 D_801E974C;

void func_801E7704(Fx1b6Split *s, SVECTOR (*faces)[3]);
s32 func_801E774C(s32 model);
Fx1b6Piece *func_801E7830(void);
void func_801E7868(s32 *thresh);

/* copied */
extern Unk800F78A4 *D_801E85B4;
/* end copied */

#endif
