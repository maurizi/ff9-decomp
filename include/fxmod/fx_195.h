#ifndef FX_195_H
#define FX_195_H

/* Effect module fx_195 (dir 13 file 0x195). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[13];
} Fx195Trail; /* 0x6C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[57];
} Fx195Trail57;

extern Unk800F78A4 *fx_195_env;

/* propagated */
void func_801E83BC(VECTOR *pos, SVECTOR *rot);

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx195Rgbc; /* primitive colour word */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx195Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    SVECTOR unk8;
    SVECTOR pos10; /* position */
    SVECTOR pos18;
    SVECTOR unk20; /* camera */
    u8 pad28[0x30 - 0x28];
    s16 unk30;
    s16 unk32;
    u8 pad34[0x44 - 0x34];
    s32 unk44;
    u8 pad48[0x54 - 0x48];
    Pool6CEntry *emit54;
    Pool6CEntry *emit58;
    s32 model5C; /* model */
    Fx195Vec3 unk60; /* model position */
    Fx195Vec3 unk6C; /* model scale */
    SVECTOR rot78; /* model rotation */
    s32 model80; /* models */
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
} Fx195Work80; /* 0x98 */

extern BtlModel fx_195_effModels[];
extern SVECTOR D_801EA228;
extern Fx195Trail D_801EA808[];
extern Fx195Trail57 D_801EAB08[];
extern Fx195Trail57 D_801EAEA0;
static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10);

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
