#ifndef FX_03F_H
#define FX_03F_H

/* Effect module fx_03f (dir 13 file 0x3F: Shell, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_03f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHELL_BLUE_STREAK    0x1 /* emitter: fan drawn as a blue comet streak + sprites that still render black */
#define FX_SHELL_BARRIER_RING   0x2 /* emitter: teal specks + ring with a blue lattice texture */
#define FX_SHELL_BLUE_RINGS     0x3 /* emitter: pale blue gouraud rings */
#define FX_SHELL_BARRIER_RING_2 0x4 /* emitter: teal specks + ring with a blue lattice texture */

/* fx_03f choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, length and ring offset. */
#define FX_SHELL_RING_OFFSET     0xFA /* the barrier rings' positions: this far above (pos18) and below (pos28) the target body */
#define FX_SHELL_RING_2_FRAME    0xA  /* op80: FX_SHELL_BARRIER_RING_2 starts above and below */
#define FX_SHELL_RING_STOP_FRAME 0xB  /* op80: the first two FX_SHELL_BARRIER_RING emitters are stopped */
#define FX_SHELL_BLUE_RINGS_FRAME 0x14 /* op80: FX_SHELL_BLUE_RINGS start */
#define FX_SHELL_FRAMES          0x2D /* op80: the task ends at this frame */

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
