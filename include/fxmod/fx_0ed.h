#ifndef FX_0ED_H
#define FX_0ED_H

/* Effect module fx_0ed (dir 13 file 0x0ED). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* end frame of the current target */
    s32 unk8; /* frame counter */
    BtlUnit *unkC; /* target */
    SVECTOR unk10; /* target position */
} Fx0edWork81; /* 0x18 */

/* View of Unk800F78A40 up to the byte fx_0ed writes (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x10];
    u8 unk10;
} Fx0edBattleState;

extern Unk800F78A4 *D_801E9224;

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx0edSeg; /* 0x10 */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0edSeg *buf, s32 abr);
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
