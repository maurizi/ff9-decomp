#ifndef FX_0FF_H
#define FX_0FF_H

/* Effect module fx_0ff (dir 13 file 0x0FF). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8[6]; /* model rotations */
    VECTOR unk38[6]; /* model positions */
    VECTOR unk98; /* model scale */
    s32 unkA8; /* model brightness */
    SVECTOR unkAC; /* target body position */
    SVECTOR unkB4; /* target position */
    s32 unkBC; /* fade-in countdown */
    s32 unkC0; /* fade-out countdown */
    u16 unkC4[3]; /* models */
} Fx0ffWork;

typedef struct {
    s16 unk0; /* radius */
    s16 unk2; /* angle */
    s16 unk4; /* spin */
} Fx0ffOrbit;

extern u16 D_801E7C68[3];
extern Unk800F78A4 *D_801E7CB8;
extern BtlModel D_801E7CC8[];
extern Fx0ffOrbit D_801E7F28[6];
void func_801E7BCC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E7C10(s32 from, s32 to, s32 t, s32 n);

#endif
