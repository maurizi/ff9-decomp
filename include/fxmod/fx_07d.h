#ifndef FX_07D_H
#define FX_07D_H

/* Effect module fx_07d (dir 13 file 0x7D): Flare (party and enemy). Radial screen grid twisted toward the centre
 * (fxm_drawRadialScreenGrid, fxm_twistVertex), rings, orbs and arcs (fxm_drawOffsetTrail). Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_07d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLARE_RED_VIOLET_RINGS 0x1 /* emitter: white-red and white-blue-violet gouraud rings */
#define FX_FLARE_ARCH             0x2 /* model: half-ring arch, white texture */
#define FX_FLARE_ORANGE_GLOW      0x3 /* particles: orange glow with a white core */
#define FX_FLARE_PINK_RING        0x5 /* rings: white-pink-red gouraud rings */
#define FX_FLARE_BLUE_LIGHTNING   0x8 /* trail: blue lightning strip */
#define FX_FLARE_WHITE_RING       0x9 /* rings: white-cream gouraud rings */
#define FX_FLARE_RED_SPARKS       0xB /* emitter: red spark sprites + white-red rings */
#define FX_FLARE_RING_DISC        0xF /* model: gouraud flat annulus */
#define FX_FLARE_BACKDROP         0x11 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_FLARE_CALLBACK_EMITTER 0x12 /* emitter: orbit, one callback element, nothing drawable in the data (look pending) */
#define FX_FLARE_FIRE_RING        0x13 /* rings: white-cyan-orange-red gouraud rings */
#define FX_FLARE_WHITE_FLASH      0x14 /* emitter: two fans drawn as solid white flash quads */
#define FX_FLARE_BACKDROP_2       0x15 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_FLARE_FIRE_DOME        0x16 /* model: dome with a fiery orange texture */
#define FX_FLARE_BACKDROP_3       0x17 /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_FLARE_TEX_RING         0x18 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time (look pending) */
#define FX_FLARE_TEX_RING_2       0x19 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time (look pending) */
#define FX_FLARE_SPARK_GLOW       0x1A /* emitter: red spark sprites + fading orange glow */
#define FX_FLARE_GOLDEN_GLOWS     0x1B /* emitter: two fans drawn as small golden glow spots */
#define FX_FLARE_CLOUD_SPHERE     0x1E /* model: sphere with a grey cloudy texture */

typedef struct {
    SVECTOR from0; /* from */
    SVECTOR to8; /* to */
    s32 strip10; /* buffer index */
    s32 *jitter14; /* per-point (t offset, amplitude, step) */
} Fx07dArc;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[15];
} Fx07dStrip; /* 0x7C */

extern Unk800F78A4 *fx_07d_env;
extern SVECTOR fx_07d_sparkPos[24];
extern s32 fx_07d_sparkPosIdx;
extern Unk80100C78Entry *fx_07d_task;
extern s32 fx_07d_twist;
extern Fx07dStrip fx_07d_strips[];
extern s32 fx_07d_jitter[64][3]; /* random arc profile */

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 scale2; /* scale */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* glow rotation */
    s16 spin14; /* spin angle */
    s16 spinSpeed16; /* spin speed */
} Fx07dOrb;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 otZ8; /* ordering-table offset */
    SVECTOR posC; /* caster body position */
    s32 unk14; /* write-only: caster body position, words */
    s32 unk18; /* write-only */
    s32 unk1C; /* write-only */
    s32 model20; /* glow model */
    s32 model24; /* model */
    s32 model28; /* model */
    s32 model2C; /* model */
    s32 pad30;
    s32 scroll34; /* backdrop scroll */
    s32 yaw38; /* yaw offset */
    Pool6CEntry *emit3C;
} Fx07dWork80; /* 0x40 */

extern BtlModelBuf fx_07d_effModels[];

typedef struct {
    SVECTOR pos0; /* caster body position */
    s32 *jitter8; /* arc profile table */
    s32 arcCountC; /* arcs spawned */
    s32 variant10; /* variant */
} Fx07dWork83; /* 0x14 */

static void fx_07d_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame);
static void fxm_twistVertex(s32 radius, s32 unused1, s32 ang, s32 unused2, s16 *out);
static void fxm_anglesFromEye(s16 *target, s16 *outAng);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx07dStrip *trail, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
static s32 fx_07d_op83PoolTask(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fx_07d_op80PoolTask(s32 mode, Fx07dOrb *orb, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool, Fx07dWork80 *work);
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
