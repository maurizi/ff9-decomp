#ifndef FX_104_H
#define FX_104_H

/* Effect module fx_104 (dir 13 file 0x104): enemy Everyone's Grudge (Tonberry). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_104 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EVERYONES_GRUDGE_PINK_RINGS   0x1 /* emitter: pink gouraud rings + pink glow-ring particles + white-yellow rings */
#define FX_EVERYONES_GRUDGE_SWIRL_TRAIL  0x3 /* emitter: flat trail spiralling, white swirl strip */
#define FX_EVERYONES_GRUDGE_SMOKE        0x4 /* emitter: grey smoke wisps */
#define FX_EVERYONES_GRUDGE_MAGIC_CIRCLE 0x5 /* emitter: mandala magic circle cycling green to blue */

/* fx_104 choreography (docs/fxdata.md, "Choreography literals"): the two ops' lengths and the swirl's trail buffer. */
#define FX_EVERYONES_GRUDGE_CHARGE_FRAMES  0x28 /* op80's length: PINK_RINGS ahead of the caster's bone 0x12 */
#define FX_EVERYONES_GRUDGE_SWIRL_TRAIL_SLOTS  0x10 /* op81: SWIRL_TRAIL's trail buffer (fx_104_op81Trails): slots */
#define FX_EVERYONES_GRUDGE_SWIRL_TRAIL_STRIDE 0x60 /* bytes per slot */
#define FX_EVERYONES_GRUDGE_CURSE_FRAMES   0x28 /* op81's length: SWIRL_TRAIL, SMOKE and MAGIC_CIRCLE at the target */

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    SVECTOR pos8;
    SVECTOR pos10;
    u8 unused18[8]; /* op80's mode 0 asks for 0x20 */
} Fx104Work80; /* 0x20 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx104Work81; /* 0x14 */

extern Unk800F78A4 *fx_104_env;
extern s32 fx_104_op81Trails[];

#endif
