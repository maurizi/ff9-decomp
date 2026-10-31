#ifndef FX_04B_H
#define FX_04B_H

/* Effect module fx_04b (dir 13 file 0x4B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR aim20; /* angles from pos10 to pos8 */
} Fx04bWork80; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* above the target */
    SVECTOR aimC; /* angles from fx_04b_casterPos to pos4 */
    s32 target14; /* target index */
} Fx04bWork81; /* 0x18 */

extern Unk800F78A4 *fx_04b_env;
extern SVECTOR fx_04b_casterPos;
extern s32 fx_04b_trails[];
extern s32 fx_04b_trails2[];

#endif
