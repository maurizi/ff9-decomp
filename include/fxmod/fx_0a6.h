#ifndef FX_0A6_H
#define FX_0A6_H

/* Effect module fx_0a6 (dir 13 file 0xA6). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* position: env->targets->centerX..Z, raised by 0xC8 */
    SVECTOR unk1C; /* position: env->targets->centerX..Z */
} Fx0a6Work;

extern Unk800F78A4 *D_801E7974;
extern s32 D_801E7984[];

#endif
