#ifndef FX_09B_H
#define FX_09B_H

/* Effect module fx_09b (dir 13 file 0x9B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    SVECTOR pos3C; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos44; /* unk64 moved -0xC8 along the angles from unk3C */
    SVECTOR pos4C; /* unk64 on the ground */
    SVECTOR pos54; /* caster body position */
    SVECTOR unk5C; /* copy of unk3C */
    SVECTOR pos64; /* unk54 + random */
    s32 unk6C; /* instance (0..4) */
    u16 model70; /* model */
} Fx09bWork;

extern s32 D_801E80F4[5]; /* particle files */
extern s32 (*fx_09b_pinParticles[5])(); /* motion callbacks */
extern BtlModel fx_09b_effModels[];
extern s32 D_801E8564[5][0x10A];
extern SVECTOR D_801E9A34[5]; /* particle positions */
extern SVECTOR D_801E9A64[5];

s32 fxm_pinParticle0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);
extern Unk800F78A4 *fx_09b_env;
static void fxm_startUnitMotion(s32 unit, s32 motion);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

#endif
