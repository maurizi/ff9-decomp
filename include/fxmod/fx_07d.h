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
#define FX_FLARE_RED_HALO 0xF /* model: flat red-orange gouraud halo ring (byte-identical to fx_0df entry 0xD) */
#define FX_FLARE_BACKDROP         0x11 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FLARE_CALLBACK_EMITTER 0x12 /* emitter: orbit, one callback element, nothing drawable in the data; runtime (docs/runtime-findings.md effect-data pass): started as the orange light rays radiate; drawn by the module callback, not ablated (look pending) */
#define FX_FLARE_FIRE_RING        0x13 /* rings: white-cyan-orange-red gouraud rings */
#define FX_FLARE_WHITE_FLASH      0x14 /* emitter: two fans drawn as solid white flash quads */
#define FX_FLARE_BACKDROP_2       0x15 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FLARE_FIRE_DOME        0x16 /* model: dome with a fiery orange texture */
#define FX_FLARE_BACKDROP_3       0x17 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FLARE_TEX_RING         0x18 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time; runtime (docs/runtime-findings.md effect-data pass): its UV box reads noise in the run-time page too (the tpage the tool resolves is not the one used); orange flame bands sweep the screen meanwhile (look pending) */
#define FX_FLARE_TEX_RING_2       0x19 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time; runtime (docs/runtime-findings.md effect-data pass): its UV box reads noise in the run-time page too (look pending) */
#define FX_FLARE_SPARK_GLOW       0x1A /* emitter: red spark sprites + fading orange glow */
#define FX_FLARE_GOLDEN_GLOWS     0x1B /* emitter: two fans drawn as small golden glow spots */
#define FX_FLARE_CLOUD_SPHERE     0x1E /* model: sphere with a grey cloudy texture */

/* fx_07d choreography (docs/fxdata.md, "Choreography literals"; the names of the Sword Magic copy fx_0df,
 * FX_SWDMAG_FLARE_*, with this module's values): op80's states, their lengths, keyframes and screen tints, the
 * twisted screen grid, the cloud pool items, the model textures and the fire dome's scrolled texture rect; op83's
 * arc kinds, lengths and buffers. fx_0df has no STATE_CHARGE (it starts at STATE_TWIST). */
