#ifndef FX_056_H
#define FX_056_H

/* Effect module fx_056 (dir 13 file 0x56). */
#include "btlfx_mod.h"

/* fx_056 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CHAKRA_DOT_RINGS   0x6 /* emitter: fans, rings of violet / yellow dots, teal-white rings (byte-identical to fx_055 entry 0x6) */
#define FX_CHAKRA_STAR_GLINTS 0x8 /* emitter: blue star glints + light streak, small glints (byte-identical to fx_055 entry 0x7) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    s32 target10; /* target index */
} Fx056Work;

extern Unk800F78A4 *fx_056_env;
extern s32 fx_056_trails[][0x168]; /* per target */

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
