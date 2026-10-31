#ifndef FX_082_H
#define FX_082_H

/* Effect module fx_082 (dir 13 file 0x82). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR unk18; /* unk8 moved 0x226 along unk20, raised by 0x145 */
    SVECTOR rot20; /* caster rotation */
    SVECTOR rot28; /* model rotation */
    s32 model30; /* models */
    s32 model34;
} Fx082Work80;

typedef struct {
    s32 unk0[3]; /* caster scale */
} Fx082Work81;

extern Unk800F78A4 *fx_082_env;
extern BtlModel fx_082_effModels[];

#endif
