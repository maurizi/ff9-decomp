#ifndef FX_186_H
#define FX_186_H

/* Effect module fx_186 (dir 13 file 0x186). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s16 pad4;
    s16 unk6; /* height */
    s16 unk8; /* velocity */
    s16 unkA;
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    s32 unk20; /* model */
} Fx186Rock;

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4[8]; /* home positions */
    SVECTOR unk44[8]; /* rotations */
    SVECTOR unk84[8]; /* spin */
    SVECTOR unkC4[8]; /* current rotations */
    s32 unk104; /* unit count */
    s32 unk108; /* bounce height */
} Fx186Work81; /* 0x10C */

extern Unk800F78A4 *D_801E9214;

typedef struct {
    s32 unk0;
    s32 unk4;
} Fx186Word2; /* an SVECTOR copied as two words */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10;
    Pool6CEntry *unk18;
    s32 unk1C[10]; /* rock models */
    s32 unk44; /* model */
    s32 unk48; /* model */
} Fx186Work80; /* 0x4C */

extern s16 D_801E9B84[]; /* StoreImage buffer */
extern u16 D_801E8DA4[10]; /* rock model files */
extern u16 D_801E8DB8[10]; /* rock model files (alternate) */
extern u32 D_801E8E14[];
extern BtlModel D_801E9224[];
s32 func_801E7704(s32 mode, Fx186Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame);

#endif
