#ifndef FX_1B7_H
#define FX_1B7_H

/* Effect module fx_1b7 (dir 13 file 0x1b7). */
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
} Fx1b7Piece; /* 0x1C */

typedef struct {
    s16 unk0; /* model count */
    s16 unk2; /* face count */
    s32 unk4[8]; /* faces per model */
    SVECTOR (*unk24)[3]; /* faces */
    SVECTOR (*unk28)[3]; /* next free face */
    SVECTOR (*unk2C[8])[3]; /* faces of each model */
    Fx1b7Piece unk4C[0x7BC];
} Fx1b7Split;

/* copied */
extern Unk800F78A4 *D_801E87E0;
extern Fx1b7Split *D_801E87E8;
/* end copied */

#endif
