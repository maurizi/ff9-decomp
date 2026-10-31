#ifndef FX_12D_H
#define FX_12D_H

/* Effect module fx_12d (dir 13 file 0x12D). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx12dWork80;

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
} Fx12dWork81;

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model scale */
    s32 unk28;
    SVECTOR unk2C; /* caster position */
    u16 unk34; /* model */
} Fx12dWork82;

extern BtlTexInfo D_801E7CC0;
extern s16 D_801E7CD8[6][2]; /* D_801E7CC0's entries in reverse order */
extern BtlTexInfo D_801E7CF0;
extern s16 D_801E7D08[6][2]; /* D_801E7CF0's entries in reverse order */
extern Unk800F78A4 *fx_12d_env;
extern BtlModel fx_12d_effModels[];
extern SVECTOR D_801E7E40;
extern s32 D_801E7E48[];
extern s32 D_801E8388[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
