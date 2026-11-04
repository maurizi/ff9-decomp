#ifndef FX_00F_H
#define FX_00F_H

/* Effect module fx_00f (dir 13 file 0x0F). */
#include "btlfx_mod.h"

/* fx_00f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FULL_LIFE_DARK_RINGS   0x6 /* emitter: faint gold-blue gouraud rings */
#define FX_FULL_LIFE_LIGHT_TRAILS 0x7 /* emitter: white light trails with faint sparkles, pink feather strip */
#define FX_FULL_LIFE_WINGS        0x9 /* emitter: white angel-wing sprites */
#define FX_FULL_LIFE_HALO_RINGS   0xA /* emitter: white-lilac gouraud rings, leaf-shaped halo outline */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body position */
} Fx00fWork;

extern Unk800F78A4 *fx_00f_env;
extern s32 fx_00f_trails[];

#endif
