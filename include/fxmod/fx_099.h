#ifndef FX_099_H
#define FX_099_H

/* Effect module fx_099 (dir 13 file 0x99): Break. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR pos8; /* caster position on the ground */
    SVECTOR pos10; /* caster body position */
    s32 unk18; /* model scale */
    s32 model1C; /* model */
    SVECTOR unk20; /* model rotation */
    s32 unk28[3]; /* model scale (VECTOR without its pad word) */
    s32 unk34[3]; /* model position (VECTOR without its pad word) */
} Fx099Work;

extern Unk800F78A4 *fx_099_env;
extern BtlModel fx_099_effModels[];

#endif
