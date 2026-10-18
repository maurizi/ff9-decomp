#ifndef FX_0CC_H
#define FX_0CC_H

/* Effect module fx_0cc (dir 13 file 0xCC). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* position */
    Pool6CEntry *unkC;
    SVECTOR unk10; /* start position */
} Fx0ccPart; /* pool entry data */

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* position: env->unkC->unk38, 0, unk3C */
    s32 unkC; /* particles spawned */
    s32 unk10; /* model */
} Fx0ccWork;

extern Unk800F78A4 *D_801E7C9C;
extern BtlModel D_801E7CAC[];

s32 func_801E7704(s32 mode, Fx0ccPart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0ccWork *work);

#endif
