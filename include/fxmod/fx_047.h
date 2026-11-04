#ifndef FX_047_H
#define FX_047_H

/* Effect module fx_047 (dir 13 file 0x47). */
#include "btlfx_mod.h"

/* fx_047 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRA_FLAME_SWIRL   0x1 /* emitter: flame-strip trails orbiting the targets */
#define FX_FIRA_HEAT_RING     0x2 /* emitter: white-red gouraud rings on the ground */
#define FX_FIRA_FLAME_BURST   0x3 /* emitter: flame explosion with ember sparks, above the targets */
#define FX_FIRA_SMOKE         0x4 /* emitter: grey smoke puffs */
#define FX_FIRA_FLAME_BURST_2 0x5 /* emitter: second flame explosion */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* above posC */
    SVECTOR posC; /* target on the ground */
    Pool6CEntry *emit14;
} Fx047Work;

extern Unk800F78A4 *fx_047_env;
extern s32 fx_047_trails[];

#endif
