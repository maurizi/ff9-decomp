#ifndef FX_115_H
#define FX_115_H

/* Effect module fx_115 (dir 13 file 0x115). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    SVECTOR unk40[4]; /* rotation trail */
    VECTOR unk60[4]; /* position trail */
    u16 unkA0; /* models */
    u16 unkA2;
} Fx115Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx115Work81;

extern Unk800F78A4 *D_801E7BB8;
extern BtlModel D_801E7BC8[];

/* propagated */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* caster body position */
} Fx115Work80_0bd;
s32 fx_115_op81(s32 mode, Fx115Work80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
