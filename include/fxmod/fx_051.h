#ifndef FX_051_H
#define FX_051_H

/* Effect module fx_051 (dir 13 file 0x51). */
#include "btlfx_mod.h"

/* fx_051 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIGHTY_GUARD_CRYSTAL_SPECKS 0x7 /* emitter: two fans + white / cyan / grey square specks */
#define FX_MIGHTY_GUARD_GOLD_HALO      0x8 /* emitter: golden ring glow with golden dots, blue orb, yellow and pale blue rings */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target body position */
    SVECTOR rot10; /* target rotation */
    s32 target18; /* target index */
} Fx051Work;

extern SVECTOR fx_051_emitRot;
extern Unk800F78A4 *fx_051_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
