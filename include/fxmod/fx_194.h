#ifndef FX_194_H
#define FX_194_H

/* Effect module fx_194 (dir 13 file 0x194). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* caster body position */
    SVECTOR unkC; /* caster position */
    SVECTOR unk14;
} Fx194Work;

extern SVECTOR D_801E7D78[6];
extern s16 D_801E7DA8[6];
extern Unk800F78A4 *D_801E7DFC;
extern s32 D_801E7E00; /* wave amplitude */
extern s32 D_801E7E04; /* wave phase */

void func_801E7704(SVECTOR *v);
void func_801E77DC(s32 frame, s32 amp, SVECTOR *v);

#endif
