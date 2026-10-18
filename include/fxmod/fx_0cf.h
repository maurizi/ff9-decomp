#ifndef FX_0CF_H
#define FX_0CF_H

/* Effect module fx_0cf (dir 13 file 0xCF). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* position: env->unkC->unk38..3C */
} Fx0cfWork80;

typedef struct {
    SVECTOR unk0; /* unit 0x10 body position */
    SVECTOR unk8; /* angles from unk0 to unk18, turned around */
    SVECTOR unk10; /* unk18 moved 0x320 along unk8 */
    SVECTOR unk18; /* position: env->unkC->unk38..3C */
    SVECTOR unk20; /* start position */
} Fx0cfWork81;

extern Unk800F78A4 *D_801E7EC8;

void func_801E78F4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7938(s32 from, s32 to, s32 t, s32 n);
s32 func_801E7990(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
s32 func_801E7AAC(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
s32 func_801E7C28(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);

#endif
