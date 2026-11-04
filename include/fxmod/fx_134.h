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

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pos4[25];
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
extern Fx134Bolt fx_134_bolts[];
extern BtlModelBuf fx_134_effModels[];
extern u8 fx_134_op80Trails[];

#endif
