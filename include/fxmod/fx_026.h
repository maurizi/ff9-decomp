#ifndef FX_026_H
#define FX_026_H

/* Effect module fx_026 (dir 13 file 0x26): Shiva, full summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* keyframe track read from the effect file: count, then the keys */
typedef struct {
    s16 unk0;
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
    s16 unk12;
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
    u16 unk6;
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
    s32 unk64;
    s32 model68; /* models */
    s32 model6C;
} Fx026Work80; /* 0x70 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180;
    } *unk54;
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx026Summon;

/* op82: the barrier model */
typedef struct {
    s32 unk0;
    s32 frame4; /* frame */
    SVECTOR pos8; /* position */
    s32 model10; /* model */
} Fx026Work82; /* 0x14 */

/* pool item of fx_026_op82PoolTask */
typedef struct {
    s16 unk0;
    s16 strip2; /* index */
    s16 kind4; /* type */
    s16 pad6;
    s32 unk8;
} Fx026Item82;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 unk6; /* colour, 0 = gap */
} Fx026Pt;

typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx026Pt unk4[32];
} Fx026Strip32; /* 0x104 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx026Pt unk4[40];
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
extern BtlModel fx_026_effModels[];
extern SVECTOR fx_026_stripOrigin;
extern Fx026CamPath fx_026_camPaths[];
extern Unk800F78A4 *fx_026_env;
extern Fx026Shatter *fx_026_shatterSets[8];
extern Fx026Shard *fx_026_shatterShards[8];
extern s32 fx_026_shatterLine; /* shatter line */

static void fxm_keyGetVec(Fx026Track *track, s32 t, SVECTOR *out);
static void fxm_keyGetVec2(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b);
static void fxm_keyGetVec3(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
static s32 fxm_keyGetValue(Fx026Track *track, s32 t);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);
static void fxm_shatterInit(s32 i, s32 n, Fx026Shatter *buf, Fx026Tri *tris);
static s32 fxm_shatterAddModel(s32 i, s32 model);
static Fx026Shard *fxm_shatterFreeShard(s32 i);
static void fxm_shatterUpdate(s32 i);
static s32 fxm_shatterRelease(s32 i);
static s32 fx_026_op81PoolTask(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work);
static s32 fx_026_op82PoolTask(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work);

#endif
