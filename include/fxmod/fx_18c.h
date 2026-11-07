#ifndef FX_18C_H
#define FX_18C_H

/* Effect module fx_18c (dir 13 file 0x18c): enemy Light (Jabberwock). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIGHT_VIOLET_STREAKS 0x1 /* emitter: one particle element, a thin violet streak sprite at run time: the violet light streaks under the target; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_LIGHT_GREEN_HALO 0x2 /* emitter: yellow-green crescent halo particles + violet-white gouraud rings */

/* fx_18c choreography (docs/fxdata.md, "Choreography literals"): the target's lift, hover and drop (countdown slots
 * of fx_18c_timers and their lengths), op80's keyframes and length. */
#define FX_LIGHT_T_RISE  0 /* fx_18c_timers slot: the target rises (eased) */
#define FX_LIGHT_T_HOVER 1 /* slot: the target wobbles around its height */
#define FX_LIGHT_T_DROP  2 /* slot: the target sinks back */
#define FX_LIGHT_TIMERS  3 /* fx_18c_timers' slots */
#define FX_LIGHT_LIFT_HEIGHT 0x320 /* the target is lifted this far over its home position */
#define FX_LIGHT_RISE_FRAMES 0x10  /* over this many frames from frame 0 */
#define FX_LIGHT_HOVER_FRAMES 0x28 /* the wobble runs this many frames from frame 0 */
#define FX_LIGHT_DROP_FRAME  24    /* the target sinks back */
#define FX_LIGHT_DROP_FRAMES 0x10  /* over this many frames */
#define FX_LIGHT_HOME_FRAME  40    /* the target is put back on its home position */
#define FX_LIGHT_FRAMES      0x48  /* op80's length (the op ends) */

typedef struct {
    SVECTOR pos0; /* home position */
    SVECTOR pos8; /* unk8.vy: height */
} Fx18cWork; /* 0x10 */

extern Unk800F78A4 *fx_18c_env;
extern s32 fx_18c_timers[FX_LIGHT_TIMERS]; /* countdowns */

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
