#ifndef FX_04E_H
#define FX_04E_H

/* Effect module fx_04e (dir 13 file 0x4E: Sleep, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* position: env->unkC->unk38..3C */
} Fx04eWork;

typedef struct {
    s32 unk0; /* frame */
    s32 unk4;
    s16 unk8; /* file slot */
} Fx04eEntry;

extern Unk800F78A4 *D_801E7930;
extern s32 D_801E7940[];
extern s32 D_801E7DA0[];

#endif
