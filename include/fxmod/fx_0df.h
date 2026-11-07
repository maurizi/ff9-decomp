#ifndef FX_0DF_H
#define FX_0DF_H

/* Effect module fx_0df (dir 13 file 0xDF): Sword Magic Flare. Arcs between recent particle positions
 * (fxm_drawOffsetTrail, fx_0df_recentPos) and a twisted radial screen grid. Evidence: [data] the party ability /
 * item tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round
 * 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0df effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_FLARE_RED_RINGS    0x3 /* rings: white-pink-red gouraud rings */
#define FX_SWDMAG_FLARE_BLAST_RINGS  0x4 /* rings: white-cyan-orange-red gouraud rings, the blast */
#define FX_SWDMAG_FLARE_FIRE_DOME    0x5 /* model: dome with a fire texture */
#define FX_SWDMAG_FLARE_SPARK_GLOW 0x6 /* emitter: red ember sprites + orange glow */
#define FX_SWDMAG_FLARE_RED_VIOLET_RINGS 0x8 /* emitter: white-crimson / white-violet gouraud rings */
#define FX_SWDMAG_FLARE_GOLDEN_GLOWS 0x9 /* emitter: two fans drawn as small golden glow spots (byte-identical to fx_07d entry 0x1B, same render) */
#define FX_SWDMAG_FLARE_BACKDROP     0xB /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SWDMAG_FLARE_CLOUD_SPHERE 0xC /* model: sphere with a grey smoke texture (page 0x13C) */
#define FX_SWDMAG_FLARE_RED_HALO     0xD /* model: flat red-orange gouraud halo ring */
#define FX_SWDMAG_FLARE_BACKDROP_2   0xE /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SWDMAG_FLARE_ORANGE_GLOW 0x10 /* particles: orange glow with a white core */
#define FX_SWDMAG_FLARE_EMBERS_2     0x11 /* emitter: red ember sprites */
#define FX_SWDMAG_FLARE_ARCH         0x12 /* model: grey arch, 15-bit texture page 0x139 (white in the render) */
#define FX_SWDMAG_FLARE_WHITE_RINGS  0x13 /* rings: white-cream gouraud rings */
#define FX_SWDMAG_FLARE_BACKDROP_3   0x18 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SWDMAG_FLARE_TEX_RING     0x19 /* rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime (docs/runtime-findings.md effect-data pass): the ring UV box reads noise in the run-time pages too (look pending) */
#define FX_SWDMAG_FLARE_TEX_RING_2   0x1A /* rings: textured ring; the render reads garbled multicolour texels (texture not in the file); runtime (docs/runtime-findings.md effect-data pass): noise in the run-time page too (look pending) */
#define FX_SWDMAG_FLARE_BLUE_LIGHTNING 0x1B /* trail: blue lightning strip (fxm_drawOffsetTrail) */
#define FX_SWDMAG_FLARE_SWORD_GLINT  0x1E /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_FLARE_SWORD_FLASH  0x1F /* rings: white 8-segment rings flashed rotating at the sword (op84) */

/* fx_0df choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and
 * screen tints, the twisted screen grid, the cloud pool items, the model textures and the fire dome's scrolled
 * texture rect; op83's arc kinds, lengths and buffers; op84's sword flash. */
