#ifndef FX_023_H
#define FX_023_H

/* Effect module fx_023 (dir 13 file 0x23). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* target position */
    SVECTOR pos1C; /* target home position */
    SVECTOR pos24;
    SVECTOR unk2C;
    s16 unk34; /* rise countdown */
    s16 unk36; /* fall countdown */
    s16 unk38; /* held up */
} Fx023Work; /* 0x3C */

extern Unk800F78A4 *fx_023_env;
extern s32 D_801E7C40[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
