#ifndef FX_08C_H
#define FX_08C_H

/* Effect module fx_08c (dir 13 file 0x8C). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame for btlfx_drawBackdrop */
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR unk10; /* unk8 raised by 0x100 */
    s32 unk18[3]; /* model position (VECTOR without its pad word) */
    s32 model24; /* model */
} Fx08cWork;

extern Unk800F78A4 *fx_08c_env;
extern BtlModel fx_08c_effModels[];

#endif
