#ifndef FX_0E3_1_H
#define FX_0E3_1_H

/* Effect module fx_0e3_1 (dir 13 file 0x0E3): Bahamut, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_0e3_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BAHAMUT_SMOKE_RING         0x1 /* 'so' model: ring band with a grey-brown smoke texture */
#define FX_BAHAMUT_EMBERS             0x4 /* emitter: spiral of tiny orange sprites */
#define FX_BAHAMUT_FIRE_BURST         0x5 /* emitter: red-white fire flashes */
#define FX_BAHAMUT_BLUE_SPARKS        0x6 /* emitter: blue-white spark sprites falling */
#define FX_BAHAMUT_LENS_FLARE         0x7 /* sprite line: lens-flare set (rainbow halo, yellow-green disc, white glow, orange ring, blue disc); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_FIRE_RING          0x8 /* 'so' model: ring band with a fire texture */
#define FX_BAHAMUT_FIRE_SPARKS        0x9 /* emitter: orange flame glows + spark sprites */
#define FX_BAHAMUT_PURPLE_BOLT        0xA /* trail: purple energy strip (fxm_drawOffsetTrail2) */
#define FX_BAHAMUT_CHARGE_RINGS       0xB /* rings: white-orange-blue gouraud rings while charging */
#define FX_BAHAMUT_BLUE_SPARKS_2      0xC /* emitter: blue-white spark sprites falling */
#define FX_BAHAMUT_FLAME_STRIP        0xD /* 'so' model: flat gouraud strip, purple-red-yellow gradient, drawn with BLAST_MODEL */
#define FX_BAHAMUT_ORANGE_STREAKS     0xE /* emitter: two fans, thin orange-white rising streaks */
#define FX_BAHAMUT_SMOKE_SHELL 0xF /* 'so' model: half-cylinder shell on a page of grey cloudy smoke at run time; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_BAHAMUT_WHITE_BOLT_2       0x11 /* trail: the white lightning strip; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_SPIN_RING          0x12 /* rings: white-blue gouraud rings, spinning */
#define FX_BAHAMUT_BLUE_RINGS         0x14 /* rings: dark-to-light blue gouraud rings */
#define FX_BAHAMUT_WHITE_BOLT         0x15 /* trail: the white lightning strip (byte-identical to fx_0e3_0 entry 0x8); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_BAHAMUT_BACKDROP           0x16 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_BAHAMUT_ENERGY_ORBS        0x17 /* emitter: cyan-green orbs + violet lightning strip */
#define FX_BAHAMUT_SPARKLE_RING       0x18 /* sprites: ring of blue-white sparkles, 16 frames */
#define FX_BAHAMUT_FIRE_RING_BURST    0x19 /* emitter: red-white fire flashes + crimson rings */
#define FX_BAHAMUT_VIOLET_RINGS       0x1A /* rings: white-cyan-violet gouraud rings */
#define FX_BAHAMUT_PINK_FLAT_RING     0x1B /* rings: white-salmon gouraud rings drawn flat */
#define FX_BAHAMUT_HALO_RINGS         0x1C /* rings: white-crimson-blue gouraud rings */
#define FX_BAHAMUT_PURPLE_GLOW        0x1D /* emitter: fan, a purple-blue glow */
#define FX_BAHAMUT_WHITE_FLASH_RING   0x1F /* rings: white gouraud rings, the flash */
#define FX_BAHAMUT_EMBERS_2           0x20 /* emitter: spiral of tiny orange sprites */
#define FX_BAHAMUT_BLUE_SLIVER        0x21 /* 'so' model: thin blue-white gouraud sliver, drawn at the blast centre */
#define FX_BAHAMUT_LIGHTNING_BOLT     0x23 /* trail: violet lightning strip (fxm_drawOffsetTrail3) */
#define FX_BAHAMUT_FIRE_DOME          0x25 /* 'so' model: dome with a fire texture, drawn for the burst and the flash */
#define FX_BAHAMUT_RED_STRIP          0x26 /* 'so' model: flat red gouraud strip with a bright centre line */
#define FX_BAHAMUT_PURPLE_GLOW_2      0x29 /* emitter: fan, a purple-blue glow */
#define FX_BAHAMUT_GREY_CONE          0x81 /* summon data: 'so' model, grey gouraud cone (dark base to white tip), drawn around the orb */
#define FX_BAHAMUT_SMOKE_ARC_2        0x84 /* summon data: 'so' model, curved band with a grey smoke texture */
#define FX_BAHAMUT_CLOUD_SPHERE       0x85 /* summon data: 'so' model, sphere with a blue-white cloud texture, drawn fading at the end (same entry as fx_0e3_0's) */

/* fx_0e3_1 choreography (docs/fxdata.md, "Choreography literals"; partial: the work size, strip shapes, bone count
 * and primitive words; op80's state machine is still numeric). */
#define FX_BAHAMUT_ARC_POINTS       13   /* points of a WHITE_BOLT arc (Fx0e31Bolt13), */
#define FX_BAHAMUT_ARC_WIDTH        0x1C /* their width */
#define FX_BAHAMUT_SPARK_POINTS     5    /* points of a WHITE_BOLT_2 spark (Fx0e31Bolt5), */
#define FX_BAHAMUT_SPARK_WIDTH      0x1A /* their width */
#define FX_BAHAMUT_BOLT_POINTS      57   /* points of a PURPLE_BOLT strip (Fx0e31Bolt) */
#define FX_BAHAMUT_LIGHTNING_POINTS 13   /* points of a LIGHTNING_BOLT strip */
#define FX_BAHAMUT_SUMMON_BONES     92   /* the summon model's bones: random arc ends (Hi_GetSummonBonePos(rand() % this)) */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_BAHAMUT_BOLT_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e31Bolt;

extern Unk800F78A4 *fx_0e3_1_env;

static void fxm_drawOffsetTrail0(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade);
static void fxm_drawOffsetTrail1(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade);
static void fxm_drawOffsetTrail2(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
void fxm_drawOffsetBranch(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 fade);
static void fxm_drawOffsetTrail3(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
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
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
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
    u8 pad3;
    SVECTOR pts4[FX_BAHAMUT_ARC_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e31Bolt13; /* 0x6C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_BAHAMUT_SPARK_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
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
