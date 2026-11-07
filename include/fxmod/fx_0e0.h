#ifndef FX_0E0_H
#define FX_0E0_H

/* Effect module fx_0e0 (dir 13 file 0xE0): Sword Magic Doomsday. Objects (pool items) pulled into a centre point,
 * offset trails (fxm_drawOffsetTrail) and a ripple grid. Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0e0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_DOOMSDAY_METEOR 0x8 /* model: polyhedral rock (rock texture) */
#define FX_SWDMAG_DOOMSDAY_WHITE_RING 0x9 /* rings: white ring fading through cream to grey, 8 frames */
#define FX_SWDMAG_DOOMSDAY_FIRE_RING        0xA /* rings: white-orange / yellow rings, 16 frames */
#define FX_SWDMAG_DOOMSDAY_BIG_METEOR 0xB /* model: egg-shaped boulder (rock texture) */
#define FX_SWDMAG_DOOMSDAY_EMBERS           0xD /* emitter: spiral of tiny orange sprites */
#define FX_SWDMAG_DOOMSDAY_STAR_TUNNEL 0x11 /* model: three-bladed funnel with a starfield texture */
#define FX_SWDMAG_DOOMSDAY_STAR_ARCH 0x12 /* model: arch / dome with a starfield texture */
#define FX_SWDMAG_DOOMSDAY_RED_DOME         0x13 /* model: shallow dark-red gouraud dome with a white centre */
#define FX_SWDMAG_DOOMSDAY_BOULDER_2        0x14 /* model: rough egg-shaped boulder (rock texture) */
#define FX_SWDMAG_DOOMSDAY_LIGHT_CONE 0x19 /* model: open cylinder with a grey-white smoke texture */
#define FX_SWDMAG_DOOMSDAY_LILAC_RINGS      0x1A /* rings: white-lilac gouraud rings at the centre */
#define FX_SWDMAG_DOOMSDAY_BLUE_RINGS       0x1B /* rings: white-cyan-blue gouraud rings at the centre */
#define FX_SWDMAG_DOOMSDAY_BACKDROP         0x1C /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SWDMAG_DOOMSDAY_PURPLE_STREAK 0x1D /* trail: purple lightning strip (fxm_drawOffsetTrail) */
#define FX_SWDMAG_DOOMSDAY_PEBBLES          0x20 /* emitter: spiral of small grey pebble sprites */
#define FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS      0x21 /* emitter: four particle elements, tumbling grey-blue rock debris */
#define FX_SWDMAG_DOOMSDAY_SWORD_FLASH      0x22 /* rings: white 8-segment rings flashed rotating at the sword (op84) */
#define FX_SWDMAG_DOOMSDAY_SWORD_GLINT      0x23 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */

/* fx_0e0 choreography (docs/fxdata.md, "Choreography literals"): op80's states, their lengths, keyframes and screen
 * tints, the debris pool items, the model textures and the scrolled star texture rect; the module's primitive
 * helpers; op84's sword flash. */
