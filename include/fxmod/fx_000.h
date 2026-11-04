#ifndef FX_000_H
#define FX_000_H

/* Effect module fx_000 (dir 13 file 0x00). */
#include "btlfx_mod.h"

/* fx_000 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRE_FIREBALL    0x5 /* emitter: fireball sprite with ember sparks, at the target body */
#define FX_FIRE_FLAME_BURST 0x6 /* emitter: flame explosion (burst, smoke, embers) rising around the target */
#define FX_FIRE_HEAT_RING   0x7 /* emitter: white-red gouraud rings at the target's feet */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR pos10; /* target position */
} Fx000Work;

extern Unk800F78A4 *fx_000_env;

#endif
