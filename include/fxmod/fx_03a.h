#ifndef FX_03A_H
#define FX_03A_H

/* Effect module fx_03a (dir 13 file 0x3A). */
#include "btlfx_mod.h"

typedef struct {
    s32 bounce0; /* bounce countdown */
    s32 powerUp4; /* power-up countdown */
    s32 powerDown8; /* power-down countdown */
    s32 gridC; /* blur countdown */
    s32 unk10[4]; /* unused (runtime watch, round 33) */
    SVECTOR pos20;
    u8 pad28[8];
} Fx03aWork; /* 0x30 */

/* bouncing model */
typedef struct {
    s32 model0; /* model */
    VECTOR scale4; /* scale */
    VECTOR pos14; /* position */
    SVECTOR rot24; /* rotation */
    s32 speed2C; /* bounce speed */
    s32 time30; /* bounce time */
} Fx03aModel; /* 0x34 */

extern Unk800F78A4 *fx_03a_env;
extern Fx03aModel fx_03a_models[10];
extern s32 fx_03a_bouncePower; /* bounce power */
extern BtlModel fx_03a_effModels[];
extern u32 fx_03a_bgOt[];

#endif
