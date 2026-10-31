#ifndef FX_18A_H
#define FX_18A_H

/* Effect module fx_18a (dir 13 file 0x18a): enemy Transform (Kuja/Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* caster body position */
    SVECTOR rot8; /* angles */
} Fx18aWork82; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR points4[15];
} Fx18aTrail; /* 0x7C */

typedef struct {
    s32 pos0; /* position, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12;
    u16 slot14; /* trail */
    s16 index16; /* trail point index */
    SVECTOR *point18; /* trail point */
} Fx18aSpark; /* 0x1C */

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx18aInst;

extern Unk800F78A4 *fx_18a_env;
extern Fx18aTrail fx_18a_trails[12];
extern s32 fx_18a_nextTrail;

static s32 fx_18a_op82PoolTask(s32 mode, Fx18aSpark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void fx_18a_resetTrails(void);
void fx_18a_spawnTrail(Fx18aInst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx18aTrail *)((e)->point18 - 2))

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* caster body position */
    s32 frame1C; /* charge counter */
    s32 unk20;
    s32 model24; /* model */
} Fx18aWork80; /* 0x28 */

extern s32 fx_18a_op80Done;
extern BtlModel fx_18a_effModels[];

#endif
