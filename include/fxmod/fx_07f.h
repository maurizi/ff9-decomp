#ifndef FX_07F_H
#define FX_07F_H

/* Effect module fx_07f (dir 13 file 0x7F). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame for btlfx_drawBackdrop */
    SVECTOR pos8; /* position: env->targets->centerX */
    SVECTOR pos10; /* unk8 raised by 0x180 */
    s32 unk18[3]; /* model position (VECTOR without its pad word) */
    SVECTOR rot24; /* model rotation */
    s32 model2C; /* model */
    s32 model30; /* model */
} Fx07fWork;

extern Unk800F78A4 *fx_07f_env;
extern BtlModel fx_07f_effModels[];

#endif