#define FX_SWDMAG_DOOMSDAY_STATE_GATHER 6    /* op80 (starts here): the boulder comes down through STAR_TUNNEL, BACKDROP, purple streaks; STAR_ARCH / LIGHT_CONE flare at the centre */
#define FX_SWDMAG_DOOMSDAY_STATE_IMPACT 7    /* a ripple grid over the screen, LILAC_RINGS / BLUE_RINGS, the screen whitens */
#define FX_SWDMAG_DOOMSDAY_STATE_DEBRIS 8    /* white screen, PEBBLES / ROCK_DEBRIS and debris pool items flying */
#define FX_SWDMAG_DOOMSDAY_STATE_FADE   0xA  /* the screen grid zooms back, the white fades (the op ends) */
#define FX_SWDMAG_DOOMSDAY_GATHER_FRAMES      0x42 /* STATE_GATHER's length (its ramps are over 66 frames) */
#define FX_SWDMAG_DOOMSDAY_FLARE_FRAME        0x32 /* STATE_GATHER: STAR_ARCH and LIGHT_CONE flare at the centre after this frame */
#define FX_SWDMAG_DOOMSDAY_IMPACT_FRAMES      0x10 /* STATE_IMPACT's length; at its end PEBBLES start */
#define FX_SWDMAG_DOOMSDAY_IMPACT_WHITE_FRAME 8    /* STATE_IMPACT: the screen turns white (additive) after this frame */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_FRAMES      0x20 /* STATE_DEBRIS's length */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_SPAWN_FRAME 0xA  /* STATE_DEBRIS: DEBRIS_PER_FRAME pool items per frame from here */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_PER_FRAME   3
#define FX_SWDMAG_DOOMSDAY_DEBRIS_FLASH_FRAME 0x1A /* STATE_DEBRIS: an additive white flash grows from here to its end */
#define FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS_2_FRAME 4   /* STATE_DEBRIS: FX_SWDMAG_DOOMSDAY_ROCK_DEBRIS starts on frame 1 and again here */
#define FX_SWDMAG_DOOMSDAY_FADE_FRAMES        0x10 /* STATE_FADE's length (the op ends) */
#define FX_SWDMAG_DOOMSDAY_IMPACT_TINT_RGB    0x58, 0, 0       /* STATE_IMPACT: additive screen tint (red), GATHER ramps up to it */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_WHITE_RGB   0xFF, 0xFF, 0xC8 /* STATE_DEBRIS: opaque screen fill (cream white) at the back of the OT */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_TINT_RGB    0x40, 0, 0       /* STATE_DEBRIS: additive (red) */
#define FX_SWDMAG_DOOMSDAY_WHITE_FLASH_RGB    0xFF, 0xFF, 0xFF /* STATE_DEBRIS's end: additive (white) */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_SLOTS       0x31 /* op80 pool items (the debris) */
#define FX_SWDMAG_DOOMSDAY_OBJ_STATE_DEBRIS   3    /* pool item state of the STATE_DEBRIS rocks (Fx0e0Spark.state0): fly, spin and shrink */
#define FX_SWDMAG_DOOMSDAY_DEBRIS_ROCK_FRAMES 16   /* pool item: an OBJ_STATE_DEBRIS rock's length */
#define FX_SWDMAG_DOOMSDAY_MODEL_COUNT        8    /* btlfx_initEffModels slots (7 registered) */
#define FX_SWDMAG_DOOMSDAY_METEOR_TPAGE 0x98 /* Hi_RegisterTexEffModel texture: 8-bit page (0x200, 0x100), abr 0, rock texture */
#define FX_SWDMAG_DOOMSDAY_METEOR_CLUT  0x3D80 /* CLUT at (0x0, 0xF6) */
#define FX_SWDMAG_DOOMSDAY_METEOR_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_METEOR_V     0x0
#define FX_SWDMAG_DOOMSDAY_BIG_METEOR_TPAGE 0x118 /* Hi_RegisterTexEffModel texture: 15-bit page (0x200, 0x100), abr 0 */
#define FX_SWDMAG_DOOMSDAY_BIG_METEOR_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_SWDMAG_DOOMSDAY_BIG_METEOR_U     0x40 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_BIG_METEOR_V     0x0
#define FX_SWDMAG_DOOMSDAY_BOULDER_2_TPAGE 0x118 /* Hi_RegisterTexEffModel texture: 15-bit page (0x200, 0x100), abr 0 */
#define FX_SWDMAG_DOOMSDAY_BOULDER_2_CLUT  0xFFFF /* no CLUT (15-bit page) */
#define FX_SWDMAG_DOOMSDAY_BOULDER_2_U     0x40 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_BOULDER_2_V     0x0
#define FX_SWDMAG_DOOMSDAY_STAR_TUNNEL_TPAGE 0x37 /* Hi_RegisterTexEffModel texture: 4-bit page (0x1C0, 0x100), abr 1, starfield */
#define FX_SWDMAG_DOOMSDAY_STAR_TUNNEL_CLUT  0x3D07 /* CLUT at (0x70, 0xF4) */
#define FX_SWDMAG_DOOMSDAY_STAR_TUNNEL_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_STAR_TUNNEL_V     0x0
#define FX_SWDMAG_DOOMSDAY_STAR_ARCH_TPAGE 0x37 /* Hi_RegisterTexEffModel texture: 4-bit page (0x1C0, 0x100), abr 1 */
#define FX_SWDMAG_DOOMSDAY_STAR_ARCH_CLUT  0x3D07 /* CLUT at (0x70, 0xF4) */
#define FX_SWDMAG_DOOMSDAY_STAR_ARCH_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_STAR_ARCH_V     0x0
#define FX_SWDMAG_DOOMSDAY_LIGHT_CONE_TPAGE 0xBC /* Hi_RegisterTexEffModel texture: 8-bit page (0x300, 0x100), abr 1, smoke texture */
#define FX_SWDMAG_DOOMSDAY_LIGHT_CONE_CLUT  0x3E00 /* CLUT at (0x0, 0xF8) */
#define FX_SWDMAG_DOOMSDAY_LIGHT_CONE_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_DOOMSDAY_LIGHT_CONE_V     0x0
/* btlfx_drawScrollMove copies the common effect page's (SRC_X, SRC_Y) into this rect in the STAR_TUNNEL / STAR_ARCH
 * page (4-bit (0x1C0, 0x100), texels from u 0x80), scrolled by the frame, while op80 is in state 3..7. */
