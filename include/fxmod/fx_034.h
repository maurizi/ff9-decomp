#ifndef FX_034_H
#define FX_034_H

/* Effect module fx_034 (dir 13 file 0x34). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx034Point;

extern Unk800F78A4 *D_801E82A0;
extern SVECTOR *D_801E82B0[8]; /* split model vertices, per primitive kind */
extern u16 D_801E82D0[8]; /* primitive counts */
extern Fx034Point D_801E82E0[]; /* primitive centres */

void func_801E7744(s32 model, s32 scale);
void func_801E7810(s32 kind, s32 count, s32 scale, SVECTOR *v);
void func_801E7A00(s32 model, s32 buf);
void func_801E7B1C(s32 kind, s32 count, SVECTOR *v);

#endif
