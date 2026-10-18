#ifndef FX_0B1_H
#define FX_0B1_H

/* Effect module fx_0b1 (dir 13 file 0xB1). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx0b1Track; /* 0x14 */

extern s32 D_801E8B90; /* frame */
extern Fx0b1Track D_801E8B9C[16];
extern SVECTOR D_801E8CDC[];
extern s32 D_801EAA54; /* fade timer */
extern s32 D_801EAA5C[4]; /* models */
extern SVECTOR D_801EAA6C; /* model rotation */
extern VECTOR D_801EAA7C; /* model scale */
extern VECTOR D_801EAA8C[4]; /* model positions */
extern SVECTOR D_801EAACC[4]; /* unit body positions */
extern BtlModel D_801EAAEC[];

void func_801E7704(void);
void func_801E7738(s32 start, u32 dst, u32 src, s16 *data);
void func_801E7788(void);
extern Unk800F78A4 *D_801E8B8C;
s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
