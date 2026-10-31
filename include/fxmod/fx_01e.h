#ifndef FX_01E_H
#define FX_01E_H

/* Effect module fx_01e (dir 13 file 0x1E). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target body position */
    SVECTOR unk18; /* angles from caster to target */
} Fx01eWork;

extern Unk800F78A4 *fx_01e_env;
extern s32 D_801E792C[];

#endif
