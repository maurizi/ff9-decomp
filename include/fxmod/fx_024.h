#ifndef FX_024_H
#define FX_024_H

/* Effect module fx_024 (dir 13 file 0x024). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* caster position */
    SVECTOR rot20; /* caster rotation */
    SVECTOR pos28; /* bone 7 point */
    SVECTOR pos30; /* bone 15 point */
    SVECTOR pos38; /* in front of the caster */
    SVECTOR unk40; /* angles from unk24 to unk34 */
    SVECTOR unk48; /* angles from unk2C to unk34 */
    s32 unk50; /* screen tint */
} Fx024Work80; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 unkC; /* target index */
} Fx024Work81;

extern Unk800F78A4 *fx_024_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

#endif
