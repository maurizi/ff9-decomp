#ifndef FX_18D_H
#define FX_18D_H

/* Effect module fx_18d (dir 13 file 0x18d): Climhazzard. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_18d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CLIMHAZZARD_FLAME_RINGS 0x1 /* emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames */

/* fx_18d choreography (docs/fxdata.md, "Choreography literals"): the caster's dash (two lerps of unit 0x10 to its
 * landing point, the camera following), the rings' keyframe and op80's length. */
#define FX_CLIMHAZZARD_DASH_REACH   0x320 /* the landing point is this far past the targets' centre, seen from unit 0x10 */
#define FX_CLIMHAZZARD_DASH_FRAMES  0xC /* the caster moves from its home position towards it over this many frames */
#define FX_CLIMHAZZARD_DASH_2_FRAME 5 /* restarts from where it is */
#define FX_CLIMHAZZARD_DASH_2_FRAMES 7 /* and arrives in this many frames */
#define FX_CLIMHAZZARD_RINGS_FRAME  13 /* FLAME_RINGS at the targets' centre */
#define FX_CLIMHAZZARD_FRAMES       0x20 /* op80's length (the op ends) */

typedef struct {
    s32 timer0; /* steps left */
    s32 duration4; /* steps */
    SVECTOR pos8;
    SVECTOR pos10; /* from */
    SVECTOR pos18; /* to */
    SVECTOR pos20; /* current */
    SVECTOR rot28; /* angles */
} Fx18dWork; /* 0x30 */

extern Unk800F78A4 *fx_18d_env;

#endif