#define FX_SWDMAG_DOOMSDAY_STARS_VRAM_X 0x1E0
#define FX_SWDMAG_DOOMSDAY_STARS_VRAM_Y 0x100
#define FX_SWDMAG_DOOMSDAY_STARS_VRAM_W 0x20
#define FX_SWDMAG_DOOMSDAY_STARS_VRAM_H 0x80
#define FX_SWDMAG_DOOMSDAY_STARS_SRC_X  0x300
#define FX_SWDMAG_DOOMSDAY_STARS_SRC_Y  0x100
#define FX_SWDMAG_DOOMSDAY_TRAIL_POINTS 25 /* points of a purple streak strip (fxm_drawOffsetTrail) */
#define FX_SWDMAG_DOOMSDAY_BULGE_GRID   24 /* fxm_drawBulgeGrid24: quads per side */
#define FX_SWDMAG_DOOMSDAY_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_DOOMSDAY_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_DOOMSDAY_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_DOOMSDAY_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_DOOMSDAY_SWORD_FLICKER_FRAMES 0xE /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_DOOMSDAY_SWORD_FADE_FRAMES    0xE /* SWORD_STATE_FADE's length */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* time */
    SVECTOR pos8; /* caster bone position */
} Fx0e0Work84;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[FX_SWDMAG_DOOMSDAY_TRAIL_POINTS]; /* ring points (btlfx_pushTrailPoint; pad: width * 2) */
} Fx0e0Bolt;

extern Unk800F78A4 *fx_0e0_env;

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip);
static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);
s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s16 state0; /* state */
    s16 phase2; /* sub-state */
    SVECTOR vel4; /* velocity (vx, vy, vz); pad = model scale index */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spinX1C; /* rotation speed */
    s16 spinY1E;
    Pool6CEntry *emit20; /* particle */
} Fx0e0Obj; /* 0x24 */

typedef struct {
    u8 pad0[8];
    SVECTOR pos8; /* target */
    u8 pad10[0x5C - 0x10];
    s32 model5C; /* model */
    u8 pad60[0x70 - 0x60];
    s32 model70; /* model */
} Fx0e0Inst;

extern s32 fx_0e0_objScales[];
/* Dead: both 0 in the module data, read only by the pool items' state 0/1 branch, and fx_0e0_op80 spawns its pool
 * items with state0 = 3 (no reads over a whole Doomsday Sword, runtime round 33). */
extern s32 fx_0e0_captureRadius;
extern s32 fx_0e0_endEarly;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e0Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10; /* target */
    SVECTOR camAt18; /* camera target */
    u8 pad20[4];
    Fx0e0Vec3 pos24; /* model position */
    Fx0e0Vec3 scale30; /* model scale */
    SVECTOR rot3C; /* model rotation */
    Fx0e0Vec3 scale44; /* scale */
    SVECTOR rot50; /* rotation */
    u8 pad58[4];
    s32 model5C; /* models */
    s32 model60;
    s32 model64;
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    u8 pad78[0xC];
} Fx0e0Work80; /* 0x84 */

typedef struct {
    s16 state0; /* state */
    s16 unk2; /* unused */
    s16 velX4;
    s16 velY6;
    s16 velZ8;
    s16 unkA; /* write-only */
    s16 posXC;
    s16 posYE;
    s16 posZ10;
    s16 unk12; /* unused */
    s16 rotX14;
    s16 rotY16;
    s16 rotZ18;
} Fx0e0Spark;

typedef union {
    VECTOR v;
    struct {
        CVECTOR col;
        s32 pad;
        SVECTOR rot;
    } s;
} Fx0e0Tmp;

extern SVECTOR fx_0e0_centerPos;
extern CVECTOR fx_0e0_rippleColors[];
extern Fx0e0Bolt fx_0e0_trailStrips[];
extern BtlModelBuf fx_0e0_effModels[];

#endif
