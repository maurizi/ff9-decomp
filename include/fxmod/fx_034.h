#ifndef FX_034_H
#define FX_034_H

/* Effect module fx_034 (dir 13 file 0x34). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx034Point;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* model rotation */
    VECTOR unk10; /* model position */
    VECTOR unk20; /* model scale */
    s32 unk30; /* colour */
    SVECTOR unk34;
    SVECTOR unk3C;
    SVECTOR unk44;
    SVECTOR unk4C;
    SVECTOR unk54;
    SVECTOR unk5C;
    SVECTOR unk64;
    s16 unk6C;
    u16 unk6E; /* models */
    u16 unk70;
} Fx034Work; /* 0x74 */

extern BtlModel D_801E88C0[];
extern u8 D_801E8990[];
extern u8 D_801EA040[];
extern Unk800F78A4 *D_801E82A0;
extern SVECTOR *D_801E82B0[8]; /* split model vertices, per primitive kind */
extern u16 D_801E82D0[8]; /* primitive counts */
extern Fx034Point D_801E82E0[]; /* primitive centres */

void func_801E7744(s32 model, s32 scale, u8 *buf);
void func_801E7810(s32 kind, s32 count, s32 scale, SVECTOR *v);
void func_801E7A00(s32 model, u8 *buf);
void func_801E7B1C(s32 kind, s32 count, SVECTOR *v);

#endif
