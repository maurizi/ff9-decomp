#ifndef FX_179_H
#define FX_179_H

/* Effect module fx_179 (dir 13 file 0x179). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0[3];
} Fx179Tri; /* model triangle */

typedef struct {
    s32 unk0; /* centre, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12; /* scale */
    s16 unk14; /* spin */
    s16 unk16;
    s16 unk18; /* frame, -1 = gone */
    s16 unk1A; /* first frame */
} Fx179Shard; /* 0x1C */

extern Unk800F78A4 *D_801E8A0C;
extern Fx179Tri D_801E8A1C[160];
extern Fx179Shard D_801E991C[160];

void func_801E7704(s32 model, SVECTOR *center);
void func_801E78E8(void);

#endif
