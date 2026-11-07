#ifndef FX_134_H
#define FX_134_H

/* Effect module fx_134 (dir 13 file 0x134): Doomsday; enemy Doomsday (Hades, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_134 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DOOMSDAY_METEOR_TRAIL  0x1 /* emitter: helix screen trail with a rock texture, red sparks */
#define FX_DOOMSDAY_NEBULA        0x3 /* sprite: purple nebula bands on black (space background layer) */
#define FX_DOOMSDAY_STARFIELD     0x4 /* sprite: blue stars on black (space background layer) */
#define FX_DOOMSDAY_GREEN_GLOW    0x5 /* emitter: green glowing cloud particles */
#define FX_DOOMSDAY_RED_RING      0x6 /* rings: white-pink-red gouraud rings */
#define FX_DOOMSDAY_METEOR        0x8 /* model: rock polyhedron (32 polys, rock texture) */
#define FX_DOOMSDAY_WHITE_RING    0x9 /* rings: white rings fading to grey (pool task) */
#define FX_DOOMSDAY_FIRE_RING     0xA /* rings: white-orange-yellow gouraud rings (pool task) */
#define FX_DOOMSDAY_BIG_METEOR    0xB /* model: large rock ellipsoid (324 polys, 15-bit rock texture) */
#define FX_DOOMSDAY_BACKDROP      0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DOOMSDAY_EMBERS        0xD /* emitter: small orange ember sprites (pool task) */
#define FX_DOOMSDAY_STAR_TUNNEL   0x11 /* model: tube with a star texture, a warp tunnel */
#define FX_DOOMSDAY_STAR_ARCH     0x12 /* model: arch with a star texture */
#define FX_DOOMSDAY_RED_DOME 0x13 /* model: shallow dark-red gouraud dome with a white centre (46 polys; byte-identical to fx_0e0 entry 0x13) */
#define FX_DOOMSDAY_BIG_METEOR_2  0x14 /* model: large rock ellipsoid (324 polys, 15-bit rock texture) */
#define FX_DOOMSDAY_EMBERS_2      0x15 /* emitter: small yellow ember sprites */
#define FX_DOOMSDAY_FLAME_BURST   0x16 /* emitter: orange flame puffs */
#define FX_DOOMSDAY_BLUE_PLANET   0x18 /* sprite: pale blue planet */
#define FX_DOOMSDAY_LIGHT_CONE    0x19 /* model: open cone with a grey streak texture */
#define FX_DOOMSDAY_LILAC_RING    0x1A /* rings: white-lilac gouraud rings */
#define FX_DOOMSDAY_BLUE_RING     0x1B /* rings: white-cyan-blue gouraud rings, four segments */
#define FX_DOOMSDAY_PURPLE_STREAK 0x1E /* trail: purple streak (fxm_drawOffsetTrail) */
#define FX_DOOMSDAY_SPACE_LAYER   0x1F /* sprite: purple-blue starry nebula strip (space background layer) */
#define FX_DOOMSDAY_RED_PLANET    0x20 /* sprite: small red planet */
#define FX_DOOMSDAY_PEBBLES 0x21 /* emitter: spiral of small grey pebble sprites (byte-identical to fx_0e0 entry 0x20) */
#define FX_DOOMSDAY_ROCK_DEBRIS   0x22 /* emitter: grey rock chunks */
#define FX_DOOMSDAY_BACKDROP_2    0x30 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_DOOMSDAY_BACKDROP_3    0x31 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

/* fx_134 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each) and their lengths, keyframes and tints, the pool items, the model textures, the scrolling star textures and
 * the primitive helpers' words. States 7..0xA and the helpers are the ones Sword Magic Doomsday (fx_0e0) copies;
 * they share its names (FX_SWDMAG_DOOMSDAY_*). */
