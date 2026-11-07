#ifndef FX_07C_H
#define FX_07C_H

/* Effect module fx_07c (dir 13 file 0x7C): Mini. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_07c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MINI_LIGHT_BEAM  0x6 /* emitter: fan drawn as a blue-white horizontal light beam + small blue rings */

/* fx_07c choreography (docs/fxdata.md, "Choreography literals"). */
#define FX_MINI_FRAMES 0x28 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* position: env->targets->centerX..Z */
} Fx07cWork;

extern Unk800F78A4 *fx_07c_env;

#endif
