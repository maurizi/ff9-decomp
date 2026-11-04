#ifndef FX_0CC_H
#define FX_0CC_H

/* Effect module fx_0cc (dir 13 file 0xCC): enemy Meteorite (Thorn/Zorn). Falling models (pool items) that burst on
 * the ground. Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0cc effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_METEORITE_IMPACT_RING   0x2 /* rings: white-pink gouraud ring at the impact */
#define FX_METEORITE_IMPACT_SMOKE  0x3 /* particles: pink-white smoke puff at the impact */
#define FX_METEORITE_RED_GLOW      0x7 /* particles: red glow around the falling rock */
#define FX_METEORITE_TRAIL_EMITTER 0x9 /* emitter: sprites following the falling rock; the render is still black (texels blank in the file's upload) (look pending) */
#define FX_METEORITE_IMPACT_FLASH  0xC /* particles: white radial flash at the impact */
#define FX_METEORITE_ROCK          0xF /* 'so' model: dark brown rock (polyhedron), drawn spinning down */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* position */
    Pool6CEntry *emitC;
    SVECTOR pos10; /* start position */
} Fx0ccPart; /* pool entry data */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* position: env->targets->centerX, 0, centerZ */
    s32 spawnCountC; /* particles spawned */
    s32 model10; /* model */
} Fx0ccWork;

extern Unk800F78A4 *fx_0cc_env;
extern BtlModelBuf fx_0cc_effModels[];

static s32 fx_0cc_op80PoolTask(s32 mode, Fx0ccPart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0ccWork *work);

#endif
