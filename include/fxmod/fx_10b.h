#ifndef FX_10B_H
#define FX_10B_H

/* Effect module fx_10b (dir 13 file 0x10B). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster */
    SVECTOR unk8; /* target */
    SVECTOR unk10;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    SVECTOR unk40[2]; /* rotation trail */
    VECTOR unk50[2]; /* position trail */
    u16 model70; /* models */
    u16 model72;
} Fx10bWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR unk4;
} Fx10bWork81;

extern Unk800F78A4 *fx_10b_env;
extern BtlModel fx_10b_effModels[];

/* propagated */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
} Fx10bWork80_0bd;
s32 fx_10b_op81(s32 mode, Fx10bWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
