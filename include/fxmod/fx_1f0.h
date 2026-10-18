#ifndef FX_1F0_H
#define FX_1F0_H

/* Effect module fx_1f0 (dir 13 file 0x1f0). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* start delay */
    s32 unk4; /* flight countdown */
    SVECTOR unk8; /* start position */
    SVECTOR unk10; /* home position */
    SVECTOR unk18;
    SVECTOR unk20; /* current position */
    Pool6CEntry *unk28;
} Fx1f0Unit; /* 0x2C */

typedef struct {
    s32 unk0[16]; /* timers */
    SVECTOR unk40[4]; /* curve control points */
    s32 unk60; /* models */
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    VECTOR unk70; /* model position */
    VECTOR unk80; /* model scale */
    VECTOR unk90;
    SVECTOR unkA0; /* model rotation */
    SVECTOR unkA8;
    s32 unkB0;
    s32 unkB4;
} Fx1f0Work; /* 0xB8 */

extern s32 D_801E84F0[];
extern Unk800F78A4 *D_801E8548;
extern Fx1f0Unit D_801E8558[4];
extern BtlModel D_801E8608[];

void func_801E7704(s32 mode, s32 n, s32 t, SVECTOR *pts, SVECTOR *out);
s32 func_801E7948(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
