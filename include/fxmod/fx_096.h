#ifndef FX_096_H
#define FX_096_H

/* Effect module fx_096 (dir 13 file 0x96): Frost (party; enemy Freeze of Agares/Gargoyle and others). A model
 * whose polygons fly apart from their centres (fx_096_explodeModel) and a second growing model. Evidence: [data]
 * the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_096 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FROST_ICE_GLINTS  0x1 /* emitter: blue particles + sprites; the frames read blue dithered blocks that look like unrelated VRAM (look pending) */
#define FX_FROST_SNOW_BURST  0x6 /* emitter: ring of ice-flake sprites + white-cyan rings */
#define FX_FROST_ICE_CRYSTAL 0xA /* 'so' model: faceted ice crystal (pale blue cracked-ice texture) */
#define FX_FROST_ICE_SHARD   0xB /* 'so' model: flat jagged ice shard (same cracked-ice texture) */
#define FX_FROST_BACKDROP    0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    SVECTOR pos8; /* caster position */
    s32 unk10; /* unused */
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
extern BtlModelBuf fx_096_effModels[];
extern u16 fx_096_verts[0xA28]; /* model A vertices */
extern u16 fx_096_restVerts[0xA28]; /* their rest positions */
extern Fx096Vec3 fx_096_polyCenters[]; /* polygon centers */
extern s32 fx_096_polyVerts[][8]; /* vertex pointers per polygon kind */
extern u16 fx_096_polyCounts[][8]; /* polygon counts */

void fx_096_explodeModel(u16 model, s32 idx, s32 t, u16 *verts, Fx096Vec3 *centers);
void fx_096_explodePolys(u32 kind, s32 count, s32 t, SVECTOR *verts, Fx096Vec3 *centers);
void fx_096_splitModel(u16 model, s32 idx, u16 *verts, Fx096Vec3 *centers);
void fx_096_calcPolyCenters(u32 kind, s32 count, SVECTOR *verts, Fx096Vec3 *centers);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
