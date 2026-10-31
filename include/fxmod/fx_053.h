#ifndef FX_053_H
#define FX_053_H

/* Effect module fx_053 (dir 13 file 0x53). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx053Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    u8 pad8[4];
    SVECTOR unkC;
} Fx053Work81;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR unk10; /* target body position */
    s32 unk18; /* target index */
} Fx053Work82;

extern Unk800F78A4 *fx_053_env;
extern s32 D_801E7CBC[];

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
