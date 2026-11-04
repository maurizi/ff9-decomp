#ifndef FX_1BD_H
#define FX_1BD_H

/* Effect module fx_1bd (dir 13 file 0x1bd): Ifrit, short summon. Fire drawn as a grid of texel quads
 * (fxm_drawTexelQuadsB). Evidence: btlfx_start and btlfx_loadFile get the same id for summons
 * (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_1bd effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_IFRIT_VEC_TRACK           0x6 /* table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x1D) */
#define FX_IFRIT_VEC_TRACK_3         0x7 /* table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x25) */
#define FX_IFRIT_POS_TRACK           0x8 /* table: position track (fxm_posTrackInit; byte-identical to fx_114 entry 0x23) */
#define FX_IFRIT_VEC_TRACK_2         0x9 /* table: vector track (fxm_vecTrackInit; byte-identical to fx_114 entry 0x24) */
#define FX_IFRIT_ORANGE_RINGS        0xA /* emitter: orange gouraud rings, 16 frames (byte-identical to fx_114 entry 0xB) */
#define FX_IFRIT_FLAME_RING          0xB /* model: flat ring, flame texture (byte-identical to fx_114 entry 0x22) */
#define FX_IFRIT_WHITE_HAZE          0xD /* model: grid with grey-white wavy streaks, the white haze under Ifrit's fist (fx_114's entry, played there) (byte-identical to fx_114 entry 0x2A); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_IFRIT_FLAME_DOME          0xE /* model: dome, flame texture (byte-identical to fx_114 entry 0x27) */
#define FX_IFRIT_RED_GLOW 0x12 /* emitter: orbiting fan, a horizontal orange-red glow (byte-identical to fx_114 entry 0x26, same render) */
#define FX_IFRIT_HEAT_DOME           0x13 /* model: hemisphere bowl, the translucent wavy dome (fx_114's entry, ablated together with its spoked ring there) (byte-identical to fx_114 entry 0x1E); runtime (docs/runtime-findings.md effect-data pass, low) (look pending: low) */
#define FX_IFRIT_SHOT0_CAMERA_KEYS_2 0x14 /* table: camera key channel (fxm_camChanAdd from frame 0 into env->camera[1]) */
#define FX_IFRIT_SHOT0_CAMERA_KEYS_1 0x15 /* table: camera key channel (fxm_camChanAdd from frame 0 into env->camera[0]) */
#define FX_IFRIT_TRAIL_PATHS         0x16 /* table: offset table of position paths (fxm_offsetTableEntry + fxm_posPathInit) for the fire trails (byte-identical to fx_114 entry 0x17) */
#define FX_IFRIT_FIRE_TRAIL          0x17 /* trail: orange strip, drawn along the paths of FX_IFRIT_TRAIL_PATHS (byte-identical to fx_114 entry 0x15) */
#define FX_IFRIT_ORANGE_RING_2       0x18 /* emitter: orange to white gouraud ring (byte-identical to fx_114 entry 0x16) */
#define FX_IFRIT_FIRE_BURST          0x26 /* emitter: ember specks + orange fire-burst clouds (byte-identical to fx_114 entry 0x21) */
#define FX_IFRIT_PIECE_TRACKS        0x28 /* table: offset table of tracks, one per piece (fxm_offsetTableEntry; byte-identical to fx_114 entry 0x29) */
#define FX_IFRIT_PIECE_MODELS        0x29 /* table: offset table of model data, one per piece (fxm_offsetTableEntry; byte-identical to fx_114 entry 0x28) */
#define FX_IFRIT_BLOCK_CLUSTER       0x2B /* model: cluster of block chunks, orange streak texture (byte-identical to fx_114 entry 0x20) */
#define FX_IFRIT_SPOKED_RING         0x2C /* model: flat ring with spokes (registered through the texture list, texture not drawn; byte-identical to fx_114 entry 0x12); runtime (docs/runtime-findings.md effect-data pass): fx_114's same entry is untextured and was only ablated together with the bowl (look pending) */

typedef struct {
    s16 x, y, z;
} Fx1bdPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1bdPoint *keys4;
} Fx1bdPath;

typedef struct {
    s16 x, y, z;
} Fx1bdKey6;

typedef struct {
    u16 count0; /* count */
    Fx1bdKey6 *keys4;
} Fx1bdTrack6;

typedef struct {
    s16 pos0, pos2, pos4; /* position */
    s16 rot6, rot8, rotA; /* rotation */
} Fx1bdKey12;

typedef struct {
    u16 count0; /* count */
    Fx1bdKey12 *keys4;
} Fx1bdTrack12;

extern BtlModelBuf fx_1bd_effModels[];


