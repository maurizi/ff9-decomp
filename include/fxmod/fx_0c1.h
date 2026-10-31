#ifndef FX_0C1_H
#define FX_0C1_H

/* Effect module fx_0c1 (dir 13 file 0xC1). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 position */
    SVECTOR rot18; /* unit 0x10 rotation */
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28; /* unit 0x10 bone 0x17 position */
    SVECTOR pos30; /* unk10 moved 0x12C along unk18 */
    SVECTOR pos38; /* btlfx_getUnitBonePos(0x10, 0x17) */
    SVECTOR unk40; /* copy of unk48 */
    SVECTOR unk48; /* angles from unk38 to unk20 */
} Fx0c1Work;

extern Unk800F78A4 *fx_0c1_env;
extern SVECTOR D_801E7C20; /* particle path start */
extern SVECTOR D_801E7C28; /* particle path end */
extern SVECTOR D_801E7C30;

static s32 fx_0c1_op80ParticleMotion(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
