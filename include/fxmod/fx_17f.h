#ifndef FX_17F_H
#define FX_17F_H

/* Effect module fx_17f (dir 13 file 0x17f): enemy Melt (Zombie). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_17f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MELT_PINK_SMOKE 0x1 /* emitter: pink smoke puffs */
#define FX_MELT_GOO_BLOB   0x2 /* emitter: purple goo blobs */
#define FX_MELT_GOO_BLOB_2 0x3 /* emitter: small dark purple goo blobs */
#define FX_MELT_GOO_RIPPLE 0x4 /* emitter: pink ripple disc */
#define FX_MELT_GOO_SPLASH 0xB /* emitter: glossy purple goo splash */

/* fx_17f choreography (docs/fxdata.md, "Choreography literals"): the melting copy of the caster (the partner unit
 * with swapped model data), op80's keyframes and length. */
#define FX_MELT_VERTICES      0x2BC /* fx_17f_vertexDrops entries: each vertex sinks 4..11 a frame */
#define FX_MELT_SPREAD_HEIGHT 0x1E  /* fx_17f_op80MeltVertex: vertices below this height spread out (x 92 / 90) */
#define FX_MELT_SPLASH_FRAME  3     /* GOO_SPLASH between the caster and the target */
#define FX_MELT_BLOB_FRAME    6     /* GOO_BLOB and PINK_SMOKE there */
#define FX_MELT_RIPPLE_FRAME  0x1C  /* GOO_BLOB_2 and GOO_RIPPLE at the caster */
#define FX_MELT_FADE_FRAME    0x37  /* the copy fades from 0x80, 8 a frame, down to 0x10 */
#define FX_MELT_FRAMES        0x47  /* op80's length: the model data is swapped back, the copy parked at z HIDE_Z */
#define FX_MELT_HIDE_Z        0x3A98

typedef struct {
    s32 scale0[3]; /* target scale */
    SVECTOR posC; /* target position */
    SVECTOR rot14; /* target rotation */
    BtlModelHeader *model1C;
    SVECTOR pos20; /* caster position */
} Fx17fWork;

extern Unk800F78A4 *fx_17f_env;
extern s32 fx_17f_vertexDrops[FX_MELT_VERTICES];
extern s32 fx_17f_modelBackup[];

void fx_17f_op80MeltVertex(SVECTOR *v, s32 i);

#endif
