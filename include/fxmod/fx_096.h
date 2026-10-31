#ifndef FX_096_H
#define FX_096_H

/* Effect module fx_096 (dir 13 file 0x96): Frost (party; enemy Freeze of Agares/Gargoyle and others). A model
 * whose polygons fly apart from their centres (fx_096_explodeModel) and a second growing model. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4;
    SVECTOR pos8; /* caster position */
    s32 unk10;
} Fx096Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model A rotation */
    SVECTOR rotC; /* model B rotation */
    VECTOR pos14; /* model A position */
    VECTOR pos24; /* model B position */
    VECTOR scale34; /* model A scale */
    VECTOR scale44; /* model B scale */
    s32 bright54; /* brightness */
    SVECTOR pos58; /* caster position */
    s16 fade60; /* fade countdown */
    u16 model62; /* models */
    u16 model64;
} Fx096Work81;

typedef struct {
    s16 vx, vy, vz;
} Fx096Vec3;

extern Unk800F78A4 *fx_096_env;
extern BtlModel fx_096_effModels[];
extern u16 fx_096_verts[0xA28]; /* model A vertices */
extern u16 fx_096_restVerts[0xA28]; /* their rest positions */
extern Fx096Vec3 fx_096_polyCenters[]; /* polygon centers */
extern s32 fx_096_polyVerts[][8]; /* vertex pointers per polygon kind */
extern u16 fx_096_polyCounts[][8]; /* polygon counts */

void fx_096_explodeModel(u16 model, s32 idx, s32 t, u16 *verts, Fx096Vec3 *centers);
void fx_096_explodePolys(u32 kind, s32 count, s32 t, SVECTOR *verts, Fx096Vec3 *centers);
void fx_096_splitModel(u16 model, s32 idx, u16 *verts, Fx096Vec3 *centers);
void fx_096_calcPolyCenters(u32 kind, s32 count, SVECTOR *verts, Fx096Vec3 *centers);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