#define FX_SWDMAG_FLARE_STATE_TWIST 1 /* op80 (starts here): the screen twists around the caster, RED_RINGS / WHITE_RINGS close in */
#define FX_SWDMAG_FLARE_STATE_DOME  2 /* FX_SWDMAG_FLARE_FIRE_DOME (scrolled fire texture), BACKDROP_3, TEX_RING */
#define FX_SWDMAG_FLARE_STATE_FLASH 3 /* the dome stays, the rings spread */
#define FX_SWDMAG_FLARE_STATE_BLAST 4 /* the backdrops, BLAST_RINGS, TEX_RING_2 and the cloud pool items (the op ends) */
#define FX_SWDMAG_FLARE_TWIST_FRAMES       10 /* STATE_TWIST's length; at its end TWIST_DARKEN, RED_VIOLET_RINGS and EMBERS_2 */
#define FX_SWDMAG_FLARE_TWIST_FLASH_FRAMES 6  /* STATE_TWIST: additive yellow-white flash fading over this many frames */
#define FX_SWDMAG_FLARE_DOME_FRAMES        14 /* STATE_DOME's length */
#define FX_SWDMAG_FLARE_DOME_FLASH_FRAMES  4  /* STATE_DOME: additive red flash on the first frames */
#define FX_SWDMAG_FLARE_DOME_TINT_FRAME    10 /* STATE_DOME: DOME_TINT on this frame */
#define FX_SWDMAG_FLARE_FLASH_FRAMES       8  /* STATE_FLASH's length; at its end FLASH_DARKEN, SPARK_GLOW and GOLDEN_GLOWS */
#define FX_SWDMAG_FLARE_BLAST_FRAMES       50 /* STATE_BLAST's length (the op ends) */
#define FX_SWDMAG_FLARE_BLAST_FLASH_FRAMES 7  /* STATE_BLAST: frame 0 BLAST_DARKEN, then an additive flash fading until this frame */
#define FX_SWDMAG_FLARE_CLOUDS_FRAMES      0x20 /* STATE_BLAST: a cloud pool item every other frame until this one */
#define FX_SWDMAG_FLARE_START_DARKEN_RGB 0x80, 0xFF, 0xFF /* init: subtractive screen tint (the screen turns red) */
#define FX_SWDMAG_FLARE_TWIST_DARKEN_RGB 0xFF, 0x80, 0x80 /* STATE_TWIST's end: subtractive (cyan) */
#define FX_SWDMAG_FLARE_DOME_TINT_RGB    0x64, 0x40, 0    /* STATE_DOME: additive (orange) */
#define FX_SWDMAG_FLARE_FLASH_TINT_RGB   0x80, 0x80, 0x80 /* STATE_FLASH frame 0: additive (grey) */
#define FX_SWDMAG_FLARE_FLASH_DARKEN_RGB 0xAA, 0xAA, 0xAA /* STATE_FLASH's end: subtractive (grey: the screen darkens) */
#define FX_SWDMAG_FLARE_BLAST_DARKEN_RGB 0, 0xFF, 0xFF    /* STATE_BLAST frame 0: subtractive (the screen turns red) */
#define FX_SWDMAG_FLARE_GRID_SEGMENTS   0x10 /* STATE_TWIST: fxm_drawRadialScreenGrid segments around, */
#define FX_SWDMAG_FLARE_GRID_RINGS      7    /* rings out, */
#define FX_SWDMAG_FLARE_GRID_RING_WIDTH 0x12 /* of this many pixels each */
#define FX_SWDMAG_FLARE_CLOUD_SLOTS        6   /* op80 pool items: smoke clouds (CLOUD_SPHERE, RED_HALO, ARCH, ORANGE_GLOW) */
#define FX_SWDMAG_FLARE_CLOUD_FRAMES       0xC /* pool item: a cloud's length */
#define FX_SWDMAG_FLARE_CLOUD_GROW_FRAMES  3   /* pool item: it brightens over this many frames, then fades */
#define FX_SWDMAG_FLARE_CLOUD_ARCH_FRAMES  8   /* pool item: FX_SWDMAG_FLARE_ARCH spins and fades over this many frames */
#define FX_SWDMAG_FLARE_MODEL_COUNT        6   /* btlfx_initEffModels slots (4 registered: ARCH, FIRE_DOME, CLOUD_SPHERE, RED_HALO) */
#define FX_SWDMAG_FLARE_ARCH_TPAGE 0x139 /* Hi_RegisterTexEffModel texture: 15-bit page (0x240, 0x100), abr 1, white in the render (15-bit page not in the file) */
#define FX_SWDMAG_FLARE_ARCH_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_SWDMAG_FLARE_ARCH_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_FLARE_ARCH_V     0x80
#define FX_SWDMAG_FLARE_FIRE_DOME_TPAGE 0x38 /* Hi_RegisterTexEffModel texture: 4-bit page (0x200, 0x100), abr 1, the scrolled fire rect below */
#define FX_SWDMAG_FLARE_FIRE_DOME_CLUT  0x3E83 /* CLUT at (0x30, 0xFA) */
#define FX_SWDMAG_FLARE_FIRE_DOME_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_FLARE_FIRE_DOME_V     0x0
#define FX_SWDMAG_FLARE_CLOUD_SPHERE_TPAGE 0x13C /* Hi_RegisterTexEffModel texture: 15-bit page (0x300, 0x100), abr 1, grey smoke */
#define FX_SWDMAG_FLARE_CLOUD_SPHERE_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_SWDMAG_FLARE_CLOUD_SPHERE_U     0x10 /* texture origin (u, v) in the page */
#define FX_SWDMAG_FLARE_CLOUD_SPHERE_V     0x80
/* FX_SWDMAG_FLARE_FIRE_DOME samples (FIRE_DOME_TPAGE 4-bit page (0x200, 0x100), u / v 0) the rect below; in
 * STATE_DOME / STATE_FLASH btlfx_drawScrollMove copies the file's fire texture from (SRC_X, SRC_Y) into it,
 * scrolled by scroll34. */
