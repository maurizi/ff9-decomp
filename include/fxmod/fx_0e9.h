#ifndef FX_0E9_H
#define FX_0E9_H

/* Effect module fx_0e9 (dir 13 file 0x0E9). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster position */
    SVECTOR unk10; /* caster rotation */
    SVECTOR unk18; /* target position */
} Fx0e9Work;

typedef struct {
    s16 unk0; /* start radius */
    s16 unk2; /* radius */
    s16 unk4; /* direction */
    s16 unk6; /* angle */
    s16 unk8; /* wobble phase */
    s16 unkA; /* countdown */
} Fx0e9Orbit;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*unk8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx0e9EffHdr;

extern Unk800F78A4 *D_801E7CFC;
extern Fx0e9Orbit D_801E7D0C;
extern SVECTOR D_801E7D1C;
extern SVECTOR D_801E7D24;
extern SVECTOR D_801E7D2C;
extern SVECTOR D_801E7D34;

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

void func_801E7C18(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7C5C(s32 from, s32 to, s32 t, s32 n);

#endif
