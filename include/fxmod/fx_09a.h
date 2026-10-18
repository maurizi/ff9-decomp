#ifndef FX_09A_H
#define FX_09A_H

/* Effect module fx_09a (dir 13 file 0x9A). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx09aSeg; /* 0x10 */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx09aSeg *buf, s32 abr);
extern Unk800F78A4 *D_801E8858;
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

#endif