#define FX_SWDMAG_FLARE_FIRE_VRAM_X 0x200
#define FX_SWDMAG_FLARE_FIRE_VRAM_Y 0x100
#define FX_SWDMAG_FLARE_FIRE_VRAM_W 0x20
#define FX_SWDMAG_FLARE_FIRE_VRAM_H 0x80
#define FX_SWDMAG_FLARE_FIRE_SRC_X  0x220
#define FX_SWDMAG_FLARE_FIRE_SRC_Y  0x100
#define FX_SWDMAG_FLARE_RECENT_POSITIONS 24 /* fx_0df_recentPos ring: ORANGE_GLOW draw positions (fx_0df_recordElemDraw), arc ends of ARCS_RECENT */
#define FX_SWDMAG_FLARE_ARCS_RECENT 0 /* op83 kind (its init arg): an arc per frame between two recent glow positions */
#define FX_SWDMAG_FLARE_ARCS_RING   1 /* op83 kind (any other): an arc per frame from the caster body outwards, turning */
#define FX_SWDMAG_FLARE_ARCS_RECENT_FRAMES 0x29 /* ARCS_RECENT's length */
#define FX_SWDMAG_FLARE_ARCS_RING_FRAMES   0x10 /* ARCS_RING's length */
#define FX_SWDMAG_FLARE_ARC_FRAMES   5  /* op83 pool item: an arc shows (and thins) for this many frames */
#define FX_SWDMAG_FLARE_ARC_SLOTS    7  /* op83 pool items and fx_0df_arcStrips; strip10 = arc count % slots */
#define FX_SWDMAG_FLARE_ARC_JITTER_ROWS 64 /* fx_0df_arcJitter rows of three random offsets (along, across, angle) */
#define FX_SWDMAG_FLARE_STRIP_POINTS 15 /* points of an arc strip (fxm_drawOffsetTrail) */
#define FX_SWDMAG_FLARE_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_FLARE_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_FLARE_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_FLARE_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_FLARE_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_FLARE_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

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
} Fx0dfPart; /* 0x18 */

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
    SVECTOR pts4[FX_SWDMAG_FLARE_STRIP_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0dfBolt;

extern Unk800F78A4 *fx_0df_env;
extern SVECTOR fx_0df_recentPos[FX_SWDMAG_FLARE_RECENT_POSITIONS]; /* recent positions */
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
