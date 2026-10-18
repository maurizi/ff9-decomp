#ifndef FX_17E_H
#define FX_17E_H

/* Effect module fx_17e (dir 13 file 0x17e). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* brightness */
    VECTOR unk8; /* model scale */
    VECTOR unk18; /* model position */
    SVECTOR unk28; /* model rotation */
    SVECTOR unk30;
    SVECTOR unk38; /* target body position */
    s32 unk40[4]; /* countdowns */
    s32 unk50;
} Fx17eWork;

extern Unk800F78A4 *D_801E7AA8;
extern BtlModel D_801E7AB8[];

#endif
