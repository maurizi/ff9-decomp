#ifndef FX_193_H
#define FX_193_H

/* Effect module fx_193 (dir 13 file 0x193). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target body position */
} Fx193Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* target body position */
    SVECTOR unk18; /* target position */
} Fx193Work81;

extern Unk800F78A4 *D_801E7CB4;
extern SVECTOR D_801E7CBC;
extern SVECTOR D_801E7CC4;
extern SVECTOR D_801E7CCC;
extern SVECTOR D_801E7CD4;
extern SVECTOR D_801E7CDC;
extern s32 D_801E7CE4[];
extern s32 D_801E80A4[];

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E774C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7794(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames);

#endif
