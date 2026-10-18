#ifndef FX_128_H
#define FX_128_H

/* Effect module fx_128 (dir 13 file 0x128). */
#include "btlfx_mod.h"

/* particle as the path callbacks see it */
typedef struct {
    VECTOR unk0; /* position */
    u8 pad10[0x1F - 0x10];
    u8 unk1F; /* index (1-based) */
} Fx128Particle;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx128Work80;

extern Unk800F78A4 *D_801E89D4;
extern SVECTOR D_801E8F9C; /* path start */
extern SVECTOR D_801E8FA4; /* path end */
extern SVECTOR D_801E8FB4[]; /* path ends */

s32 func_801E7704(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77C8(Fx128Particle *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);

typedef struct {
    s16 x, y, z;
} Fx128Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx128Point *unk4;
} Fx128Path;
void func_801E87F0(Fx128Path *path, u16 *data);
void func_801E889C(Fx128Path *path, u16 *data);
s32 func_801E88B4(Fx128Path *path, s32 t, SVECTOR *out);
void func_801E8918(Fx128Path *path, u16 *data);

void func_801E8754(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E8798(s32 from, s32 to, s32 t, s32 n);

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx128Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx128Key *unk4;
} Fx128Path_0b4;
s32 func_801E8808(Fx128Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    s16 *unk4;
} Fx128Keys;
s32 func_801E8930(Fx128Keys *keys, s32 i, s32 *out);

#endif
