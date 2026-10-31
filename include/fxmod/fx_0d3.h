#ifndef FX_0D3_H
#define FX_0D3_H

/* Effect module fx_0d3 (dir 13 file 0xD3): Phoenix, full summon. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Also fxm_drawTexRibbonTrail. Evidence: btlfx_start and btlfx_loadFile get the same id for
 * summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_0d3_env;
extern s32 fx_0d3_spawnBones[5];
extern s32 fx_0d3_spawnBoneNext;
extern u16 fx_0d3_camKeyBlocks[3]; /* file ids for op87 tracks, end frame */
extern s32 fx_0d3_waveAmp;
extern s32 fx_0d3_wavePhase;

static void fx_0d3_op80GridVertex(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
static s32 fx_0d3_op81SpawnPoint(SVECTOR *v);
static void fxm_waveVertexX(s16 *v);
typedef struct {
    s16 unk0;
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
static void fxm_keyGetVec(Fx0d3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b);
static void fxm_keyGetVec3(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
static s32 fxm_keyGetValue(Fx0d3Track *track, s32 t);
/* ribbon history: two edge points per step, edgeA0.pad = live */
typedef struct {
    SVECTOR edgeA0;
    SVECTOR edgeB8;
} Fx0d3Seg; /* 0x10 */

#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

static void fxm_drawTexRibbonTrail(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 k);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0d3Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8; /* summon position */
    Fx0d3Vec3 summonPos10;
    Fx0d3Vec3 summonScale1C; /* summon scale */
    SVECTOR rot28; /* summon rotation */
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
    u8 pad38[0x60 - 0x38];
    s32 model60; /* models */
    s32 model64;
    s32 scroll68; /* tint */
} Fx0d3Work80; /* 0x6C */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180;
    } *unk54;
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d3Summon;

extern SVECTOR fx_0d3_summonPos;
extern s32 fx_0d3_sharedModel;
extern u8 fx_0d3_trails[];
extern BtlModel fx_0d3_effModels[];
s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8;
    Fx0d3Vec3 summonPos10; /* summon position */
    Fx0d3Vec3 summonScale1C; /* summon scale */
    SVECTOR rot28; /* summon rotation */
    SVECTOR pos30;
    SVECTOR pos38;
    SVECTOR camAt40; /* camera */
    u8 pad48[4];
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    s32 model54; /* models */
    s32 model58;
    s32 model5C;
    s32 model60;
} Fx0d3Work81; /* 0x64 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[4];
    } *unk54;
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d3Summon81;

typedef struct {
    s32 bone0;
    s32 bone4;
} Fx0d3BonePair;

extern SVECTOR fx_0d3_centerPos;
extern Fx0d3BonePair fx_0d3_ribbonBones[6];
extern CVECTOR fx_0d3_rippleColors[];
extern CVECTOR fx_0d3_rippleColors2[];
extern CVECTOR fx_0d3_rippleColors3[];
extern u8 fx_0d3_trails2[];
extern u8 fx_0d3_trails3[];
extern Fx0d3Seg fx_0d3_ribbonSegs[6][12];
extern u16 fx_0d3_texels[];
s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */
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
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
