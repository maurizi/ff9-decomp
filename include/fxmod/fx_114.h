#ifndef FX_114_H
#define FX_114_H

/* Effect module fx_114 (dir 13 file 0x114): Ifrit, full summon. Fire drawn as a grid of texel quads
 * (fxm_drawTexelQuadsB). Evidence: btlfx_start and btlfx_loadFile get the same id for summons
 * (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_114 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_IFRIT_EMBERS              0x3 /* emitter: tiny ember specks orbiting */
#define FX_IFRIT_FIRE_RING           0x5 /* emitter: orange-red to white gouraud ring */
#define FX_IFRIT_BACKDROP            0x6 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_IFRIT_EMBER_TRAIL         0x7 /* emitter: orbiting screen trail (red strip) + ember sprites */
#define FX_IFRIT_ROCK_RINGS          0x8 /* emitter: rings textured with a golden rock pattern, 16 frames */
#define FX_IFRIT_FLAME_LINE          0x9 /* emitter: two orbiting fans, a thin horizontal orange flame line */
#define FX_IFRIT_ORANGE_RINGS        0xB /* emitter: orange gouraud rings, 16 frames */
#define FX_IFRIT_SPOKED_RING         0x12 /* model: flat ring with spokes (registered through the texture list; the tool does not follow it, so the texture is not drawn); runtime (docs/runtime-findings.md effect-data pass): untextured; only ablated together with the bowl, the change is attributed to the dome; pass 2: ablating fx_1bd's byte-identical entry alone changes only a few hundred scattered px (look pending) */
#define FX_IFRIT_FIRE_TRAIL          0x15 /* trail: orange strip, drawn along the paths of entry 0x17 */
#define FX_IFRIT_ORANGE_RING_2       0x16 /* emitter: orange to white gouraud ring */
#define FX_IFRIT_TRAIL_PATHS         0x17 /* table: offset table of position paths (fxm_offsetTableEntry + fxm_posPathInit) for the fire trails */
#define FX_IFRIT_FLAME_SWIRL         0x19 /* emitter: flat trail, flame swirl strip (byte-identical to entries 0x1A-0x1C) */
#define FX_IFRIT_FLAME_SWIRL_2       0x1A /* emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) */
#define FX_IFRIT_FLAME_SWIRL_3       0x1B /* emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) */
#define FX_IFRIT_FLAME_SWIRL_4       0x1C /* emitter: flat trail, flame swirl strip (byte-identical to entry 0x19) */
#define FX_IFRIT_VEC_TRACK           0x1D /* table: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_HEAT_DOME           0x1E /* model: hemisphere bowl, the large translucent wavy dome over the scene, a bright yellow-white glare (ablated together with entry 0x12); runtime (docs/runtime-findings.md effect-data pass; pass 2 ablated fx_1bd's byte-identical entry alone) */
#define FX_IFRIT_BLOCK_CLUSTER       0x20 /* model: cluster of block chunks, orange streak texture */
#define FX_IFRIT_FIRE_BURST          0x21 /* emitter: ember specks + orange fire-burst clouds */
#define FX_IFRIT_FLAME_RING          0x22 /* model: flat ring, flame texture */
#define FX_IFRIT_POS_TRACK           0x23 /* table: position track (fxm_posTrackInit) */
#define FX_IFRIT_VEC_TRACK_2         0x24 /* table: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_VEC_TRACK_3         0x25 /* table: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_RED_GLOW            0x26 /* emitter: orbiting fan, a horizontal orange-red glow */
#define FX_IFRIT_FLAME_DOME          0x27 /* model: dome, flame texture */
#define FX_IFRIT_PIECE_MODELS        0x28 /* table: offset table of model data, one per piece (fxm_offsetTableEntry) */
#define FX_IFRIT_PIECE_TRACKS        0x29 /* table: offset table of tracks, one per piece (fxm_offsetTableEntry) */
#define FX_IFRIT_WHITE_HAZE          0x2A /* model: grid on a page of grey-white wavy streaks by the time it is fetched, the white haze under Ifrit's fist; runtime (docs/runtime-findings.md effect-data pass, medium; pass 2 ablated fx_1bd's byte-identical entry: the grey streaky haze over the floor) */
#define FX_IFRIT_POS_PATH            0x2B /* table: position path (fxm_posPathInit) */
#define FX_IFRIT_POS_PATH_2          0x2C /* table: position path (fxm_posPathInit) */
#define FX_IFRIT_FIREBALL            0x80 /* summon data entry 0: 'so' model, ball with a fire texture and a flame crown */
#define FX_IFRIT_CANOPY              0x81 /* summon data entry 1: textured model, canopy on legs (registered through the texture list; the tool does not follow it, so the texture is not drawn); runtime (docs/runtime-findings.md effect-data pass): untextured model at run time; ablating it (with 0x83) leaves the screen pixel-identical (look pending) */
#define FX_IFRIT_FIRE_COLUMN         0x82 /* summon data entry 2: textured model, cylinder with a fire texture */
#define FX_IFRIT_CYLINDER            0x83 /* summon data entry 3: textured model, cylinder on tpage 0x117 (a yellow placeholder page in the render); runtime (docs/runtime-findings.md effect-data pass): fetched while its page still holds the yellow dummy .TIM placeholder; ablating it leaves the screen pixel-identical (look pending) */
#define FX_IFRIT_SUMMON_VEC_TRACK_4  0x84 /* summon data entry 4: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_GOURAUD_BOWL        0x85 /* summon data entry 5: gouraud model, grey-white gouraud bowl */
#define FX_IFRIT_SUMMON_VEC_TRACK_6  0x86 /* summon data entry 6: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_SUMMON_VEC_TRACK_7  0x87 /* summon data entry 7: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_SUMMON_VEC_TRACK_8  0x88 /* summon data entry 8: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_SUMMON_VEC_TRACK_9  0x89 /* summon data entry 9: vector track (fxm_vecTrackInit) */
#define FX_IFRIT_SUMMON_POS_PATH_10  0x8A /* summon data entry 10: position path (fxm_posPathInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_13 0x8D /* summon data entry 13: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_14 0x8E /* summon data entry 14: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_15 0x8F /* summon data entry 15: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_16 0x90 /* summon data entry 16: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_17 0x91 /* summon data entry 17: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_TRACK_18 0x92 /* summon data entry 18: position track (fxm_posTrackInit) */
#define FX_IFRIT_SUMMON_POS_PATH_20  0x94 /* summon data entry 20: position path (fxm_posPathInit) */
#define FX_IFRIT_SUMMON_VEC_PATH_21  0x95 /* summon data entry 21: vector path (fxm_vecPathInit) */
#define FX_IFRIT_SUMMON_POS_PATH_22  0x96 /* summon data entry 22: position path (fxm_posPathInit) */

