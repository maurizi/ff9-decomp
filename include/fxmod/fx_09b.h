#ifndef FX_09B_H
#define FX_09B_H

/* Effect module fx_09b (dir 13 file 0x9B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* model rotation */
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    SVECTOR unk3C; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR unk44; /* unk64 moved -0xC8 along the angles from unk3C */
    SVECTOR unk4C; /* unk64 on the ground */
    SVECTOR unk54; /* caster body position */
    SVECTOR unk5C; /* copy of unk3C */
    SVECTOR unk64; /* unk54 + random */
    s32 unk6C; /* instance (0..4) */
    u16 unk70; /* model */
} Fx09bWork;

extern s32 D_801E80F4[5]; /* particle files */
extern s32 (*D_801E8108[5])(); /* motion callbacks */
extern BtlModel D_801E8174[];
extern s32 D_801E8564[5][0x10A];
extern SVECTOR D_801E9A34[5]; /* particle positions */
extern SVECTOR D_801E9A64[5];

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E776C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77A0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77D4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7808(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);
s32 func_801E7FD8(s32 unit);
void func_801E805C(s32 unit);
extern Unk800F78A4 *D_801E8164;
void func_801E7F6C(s32 unit, s32 motion);
void func_801E80A8(s32 unit);

#endif
