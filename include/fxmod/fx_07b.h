#ifndef FX_07B_H
#define FX_07B_H

/* Effect module fx_07b (dir 13 file 0x7B: Protect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PROTECT_BLUE_HELIX 0x1 /* emitter: blue flat trail spiralling up + sprite */
#define FX_PROTECT_GOLD_RING  0x3 /* emitter: golden ring particles with blue glints */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position, moved back */
    SVECTOR rot10; /* target rotation */
} Fx07bWork;

extern SVECTOR fx_07b_emitRot;
extern Unk800F78A4 *fx_07b_env;
extern s32 fx_07b_trails[];

#endif
