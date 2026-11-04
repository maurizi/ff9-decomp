#ifndef FX_0DF_H
#define FX_0DF_H

/* Effect module fx_0df (dir 13 file 0xDF): Sword Magic Flare. Arcs between recent particle positions
 * (fxm_drawOffsetTrail, fx_0df_recentPos) and a twisted radial screen grid. Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0df effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLARE_SWORD_RED_RINGS    0x3 /* rings: white-pink-red gouraud rings */
#define FX_FLARE_SWORD_BLAST_RINGS  0x4 /* rings: white-cyan-orange-red gouraud rings, the blast */
#define FX_FLARE_SWORD_FIRE_DOME    0x5 /* model: dome with a fire texture */
#define FX_FLARE_SWORD_EMBERS       0x6 /* emitter: red ember sprites + orange glow */
#define FX_FLARE_SWORD_SHOCK_RINGS  0x8 /* emitter: white-crimson / white-violet gouraud rings */
#define FX_FLARE_SWORD_FAN_EMITTER  0x9 /* emitter: two fan elements (not rendered) (look pending) */
#define FX_FLARE_SWORD_BACKDROP     0xB /* backdrop layers (btlfx_drawBackdrop, not rendered) (look pending) */
#define FX_FLARE_SWORD_SPHERE       0xC /* model: sphere, texture page 0x13C not in the file (look pending) */
#define FX_FLARE_SWORD_GOURAUD_RING 0xD /* model: flat gouraud annulus (colours not rendered) (look pending) */
#define FX_FLARE_SWORD_BACKDROP_2   0xE /* backdrop layers (btlfx_drawBackdrop, not rendered) (look pending) */
#define FX_FLARE_SWORD_GLOW         0x10 /* particles: orange glow with a white core */
#define FX_FLARE_SWORD_EMBERS_2     0x11 /* emitter: red ember sprites */
#define FX_FLARE_SWORD_ARCH         0x12 /* model: arch, 15-bit texture page 0x139 (look pending) */
#define FX_FLARE_SWORD_WHITE_RINGS  0x13 /* rings: white-cream gouraud rings */
#define FX_FLARE_SWORD_BACKDROP_3   0x18 /* backdrop layers (btlfx_drawBackdrop, not rendered) (look pending) */
#define FX_FLARE_SWORD_TEX_RING     0x19 /* rings: textured ring, texture not in the file (look pending) */
#define FX_FLARE_SWORD_TEX_RING_2   0x1A /* rings: textured ring, texture not in the file (look pending) */
#define FX_FLARE_SWORD_BLUE_BOLT    0x1B /* trail: blue lightning strip (fxm_drawOffsetTrail) */
#define FX_FLARE_SWORD_BLADE_GLINT  0x1E /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_FLARE_SWORD_BLADE_FLASH  0x1F /* rings: white 8-segment rings flashed rotating at the sword (op84) */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0dfWork84;

typedef struct {
    s32 vx, vy, vz;
} Fx0dfVec3;

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 size2; /* size */
    SVECTOR pos4; /* position */
    SVECTOR rotC; /* rotation */
    u16 roll14; /* roll */
    u16 rollSpeed16; /* roll speed */
} Fx0dfPart;

typedef struct {
    u8 pad0[0x20];
    s32 model20; /* models */
    s32 model24;
    s32 model28;
    s32 model2C;
} Fx0dfWork;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[15]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0dfBolt;

extern Unk800F78A4 *fx_0df_env;
extern SVECTOR fx_0df_recentPos[24]; /* recent positions */
extern s32 fx_0df_recentNext; /* next slot in fx_0df_recentPos */
extern Unk80100C78Entry *fx_0df_task;

/* Never called: no reference in the module or effect file 0xDF, no hit on an exec breakpoint (runtime, round 33),
 * so fx_0df_recentPos keeps its data image. */
void fx_0df_recordElemDraw(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *alpha, s32 blend, s32 frame);
static s32 fx_0df_op80PoolTask(s32 mode, Fx0dfPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0dfWork *work);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0dfBolt *bolt, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth);
s32 fx_0df_op84(s32 mode, Fx0dfWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */
extern s32 fx_0df_twist;
static void fxm_twistVertex(s32 radius, s32 unused1, s32 ang, s32 unused2, s16 *out);
static void fxm_anglesFromEye(SVECTOR *target, SVECTOR *outAng);

typedef struct {
    SVECTOR pos0; /* from */
    SVECTOR pos8; /* to */
    s32 strip10; /* buffer index */
    s32 *jitter14; /* per-point (t offset, amplitude, step) */
} Fx0dfArc;

extern Fx0dfBolt fx_0df_arcStrips[];
static s32 fx_0df_op83PoolTask(s32 mode, Fx0dfArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
extern s32 fx_0df_arcJitter[][3]; /* random arc profile */

typedef struct {
    SVECTOR pos0; /* caster body position */
    s32 *jitter8; /* arc profile table */
    s32 arcCountC; /* arcs spawned */
    s32 kind10; /* variant */
} Fx0dfWork83; /* 0x14 */
s32 fx_0df_op83(s32 mode, Fx0dfWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 otZ8; /* ordering-table offset */
    SVECTOR posC; /* caster body position */
    s32 unk14; /* write-only: caster body position, words */
    s32 unk18; /* write-only */
    s32 unk1C; /* write-only */
    s32 model20; /* models */
    s32 model24;
    s32 model28;
    s32 model2C;
    s32 pad30;
    s32 scroll34; /* scroll */
    s32 yaw38; /* yaw offset */
    Pool6CEntry *emit3C;
} Fx0dfWork80; /* 0x40 */

extern BtlModelBuf fx_0df_effModels[];
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));
s32 fx_0df_op80(s32 mode, Fx0dfWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
