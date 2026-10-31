#ifndef FX_0E3_1_H
#define FX_0E3_1_H

/* Effect module fx_0e3_1 (dir 13 file 0x0E3): Bahamut, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR unk4[57];
} Fx0e31Bolt;

extern Unk800F78A4 *fx_0e3_1_env;

static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 arg7, s32 arg8);
static void fxm_drawOffsetTrail2(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col);
void fxm_drawOffsetBranch(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 arg3);
static void fxm_drawOffsetTrail3(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col);
void fxm_rollTowardEye(VECTOR *pos, SVECTOR *rot);

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0e31Rgbc; /* primitive colour word */

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
} Fx0e31Vec3;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR center8; /* centre */
    Fx0e31Vec3 summonPos10; /* summon position */
    Fx0e31Vec3 summonScale1C; /* summon scale */
    SVECTOR rot28; /* summon rotation */
    SVECTOR pos30; /* hand position */
    SVECTOR pos38;
    SVECTOR camTarget40; /* camera target */
    SVECTOR swingBase48; /* point ahead of the hand bone, origin of the camera target swing (btlfx_pointAhead) */
    u8 pad50[8];
    SVECTOR handRot58; /* hand rotation */
    Pool6CEntry *emit60;
    Pool6CEntry *emit64;
    Pool6CEntry *emit68;
    s32 unk6C; /* unused (runtime watch, round 33) */
    Pool6CEntry *emit70;
    Pool6CEntry *emit74;
    s16 sound78;
    s16 sound7A;
    s32 model7C; /* model */
    SVECTOR rot80;
    Fx0e31Vec3 pos88;
    Fx0e31Vec3 scale94;
    s32 modelA0; /* model */
    Fx0e31Vec3 posA4;
    Fx0e31Vec3 scaleB0;
    SVECTOR rotBC;
    s32 modelC4; /* models */
    s32 modelC8;
    s32 modelCC;
    s32 modelD0;
    s32 modelD4;
    s32 modelD8;
    s32 modelDC;
    s32 modelE0;
} Fx0e31Work80; /* 0xE4 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR unk4[13];
} Fx0e31Bolt13; /* 0x6C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 unk3;
    SVECTOR unk4[5];
} Fx0e31Bolt5; /* 0x2C */

typedef struct {
    u8 pad0[0x198];
    BtlMotion *motion198;
    BtlMotion *motion19C;
} Fx0e31SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0e31SummonData *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e31Summon;

#define SUMMON(env) ((Fx0e31Summon *)(env)->loader)

extern SVECTOR fx_0e3_1_origin;
extern Fx0e31Bolt13 fx_0e3_1_arcs[];
extern Fx0e31Bolt fx_0e3_1_bolts[3];

#endif
