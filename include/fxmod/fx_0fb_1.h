#ifndef FX_0FB_1_H
#define FX_0FB_1_H

/* Effect module fx_0fb_1 (dir 13 file 0x0FB). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* unit home position */
    SVECTOR unkC; /* unit rotation */
    s16 unk14; /* spin speed */
    s16 unk16;
    s16 unk18;
    u8 pad1A[0x1C - 0x1A];
    SVECTOR unk1C; /* current rotation */
    s32 unk24; /* unit count */
    s32 unk28; /* lift height */
    s32 unk2C; /* unit */
} Fx0fb1Work82; /* 0x30 */

typedef struct {
    s32 vx, vy, vz;
} Fx0fb1Vec3;

typedef struct {
    Fx0fb1Vec3 unk0[8]; /* unit scales */
    SVECTOR unk60[8]; /* unit home positions */
    SVECTOR unkA0[8]; /* lifted positions */
    SVECTOR unkE0[8]; /* unit rotations */
    SVECTOR unk120[8]; /* spin speeds */
    SVECTOR unk160[8]; /* current rotations */
    s32 unk1A0; /* unit count */
    s32 unk1A4;
} Fx0fb1Work81; /* 0x1A8 */

extern Unk800F78A4 *D_801F0D2C;
extern s32 D_801F0D30; /* set when the units are put back */
extern s32 D_801F0D34; /* unit brightness */

void func_801EC738(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);
s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s16 unk0; /* kind */
    s16 unk2; /* state */
    s16 unk4; /* velocity */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* speed */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
} Fx0fb1Rock; /* 0x28 */

typedef struct {
    u8 pad0[0x4C];
    s32 unk4C; /* model */
    s32 unk50; /* model */
    u8 pad54[0x5C - 0x54];
    s32 unk5C; /* ground height */
    u8 pad60[0x6C - 0x60];
    SVECTOR unk6C; /* summon position */
    u8 pad74[0xC0 - 0x74];
    s32 unkC0; /* model */
} Fx0fb1Work80;

extern s32 D_801F0D38;

#endif
