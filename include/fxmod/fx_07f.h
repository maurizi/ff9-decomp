#ifndef FX_07F_H
#define FX_07F_H

/* Effect module fx_07f (dir 13 file 0x7F). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame for btlfx_drawBackdrop */
    SVECTOR unk8; /* position: env->targets->centerX */
    SVECTOR unk10; /* unk8 raised by 0x180 */
    s32 unk18[3]; /* model position (VECTOR without its pad word) */
    SVECTOR unk24; /* model rotation */
    s32 unk2C; /* model */
    s32 unk30; /* model */
} Fx07fWork;

extern Unk800F78A4 *D_801E7B4C;
extern BtlModel D_801E7B5C[];

#endif
