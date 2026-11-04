#ifndef FX_0A9_H
#define FX_0A9_H

/* Effect module fx_0a9 (dir 13 file 0xA9): enemy Lightning (Chimera and others). Jagged bolts (fxm_spawnBolt)
 * whose points are pool particles. Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIGHTNING_BOLT_PILLAR   0x1 /* trail: violet lightning pillar strip */
#define FX_LIGHTNING_ELECTRIC_ARCS 0x3 /* emitter: blue / green electric arcs + embers */
#define FX_LIGHTNING_TRAIL_2       0x4 /* trail: one frame, still not rendered by the tool (look pending) */
#define FX_LIGHTNING_BLUE_RING     0x6 /* rings: white-blue-violet gouraud rings */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[11]; /* bolt points */
} Fx0a9Bolt; /* 0x5C */

typedef struct {
    s32 pos0[3]; /* position, 20.12 */
    s16 velC[3]; /* velocity */
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    u16 bolt14; /* bolt */
    u8 fromEnd16; /* point index from the end */
    u8 kind17;
    SVECTOR *point18; /* bolt point */
} Fx0a9Part; /* pool entry data */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster body position */
} Fx0a9Work80;

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
} Fx0a9Work83;

extern Unk800F78A4 *fx_0a9_env;
extern Fx0a9Bolt fx_0a9_bolts[16];
extern s32 fx_0a9_nextBolt; /* next bolt */

static s32 fx_0a9_op83PoolTask(s32 mode, Fx0a9Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0a9Work83 *work);
static void fxm_resetBolts(void);
static void fxm_spawnBolt(Unk80100C78Entry *entry, SVECTOR *from, SVECTOR *to);

#endif