#define FX_FLARE_STATE_CHARGE 0 /* op80 (starts here): FIRE_RING / WHITE_RING round the caster, the screen grid twists slowly, BACKDROP_2 */
#define FX_FLARE_STATE_TWIST  1 /* the screen twists around the caster, PINK_RING / WHITE_RING close in */
#define FX_FLARE_STATE_DOME   2 /* FX_FLARE_FIRE_DOME (scrolled fire texture), BACKDROP_3, TEX_RING */
#define FX_FLARE_STATE_FLASH  3 /* the dome stays, the rings spread */
#define FX_FLARE_STATE_BLAST  4 /* the backdrops, FIRE_RING, TEX_RING_2 and the cloud pool items (the op ends) */
#define FX_FLARE_CHARGE_FRAMES        50   /* STATE_CHARGE's length (and its ramp's); at its end START_DARKEN and the models are registered */
#define FX_FLARE_CHARGE_FLASH_FRAME   0x27 /* STATE_CHARGE: additive grey CHARGE_FLASH on this frame */
#define FX_FLARE_CHARGE_FLASH_2_FRAME 0x2B /* and on this one */
#define FX_FLARE_CALLBACK_EMITTER_FRAME 14 /* STATE_CHARGE: FX_FLARE_CALLBACK_EMITTER starts (its glows drawn by fx_07d_op80ElemDraw) */
#define FX_FLARE_CHARGE_DARKEN_FRAME  (FX_FLARE_CHARGE_FRAMES - 1) /* STATE_CHARGE: subtractive CHARGE_DARKEN on its last frame */
#define FX_FLARE_TWIST_FRAMES       10 /* STATE_TWIST's length; at its end TWIST_DARKEN, RED_VIOLET_RINGS and RED_SPARKS */
#define FX_FLARE_TWIST_FLASH_FRAMES 6  /* STATE_TWIST: additive yellow-white flash fading over this many frames */
#define FX_FLARE_DOME_FRAMES        14 /* STATE_DOME's length */
#define FX_FLARE_DOME_FLASH_FRAMES  4  /* STATE_DOME: additive red flash on the first frames */
#define FX_FLARE_DOME_TINT_FRAME    10 /* STATE_DOME: DOME_TINT on this frame */
#define FX_FLARE_FLASH_FRAMES       8  /* STATE_FLASH's length; at its end FLASH_DARKEN, SPARK_GLOW and GOLDEN_GLOWS */
#define FX_FLARE_BLAST_FRAMES       50 /* STATE_BLAST's length (the op ends) */
#define FX_FLARE_BLAST_FLASH_FRAMES 7  /* STATE_BLAST: frame 0 BLAST_DARKEN, then an additive flash fading until this frame */
#define FX_FLARE_CLOUDS_FRAMES      0x20 /* STATE_BLAST: a cloud pool item every other frame until this one */
#define FX_FLARE_CHARGE_FLASH_RGB  0x40, 0x40, 0x40 /* STATE_CHARGE: additive (grey) */
#define FX_FLARE_CHARGE_DARKEN_RGB 0, 0, 0x80       /* STATE_CHARGE's last frame: subtractive (the screen turns yellow) */
#define FX_FLARE_START_DARKEN_RGB  0x80, 0xFF, 0xFF /* STATE_CHARGE's end (fx_0df's init): subtractive (the screen turns red) */
#define FX_FLARE_TWIST_DARKEN_RGB  0xFF, 0x80, 0x80 /* STATE_TWIST's end: subtractive (cyan) */
#define FX_FLARE_DOME_TINT_RGB     0x64, 0x40, 0    /* STATE_DOME: additive (orange) */
#define FX_FLARE_FLASH_TINT_RGB    0x80, 0x80, 0x80 /* STATE_FLASH frame 0: additive (grey) */
#define FX_FLARE_FLASH_DARKEN_RGB  0xAA, 0xAA, 0xAA /* STATE_FLASH's end: subtractive (grey: the screen darkens) */
#define FX_FLARE_BLAST_DARKEN_RGB  0, 0xFF, 0xFF    /* STATE_BLAST frame 0: subtractive (the screen turns red) */
#define FX_FLARE_GRID_SEGMENTS   0x10 /* STATE_CHARGE / _TWIST: fxm_drawRadialScreenGrid segments around, */
#define FX_FLARE_GRID_RINGS      7    /* rings out, */
#define FX_FLARE_GRID_RING_WIDTH 0x11 /* of this many pixels each */
#define FX_FLARE_GRID_MIN_X      0x78 /* the grid's centre (the caster on screen) is kept inside this screen box */
#define FX_FLARE_GRID_MAX_X      0xC8
#define FX_FLARE_GRID_MIN_Y      0x69
#define FX_FLARE_GRID_MAX_Y      0x73
#define FX_FLARE_FIRE_DOME_SCALE    0x400 /* STATE_DOME / _FLASH: FX_FLARE_FIRE_DOME's scale */
#define FX_FLARE_FIRE_DOME_OFFSET_X 0x200 /* and its x offset from the caster */
#define FX_FLARE_CLOUD_SLOTS        6   /* op80 pool items: smoke clouds (CLOUD_SPHERE, RED_HALO, ARCH, ORANGE_GLOW) */
#define FX_FLARE_CLOUD_FRAMES       0xC /* pool item: a cloud's length */
#define FX_FLARE_CLOUD_GROW_FRAMES  3   /* pool item: it brightens over this many frames, then fades */
#define FX_FLARE_CLOUD_ARCH_FRAMES  8   /* pool item: FX_FLARE_ARCH spins and fades over this many frames */
#define FX_FLARE_MODEL_COUNT        6   /* btlfx_initEffModels slots (4 registered: ARCH, FIRE_DOME, CLOUD_SPHERE, RED_HALO) */
#define FX_FLARE_ARCH_TPAGE 0x139 /* Hi_RegisterTexEffModel texture: 15-bit page (0x240, 0x100), abr 1, white in the render (15-bit page not in the file) */
#define FX_FLARE_ARCH_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_FLARE_ARCH_U     0x0 /* texture origin (u, v) in the page */
#define FX_FLARE_ARCH_V     0x80
#define FX_FLARE_FIRE_DOME_TPAGE 0x38 /* Hi_RegisterTexEffModel texture: 4-bit page (0x200, 0x100), abr 1, the scrolled fire rect below */
#define FX_FLARE_FIRE_DOME_CLUT  0x3E83 /* CLUT at (0x30, 0xFA) */
#define FX_FLARE_FIRE_DOME_U     0x0 /* texture origin (u, v) in the page */
#define FX_FLARE_FIRE_DOME_V     0x0
#define FX_FLARE_CLOUD_SPHERE_TPAGE 0x13C /* Hi_RegisterTexEffModel texture: 15-bit page (0x300, 0x100), abr 1, grey smoke */
#define FX_FLARE_CLOUD_SPHERE_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_FLARE_CLOUD_SPHERE_U     0x10 /* texture origin (u, v) in the page */
#define FX_FLARE_CLOUD_SPHERE_V     0x80
/* FX_FLARE_FIRE_DOME samples (FIRE_DOME_TPAGE 4-bit page (0x200, 0x100), u / v 0) the rect below; in STATE_DOME /
 * STATE_FLASH btlfx_drawScrollMove copies the file's fire texture from (SRC_X, SRC_Y) into it, scrolled by
 * scroll34. */