/* fx_114 choreography (docs/fxdata.md, "Choreography literals"): op80's camera shots and Ifrit's motion keys, the
 * ops' sizes and lengths, trail buffers, model textures and the scrolled flame rect. The op80 effect keys inside
 * the shots and ops 81 .. 83 are still numeric. */
#define FX_IFRIT_MODEL_SLOTS        0x20  /* btlfx_initEffModels slots */
#define FX_IFRIT_SHOT1_FRAME        0x2F  /* op80: camera shot 1 (position tracks 16 / 15); shot 0 (paths 22 / 10) before it */
#define FX_IFRIT_MOTION_1_FRAME     0x30  /* summon motion 1 */
#define FX_IFRIT_SHOT2_FRAME        0x61  /* camera shot 2 (tracks 17 / 13); summon motion 2 the frame after */
#define FX_IFRIT_SHOT3_FRAME        0x7A  /* camera shot 3 (tracks 18 / 14); summon motion 3 the frame after */
#define FX_IFRIT_SHOT4_FRAME        0xA7  /* camera shot 4 (POS_PATH / POS_PATH_2); the tracks run until the frame after */
#define FX_IFRIT_SHOT4_END_FRAME    0xDE  /* shot 4's paths run until this frame */
#define FX_IFRIT_FRAMES             0x118 /* op80's length (the op ends) */
#define FX_IFRIT_SWIRL_TRAIL_SLOTS  1     /* the four FLAME_SWIRLs' trail buffers (fx_114_op80Trails1 .. 4): slots */
#define FX_IFRIT_SWIRL_TRAIL_STRIDE 0xD0  /* bytes per slot */
#define FX_IFRIT_EMBER_TRAIL_SLOTS  0x55  /* EMBER_TRAIL's (fx_114_op80Trails0): slots */
#define FX_IFRIT_EMBER_TRAIL_STRIDE 0x28  /* bytes per slot */
#define FX_IFRIT_OP81_FRAMES        0x14  /* op81's length */
#define FX_IFRIT_OP82_FRAMES        0x1E  /* op82's length */
#define FX_IFRIT_OP83_FRAMES        0x3C  /* op83's length */
#define FX_IFRIT_OP85_FRAMES        0x10  /* op85's length */
#define FX_IFRIT_FLAME_TPAGE        0x9A   /* Hi_RegisterTexEffModel texture of FIRE_COLUMN, FLAME_DOME, FLAME_RING: 8-bit page (0x280, 0x100), abr 0 */
#define FX_IFRIT_FLAME_CLUT         0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_IFRIT_FLAME_U            0      /* texture origin (u, v) in the page */
#define FX_IFRIT_FLAME_V            0
#define FX_IFRIT_CYLINDER_TPAGE     0x117  /* texture of CYLINDER and WHITE_HAZE: 15-bit page (0x1C0, 0x100), abr 0 */
#define FX_IFRIT_CYLINDER_CLUT      0x0
#define FX_IFRIT_CYLINDER_U         0
#define FX_IFRIT_CYLINDER_V         0
#define FX_IFRIT_BLOCK_CLUSTER_TPAGE 0x17   /* texture of BLOCK_CLUSTER: 4-bit page (0x1C0, 0x100), abr 0 */
#define FX_IFRIT_BLOCK_CLUSTER_CLUT  0x3D00 /* CLUT at (0x0, 0xF4) */
#define FX_IFRIT_BLOCK_CLUSTER_U     0x80
#define FX_IFRIT_BLOCK_CLUSTER_V     0xE0
#define FX_IFRIT_FLAME_VRAM_X       0x280 /* btlfx_drawScrollMove rect: the flame models' 0x80 x 0x80 8-bit texels (page 0x9A) */
#define FX_IFRIT_FLAME_VRAM_Y       0x100
#define FX_IFRIT_FLAME_VRAM_W       0x40
#define FX_IFRIT_FLAME_VRAM_H       0x80
#define FX_IFRIT_FLAME_SRC_X        0x240 /* from this source */
#define FX_IFRIT_FLAME_SRC_Y        0x100

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 color30; /* model brightness */
    SVECTOR pos34; /* summon bone position */
    SVECTOR pos3C;
    s16 timer44; /* fade-in countdown */
    u16 model46; /* model */
} Fx114Work81; /* 0x48 */

