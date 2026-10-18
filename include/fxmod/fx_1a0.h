#ifndef FX_1A0_H
#define FX_1A0_H

/* Effect module fx_1a0 (dir 13 file 0x1a0). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1a0Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1a0Point *unk4;
} Fx1a0Path;

typedef struct {
    u16 unk0; /* bone */
    u16 unk2; /* distance */
} Fx1a0Bone;

typedef struct {
    Pool6CEntry *unk0[6];
    SVECTOR unk18[6];
} Fx1a0Work80; /* 0x48 */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8;
    SVECTOR unkC; /* target body position */
    s32 unk14; /* index */
    s32 unk18;
} Fx1a0Work81; /* 0x1C */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4;
} Fx1a0Work82;

extern Fx1a0Bone D_801E7EF8[6];
extern s16 D_801E7F10[6];
extern s16 D_801E7F1C[6];
extern s32 (*D_801E7F28[6])();
extern Unk800F78A4 *D_801E7F88;
extern s32 D_801E7F98[6][0x1C];
extern Fx1a0Path D_801E8238[6];
extern s32 D_801E8268[6]; /* frames */
extern SVECTOR D_801E8280;
extern SVECTOR D_801E8288[6];
extern SVECTOR D_801E82B8[6];

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7748(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E778C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E77D0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7858(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E789C(Pool30Entry *e, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames);
void func_801E7E7C(Fx1a0Path *path, u16 *data);
s32 func_801E7E94(Fx1a0Path *path, s32 t, SVECTOR *out);

#endif
