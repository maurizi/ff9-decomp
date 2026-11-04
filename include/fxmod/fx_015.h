#ifndef FX_015_H
#define FX_015_H

/* Effect module fx_015 (dir 13 file 0x15). */
#include "btlfx_mod.h"

/* fx_015 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POISON_PURPLE_SMOKE 0x1 /* emitter: orbiting trail with purple smoke clouds */
#define FX_POISON_TOXIC_BURST  0x8 /* emitter: purple clouds + purple droplet sprites */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* above the target */
    SVECTOR pos10; /* target position */
} Fx015Work;

extern Unk800F78A4 *fx_015_env;
extern s32 fx_015_trails[];

#endif