/* View of Unk800F78A44 up to the word fx_114 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x60];
    s32 summonModel60; /* summon model */
} Fx114Summon;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    SVECTOR unk2C; /* write-only (0, 0, 0) */
    VECTOR unk34; /* write-only (0, 0, 0x320) */
    VECTOR unk44; /* write-only (0x1000 each) */
    u8 pad54[0x58 - 0x54];
    SVECTOR pos58;
    s16 timer60; /* fade-in countdown */
    s16 timer62; /* fade-out countdown */
    u16 frame64; /* frame */
    u16 model66; /* model */
    u16 model68; /* model */
} Fx114Work82; /* 0x6C */


typedef struct {
    s16 pos0, pos2, pos4; /* position */
    s16 rot6, rot8, rotA; /* rotation */
} Fx114Key12;

typedef struct {
    u16 count0; /* count */
    Fx114Key12 *keys4;
} Fx114Track12;
static void fxm_posTrackInit(Fx114Track12 *track, u16 *data);

typedef struct {
    s16 x, y, z;
} Fx114Key6;

typedef struct {
    u16 count0; /* count */
    Fx114Key6 *keys4;
} Fx114Track6;
static s32 fxm_posTrackGet(Fx114Track6 *track, s32 t, SVECTOR *out);

typedef struct {
    s16 x, y, z;
} Fx114Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx114Point *keys4;
} Fx114Path;
static s32 fxm_posPathGet(Fx114Path *path, s32 t, SVECTOR *out);
static void fxm_rotPosTrackInit(Fx114Track12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecPathInit(Fx114Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx114Path *path, s32 t, VECTOR *out);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx114Path path0;
    s16 start8;
    s16 countA;
    s16 fadeC; /* brightness */
    s16 scaleE;
    s16 fadeFrom10;
    s16 fadeTo12;
} Fx114Wave;

