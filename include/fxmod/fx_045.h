#ifndef FX_045_H
#define FX_045_H

/* Effect module fx_045 (dir 13 file 0x045). */
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
    SVECTOR pos28; /* bone 28 point */
    SVECTOR pos30; /* bone 54 point */
    SVECTOR pos38; /* in front of the caster */
    SVECTOR aim40; /* angles from pos28 to pos38 */
    SVECTOR aim48; /* angles from pos30 to pos38 */
    s32 tint50; /* screen tint */
} Fx045Work80; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 targetC; /* target index */
} Fx045Work81;

extern Unk800F78A4 *fx_045_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
