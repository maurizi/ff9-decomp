#ifndef FX_026_H
#define FX_026_H

/* Effect module fx_026 (dir 13 file 0x26): Shiva, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_026 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHIVA_STREAK_PATH      0x2 /* track: path of the ice streak strips (op82 pool items) */
#define FX_SHIVA_ICE_CRESCENTS    0x4 /* emitter: blue ice crescent sprites + small shard sprites */
#define FX_SHIVA_ICE_SHARD_BURST  0x5 /* emitter: four ice-crescent sprite elements + cyan-white gouraud rings */
#define FX_SHIVA_ICE_CHUNKS       0x6 /* emitter: falling ice chunk sprites, faint sparkles */
#define FX_SHIVA_STAR_GLINTS      0x7 /* emitter: blue glows, white / green star glints, blue glow domes */
#define FX_SHIVA_PATH             0xB /* track: keyframed position (camera point 2) in op80 state 1 */
#define FX_SHIVA_PATH_2           0xE /* track: keyframed position (camera point 2) in op80 state 2 */
#define FX_SHIVA_PATH_3           0x11 /* track: keyframed position in op80 state 3 */
#define FX_SHIVA_ICE_SPRAY        0x17 /* emitter: white ice chunk sprites */
#define FX_SHIVA_ICE_CHUNKS_2     0x19 /* emitter: falling ice chunk sprites, faint sparkles */
#define FX_SHIVA_SHATTER_RINGS    0x1A /* emitter: blue gouraud rings + ice chunk sprites, sparks (pool item per piece) */
#define FX_SHIVA_BLUE_TRAIL       0x1F /* emitter: spiralling trail, blue strip texture */
#define FX_SHIVA_ICE_DEBRIS       0x20 /* emitter: small ice chunk sprites */
#define FX_SHIVA_ICE_DEBRIS_2     0x21 /* emitter: small ice chunk sprites */
#define FX_SHIVA_CYAN_RING        0x22 /* emitter: cyan-white gouraud ring */
#define FX_SHIVA_ICE_STREAK_3     0x34 /* trail: blue ice strip (third streak kind) */
#define FX_SHIVA_ICE_STREAK_2     0x35 /* trail: blue ice strip (second streak kind) */
#define FX_SHIVA_ICE_STREAK       0x36 /* trail: blue ice strip */
#define FX_SHIVA_SO_MODEL         0x85 /* 'so' model registered at the end of op80 state 1 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_TRACK_EMITTER    0x86 /* emitter started at each op80 pool item's track point (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_SO_MODEL_2       0x87 /* 'so' model registered with 0x85 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_STREAK_PATH_2    0x9D /* track: path of the ice streak strips (op82 pool items) (summon data 0x80+, not analysed by the tool) */
#define FX_SHIVA_IMPACT_EMITTER_2 0x9E /* emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_IMPACT_EMITTER   0x9F /* emitter at the target, op82 frame 39 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_BONE19_RINGS     0xA0 /* rings drawn at summon bone 0x19, growing (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_ROOT_EMITTER_3   0xA1 /* emitter at summon bone 0, op80 state 2 start (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_ROOT_EMITTER     0xA2 /* emitter at summon bone 0, op80 state 1 frame 38 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_ROOT_EMITTER_2   0xA3 /* emitter at summon bone 0, op80 state 1 frame 42 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_ROOT_EMITTER_4   0xA4 /* emitter at summon bone 0, op80 state 2 end (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_BONE19_EMITTER_2 0xA5 /* emitter at summon bone 0x19, op80 state 3 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_BONE19_EMITTER   0xA6 /* emitter at summon bone 0x19, op80 state 2 frame 29 (summon data 0x80+, not analysed by the tool) (look pending) */
#define FX_SHIVA_SPIN_MODEL       0xAB /* 'so' model drawn spinning and widening at the target (op82) (summon data 0x80+, not analysed by the tool) (look pending) */

/* keyframe track read from the effect file: count, then the keys */
typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx026Track;

/* shattered model: triangles (three vertices each) that fall off as particles */
typedef struct {
    SVECTOR vert0; /* pad: already split off */
    SVECTOR vert8;
    SVECTOR vert10;
} Fx026Tri; /* 0x18 */

typedef struct {
    s16 life0;
    s16 triCount2; /* triangle count */
    s32 lastCount4; /* triangles of the last model */
    Fx026Tri *tris8;
    Fx026Tri *nextTriC; /* next free triangle */
    Fx026Tri *lastTris10; /* first triangle of the last model */
} Fx026Shatter; /* 0x14 */

typedef struct {
    s32 x0; /* position (fixed 20.12) */
    s32 y4;
    s32 z8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 drag12; /* velocity factor per frame (btlfx_stepModuleParticle), 0x1000 = none */
    s16 spin14; /* spin */
    s16 spin16;
    s16 frame18; /* frame, -1 = free */
    s16 tri1A; /* triangle */
} Fx026Shard; /* 0x1C */

