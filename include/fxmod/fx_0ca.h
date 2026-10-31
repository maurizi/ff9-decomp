#ifndef FX_0CA_H
#define FX_0CA_H

/* Effect module fx_0ca (dir 13 file 0xCA): Poison. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* pos10 raised by 0x190 */
    SVECTOR pos10; /* position: env->targets->centerX..Z */
} Fx0caWork;

extern Unk800F78A4 *fx_0ca_env;
extern s32 fx_0ca_trails[];

#endif
