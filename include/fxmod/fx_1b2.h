#ifndef FX_1B2_H
#define FX_1B2_H

/* Effect module fx_1b2 (dir 13 file 0x1b2): enemy Tidal Wave (Nova Dragon). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_TIDAL_WAVE_PATH        0x1 /* table: vector path (fxm_vecPathInit) */
#define FX_TIDAL_WAVE_WATER_CURL  0x2 /* model: curved sheet like a breaking wave; page 0x117 holds blue-white water at run time: the white curling wave crest; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_TIDAL_WAVE_BACKDROP    0x3 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_TIDAL_WAVE_WATER_ORBS  0x7 /* emitter: orbiting blue water-caustic orbs + grey-blue shard sprites */
#define FX_TIDAL_WAVE_RED_TRAIL   0x8 /* emitter: helix flat trail, red caustic strip */
#define FX_TIDAL_WAVE_WATER_TRAIL 0x9 /* emitter: spiralling screen trail, grey-blue water streak strip */
#define FX_TIDAL_WAVE_PALE_RINGS  0xB /* emitter: white / cream / olive gouraud rings */
#define FX_TIDAL_WAVE_SURGE       0xC /* model: flat soft-edged disc, the big grey water surge sweeping over the party; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_TIDAL_WAVE_FOAM        0xD /* emitter: orbiting grey-white foam cloud quads, 17 frames */

/* fx_1b2 choreography (docs/fxdata.md, "Choreography literals"): op80's three waves (every 16 frames the curl model
 * keyed by PATH, orbs, rings, two water trails from the caster's bones, hit motions), op81's foam and red trails,
 * op82's surge model sampling the background rendered by op83; textures, trail buffers, keyframes and lengths. */
#define FX_TIDAL_WAVE_CURL_TPAGE 0x117 /* Hi_RegisterTexEffModel texture: 15-bit page (0x1C0, 0x100), abr 0 (run-time water) */
#define FX_TIDAL_WAVE_CURL_CLUT  0     /* unused by a 15-bit page */
#define FX_TIDAL_WAVE_CURL_U     0
#define FX_TIDAL_WAVE_CURL_V     0
#define FX_TIDAL_WAVE_MODEL_Z    0x44C /* op80 / op82: the curl and surge models sit this far towards -z */
#define FX_TIDAL_WAVE_INTERVAL   0x10  /* op80: a wave starts every this many frames */
#define FX_TIDAL_WAVE_CURL_FRAMES 0xB  /* the curl model plays its PATH keys for this many frames (timer5C) */
#define FX_TIDAL_WAVE_CURL_FADE_FRAME 8 /* and fades from this key */
#define FX_TIDAL_WAVE_CURL_FADE_FRAMES 4 /* over this many frames (timer60) */
#define FX_TIDAL_WAVE_LAST_WAVE  2     /* the wave (count64) that starts no hit motions */
#define FX_TIDAL_WAVE_TRAIL_BONE   0xC  /* unit 0x10's bones the trails start from */
#define FX_TIDAL_WAVE_TRAIL_2_BONE 0x29
#define FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_SLOTS  0x3C /* op80: each WATER_TRAIL's buffer fx_1b2_op80Trails0 / 1[wave]: slots of */
#define FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_STRIDE 0x28 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_TIDAL_WAVE_FRAMES     0x2C  /* op80's length (the op ends) */
#define FX_TIDAL_WAVE_MODELS     3     /* op81: btlfx_initEffModels slots */
#define FX_TIDAL_WAVE_FOAM_FRAME 6     /* op81: FOAM at the caster's feet */
#define FX_TIDAL_WAVE_RED_TRAIL_FRAME 9 /* op81: two RED_TRAILs from the trail bones */
#define FX_TIDAL_WAVE_RED_TRAIL_TRAIL_SLOTS  0xA  /* their buffers fx_1b2_op81Trails0 / 1: slots of */
#define FX_TIDAL_WAVE_RED_TRAIL_TRAIL_STRIDE 0x80 /* this many bytes */
#define FX_TIDAL_WAVE_TRAILS_FRAMES 0x28 /* op81's length (the op ends) */
#define FX_TIDAL_WAVE_SURGE_TPAGE 0x119 /* Hi_RegisterTexEffModel texture: 15-bit page (0x240, 0x100), abr 0: the BG_VRAM rect */
#define FX_TIDAL_WAVE_SURGE_CLUT  0
#define FX_TIDAL_WAVE_SURGE_U     0
#define FX_TIDAL_WAVE_SURGE_V     0
#define FX_TIDAL_WAVE_SURGE_FADE_IN_FRAMES  8    /* op82: the surge fades in over this many frames (timer2C) */
#define FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAME  0x28 /* fades out from this frame */
#define FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAMES 0xA  /* over this many (timer30), then stops drawing */
#define FX_TIDAL_WAVE_SURGE_FRAMES 0x50 /* op82's length (the op ends) */
#define FX_TIDAL_WAVE_BG_VRAM_X  0x240 /* op83: btlfx_drawBgOffscreen renders the battle background into this rect */
#define FX_TIDAL_WAVE_BG_VRAM_Y  0x100
#define FX_TIDAL_WAVE_BG_VRAM_W  0x80
#define FX_TIDAL_WAVE_BG_VRAM_H  0x80
#define FX_TIDAL_WAVE_BG_FRAMES  5     /* op83's length */

typedef struct {
    s16 x, y, z;
} Fx1b2Key6;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b2Key6 *keys4;
} Fx1b2Track6;

typedef struct {
    s16 x, y, z;
} Fx1b2Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b2Point *keys4;
} Fx1b2Path;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR rot10; /* model rotation */
    VECTOR pos18; /* model position */
    VECTOR scale28; /* model scale */
    s32 fade38; /* brightness */
    SVECTOR rot3C;
    SVECTOR pos44; /* bone positions */
    SVECTOR pos4C;
    SVECTOR pos54;
    s32 timer5C; /* countdown */
    s32 timer60; /* fade countdown */
    s32 count64;
    s32 index68;
    u16 model6C; /* model */
} Fx1b2Work80; /* 0x70 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* caster position */
    SVECTOR pos14; /* bone positions */
    SVECTOR pos1C;
} Fx1b2Work81; /* 0x24 */

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model scale */
    s32 fade28; /* brightness */
    s32 timer2C; /* fade-in countdown */
    s32 timer30; /* fade-out countdown */
    u16 model34; /* model */
} Fx1b2Work82; /* 0x38 */

extern Unk800F78A4 *fx_1b2_env;
extern BtlModelBuf fx_1b2_effModels[];
extern Fx1b2Path fx_1b2_vecPath;
extern u32 fx_1b2_bgOt[];
extern s32 fx_1b2_op80Trails0[][FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_SLOTS * FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_STRIDE / 4];
extern s32 fx_1b2_op80Trails1[][FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_SLOTS * FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_STRIDE / 4];
extern s32 fx_1b2_op81Trails0[];
extern s32 fx_1b2_op81Trails1[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_vecPathInit(Fx1b2Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx1b2Track6 *track, s32 t, VECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHitMotionIfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_1b2_env;
/* end copied */

#endif
