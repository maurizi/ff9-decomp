#ifndef FX_08C_H
#define FX_08C_H

/* Effect module fx_08c (dir 13 file 0x8C). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame for func_800D3030 */
    SVECTOR unk8; /* position: env->unkC->unk38..3C */
    SVECTOR unk10; /* unk8 raised by 0x100 */
    s32 unk18[3]; /* model position (VECTOR without its pad word) */
    s32 unk24; /* model */
} Fx08cWork;

extern Unk800F78A4 *D_801E7AA8;
extern BtlModel D_801E7AB8[];

#endif
