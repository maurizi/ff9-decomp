#ifndef FX_0A8_H
#define FX_0A8_H

/* Effect module fx_0a8 (dir 13 file 0xA8). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 body position */
    SVECTOR rot10; /* unit 0x10 rotation */
} Fx0a8Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    s32 unk14;
    Pool6CEntry *emit18;
    s32 unk1C;
    SVECTOR pos20; /* position: env->targets->centerX..Z */
} Fx0a8Work81;

extern Unk800F78A4 *fx_0a8_env;
extern s32 D_801E7B78[];

#endif
