#ifndef FX_0BF_H
#define FX_0BF_H

/* Effect module fx_0bf (dir 13 file 0xBF). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* caster body position */
    s32 unkC; /* screen tint */
} Fx0bfWork80;

typedef struct {
    SVECTOR unk0; /* unit 0x10 position */
} Fx0bfWork81;

extern Unk800F78A4 *fx_0bf_env;

#endif
