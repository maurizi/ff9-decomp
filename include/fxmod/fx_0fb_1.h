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

s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
