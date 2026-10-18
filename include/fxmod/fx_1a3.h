#ifndef FX_1A3_H
#define FX_1A3_H

/* Effect module fx_1a3 (dir 13 file 0x1a3). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster rotation */
    SVECTOR unk10; /* target body position */
    SVECTOR unk18;
    SVECTOR unk20;
} Fx1a3Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14;
} Fx1a3Work81;

extern Unk800F78A4 *D_801E7C04;
extern s32 D_801E7C14[];
extern s32 D_801E8074[];
extern s32 D_801E86D4[];
extern SVECTOR D_801E93F4; /* from */
extern SVECTOR D_801E93FC; /* to */
extern SVECTOR D_801E9404;

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
