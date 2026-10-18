#ifndef FX_0D2_H
#define FX_0D2_H

/* Effect module fx_0d2 (dir 13 file 0xD2). */
#include "btlfx_mod.h"

typedef struct {
    u16 unk0;
    u16 unk2; /* count */
    s16 *unk4;
} Fx0d2Keys;

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx0d2Track; /* 0x10 */

typedef struct {
    Pool6CEntry *unk0[8];
    SVECTOR unk20[8]; /* unit positions */
    SVECTOR unk60[8];
    s16 unkA0[8]; /* done */
} Fx0d2Work84; /* 0xB0 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d2Vec3;

typedef struct {
    SVECTOR unk0; /* rotation */
    Fx0d2Vec3 unk8; /* position */
    s32 pad14;
    Fx0d2Vec3 unk18; /* scale */
    s32 pad24;
} Fx0d2Model; /* 0x28 */

typedef struct {
    u16 unk0; /* file id */
    u16 unk2; /* start frame */
    u16 unk4; /* released */
    s16 unk6[3]; /* position */
    u16 unkC;
} Fx0d2Spawn; /* 0xE */

typedef struct {
    Pool6CEntry *unk0[7];
    Fx0d2Model unk1C[4];
    SVECTOR unkBC[7];
    u16 unkF4[10]; /* models: 0 ring, 1-2 morph pair, 3-8 slices, 9 */
} Fx0d2Work82; /* 0x108 */

extern Fx0d2Keys D_801EF240;
extern Fx0d2Keys D_801EF258;
extern Fx0d2Keys D_801EF270[6];
extern u16 D_801EA440[6];
extern SVECTOR D_801EA474;
extern Fx0d2Spawn D_801EA3DC[7];
extern Fx0d2Track D_801F0060[64];
extern s32 D_801F0460[4];

void func_801E77A4(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
s32 func_801E9DBC(u8 *base, u16 i);
void func_801E9DD4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E9E18(s32 from, s32 to, s32 t, s32 n);
s32 func_801E9FB0(Fx0d2Keys *keys, s32 i, s32 *out);
s32 func_801EA108(s32 a0, s32 t, s32 n, SVECTOR *b);
void func_801EA1D4(void);
void func_801EA220(s16 start, u16 dst, s16 ch, s16 *data);
void func_801EA27C(s32 ch);
typedef struct {
    s16 x, y, z;
} Fx0d2Point;
typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx0d2Point *unk4;
} Fx0d2Path;
typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0d2Key; /* 0xC */
typedef struct {
    s16 x, y, z;
} Fx0d2Key6;
typedef struct {
    u16 unk0; /* count */
    Fx0d2Key6 *unk4;
} Fx0d2Track6;
void func_801E9E70(Fx0d2Path *path, u16 *data);
s32 func_801E9E88(Fx0d2Path *path, s32 t, SVECTOR *out);
void func_801E9EEC(Fx0d2Path *path, u16 *data);
s32 func_801E9F04(Fx0d2Path *path, s32 i, SVECTOR *rot, SVECTOR *pos);
void func_801E9F98(Fx0d2Path *path, u16 *data);
void func_801E9FF4(Fx0d2Path *path, u16 *data);
s32 func_801EA00C(Fx0d2Track6 *track, s32 t, VECTOR *out);
extern Unk800F78A4 *D_801ED8E0;
void func_801EA070(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

#endif
