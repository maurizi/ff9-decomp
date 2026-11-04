#ifndef FX_0D3_H
#define FX_0D3_H

/* Effect module fx_0d3 (dir 13 file 0xD3): Phoenix, full summon. Fire / ice drawn as a grid of texel quads
 * (fxm_drawTexelQuads). Also fxm_drawTexRibbonTrail. Evidence: btlfx_start and btlfx_loadFile get the same id for
 * summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0d3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PHOENIX_RED_RINGS        0x1 /* emitter: grey and white-red gouraud rings */
#define FX_PHOENIX_FIRE_TRAILS      0x2 /* emitter: fire-textured trails and a swirling ember ring */
#define FX_PHOENIX_GLOW_RINGS       0x3 /* emitter: pale yellow / grey gouraud rings (+ textured rings, texture not in the file) */
#define FX_PHOENIX_PEACH_GLOW       0x4 /* emitter: two fans, a narrow peach half-disc glow */
#define FX_PHOENIX_RAINBOW_FAN      0x5 /* emitter: fan of a rainbow gradient strip, the prismatic light rays behind Phoenix; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_FEATHERS         0x6 /* emitter: two fans, grey and tan feathers with light streaks */
#define FX_PHOENIX_LENS_FLARE       0x7 /* sprite line: lens-flare set (rainbow halo ring, yellow-green disc, white glow, red ring, blue disc) across Phoenix; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_FIRE_TRAILS_2    0xB /* emitter: orbiting fire-textured trails */
#define FX_PHOENIX_FIRE_DOME        0xC /* 'so' model: dome with a fire texture */
#define FX_PHOENIX_FIRE_DISC        0xD /* 'so' model: flat disc with a fire texture */
#define FX_PHOENIX_SMOKE_ARC        0xE /* 'so' model: thin arc with a grey smoke texture */
#define FX_PHOENIX_EMBERS           0xF /* emitter: glowing ember sprites */
#define FX_PHOENIX_FIRE_FEATHERS    0x10 /* emitter: orbiting fire trails and flame-feather quads */
#define FX_PHOENIX_PINK_RINGS       0x11 /* emitter: white / pink / copper gouraud rings */
#define FX_PHOENIX_GOLD_FEATHERS    0x12 /* emitter: golden feather quads */
#define FX_PHOENIX_FEATHERS_2       0x13 /* emitter: two fans, grey and tan feathers with light streaks */
#define FX_PHOENIX_RAINBOW_FAN_2    0x14 /* emitter: the same rainbow gradient fan, later rays; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_PHOENIX_BACKDROP         0x15 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_PHOENIX_FLAME_SWIRL      0x16 /* 'so' model: cone of swirling brown-orange flame tongues (fx_0d3_sharedModel) */
#define FX_PHOENIX_FLAME_FEATHERS   0x17 /* emitter: flame-feather quads */
#define FX_PHOENIX_FLAME_FEATHERS_2 0x18 /* emitter: flame-feather quads (cylinder) */
#define FX_PHOENIX_GOLD_FEATHERS_2  0x19 /* emitter: golden feather sprites */
#define FX_PHOENIX_EMBERS_2         0x1A /* emitter: glowing ember sprites */
#define FX_PHOENIX_COPPER_RINGS     0x1B /* emitter: orbiting copper / brown gouraud rings */
#define FX_PHOENIX_EMBERS_3         0x1C /* emitter: glowing ember sprites and red sparks */
#define FX_PHOENIX_FLAME_FEATHERS_3 0x1D /* emitter: flame-feather quads */
#define FX_PHOENIX_FLAME_SWIRL_2    0x26 /* 'so' model: cone of swirling brown-orange flame tongues (other texture page) */

extern Unk800F78A4 *fx_0d3_env;
extern s32 fx_0d3_spawnBones[5];
extern s32 fx_0d3_spawnBoneNext;
extern u16 fx_0d3_camKeyBlocks[3]; /* file ids for op87 tracks, end frame */
extern s32 fx_0d3_waveAmp;
extern s32 fx_0d3_wavePhase;

static void fx_0d3_op80GridVertex(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v);
static s32 fx_0d3_op81SpawnPoint(SVECTOR *v);
static void fxm_waveVertexX(s16 *vert);
typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx0d3Track;
static void fxm_keyGetVec(Fx0d3Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1);
static void fxm_keyGetVec3(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2);
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

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
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
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx0d3Summon;

extern SVECTOR fx_0d3_summonPos;
extern s32 fx_0d3_sharedModel;
extern u8 fx_0d3_trails[];
extern BtlModelBuf fx_0d3_effModels[];
s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

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
        BtlMotion *motion180[4];
    } *summon54; /* g_btlfxLoader.summon */
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
s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

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
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
