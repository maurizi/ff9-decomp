#ifndef FX_1BE_H
#define FX_1BE_H

/* Effect module fx_1be (dir 13 file 0x1be). */
#include "btlfx_mod.h"

typedef struct {
    u8 u;
    u8 v;
} Fx1beUV;

extern SVECTOR D_801E8F38[9];
extern SVECTOR D_801E8FE8[64];

s32 func_801E7704(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out);

/* copied */
extern Unk800F78A4 *D_801E8FD8;
/* end copied */

typedef struct {
    u8 unk0[0x20]; /* counters */
    Pool6CEntry *unk20[16];
    s32 unk60; /* model */
    VECTOR unk64; /* position */
    VECTOR unk74; /* scale */
    SVECTOR unk84; /* rotation */
    SVECTOR unk8C;
    s32 unk94; /* models */
    s32 unk98;
    SVECTOR unk9C;
    s32 unkA4;
    s32 unkA8;
    VECTOR unkAC;
    SVECTOR unkBC;
    SVECTOR unkC4;
    SVECTOR unkCC;
    SVECTOR unkD4;
} Fx1beWork80; /* 0xDC */

typedef struct {
    SVECTOR unk0; /* rotation */
    VECTOR unk8; /* scale */
    SVECTOR unk18; /* home position */
    SVECTOR unk20; /* rotation */
    SVECTOR unk28; /* spin */
    s32 unk30;
    s32 unk34; /* delay */
} Fx1beUnit; /* 0x38 */

extern BtlModel D_801E91E8[];
extern s32 D_801E95D0;
extern s32 D_801E95D4;
extern s32 D_801E95D8;
extern Fx1beUnit D_801E95E8[];
s32 func_801E7840(s32 curve, s32 n, s32 t, s32 from, s32 to);
void func_801E89A8(SVECTOR *pos, SVECTOR *rot, s32 step, s32 tpage, s32 clut, s32 abr);
s32 func_801E8E10(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