#define FX_DOOMSDAY_STATE_SPACE   0    /* space layers (NEBULA, STARFIELD, SPACE_LAYER) slide in, GREEN_GLOW and RED_RING */
#define FX_DOOMSDAY_SPACE_FRAMES  0x28
#define FX_DOOMSDAY_RED_RING_FRAME 8   /* STATE_SPACE: RED_RING (subtractive) from here */
#define FX_DOOMSDAY_GLOW_FRAME    0x18 /* STATE_SPACE: GREEN_GLOW starts */
#define FX_DOOMSDAY_STATE_METEOR  1    /* BIG_METEOR crosses the planets, meteor orbs (pool) fly into it */
#define FX_DOOMSDAY_METEOR_FRAMES 0x40
#define FX_DOOMSDAY_FLASH_FRAME   0x3E /* STATE_METEOR: two frames of additive flash, */
#define FX_DOOMSDAY_FLASH_RGB     0x46, 0x46, 0x46
#define FX_DOOMSDAY_FLASH_2_RGB   0x80, 0x80, 0x80
#define FX_DOOMSDAY_STATE_ARCH    2    /* BIG_METEOR_2 and STAR_ARCH copies; flashes on its first two frames */
#define FX_DOOMSDAY_ARCH_FRAMES   6
#define FX_DOOMSDAY_ARCH_FLASH_RGB   0xC0, 0xC0, 0xC0 /* frame 0 */
#define FX_DOOMSDAY_ARCH_FLASH_2_RGB 0x60, 0x60, 0x60 /* frame 1 */
#define FX_DOOMSDAY_STATE_TUNNEL  3    /* STAR_TUNNEL and RED_DOME round the meteor, STAR_ARCH columns */
#define FX_DOOMSDAY_TUNNEL_FRAMES 0x18
#define FX_DOOMSDAY_STATE_CONE    4    /* LIGHT_CONE, EMBERS_2 / FLAME_BURST, star-arch risers (pool), the planets scroll */
#define FX_DOOMSDAY_CONE_FRAMES   0x28
#define FX_DOOMSDAY_STATE_RUSH    5    /* the tunnel texture streams, purple streak bolts */
#define FX_DOOMSDAY_RUSH_FRAMES   0x17
#define FX_DOOMSDAY_STATE_GATHER  6    /* the meteor comes down, STAR_ARCH and LIGHT_CONE flare after GATHER_FLARE_FRAME */
#define FX_DOOMSDAY_GATHER_FRAMES 0x38 /* (its ramps are over 56 frames) */
#define FX_DOOMSDAY_GATHER_FLARE_FRAME 0x28
#define FX_DOOMSDAY_STATE_IMPACT  7    /* a ripple grid over the screen, LILAC_RING / BLUE_RING, the screen whitens */
#define FX_DOOMSDAY_IMPACT_FRAMES 0x10 /* at its end PEBBLES start */
#define FX_DOOMSDAY_IMPACT_WHITE_FRAME 8 /* STATE_IMPACT: the screen turns white (additive) after this frame */
#define FX_DOOMSDAY_IMPACT_TINT_RGB 0x58, 0, 0 /* STATE_IMPACT: additive red (GATHER ramps up to it) */
#define FX_DOOMSDAY_STATE_DEBRIS  8    /* white screen, ROCK_DEBRIS and debris pool items flying */
#define FX_DOOMSDAY_DEBRIS_FRAMES 0x20
#define FX_DOOMSDAY_ROCK_DEBRIS_2_FRAME 4  /* STATE_DEBRIS: ROCK_DEBRIS starts on frame 1 and again here */
#define FX_DOOMSDAY_DEBRIS_SPAWN_FRAME  0xA /* STATE_DEBRIS: DEBRIS_PER_FRAME pool items per frame from here */
#define FX_DOOMSDAY_DEBRIS_PER_FRAME    3
#define FX_DOOMSDAY_DEBRIS_FLASH_FRAME  0x1A /* STATE_DEBRIS: an additive white flash grows from here to its end */
#define FX_DOOMSDAY_DEBRIS_WHITE_RGB 0xFF, 0xFF, 0xC8 /* STATE_DEBRIS: opaque cream-white fill at the back of the OT */
#define FX_DOOMSDAY_DEBRIS_TINT_RGB  0x40, 0, 0       /* STATE_DEBRIS: additive red */
#define FX_DOOMSDAY_WHITE_FLASH_RGB  0xFF, 0xFF, 0xFF /* STATE_DEBRIS's end: additive white */
#define FX_DOOMSDAY_STATE_FADE    0xA  /* the screen grid zooms back, the white fades (the op ends; the models are cleared) */
#define FX_DOOMSDAY_FADE_FRAMES   0x20
#define FX_DOOMSDAY_POOL_SLOTS    0x31 /* op80 pool items (Fx134Orb): */
#define FX_DOOMSDAY_ORB_KIND_RISER  2  /* Fx134Orb.kind0: a STAR_ARCH copy rising from the cone (STATE_CONE, every 5th frame), */
#define FX_DOOMSDAY_RISER_FRAMES  24
#define FX_DOOMSDAY_ORB_KIND_DEBRIS 3  /* a METEOR rock flying, spinning and shrinking (STATE_DEBRIS) */
#define FX_DOOMSDAY_DEBRIS_ROCK_FRAMES 16
#define FX_DOOMSDAY_MODEL_COUNT   8    /* btlfx_initEffModels slots (7 registered) */
#define FX_DOOMSDAY_METEOR_TPAGE  0x98   /* 8-bit page (0x200, 0x100), rock texture */
#define FX_DOOMSDAY_METEOR_CLUT   0x3D80 /* CLUT (0, 0xF6) */
#define FX_DOOMSDAY_METEOR_U      0
#define FX_DOOMSDAY_METEOR_V      0
#define FX_DOOMSDAY_BIG_METEOR_TPAGE 0x118  /* BIG_METEOR and BIG_METEOR_2: 15-bit page (0x200, 0x100) */
#define FX_DOOMSDAY_BIG_METEOR_CLUT  0xFFFF /* (no CLUT) */
#define FX_DOOMSDAY_BIG_METEOR_U     0x40
#define FX_DOOMSDAY_BIG_METEOR_V     0
#define FX_DOOMSDAY_STAR_TUNNEL_TPAGE 0x37  /* STAR_TUNNEL and STAR_ARCH: 4-bit page (0x1C0, 0x100), additive, starfield */
#define FX_DOOMSDAY_STAR_TUNNEL_CLUT  0x3D07 /* CLUT (0x70, 0xF4) */
#define FX_DOOMSDAY_STAR_TUNNEL_U     0
#define FX_DOOMSDAY_STAR_TUNNEL_V     0
#define FX_DOOMSDAY_LIGHT_CONE_TPAGE  0xBC  /* 8-bit page (0x300, 0x100), additive, smoke texture */
#define FX_DOOMSDAY_LIGHT_CONE_CLUT   0x3E00 /* CLUT (0, 0xF8) */
#define FX_DOOMSDAY_LIGHT_CONE_U      0x80
#define FX_DOOMSDAY_LIGHT_CONE_V      0
#define FX_DOOMSDAY_METEOR_TRAIL_TRAIL_SLOTS  0x40 /* METEOR_TRAIL's trail buffer fx_134_op80Trails: slots of */
#define FX_DOOMSDAY_METEOR_TRAIL_TRAIL_STRIDE 0xCC /* this many bytes (Pool6CEntry trailSlots / trailStride) */
/* Scrolling textures in the STAR_TUNNEL page: STATE_RUSH streams (SRC_X, SRC_Y) 0x14 a frame into the TUNNEL rect (v 0x80);
 * states 3..7 scroll the common effect page's stars into the STARS rect (u 0x80). */
