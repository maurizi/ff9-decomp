#ifndef FX_036_H
#define FX_036_H

/* Effect module fx_036 (dir 13 file 0x36). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx036Work; /* 0x2C */

extern s16 D_801E7BCC[];
extern s32 D_801E7BDC; /* particles spawned */
extern Unk800F78A4 *fx_036_env;
extern SVECTOR D_801E7C30; /* target position */

static s32 fx_036_op80SpawnPoint(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
