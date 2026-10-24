#ifndef FX_0AA_H
#define FX_0AA_H

/* Effect module fx_0aa (dir 13 file 0xAA). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    SVECTOR unk10; /* func_800DC818(0x10, 0x12) */
    SVECTOR unk18; /* func_800DC818(0x10, 0x2B) */
} Fx0aaWork80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    Pool6CEntry *unkC;
    Pool6CEntry *unk10;
    SVECTOR unk14; /* position: env->targets->centerX..Z */
} Fx0aaWork81;

extern Unk800F78A4 *D_801E7D78;
extern s32 D_801E7D88[];
extern s32 D_801E82C8[];
extern s32 D_801E8808[];
extern SVECTOR D_801E9088; /* particle paths: starts */
extern SVECTOR D_801E9090;
extern SVECTOR D_801E9098; /* ends */
extern SVECTOR D_801E90A0;
extern SVECTOR D_801E90A8;
extern SVECTOR D_801E90B0;

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7744(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7784(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 n);

#endif