typedef struct {
    s32 path0; /* camera path */
    SVECTOR target4; /* camera target */
    SVECTOR posC; /* camera position */
} Fx026Work87; /* 0x14 */

/* camera paths: position track, target track, length */
typedef struct {
    u16 posTrack0;
    u16 targetTrack2;
    u16 length4;
    u16 unk6; /* not read by the module */
} Fx026CamPath;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx026Vec3;

/* pool item of fx_026_op80PoolTask: a model following a keyframe track */
typedef struct {
    s16 done0; /* done */
    u16 spin2; /* spin */
    s16 track4; /* track: fx_026_op80TrackBlocks index */
    s16 pad6;
    Pool6CEntry *emit8;
    SVECTOR posC;
} Fx026Item;

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    u8 pad8[8];
    SVECTOR pos10;
    u8 pad18[0x10];
    SVECTOR pos28; /* summon bone positions */
    SVECTOR pos30;
    SVECTOR pos38;
    Fx026Vec3 pos40; /* summon position */
    Fx026Vec3 scale4C; /* summon scale */
    SVECTOR rot58; /* summon rotation */
    u8 pad60[4];
    s32 unk64; /* unused (runtime watch, round 33) */
    s32 model68; /* models */
    s32 model6C;
} Fx026Work80; /* 0x70 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx026Summon;

/* op82: the barrier model */
typedef struct {
    s32 unk0; /* cleared at init and tested for 0, never set (always 0) */
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    s32 model10; /* model */
} Fx026Work82; /* 0x14 */

/* pool item of fx_026_op82PoolTask */
typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 strip2; /* index */
    s16 kind4; /* type */
    s16 pad6;
    s32 unk8; /* write-only (cleared at spawn) */
} Fx026Item82;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal; 0 = gap) */
} Fx026Pt;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx026Pt pts4[32]; /* ring points (btlfx_pushTrailPoint) */
} Fx026Strip32; /* 0x104 */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx026Pt pts4[40]; /* ring points (btlfx_pushTrailPoint) */
} Fx026Strip40; /* 0x144 */

extern Fx026Strip32 fx_026_strips0[];
extern Fx026Strip32 fx_026_strips1[];
extern Fx026Strip40 fx_026_strips2[];
/* op81: the shattering barrier */
typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    s32 pieceModels18[6][3]; /* models of the six pieces */
    s32 pad60[3];
    s32 model6C[7];
} Fx026Work81; /* 0x88 */

/* pool item of fx_026_op81PoolTask */
typedef struct {
    s16 state0; /* state */
    s16 piece2; /* piece */
    s32 pad4[2];
    SVECTOR posC;
} Fx026Item81;

typedef struct {
    s32 setOfs0; /* offsets from env->workBuf */
    s32 trisOfs4;
} Fx026ShatterSrc;

extern s32 fx_026_pieceModelBlocks[6][3];
extern s32 fx_026_pieceTrackBlocks[];
extern SVECTOR fx_026_pieceOffsets[];
extern s32 fx_026_pieceStages[];
extern Fx026ShatterSrc fx_026_shatterBufs[];
extern s32 fx_026_op80Model;
extern s32 fx_026_op80Model2;
extern Fx026Shatter fx_026_shatterBuf6[];
extern Fx026Tri fx_026_shatterTris6[];
extern s32 fx_026_op81ModelBlocks[7];
extern SVECTOR fx_026_op81Pos;
extern s32 fx_026_pieceOrder[];
extern s32 fx_026_shatterNow;
extern s32 fx_026_shatterNow6;
extern s32 fx_026_shatterNow5;
extern u8 fx_026_trails[];
extern s16 fx_026_op80TrackBlocks[];
extern BtlModelBuf fx_026_effModels[];
extern SVECTOR fx_026_stripOrigin;
extern Fx026CamPath fx_026_camPaths[];
extern Unk800F78A4 *fx_026_env;
extern Fx026Shatter *fx_026_shatterSets[8];
extern Fx026Shard *fx_026_shatterShards[8];
extern s32 fx_026_shatterLine; /* shatter line */

static void fxm_keyGetVec(Fx026Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1);
static void fxm_keyGetVec3(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2);
static s32 fxm_keyGetValue(Fx026Track *track, s32 t);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);
static void fxm_shatterInit(s32 slot, s32 life, Fx026Shatter *buf, Fx026Tri *tris);
static s32 fxm_shatterAddModel(s32 slot, s32 model);
static Fx026Shard *fxm_shatterFreeShard(s32 slot);
static void fxm_shatterUpdate(s32 slot);
static s32 fxm_shatterRelease(s32 slot);
static s32 fx_026_op81PoolTask(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work);
static s32 fx_026_op82PoolTask(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work);

#endif
