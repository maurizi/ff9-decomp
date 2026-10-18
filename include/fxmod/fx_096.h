#ifndef FX_096_H
#define FX_096_H

/* Effect module fx_096 (dir 13 file 0x96). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    s32 unk4;
    SVECTOR unk8; /* caster position */
    s32 unk10;
} Fx096Work80;

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* model A rotation */
    SVECTOR unkC; /* model B rotation */
    VECTOR unk14; /* model A position */
    VECTOR unk24; /* model B position */
    VECTOR unk34; /* model A scale */
    VECTOR unk44; /* model B scale */
    s32 unk54; /* brightness */
    SVECTOR unk58; /* caster position */
    s16 unk60; /* fade countdown */
    u16 unk62; /* models */
    u16 unk64;
} Fx096Work81;

typedef struct {
    s16 vx, vy, vz;
} Fx096Vec3;

extern Unk800F78A4 *D_801E831C;
extern BtlModel D_801E832C[];
extern u16 D_801E84BC[0xA28]; /* model A vertices */
extern u16 D_801E990C[0xA28]; /* their rest positions */
extern Fx096Vec3 D_801EAD5C[]; /* polygon centers */
extern s32 D_801EB28C[][8]; /* vertex pointers per polygon kind */
extern u16 D_801EB2CC[][8]; /* polygon counts */

void func_801E7744(u16 model, s32 idx, s32 t, u16 *verts, Fx096Vec3 *centers);
void func_801E7840(u32 kind, s32 count, s32 t, SVECTOR *verts, Fx096Vec3 *centers);
void func_801E7A74(u16 model, s32 idx, u16 *verts, Fx096Vec3 *centers);
void func_801E7BC4(u32 kind, s32 count, SVECTOR *verts, Fx096Vec3 *centers);
void func_801E8238(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E827C(s32 from, s32 to, s32 t, s32 n);

#endif
