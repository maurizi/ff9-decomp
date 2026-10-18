#ifndef FX_125_H
#define FX_125_H

/* Effect module fx_125 (dir 13 file 0x125). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* target position */
    SVECTOR unkC;
    SVECTOR unk14; /* target body position */
    SVECTOR unk1C;
    s32 unk24; /* models */
    s32 unk28[2];
} Fx125Work;

extern Unk800F78A4 *D_801E7D78;
extern BtlModel D_801E7D88[];

#endif
