#ifndef FX_1F5_H
#define FX_1F5_H

/* Effect module fx_1f5 (dir 13 file 0x1f5): Night; enemy Night (Nymph, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_NIGHT_MOON_CLOUDS     0x1 /* emitter: dark clouds and a full moon */
#define FX_NIGHT_PINK_GLOW_SWIRL 0x5 /* emitter: helix of white screen trails with pink glows */
#define FX_NIGHT_PINK_WISPS      0x6 /* emitter: pink wisp sprites */

/* fx_1f5 choreography (docs/fxdata.md, "Choreography literals"): the three ops' lengths and the swirl's trails. */
#define FX_NIGHT_MOON_FRAMES        0x32 /* op80 (MOON_CLOUDS above the field, also camera point 0): the task ends at this frame */
#define FX_NIGHT_SWIRL_TRAIL_SLOTS  0x1E /* op81: PINK_GLOW_SWIRL's trail buffer (fx_1f5_op81Trails): slots of */
#define FX_NIGHT_SWIRL_TRAIL_STRIDE 0x68 /*   this many bytes */
#define FX_NIGHT_SWIRL_FRAMES       0x28 /* op81: the task ends at this frame */
#define FX_NIGHT_WISPS_FRAMES       0x28 /* op82 (PINK_WISPS at one target): the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
} Fx1f5Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 unitC; /* unit */
} Fx1f5Work82;

extern Unk800F78A4 *fx_1f5_env;
extern s32 fx_1f5_op81Trails[];

#endif
