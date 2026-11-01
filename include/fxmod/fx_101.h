#ifndef FX_101_H
#define FX_101_H

/* Effect module fx_101 (dir 13 file 0x101): Trance transformation. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[12];
} Fx101Bolt; /* 0x64 */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    s16 index14; /* bolt */
    s16 index16; /* point index from the tip */
    SVECTOR *pos18; /* bolt point */
} Fx101Spark; /* 0x1C */

typedef struct {
    s32 workSize0; /* work size */
    s32 poolCount4; /* count */
    s32 elemSize8; /* entry size */
    u8 padC[0x14 - 0xC];
    void *elemFn14; /* entry callback */
} Fx101PoolCfg;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* loop counter */
    s32 flag20; /* camera shake on */
    s32 model24; /* model */
} Fx101Work80;

/* View of Unk800F78A40 up to the byte fx_101 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x10];
    u8 signal10;
} Fx101BattleState;

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR angle8; /* angles */
} Fx101Work82;

extern s32 fx_101_stopBolts; /* bolts stop */
extern BtlModelBuf fx_101_effModels[];
extern Unk800F78A4 *fx_101_env;
extern Fx101Bolt fx_101_bolts[8];
extern s32 fx_101_nextBolt; /* next bolt */

s32 fx_101_op80(s32 mode, Fx101Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_101_op82PoolTask(s32 mode, Fx101Spark *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_101_resetBolts(void);
void fx_101_spawnBolt(Unk80100C78Entry *entry, SVECTOR *origin, s32 ang, SVECTOR *from, SVECTOR *to);
s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg);

#endif
