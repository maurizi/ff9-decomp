#ifndef FX_082_H
#define FX_082_H

/* Effect module fx_082 (dir 13 file 0x82). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10;
    SVECTOR unk18; /* unk8 moved 0x226 along unk20, raised by 0x145 */
    SVECTOR unk20; /* caster rotation */
    SVECTOR unk28; /* model rotation */
    s32 unk30; /* models */
    s32 unk34;
} Fx082Work80;

typedef struct {
    s32 unk0[3]; /* caster scale */
} Fx082Work81;

extern Unk800F78A4 *D_801E7E64;
extern BtlModel D_801E7E74[];

#endif
