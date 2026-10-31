#ifndef FX_127_H
#define FX_127_H

/* Effect module fx_127 (dir 13 file 0x127). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    u8 pad1C[0x24 - 0x1C];
    SVECTOR pos24; /* caster position */
    SVECTOR rot2C; /* caster rotation */
    SVECTOR pos34; /* target body position */
    SVECTOR pos3C; /* target position */
    SVECTOR unk44; /* caster bone position */
    SVECTOR pos4C;
    SVECTOR unk54;
    SVECTOR unk5C;
    SVECTOR unk64;
    SVECTOR unk6C;
    SVECTOR unk74;
    SVECTOR unk7C;
} Fx127Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* target body position */
    SVECTOR unk18;
    SVECTOR unk20;
    SVECTOR unk28;
    SVECTOR pos30;
    SVECTOR pos38;
} Fx127Work81;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*unk8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx127EffHdr;

extern SVECTOR D_801E8310;
extern SVECTOR D_801E8318;
extern SVECTOR D_801E8320;
extern SVECTOR D_801E8328;
extern SVECTOR D_801E8330;
extern SVECTOR D_801E8338;
extern s32 D_801E8350[];
extern s32 D_801E9D00[];
extern Unk800F78A4 *fx_127_env;
extern SVECTOR D_801E82F8; /* path start */
extern SVECTOR D_801E8300; /* path end */
extern SVECTOR D_801E8308;
extern Pool6CEntry *fx_127_emitter;

static s32 fx_127_op80ParticleMotion0(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_127_op80ParticleMotion1(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7818(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n);

#endif
