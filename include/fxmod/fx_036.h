#ifndef FX_036_H
#define FX_036_H

/* Effect module fx_036 (dir 13 file 0x36). */
#include "btlfx_mod.h"

/* fx_036 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLOWUP_EXPLOSION       0x1 /* emitter: fire explosion + red gouraud rings */
#define FX_BLOWUP_EXPLOSION_SMOKE 0x2 /* emitter: fire explosion + grey smoke puffs */
#define FX_BLOWUP_BIG_EXPLOSION   0x3 /* emitter: three fire explosions */
#define FX_BLOWUP_RED_RING        0x4 /* emitter: fan + white-red gouraud ring */
#define FX_BLOWUP_SPARKS          0xB /* emitter: spark sprites (texture not in the file), at the targets (look pending) */
#define FX_BLOWUP_SPARKS_2        0xC /* emitter: spark sprites (texture not in the file), at the targets (look pending) */
#define FX_BLOWUP_SMOKE           0xD /* emitter: grey smoke puffs */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR pos1C;
    SVECTOR pos24;
} Fx036Work; /* 0x2C */

extern s16 fx_036_spawnX[];
extern s32 fx_036_spawnCount; /* particles spawned */
extern Unk800F78A4 *fx_036_env;
extern SVECTOR fx_036_targetPos; /* target position */

static s32 fx_036_op80SpawnPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n);

#endif
