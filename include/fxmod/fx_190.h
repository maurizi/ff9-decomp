#ifndef FX_190_H
#define FX_190_H

/* Effect module fx_190 (dir 13 file 0x190). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* count */
    SVECTOR unk4; /* angles */
    SVECTOR unkC; /* angles */
    SVECTOR unk14[4];
    SVECTOR pos34; /* caster position */
    SVECTOR pos3C; /* target position */
    SVECTOR pos44;
    SVECTOR pos4C;
    Pool6CEntry *emit54[4];
    Pool6CEntry *emit64[4];
} Fx190Work; /* 0x74 */

extern Unk800F78A4 *fx_190_env;

#endif
