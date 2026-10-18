#ifndef FX_1E7_H
#define FX_1E7_H

/* Effect module fx_1e7 (dir 13 file 0x1e7). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1e7Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1e7Point *unk4;
} Fx1e7Path;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* target position */
    s16 unk10;
} Fx1e7Work80; /* 0x14 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4[5];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* target body position */
    SVECTOR unk28; /* target position */
    s32 unk30;
    s32 unk34; /* waiting for the motion to end */
} Fx1e7Work81; /* 0x38 */

extern u16 D_801E826C[5]; /* files */
extern u16 D_801E8278[5]; /* path files */
extern s32 (*D_801E8284[5])(SVECTOR *, SVECTOR *, s32, s32);
extern Unk800F78A4 *D_801E82E0;
extern s32 D_801E82E4; /* op81 frame */
extern Fx1e7Path D_801E82E8;
extern Fx1e7Path D_801E82F0;
extern Fx1e7Path D_801E8300[5];
extern s32 D_801E8330[];
extern s32 D_801E8400[];
extern SVECTOR D_801E84D0;
extern SVECTOR D_801E84D8;
extern SVECTOR D_801E84E0; /* angles */
extern SVECTOR D_801E84E8;
extern SVECTOR D_801E84F0[5];

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7738(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E776C(Pool30Entry *e, SVECTOR *pos, Fx1e7Path *path, s32 frame, s32 frames);
void func_801E79BC(SVECTOR *out, Fx1e7Path *path, s32 t);
void func_801E8054(Fx1e7Path *path, u16 *data);
s32 func_801E806C(Fx1e7Path *path, s32 t, SVECTOR *out);
void func_801E80D0(s32 unit, s32 motion);
s32 func_801E813C(s32 unit);
void func_801E820C(s32 unit);

#endif
