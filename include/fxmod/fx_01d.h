#ifndef FX_01D_H
#define FX_01D_H

/* Effect module fx_01d (dir 13 file 0x1D). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR unk14; /* caster position */
    SVECTOR rot1C; /* caster rotation */
    SVECTOR pos24; /* bone 7 point */
    SVECTOR pos2C; /* bone 15 point */
    SVECTOR pos34; /* in front of the caster */
    SVECTOR unk3C; /* angles from unk24 to unk34 */
    SVECTOR unk44; /* angles from unk2C to unk34 */
    s32 unk4C; /* screen tint */
} Fx01dWork80; /* 0x50 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 unkC; /* target index */
} Fx01dWork81;

extern Unk800F78A4 *fx_01d_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
