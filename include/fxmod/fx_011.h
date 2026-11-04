#ifndef FX_011_H
#define FX_011_H

/* Effect module fx_011 (dir 13 file 0x11). */
#include "btlfx_mod.h"

/* fx_011 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIFE_FEATHER_TRAILS 0x7 /* emitter: two orbiting trails with a feather texture */
#define FX_LIFE_HALO_RINGS     0x8 /* emitter: white-lilac gouraud rings, leaf-shaped halo outline */
#define FX_LIFE_WINGS          0x9 /* emitter: white angel-wing sprites */

extern Unk800F78A4 *fx_011_env;
extern s32 fx_011_trails[];

#endif
