#ifndef FX_01C_H
#define FX_01C_H

/* Effect module fx_01c (dir 13 file 0x1C). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* target (body) position */
    SVECTOR aim24; /* angles from pos14 to pos1C */
} Fx01cWork; /* 0x2C */

extern Unk800F78A4 *fx_01c_env;
extern s32 fx_01c_trails[];

#endif
