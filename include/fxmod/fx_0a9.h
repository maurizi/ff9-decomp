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
#define FX_LIGHTNING_TRAIL_2       0x4 /* trail: one frame, still not rendered by the tool; runtime (docs/runtime-findings.md effect-data pass 2): drawn from the start (99 calls); not rendered by the tool, not ablated (look pending) */
#define FX_LIGHTNING_BLUE_RING     0x6 /* rings: white-blue-violet gouraud rings */

/* fx_0a9 choreography (docs/fxdata.md, "Choreography literals"): op80's length; op83's bolt pool. */
#define FX_LIGHTNING_STATE_ARCS     0    /* op80's only state */
#define FX_LIGHTNING_ARCS_FRAMES    0x20 /* op80 (FX_LIGHTNING_ELECTRIC_ARCS)'s length */
#define FX_LIGHTNING_BOLT_POINTS    11   /* points per bolt (Fx0a9Bolt.pts4), each a pool particle */
#define FX_LIGHTNING_BOLT_SLOTS     16   /* bolt ring fx_0a9_bolts (fx_0a9_nextBolt wraps) */
#define FX_LIGHTNING_BOLT_FRAMES    8    /* a bolt point particle lives (fading) this many frames */
#define FX_LIGHTNING_BOLT_WIDTH     0x18 /* trail width written to each point (SVECTOR.pad) */
#define FX_LIGHTNING_BOLTS_PER_FRAME 2   /* op83 spawns this many bolts per frame from the unit's bone to the target */
#define FX_LIGHTNING_BOLTS_FRAMES   0x18 /* for this many frames (the op ends; the ring pulse is a half sine over it) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_LIGHTNING_BOLT_POINTS]; /* bolt points */
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
extern Fx0a9Bolt fx_0a9_bolts[FX_LIGHTNING_BOLT_SLOTS];
extern s32 fx_0a9_nextBolt; /* next bolt */

static s32 fx_0a9_op83PoolTask(s32 mode, Fx0a9Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0a9Work83 *work);
static void fxm_resetBolts(void);
static void fxm_spawnBolt(Unk80100C78Entry *entry, SVECTOR *from, SVECTOR *to);

#endif
