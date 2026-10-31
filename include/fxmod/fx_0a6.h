#ifndef FX_0A6_H
#define FX_0A6_H

/* Effect module fx_0a6 (dir 13 file 0xA6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR unk14; /* position: env->targets->centerX..Z, raised by 0xC8 */
    SVECTOR unk1C; /* position: env->targets->centerX..Z */
} Fx0a6Work;

extern Unk800F78A4 *fx_0a6_env;
extern s32 D_801E7984[];

#endif
