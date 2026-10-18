#ifndef FX_184_H
#define FX_184_H

/* Effect module fx_184 (dir 13 file 0x184). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* target body position */
    SVECTOR unk10; /* from */
    SVECTOR unk18; /* model rotation */
    VECTOR unk20; /* model position */
    VECTOR unk30; /* model scale */
    u16 unk40; /* model */
} Fx184Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx184Work81;

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    SVECTOR unk10; /* target position */
    SVECTOR unk18; /* destination */
    SVECTOR unk20; /* angles */
    SVECTOR unk28;
    s32 unk30; /* state */
    s32 unk34; /* steps */
    s32 unk38; /* step */
} Fx184Work82;

extern Unk800F78A4 *D_801E7DC4;
extern BtlModel D_801E7DD4[];

void func_801E7CE0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7D24(s32 from, s32 to, s32 t, s32 n);

/* propagated */
s32 func_801E7BC4(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);

#endif