#define FX_DOOMSDAY_TUNNEL_VRAM_X 0x1C0
#define FX_DOOMSDAY_TUNNEL_VRAM_Y 0x180
#define FX_DOOMSDAY_TUNNEL_VRAM_W 0x18
#define FX_DOOMSDAY_TUNNEL_VRAM_H 0x60
#define FX_DOOMSDAY_TUNNEL_SRC_X  0x320
#define FX_DOOMSDAY_TUNNEL_SRC_Y  0x100
#define FX_DOOMSDAY_STARS_VRAM_X  0x1E0
#define FX_DOOMSDAY_STARS_VRAM_Y  0x100
#define FX_DOOMSDAY_STARS_VRAM_W  0x20
#define FX_DOOMSDAY_STARS_VRAM_H  0x80
#define FX_DOOMSDAY_STARS_SRC_X   0x300
#define FX_DOOMSDAY_STARS_SRC_Y   0x100
#define FX_DOOMSDAY_BOLTS         3    /* STATE_RUSH: fx_134_bolts (purple streaks) */
#define FX_DOOMSDAY_TRAIL_POINTS  25   /* points of a purple streak strip (fxm_drawOffsetTrail) */
#define FX_DOOMSDAY_BULGE_GRID    24   /* fxm_drawBulgeGrid24: quads per side */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[FX_DOOMSDAY_TRAIL_POINTS];
} Fx134Bolt;

extern Unk800F78A4 *fx_134_env;

static void fxm_drawMirrorSprite(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
static void fxm_drawSprite3D(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth);

/* propagated */
static void fxm_drawTiledStrip(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip);
static void fxm_drawBulgeGrid24(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 bright, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx134Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10; /* target */
    SVECTOR pos18; /* camera target */
    u8 pad20[4];
    Fx134Vec3 pos24; /* model position */
    Fx134Vec3 scale30; /* model scale */
    SVECTOR rot3C; /* model rotation */
    Fx134Vec3 scale44; /* scale */
    SVECTOR rot50; /* rotation */
    s32 scroll58; /* scroll */
    s32 model5C; /* models */
    s32 model60;
    s32 model64;
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    u8 pad78[4];
    Pool6CEntry *emit7C;
    Pool6CEntry *emit80;
} Fx134Work; /* 0x84 */

typedef struct {
    s16 kind0; /* state */
    s16 phase2; /* phase */
    SVECTOR vel4; /* velocity; pad = size index */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spin1C;
    s16 spin1E;
    Pool6CEntry *emit20;
} Fx134Orb; /* 0x24 */

extern s32 fx_134_orbScales[]; /* orb sizes */
extern s32 fx_134_catchRadius; /* catch radius */
extern s32 fx_134_stopOrbs;

static s32 fx_134_op80PoolTask(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent);

extern SVECTOR fx_134_origin; /* caster position */
extern CVECTOR fx_134_rippleColors0[];
extern s16 fx_134_orbSpeeds[4];
extern CVECTOR fx_134_rippleColors1[];
extern CVECTOR fx_134_rippleColors2[2];
extern Fx134Bolt fx_134_bolts[FX_DOOMSDAY_BOLTS];
extern BtlModelBuf fx_134_effModels[];
extern u8 fx_134_op80Trails[];

#endif
