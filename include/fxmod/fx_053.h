#ifndef FX_053_H
#define FX_053_H

/* Effect module fx_053 (dir 13 file 0x53). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx053Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    u8 pad8[4];
    SVECTOR posC;
} Fx053Work81;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    s32 target18; /* target index */
} Fx053Work82;

extern Unk800F78A4 *fx_053_env;
extern s32 fx_053_trails[];

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
