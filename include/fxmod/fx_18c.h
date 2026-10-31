#ifndef FX_18C_H
#define FX_18C_H

/* Effect module fx_18c (dir 13 file 0x18c): enemy Light (Jabberwock). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR pos0; /* home position */
    SVECTOR pos8; /* unk8.vy: height */
} Fx18cWork;

extern Unk800F78A4 *fx_18c_env;
extern s32 fx_18c_timers[3]; /* countdowns */

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