typedef struct {
    Fx1bdPath path0;
    s16 start8;
    s16 countA;
    s16 alphaC; /* alpha */
    s16 scaleE; /* btlfx_drawTrail width scale, 0x1000 (ignored: the call pushes no point) */
    s16 alpha10;
    s16 alpha12;
} Fx1bdLine;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
    SVECTOR unk2C; /* write-only (0, 0, 0) */
    VECTOR unk34; /* write-only (0, 0, 0x320) */
    VECTOR unk44; /* write-only (0x1000 each) */
    u8 pad54[4];
    SVECTOR pos58;
    s16 timer60; /* fade-in countdown */
    s16 timer62; /* fade-out countdown */
    u16 unk64; /* write-only: frame counter */
    u16 model66; /* model */
    u16 model68; /* model */
} Fx1bdWork82; /* 0x6C */

extern Fx1bdLine fx_1bd_pathTrail;
extern u8 fx_1bd_op82Trails[13][0x50]; /* trails */
extern u32 fx_1bd_bgOt[];

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_posPathInit(Fx1bdPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1bdPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1bd_env;
/* end copied */

/* propagated */

static void fxm_buildPathTrail(Fx1bdPath *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static u16 *fxm_offsetTableEntry(u16 *base, u16 idx);

typedef struct {
    s16 *keys0;
    s16 frame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx1bdTrack; /* 0x10 */
extern Fx1bdTrack fx_1bd_camKeys[];
#define FX_1BD_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_1bd_camChanFrames[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    SVECTOR rot0; /* rotation */
    VECTOR pos8; /* position */
    VECTOR scale18; /* scale */
} Fx1bdModel; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC; /* write-only: rotation */
    SVECTOR pos14; /* position (only copied into unk104) */
    s32 unk1C; /* write-only (0x1800) */
    Fx1bdModel xform20;
    Fx1bdModel xform48;
    Fx1bdModel xform70;
    s32 fade98;
    Fx1bdModel xform9C;
    s32 fadeC4;
    s32 fadeC8;
    Fx1bdModel xformCC;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR unk104; /* write-only: pos14 raised by 0xF0 */
    s16 frame10C; /* frame */
    s16 timer10E; /* fade countdowns */
    s16 timer110;
    s16 timer112;
    s16 timer114;
    s16 timer116;
    u16 model118[24]; /* models */
    u16 model148;
    u16 model14A;
    u16 model14C;
} Fx1bdWork83; /* 0x150 */

typedef struct {
    u8 frame0; /* start frame */
    u8 flag1; /* flags */
    u8 done2; /* done */
    u8 pad3;
    s16 speed4; /* speed */
    s16 vel6; /* lift */
    SVECTOR angle8; /* direction */
    SVECTOR rot10; /* rotation */
    VECTOR pos18; /* position */
} Fx1bdRock; /* 0x28 */
/* Fx1bdRock.flag1: picked when the piece launches (op 0x83), from its heading angle8.vy < 0x800; while it flies it spins
 * +0x40 a frame about x / z with SPIN_X_POS / SPIN_Z_POS, else -0x40. Evidence: [code] */
#define FX1BD_ROCK_SPIN_X_POS 0x1
#define FX1BD_ROCK_SPIN_X_NEG 0x2 /* set instead of SPIN_X_POS, never tested */
#define FX1BD_ROCK_SPIN_Z_POS 0x4
#define FX1BD_ROCK_SPIN_Z_NEG 0x8 /* set instead of SPIN_Z_POS, never tested */

extern Fx1bdRock fx_1bd_op83Pieces[23];
extern u8 fx_1bd_op83TexSelect[];
extern s16 fx_1bd_texInfo0[];
extern s16 fx_1bd_texPos0[];
extern s16 fx_1bd_texInfo1[];
extern s16 fx_1bd_texInfo2[];
extern s16 fx_1bd_texPos1[];
extern s16 fx_1bd_texPos2[];
extern s16 fx_1bd_texInfo3[];
extern s16 fx_1bd_texPos3[];
extern Fx1bdTrack6 fx_1bd_vecTrack0;
extern Fx1bdTrack12 fx_1bd_rotPosTrack[23];
extern Fx1bdTrack6 fx_1bd_posTrack;
extern Fx1bdTrack6 fx_1bd_vecTrack1;
extern Fx1bdTrack6 fx_1bd_vecTrack2;

static void fxm_posTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_posTrackGet(Fx1bdTrack6 *track, s32 t, SVECTOR *out);
static void fxm_rotPosTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx1bdTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx1bdTrack6 *track, s32 t, VECTOR *out);


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

#endif
