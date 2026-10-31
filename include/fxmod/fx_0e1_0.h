#ifndef FX_0E1_0_H
#define FX_0E1_0_H

/* Effect module fx_0e1_0 (dir 13 file 0x0E1): Rebirth Flame (Phoenix's second attack), load group 0. Fire / ice
 * drawn as a grid of texel quads (fxm_drawTexelQuads). Also fxm_drawTexRibbonTrail. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* the effect environment block as these callbacks read it */
typedef struct {
    u8 pad0[0x60];
    s32 summonModel60; /* summon model */
} Fx0e10EnvB;

typedef struct {
    SVECTOR a0; /* pad: point valid */
    SVECTOR b8;
} Fx0e10Seg; /* 0x10: one step of the ribbon history */

extern s32 fx_0e1_0_spawnBones[];
extern s32 fx_0e1_0_spawnIndex;
extern s32 fx_0e1_0_boneSpawnOn;
extern Unk800F78A4 *fx_0e1_0_env;

static void fxm_drawTexRibbonTrail(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10Seg *buf, s32 abr, s32 phase);
void fx_0e1_0_drawWaveQuadGrid(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy);
static s32 fx_0e1_0_op80SpawnPoint(SVECTOR *out);
/* Never called (no reference, no hit on an exec breakpoint over Rebirth Flame, runtime round 33). */
void fx_0e1_0_scaleXY(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e10Vec3;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* summon position */
    Fx0e10Vec3 summonPos10; /* summon model position */
    Fx0e10Vec3 summonScale1C; /* summon model scale */
    SVECTOR rot28; /* summon model rotation */
    s32 unk30; /* unused (runtime watch, round 33) */
    SVECTOR pos34; /* bone position */
    SVECTOR camPos3C;
    Pool6CEntry *emit44;
    Pool6CEntry *emit48;
    s32 ribbonReset4C; /* ribbons need a reset */
    s32 model50;
    s32 model54;
    s32 model58;
    s32 model5C;
    s32 model60;
    SVECTOR rot64;
    Fx0e10Vec3 morphPos6C;
    Fx0e10Vec3 morphScale78;
    Fx0e10Vec3 pos84;
    Fx0e10Vec3 scale90;
    SVECTOR rot9C;
} Fx0e10Work80; /* 0xA4 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0e10Summon;

typedef struct {
    s32 boneA0;
    s32 boneB4;
} Fx0e10BonePair;

extern SVECTOR fx_0e1_0_origin;
extern Fx0e10BonePair fx_0e1_0_ribbonBones[4];
extern u8 fx_0e1_0_gradient[];
extern Fx0e10Seg fx_0e1_0_ribbons[4][21];
extern BtlModel fx_0e1_0_effModels[];
extern u8 fx_0e1_0_op80Trails0[];
extern u8 fx_0e1_0_op80Trails1[];

/* propagated */

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))
static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
