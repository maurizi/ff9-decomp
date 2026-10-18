#ifndef FX_099_H
#define FX_099_H

/* Effect module fx_099 (dir 13 file 0x99). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* caster position on the ground */
    SVECTOR unk10; /* caster body position */
    s32 unk18; /* model scale */
    s32 unk1C; /* model */
    SVECTOR unk20; /* model rotation */
    s32 unk28[3]; /* model scale (VECTOR without its pad word) */
    s32 unk34[3]; /* model position (VECTOR without its pad word) */
} Fx099Work;

extern Unk800F78A4 *D_801E7C28;
extern BtlModel D_801E7C38[];

#endif
