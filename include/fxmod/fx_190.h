#ifndef FX_190_H
#define FX_190_H

/* Effect module fx_190 (dir 13 file 0x190). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* count */
    SVECTOR unk4; /* angles */
    SVECTOR unkC; /* angles */
    SVECTOR unk14[4];
    SVECTOR unk34; /* caster position */
    SVECTOR unk3C; /* target position */
    SVECTOR unk44;
    SVECTOR unk4C;
    Pool6CEntry *unk54[4];
    Pool6CEntry *unk64[4];
} Fx190Work; /* 0x74 */

extern Unk800F78A4 *D_801E7AF8;

#endif
