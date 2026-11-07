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
#define FX_BLOWUP_SPARK_DOTS      0xB /* emitter: faint spark dots (btlfx_emitDots), at the targets */
#define FX_BLOWUP_SPARK_DOTS_2    0xC /* emitter: faint spark dots (btlfx_emitDots), at the targets */
#define FX_BLOWUP_SMOKE           0xD /* emitter: grey smoke puffs */

/* fx_036 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length; op81's fade (the work is the level). */
#define FX_BLOWUP_RED_RING_FRAME  1    /* op80: FX_BLOWUP_RED_RING starts at the caster's bone 1 */
#define FX_BLOWUP_EXPLOSION_FRAME 5    /* op80: FX_BLOWUP_EXPLOSION, BIG_EXPLOSION and SMOKE start there */
#define FX_BLOWUP_SPREAD_FRAME    9    /* op80: FX_BLOWUP_EXPLOSION_SMOKE (spawn points round the targets) and the spark dots start */
#define FX_BLOWUP_FRAMES          0x28 /* op80: the task ends at this frame */
#define FX_BLOWUP_FADE_LEVEL      0x80 /* op81: the caster's fade level starts here (btlfx_fadeUnitById), */
#define FX_BLOWUP_FADE_STEP       8    /* drops this much per frame; the op ends below 0 */

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
