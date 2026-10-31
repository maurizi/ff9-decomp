#ifndef FX_038_H
#define FX_038_H

/* Effect module fx_038 (dir 13 file 0x38). */
#include "btlfx_mod.h"

/* falling rock, a pool item */
typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* rotation */
    Pool6CEntry *emit14;
    SVECTOR pos18; /* landing position */
    s32 strip20; /* index */
} Fx038Item; /* 0x24 */

/* trail strip for btlfx_drawTrail */
typedef struct {
    u8 pad0[0x8C];
} Fx038Strip;

extern Fx038Strip fx_038_strips[];

typedef struct {
    s32 stop0; /* dead early-out flag, always 0 (see Fx046Work.stop0) */
    SVECTOR pos4; /* target position */
    s32 countC; /* items spawned */
    s32 model10; /* models */
    s32 model14;
    s32 model18;
} Fx038Work; /* 0x1C */

extern u8 fx_038_spawnFrames[0x18]; /* spawn frames */
extern Unk800F78A4 *fx_038_env;
extern BtlModel fx_038_effModels[];

static s32 fx_038_op80PoolTask(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work);

#endif
