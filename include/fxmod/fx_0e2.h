#ifndef FX_0E2_H
#define FX_0E2_H

/* Effect module fx_0e2 (dir 13 file 0x0E2). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[1]; /* (size - 4) / 8 points */
} Fx0e2Trail;

typedef struct {
    DVECTOR unk0; /* screen xy of one edge end */
    DVECTOR unk4; /* screen xy of the other */
    s32 unk8; /* ot z */
} Fx0e2Edge;

typedef struct {
    SVECTOR unk0; /* unit position */
    SVECTOR unk8; /* unit rotation */
    s16 unk10; /* distance from the centre */
    s16 unk12; /* angle around it */
} Fx0e2Orbit;

extern Unk800F78A4 *D_801ECC0C;
extern Fx0e2Orbit D_801EE8FC[];
extern u8 D_801EFDEC[][0x240]; /* per-unit texture animation */

typedef struct {
    Pool6CEntry *unk0[8];
    Pool6CEntry *unk20[8];
    SVECTOR unk40[8]; /* unit positions */
    SVECTOR unk80[8]; /* effect positions */
    s16 unkC0[8]; /* unit landed */
} Fx0e2Work84; /* 0xD0 */
extern Fx0e2Edge D_801EE62C[60];
extern SVECTOR D_801F0FEC; /* trail origin */
extern SVECTOR D_801F0FF4; /* trail head position */
extern SVECTOR D_801F0FFC; /* trail head rotation */
extern SVECTOR D_801F1004; /* trail aim target */

void func_801E77C0(void *trackA, void *trackB, s32 t, Fx0e2Trail *trail, s32 size, s32 pad);
void func_801E7A0C(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 pad);
void func_801E7BE8(void *track, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad);
void func_801E7D5C(void *trackA, void *trackB, Fx0e2Trail *trail, s32 size, s32 base, s32 count, s32 pad, s32 t,
                   s32 rotY);
void func_801E7FBC(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void func_801E83B0(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
void func_801E86D0(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);
typedef struct {
    Pool6CEntry *unk0[5];
    SVECTOR unk14; /* tower rotation */
    VECTOR unk1C; /* tower position */
    VECTOR unk2C; /* tower scale */
    SVECTOR unk3C; /* piece rotation */
    VECTOR unk44; /* piece position */
    VECTOR unk54; /* piece scale */
    SVECTOR unk64[4]; /* block rotations */
    VECTOR unk84[4]; /* block positions */
    VECTOR unkC4[4]; /* block scales */
    SVECTOR unk104[4]; /* effect positions */
    u16 unk124[12]; /* models: 3 tower stages, 5 pieces, 4 blocks */
} Fx0e2Work81; /* 0x13C */

extern u16 D_801ECB68[3];
extern u16 D_801ECB70[5];
extern u32 D_801EE9CC[];
extern SVECTOR D_801F103C[4];

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0e2_op84(s32 mode, Fx0e2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

void func_801EC5B4(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
void func_801EC3F0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801EC434(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx0e2Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx0e2Point *unk4;
} Fx0e2Path;
void func_801EC48C(Fx0e2Path *path, u16 *data);
extern Fx0e2Path D_801EE99C[5]; /* piece tracks */
void func_801EC920(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0e2Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0e2Key *unk4;
} Fx0e2Path_0b4;
s32 func_801EC520(Fx0e2Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
s32 func_801EC3D8(u8 *base, u16 i);
s32 func_801EC64C(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx0e2Track; /* 0x10 */
extern Fx0e2Track D_801F10DC[];
extern s32 D_801F14DC[];
void func_801EC718(void);
void func_801EC764(s16 start, u16 dst, s16 ch, s16 *data);
void func_801EC7C0(s32 ch);

#endif
