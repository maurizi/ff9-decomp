#ifndef FX_188_H
#define FX_188_H

/* Effect module fx_188 (dir 13 file 0x188): LV3 Def-less. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 speed4; /* speed */
    s32 turn8; /* turn rate */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* direction */
    Pool6CEntry *emit1C;
    SVECTOR target20; /* target */
} Fx188Elem; /* 0x28 */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* target */
    SVECTOR posC; /* caster body position */
    SVECTOR rot14; /* caster rotation */
    s32 count1C; /* spawned */
} Fx188Work; /* 0x20 */

extern Unk800F78A4 *fx_188_env;
extern s32 fx_188_op80Trails[];
extern s32 fx_188_particleScale; /* scale */

static s32 fx_188_op80PoolTask(s32 mode, Fx188Elem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
