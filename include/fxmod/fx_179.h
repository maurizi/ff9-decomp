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

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* orb position */
    SVECTOR unk10; /* caster position */
    s32 unk18; /* model */
} Fx179Work;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx179Rgbc; /* primitive colour word */

extern Unk800F78A4 *D_801E8A0C;
extern BtlModel D_801EAA9C[];
extern u8 D_801EAB6C[];
extern Fx179Tri D_801E8A1C[160];
extern Fx179Shard D_801E991C[160];

void func_801E7704(s32 model, SVECTOR *center);
void func_801E78E8(void);
void func_801E7B68(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);
s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
