#ifndef FX_0EC_H
#define FX_0EC_H

/* Effect module fx_0ec (dir 13 file 0x0EC). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* split models */
    s16 unk2; /* vertices */
    s32 unk4[2]; /* vertex counts */
    u8 *unkC; /* vertex buffer */
    u8 *unk10; /* next free vertex */
    u8 *unk14[2]; /* model vertices */
} Fx0ecSplit;

typedef struct {
    u8 pad0[6];
    s16 unk6;
    u8 pad8[0x18 - 8];
} Fx0ecVertex; /* 0x18 */

typedef struct {
    s32 vx, vy, vz;
} Fx0ecVec3;

typedef struct {
    Fx0ecVec3 unk0; /* position (.12) */
    SVECTOR unkC; /* velocity; unkC.pad: scale */
    s16 unk14; /* spin */
    s16 unk16;
    s16 unk18; /* age, < 0: free */
    s16 unk1A; /* triangle */
} Fx0ecShard; /* 0x1C */

typedef struct {
    SVECTOR unk0[3];
} Fx0ecTri;

extern Fx0ecTri D_801EB3D0[0x70];
extern Fx0ecShard D_801EBE50[0x70];

typedef struct {
    u8 pad0[0x10];
    Unk800DA1B8 *unk10; /* particle pool */
} Fx0ecWork;

extern Unk800F78A4 *D_801E98A0;
extern Fx0ecWork *D_801E98A4;
extern SVECTOR D_801E98A8; /* shard rotation */
extern SVECTOR D_801E98B0; /* shard origin */
extern VECTOR D_801E98C0; /* shard scale */
extern Fx0ecSplit D_801ECA90;
extern Fx0ecShard D_801ECAB0[300];

s32 func_801E7704(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E77FC(SVECTOR *pos);
void func_801E798C(s32 model, SVECTOR *center);
s32 func_801E7F00(u8 *buf);
s32 func_801E7F40(s32 model);
Fx0ecShard *func_801E802C(void);
void func_801E82D8(void);

#endif
