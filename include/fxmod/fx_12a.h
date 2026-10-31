#ifndef FX_12A_H
#define FX_12A_H

/* Effect module fx_12a (dir 13 file 0x12A). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* bone positions */
    SVECTOR pos28;
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR unk40; /* aim targets */
    SVECTOR unk48;
    SVECTOR unk50; /* aim angles */
    SVECTOR unk58;
} Fx12aWork80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx12aWork81;

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    SVECTOR pos8;
    s32 unk10; /* target index */
} Fx12aWork82;

extern Unk800F78A4 *fx_12a_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 value);

#endif
