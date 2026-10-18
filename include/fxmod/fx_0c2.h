#ifndef FX_0C2_H
#define FX_0C2_H

/* Effect module fx_0c2 (dir 13 file 0xC2). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* unit 0x10 bone 0x17 position */
} Fx0c2Work80;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* unit 0x10 position */
    SVECTOR unk14; /* unit 0x10 rotation */
    SVECTOR unk1C; /* target body position */
    SVECTOR unk24; /* target rotation */
    SVECTOR unk2C; /* unkC moved 0x12C along unk14 */
    SVECTOR unk34; /* func_800DC818(0x10, 0x17) */
    SVECTOR unk3C; /* copy of unk44 */
    SVECTOR unk44; /* angles from unk34 to unk1C */
    s32 unk4C; /* target unit */
} Fx0c2Work81;

typedef struct {
    SVECTOR unk0; /* rotation step total */
    SVECTOR unk8; /* position: env->unkC->unk38..3C */
} Fx0c2Work82;

typedef struct {
    SVECTOR unk0; /* turn angles */
    SVECTOR unk8; /* unit position */
    s32 unk10; /* unit */
} Fx0c2Work83;

extern SVECTOR D_801E8484; /* particle path start */
extern SVECTOR D_801E848C; /* particle path end */
extern SVECTOR D_801E8494;

s32 func_801E7804(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7EC4(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta);
s32 func_801E8040(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang);
void func_801E7D6C(s32 unit, s32 motion);
void func_801E8388(s32 unit);
void func_801E8298(s32 unit, s32 motion);
void func_801E83D4(s32 unit);
void func_801E8420(Unk800F78A4 *env, s32 i, s32 v);
extern Unk800F78A4 *D_801E847C;
s32 func_801E8304(s32 unit);

#endif
