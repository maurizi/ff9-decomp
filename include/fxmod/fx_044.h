#ifndef FX_044_H
#define FX_044_H

/* Effect module fx_044 (dir 13 file 0x44). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR unk8; /* target body position */
    SVECTOR unk10;
    SVECTOR unk18; /* angles from unk10 to unk8 */
} Fx044Work80; /* 0x20 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC; /* target body position */
    SVECTOR unk14; /* target position */
    SVECTOR unk1C; /* above the target body */
} Fx044Work81; /* 0x24 */

extern Unk800F78A4 *fx_044_env;
extern s32 D_801E7A74[];

#endif
