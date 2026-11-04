#ifndef FX_0CA_H
#define FX_0CA_H

/* Effect module fx_0ca (dir 13 file 0xCA): Poison. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_0ca effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_POISON_BUBBLES 0x1 /* emitter: purple poison blobs and bubbles */
#define FX_POISON_SWIRL   0x2 /* emitter: orbiting purple poison trail */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* pos10 raised by 0x190 */
    SVECTOR pos10; /* position: env->targets->centerX..Z */
} Fx0caWork;

extern Unk800F78A4 *fx_0ca_env;
extern s32 fx_0ca_trails[];

#endif
