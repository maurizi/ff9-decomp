#ifndef FX_18C_H
#define FX_18C_H

/* Effect module fx_18c (dir 13 file 0x18c): enemy Light (Jabberwock). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIGHT_VIOLET_STREAKS 0x1 /* emitter: one particle element, a thin violet streak sprite at run time: the violet light streaks under the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LIGHT_GREEN_HALO 0x2 /* emitter: yellow-green crescent halo particles + violet-white gouraud rings */

typedef struct {
    SVECTOR pos0; /* home position */
    SVECTOR pos8; /* unk8.vy: height */
} Fx18cWork;

extern Unk800F78A4 *fx_18c_env;
extern s32 fx_18c_timers[3]; /* countdowns */

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
