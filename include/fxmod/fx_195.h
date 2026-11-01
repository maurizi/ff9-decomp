#ifndef FX_195_H
#define FX_195_H

/* Effect module fx_195 (dir 13 file 0x195): Bahamut (short); Mega Flare. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[13];
} Fx195Trail; /* 0x6C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[57];
} Fx195Trail57;

extern Unk800F78A4 *fx_195_env;

/* propagated */
void fxm_rollTowardEye(VECTOR *pos, SVECTOR *rot);

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
    s32 state0; /* state */
    s32 frame4; /* counter */
    SVECTOR unk8; /* write-only (copy of fx_195_basePos) */
    SVECTOR pos10; /* position */
    SVECTOR pos18;
    SVECTOR camPos20; /* camera */
    u8 pad28[0x30 - 0x28];
    s16 sound30;
    s16 sound32;
    u8 pad34[0x44 - 0x34];
    s32 unk44; /* write-only (cleared at init) */
    u8 pad48[0x54 - 0x48];
    Pool6CEntry *emit54;
    Pool6CEntry *emit58;
    s32 model5C; /* model */
    Fx195Vec3 pos60; /* model position */
    Fx195Vec3 scale6C; /* model scale */
    SVECTOR rot78; /* model rotation */
    s32 model80; /* models */
    s32 model84;
    s32 model88;
    s32 model8C;
    s32 model90;
    s32 model94;
} Fx195Work80; /* 0x98 */

extern BtlModelBuf fx_195_effModels[];
extern SVECTOR fx_195_basePos;
extern Fx195Trail fx_195_shortTrails[];
extern Fx195Trail57 fx_195_longTrails[];
extern Fx195Trail57 fx_195_longTrail2;
static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10);

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
