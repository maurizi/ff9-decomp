#ifndef FX_0EA_H
#define FX_0EA_H

/* Effect module fx_0ea (dir 13 file 0x0EA). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster position */
    SVECTOR unk10;
    u16 unk18;
    u16 unk1A;
} Fx0eaWork80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* target body position */
    SVECTOR unk18; /* target position */
    s32 unk20; /* target index */
} Fx0eaWork81;

typedef s32 (*Fx0eaPathFn)(VECTOR *, s32, SVECTOR *, s32, s32, s32);

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    Fx0eaPathFn unk8;
} Fx0eaEffHdr;

extern u16 D_801E7DF4[4];
extern Fx0eaPathFn D_801E7DFC[4];
extern Unk800F78A4 *D_801E7E54;
extern SVECTOR D_801E7E5C; /* path start */
extern SVECTOR D_801E7E64[4]; /* path ends */
extern SVECTOR D_801E7E84;

s32 func_801E77D4(Pool30Entry *pt, SVECTOR *pos, SVECTOR *to, s32 frame, s32 n);
void func_801E7DE0(Unk800F78A4 *env, s32 i, s32 value);

/* propagated */
s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E776C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77A0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

#endif
