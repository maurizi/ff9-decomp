#ifndef FX_1E9_H
#define FX_1E9_H

/* Effect module fx_1e9 (dir 13 file 0x1e9): Trance command (not in Trance). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* loop counter */
    s32 unk20;
    s32 model24; /* model */
} Fx1e9Work80; /* 0x28 */

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR angle8; /* angles */
} Fx1e9Work82; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR pos4[9];
} Fx1e9Trail; /* 0x4C */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    s16 velXC; /* velocity */
    s16 velYE;
    s16 velZ10;
    s16 unk12;
    u16 index14; /* trail */
    s16 index16; /* trail point index */
    SVECTOR *pos18; /* trail point */
} Fx1e9Spark; /* 0x1C */

typedef struct {
    u8 pad0[0x10];
    s16 *pool10; /* pool */
} Fx1e9Inst;

extern s32 fx_1e9_stopBolts;
extern Unk800F78A4 *fx_1e9_env;
extern BtlModel fx_1e9_effModels[];
extern Fx1e9Trail fx_1e9_bolts[8];
extern s32 fx_1e9_nextBolt;

static s32 fx_1e9_op82PoolTask(s32 mode, Fx1e9Spark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_1e9_resetBolts(void);
void fx_1e9_spawnBolt(Fx1e9Inst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx1e9Trail *)((e)->pos18 - 2))

#endif
