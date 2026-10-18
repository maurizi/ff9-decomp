#ifndef FX_106_H
#define FX_106_H

/* Effect module fx_106 (dir 13 file 0x106). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster rotation */
    SVECTOR unk10; /* target body position */
    SVECTOR unk18;
    SVECTOR unk20;
} Fx106Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target position */
    SVECTOR unk14;
} Fx106Work81;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*unk8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx106EffHdr;

extern Unk800F78A4 *D_801E7C10;
extern s32 D_801E7C20[];
extern s32 D_801E8080[];
extern s32 D_801E86E0[];
extern SVECTOR D_801E9400; /* path start */
extern SVECTOR D_801E9408; /* path end */
extern SVECTOR D_801E9410;

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
