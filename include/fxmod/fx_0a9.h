#ifndef FX_0A9_H
#define FX_0A9_H

/* Effect module fx_0a9 (dir 13 file 0xA9): enemy Lightning (Chimera and others). Jagged bolts (fxm_spawnBolt)
 * whose points are pool particles. Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR pts4[11]; /* bolt points */
} Fx0a9Bolt; /* 0x5C */

typedef struct {
    s32 pos0[3]; /* position, 20.12 */
    s16 velC[3]; /* velocity */
    s16 unk12;
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
