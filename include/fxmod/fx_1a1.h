#ifndef FX_1A1_H
#define FX_1A1_H

/* Effect module fx_1a1 (dir 13 file 0x1a1): Climhazzard (Seiken). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CLIMHAZZARD_FLAME_RINGS 0x1 /* emitter: rings textured with a yellow-red flame wall + white / pale-yellow gouraud rings, 16 frames (byte-identical to fx_18d entry 0x1) */

/* fx_1a1 choreography (docs/fxdata.md, "Choreography literals"): the caster's dash (two lerps of unit 0x10 to its
 * landing point, the camera following), the rings' keyframe and op80's length. */
#define FX_CLIMHAZZARD_DASH_REACH   0x514 /* the landing point is this far past the targets' centre, seen from unit 0x10 */
#define FX_CLIMHAZZARD_DASH_FRAMES  5 /* the caster moves from its home position towards it over this many frames */
#define FX_CLIMHAZZARD_DASH_2_FRAME 2 /* restarts from where it is */
#define FX_CLIMHAZZARD_DASH_2_FRAMES 5 /* and arrives in this many frames */
#define FX_CLIMHAZZARD_RINGS_FRAME  10 /* FLAME_RINGS at the targets' centre */
#define FX_CLIMHAZZARD_FRAMES       0x20 /* op80's length (the op ends) */

typedef struct {
    s32 timer0; /* steps left */
    s32 count4; /* steps */
    SVECTOR pos8;
    SVECTOR pos10; /* from */
    SVECTOR pos18; /* to */
    SVECTOR pos20; /* current */
    SVECTOR angle28; /* angles */
} Fx1a1Work; /* 0x30 */

extern Unk800F78A4 *fx_1a1_env;

#endif
