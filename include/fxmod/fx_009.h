#ifndef FX_009_H
#define FX_009_H

/* Effect module fx_009 (dir 13 file 0x09). */
#include "btlfx_mod.h"

/* fx_009 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURE_SPARKLE_SWIRL 0x3 /* emitter: star-sparkle trails spiralling up, sparkles and bubbles */

typedef struct {
    Pool6CEntry *emit0;
} Fx009Work;

extern Unk800F78A4 *fx_009_env;
extern s32 fx_009_trails[];

#endif
