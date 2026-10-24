#ifndef FX_0A0_H
#define FX_0A0_H

/* Effect module fx_0a0 (dir 13 file 0xA0). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[2];
} Fx0a0Line;

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unkC;
    SVECTOR unk14; /* caster body position */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx0a0Work;

extern Fx0a0Line D_801E7D84;
extern Unk800F78A4 *D_801E7D98;
extern s32 D_801E7DA4[];
extern SVECTOR D_801E8864;
extern Unk80100C78Entry *D_801E886C;
extern s32 D_801E8870;

#define gte_stsxy01(r0, r1) __asm__ volatile( \
    "swc2\t$12, 0(%0);" \
    "swc2\t$13, 0(%1)" \
    : : "r"(r0), "r"(r1) : "memory")

void func_801E7704(SVECTOR *a, SVECTOR *b, CVECTOR *c0, CVECTOR *c1, s32 p, s32 abr);
void func_801E783C(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_801E78F8(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame);

#endif