extern Unk800F78A4 *fx_114_env;
extern Fx114Wave fx_114_pathTrail;
extern Fx114Track6 fx_114_vecTrack3; /* scale keys */
extern u32 fx_114_bgOt[];
extern u8 fx_114_op82Trails[13][0x50];

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_posPathInit(Fx114Path *path, u16 *data);
static void fxm_vecTrackInit(Fx114Track6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx114Track6 *track, s32 t, VECTOR *out);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */
extern SVECTOR fx_114_pinPoints[]; /* particle positions */
static s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

static void fxm_buildPathTrail(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static s32 fxm_offsetTableEntry(s32 base, s32 idx);

typedef struct {
    s16 *keys0;
    s16 frame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstSlotsA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx114Track; /* 0x10 */
extern Fx114Track fx_114_camKeys[];
#define FX_114_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_114_camChanFrames[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);


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
/* gte_rtps_b with a memory clobber: fx_114's copy of the fx_007 colour grid reloads c3 after it */
#define gte_rtps_bm() __asm__ volatile(".word\t0x4A180001" : : : "memory")

static void fxm_drawTexelQuadsB(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx114Vec3;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC; /* write-only: rotation */
    SVECTOR pos14; /* position (only copied into unk104) */
    s32 unk1C; /* write-only (0x1800) */
    SVECTOR rot20; /* body rotation */
    Fx114Vec3 pos28; /* body position */
    s32 pad34;
    Fx114Vec3 scale38; /* body scale */
    s32 pad44;
    SVECTOR rot48; /* rock rotation */
    Fx114Vec3 pos50; /* rock position */
    s32 pad5C;
    Fx114Vec3 scale60; /* rock scale */
    s32 pad6C;
    SVECTOR rot70;
    Fx114Vec3 pos78;
    s32 pad84;
    Fx114Vec3 scale88;
    s32 unk94; /* unused */
    s32 color98;
    SVECTOR rot9C;
    Fx114Vec3 posA4;
    s32 padB0;
    Fx114Vec3 scaleB4;
    s32 padC0;
    s32 colorC4;
    s32 colorC8;
    SVECTOR rotCC;
    Fx114Vec3 posD4;
    s32 padE0;
    Fx114Vec3 scaleE4;
    s32 padF0;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR unk104; /* write-only: pos14 raised by 0xF0 */
    s16 frame10C; /* frame */
    s16 timer10E;
    s16 timer110;
    s16 timer112;
    s16 timer114;
    s16 timer116;
    u16 model118[24]; /* models */
    u16 model148;
    u16 model14A;
    u16 model14C;
} Fx114Work83; /* 0x150 */

typedef struct {
    u8 frame0; /* start frame */
    u8 flags1; /* spin flags */
    u8 done2; /* landed */
    u8 pad3;
    s16 speed4; /* speed */
    s16 vel6; /* lift */
    SVECTOR angle8; /* direction */
    SVECTOR rot10; /* rotation */
    Fx114Vec3 pos18; /* position */
    s32 pad24;
} Fx114Rock; /* 0x28 */
/* Fx114Rock.flags1: picked when the piece launches (op 0x83), from its heading angle8.vy < 0x800; while it flies it spins
 * +0x40 a frame about x / z with SPIN_X_POS / SPIN_Z_POS, else -0x40. Evidence: [code] */
#define FX114_ROCK_SPIN_X_POS 0x1
#define FX114_ROCK_SPIN_X_NEG 0x2 /* set instead of SPIN_X_POS, never tested */
#define FX114_ROCK_SPIN_Z_POS 0x4
#define FX114_ROCK_SPIN_Z_NEG 0x8 /* set instead of SPIN_Z_POS, never tested */

extern Fx114Rock fx_114_op83Pieces[];
extern u8 fx_114_op83TexSelect[];
extern u8 fx_114_texInfo1[];
extern s16 fx_114_texPos1[];
extern u8 fx_114_texInfo2[];
extern u8 fx_114_texInfo3[];
extern s16 fx_114_texPos2[];
extern s16 fx_114_texPos3[];
extern u8 fx_114_texInfo4[];
extern s16 fx_114_texPos4[];
extern Fx114Track12 fx_114_rotPosTrack[];
extern Fx114Track12 fx_114_posTrack2;
extern Fx114Track6 fx_114_vecTrack4;
extern Fx114Track6 fx_114_vecTrack5;

/* Hi_RegisterTexListModel called with three arguments (the original leaves a3 unset) */
#define REG_TEX_LIST(ex) ((u16 (*)(void *, void *, void *))(ex)->Hi_RegisterTexListModel)

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[4]; /* summon motions */
    } *summon54;
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx114Summon80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    s32 padC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR rot20; /* summon rotation */
    Fx114Vec3 pos28; /* summon position */
    s32 pad34;
    Fx114Vec3 scale38; /* summon scale */
    s32 pad44;
    s32 color48;
    SVECTOR rot4C;
    SVECTOR pos54;
    s32 scale5C;
    SVECTOR rot60;
    Fx114Vec3 pos68;
    s32 pad74;
    Fx114Vec3 scale78;
    s32 pad84;
    SVECTOR rot88;
    Fx114Vec3 pos90;
    s32 pad9C;
    Fx114Vec3 scaleA0;
    s32 padAC;
    s32 colorB0;
    SVECTOR rotB4;
    Fx114Vec3 posBC;
    s32 padC8;
    Fx114Vec3 scaleCC;
    s32 padD8;
    s32 colorDC;
    SVECTOR rotE0;
    Fx114Vec3 posE8;
    s32 padF4;
    Fx114Vec3 scaleF8;
    s32 pad104;
    s32 color108;
    SVECTOR rot10C;
    Fx114Vec3 pos114;
    s32 pad120;
    Fx114Vec3 scale124;
    s32 pad130;
    SVECTOR pos134;
    SVECTOR pos13C;
    SVECTOR unk144; /* write-only (0, 0, -0x4B0) */
    SVECTOR pos14C;
    u8 pad154[8];
    u8 color15C;
    u8 color15D;
    u8 color15E;
    u8 pad15F;
    s32 index160;
    s32 loopMotion164; /* Hi_DrawSummonModel flags bit 0: 1 loops Ifrit's motion (before frame 0x30), 0 holds its last frame */
    s32 frame168;
    s16 frame16C;
    s16 timer16E;
    s16 timer170;
    s16 timer172;
    s16 timer174;
    s16 timer176;
    u16 model178;
    u16 model17A;
    u16 model17C;
    u16 model17E;
    u16 model180;
    u8 pad182[0x198 - 0x182];
} Fx114Work80; /* 0x198 */

extern BtlModelBuf fx_114_effModels[];
extern u8 fx_114_texInfo0[];
extern u8 fx_114_texPos0[];
extern Fx114Path fx_114_posPath0;
extern Fx114Path fx_114_posPath1;
extern Fx114Path fx_114_posPath2;
extern Fx114Path fx_114_posPath3;
extern Fx114Track6 fx_114_posTrack0;
extern Fx114Track6 fx_114_posTrack1;
extern Fx114Path fx_114_posPath4;
extern Fx114Path fx_114_vecPath;
extern Fx114Track6 fx_114_vecTrack0;
extern Fx114Track6 fx_114_vecTrack1;
extern Fx114Track6 fx_114_vecTrack2;
extern SVECTOR fx_114_emitPos0;
extern SVECTOR fx_114_emitPos1[4];
extern u8 fx_114_op80Trails1[];
extern u8 fx_114_op80Trails2[];
extern u8 fx_114_op80Trails3[];
extern u8 fx_114_op80Trails4[];
extern u8 fx_114_op80Trails0[];
extern u16 fx_114_texelColors[];
extern u8 fx_114_pinBones[];

#endif
