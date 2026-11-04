#ifndef FX_03F_H
#define FX_03F_H

/* Effect module fx_03f (dir 13 file 0x3F: Shell, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_03f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHELL_FAN_EMITTER    0x1 /* emitter: fan + sprites that render black (look pending) */
#define FX_SHELL_BARRIER_RING   0x2 /* emitter: teal specks + ring with a blue lattice texture */
#define FX_SHELL_BLUE_RINGS     0x3 /* emitter: pale blue gouraud rings */
#define FX_SHELL_BARRIER_RING_2 0x4 /* emitter: teal specks + ring with a blue lattice texture */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* above the target */
    SVECTOR pos20; /* target body position */
    SVECTOR pos28; /* below the target */
} Fx03fWork; /* 0x30 */

extern Unk800F78A4 *fx_03f_env;

#endif
