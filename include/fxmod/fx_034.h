#ifndef FX_034_H
#define FX_034_H

/* Effect module fx_034 (dir 13 file 0x34). */
#include "btlfx_mod.h"

/* fx_034 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COLD_BREATH_ICE_CRYSTAL   0x1 /* model: ice crystal shape with a cracked-ice texture */
#define FX_COLD_BREATH_FROST_BREATH  0x2 /* emitter: violet-white frost mist streams */
#define FX_COLD_BREATH_ICE_CHUNKS    0x3 /* emitter: grey ice lumps + teal glows */
#define FX_COLD_BREATH_ICE_CRYSTAL_2 0xA /* model: the same crystal shape (texture page 0x9A not in the file), one of the ice crystal shards on the target; runtime (docs/runtime-findings.md effect-data pass 2) */

/* fx_034 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and the crystal's phases, the models' textures and
 * the gleam scroll; the model-split helpers' primitive kinds. */
#define FX_COLD_BREATH_FROST_FRAME     15 /* op80: FX_COLD_BREATH_FROST_BREATH starts behind the target, the crystal appears */
#define FX_COLD_BREATH_GROW_FRAMES     4  /* the crystal grows (scale 0 -> 0x800) over this many frames */
#define FX_COLD_BREATH_CRYSTAL_FRAMES  30 /* the crystal is drawn for this many frames */
#define FX_COLD_BREATH_GLEAM_FRAME     24 /* op80: from here until SHATTER_FRAME the reflection map scrolls */
#define FX_COLD_BREATH_SHATTER_FRAME   28 /* op80: the crystal model is split, from here it explodes and fades (ICE_CRYSTAL_2 is no longer drawn) */
#define FX_COLD_BREATH_FRAMES          60 /* op80: the task ends at this frame */
#define FX_COLD_BREATH_CRYSTAL_DIST    0x320 /* the crystal stands this far behind the target (seen from the caster) */
#define FX_COLD_BREATH_MODEL_COUNT     2  /* btlfx_initEffModels slots: ICE_CRYSTAL, ICE_CRYSTAL_2 */
#define FX_COLD_BREATH_PRIM_KINDS BTL_MESH_PRIM_COUNT  /* Hi_GetMdlVertexPtr primitive kinds: even = quads (4 vertices), odd = triangles */
#define FX_COLD_BREATH_ICE_CHUNKS_TRAIL_SLOTS  0x28 /* FX_COLD_BREATH_ICE_CHUNKS' trail buffer fx_034_trails: slots of */
#define FX_COLD_BREATH_ICE_CHUNKS_TRAIL_STRIDE 0x30 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* FX_COLD_BREATH_ICE_CRYSTAL: 8-bit page (0x240, 0x100), clut (0, 0xF6). */
#define FX_COLD_BREATH_ICE_CRYSTAL_TPAGE 0x99
#define FX_COLD_BREATH_ICE_CRYSTAL_CLUT  0x3D80
/* FX_COLD_BREATH_ICE_CRYSTAL_2: 8-bit page (0x280, 0x100), clut (0, 0xF7), v 0x80 = VRAM (0x280, 0x180), the rect
 * btlfx_drawScrollMove fills from (SRC_X, SRC_Y) in the gleam frames. */
#define FX_COLD_BREATH_ICE_CRYSTAL_2_TPAGE 0x9A
#define FX_COLD_BREATH_ICE_CRYSTAL_2_CLUT  0x3DC0
#define FX_COLD_BREATH_ICE_CRYSTAL_2_V     0x80
#define FX_COLD_BREATH_GLEAM_VRAM_X    0x280
#define FX_COLD_BREATH_GLEAM_VRAM_Y    0x180
#define FX_COLD_BREATH_GLEAM_VRAM_W    0x40
#define FX_COLD_BREATH_GLEAM_VRAM_H    0x80
#define FX_COLD_BREATH_GLEAM_SRC_X     0x280
#define FX_COLD_BREATH_GLEAM_SRC_Y     0x100

typedef struct {
    s16 x, y, z;
} Fx034Point;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 rgb30; /* colour */
    SVECTOR pos34;
    SVECTOR rot3C;
    SVECTOR pos44;
    SVECTOR pos4C;
    SVECTOR aim54;
    SVECTOR aim5C;
    SVECTOR pos64;
    s16 scroll6C;
    u16 model6E; /* models */
    u16 model70;
} Fx034Work; /* 0x74 */

extern BtlModelBuf fx_034_effModels[];
extern u8 fx_034_splitBuf[];
extern u8 fx_034_trails[];
extern Unk800F78A4 *fx_034_env;
extern SVECTOR *fx_034_primVerts[FX_COLD_BREATH_PRIM_KINDS]; /* split model vertices, per primitive kind */
extern u16 fx_034_primCounts[FX_COLD_BREATH_PRIM_KINDS]; /* primitive counts */
extern Fx034Point fx_034_primCentres[]; /* primitive centres */

void fx_034_explodeModel(s32 model, s32 scale, u8 *buf);
void fx_034_pushPrims(s32 kind, s32 count, s32 scale, SVECTOR *verts);
void fx_034_splitModel(s32 model, u8 *buf);
void fx_034_calcPrimCentres(s32 kind, s32 count, SVECTOR *verts);

#endif
