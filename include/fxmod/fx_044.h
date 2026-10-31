#ifndef FX_044_H
#define FX_044_H

/* Effect module fx_044 (dir 13 file 0x44). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10;
    SVECTOR aim18; /* angles from pos10 to pos8 */
} Fx044Work80; /* 0x20 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
    SVECTOR pos14; /* target position */
    SVECTOR pos1C; /* above the target body */
} Fx044Work81; /* 0x24 */

extern Unk800F78A4 *fx_044_env;
extern s32 fx_044_trails[];

#endif
