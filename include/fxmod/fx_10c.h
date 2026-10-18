#ifndef FX_10C_H
#define FX_10C_H

/* Effect module fx_10c (dir 13 file 0x10C). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    SVECTOR unk40[2]; /* rotation trail */
    VECTOR unk50[2]; /* position trail */
    u16 unk70; /* models */
    u16 unk72;
} Fx10cWork80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx10cWork81;

extern Unk800F78A4 *D_801E7B50;
extern BtlModel D_801E7B60[];

/* propagated */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* caster body position */
} Fx10cWork80_0bd;
s32 fx_10c_op81(s32 mode, Fx10cWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