#define FX_FLARE_FIRE_VRAM_X 0x200
#define FX_FLARE_FIRE_VRAM_Y 0x100
#define FX_FLARE_FIRE_VRAM_W 0x20
#define FX_FLARE_FIRE_VRAM_H 0x80
#define FX_FLARE_FIRE_SRC_X  0x220
#define FX_FLARE_FIRE_SRC_Y  0x100
#define FX_FLARE_RECENT_POSITIONS 24 /* fx_07d_sparkPos ring: ORANGE_GLOW draw positions (fx_07d_op80ElemDraw), arc ends of ARCS_RECENT */
#define FX_FLARE_ARCS_RECENT 0 /* op83 variant (its init arg): an arc per frame between two recent glow positions */
#define FX_FLARE_ARCS_RING   1 /* op83 variant (any other): an arc per frame from the caster body outwards, turning */
#define FX_FLARE_ARCS_RECENT_FRAMES 0x29 /* ARCS_RECENT's length */
#define FX_FLARE_ARCS_RING_FRAMES   0x10 /* ARCS_RING's length */
#define FX_FLARE_ARC_FRAMES   5  /* op83 pool item: an arc shows (and thins) for this many frames */
#define FX_FLARE_ARC_SLOTS    7  /* op83 pool items and fx_07d_strips; strip10 = arc count % slots */
#define FX_FLARE_ARC_JITTER_ROWS 64 /* fx_07d_jitter rows of three random offsets (along, across, angle) */
#define FX_FLARE_STRIP_POINTS 15 /* points of an arc strip (fxm_drawOffsetTrail) */

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
    SVECTOR pts4[FX_FLARE_STRIP_POINTS];
} Fx07dStrip; /* 0x7C */

extern Unk800F78A4 *fx_07d_env;
extern SVECTOR fx_07d_sparkPos[FX_FLARE_RECENT_POSITIONS];
extern s32 fx_07d_sparkPosIdx;
extern Unk80100C78Entry *fx_07d_task;
extern s32 fx_07d_twist;
extern Fx07dStrip fx_07d_strips[];
extern s32 fx_07d_jitter[FX_FLARE_ARC_JITTER_ROWS][3]; /* random arc profile */

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
