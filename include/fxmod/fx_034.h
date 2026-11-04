#ifndef FX_034_H
#define FX_034_H

/* Effect module fx_034 (dir 13 file 0x34). */
#include "btlfx_mod.h"

/* fx_034 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_COLD_BREATH_ICE_CRYSTAL   0x1 /* model: ice crystal shape with a cracked-ice texture */
#define FX_COLD_BREATH_FROST_BREATH  0x2 /* emitter: violet-white frost mist streams */
#define FX_COLD_BREATH_ICE_CHUNKS    0x3 /* emitter: grey ice lumps + teal glows */
#define FX_COLD_BREATH_ICE_CRYSTAL_2 0xA /* model: the same crystal shape, texture page 0x9A not in the file (look pending) */

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
extern SVECTOR *fx_034_primVerts[8]; /* split model vertices, per primitive kind */
extern u16 fx_034_primCounts[8]; /* primitive counts */
extern Fx034Point fx_034_primCentres[]; /* primitive centres */

void fx_034_explodeModel(s32 model, s32 scale, u8 *buf);
void fx_034_pushPrims(s32 kind, s32 count, s32 scale, SVECTOR *verts);
void fx_034_splitModel(s32 model, u8 *buf);
void fx_034_calcPrimCentres(s32 kind, s32 count, SVECTOR *verts);

#endif
