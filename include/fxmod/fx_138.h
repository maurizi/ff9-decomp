#ifndef FX_138_H
#define FX_138_H

/* Effect module fx_138 (dir 13 file 0x138): Lucky Seven. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_138 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LUCKY_SEVEN_FIRE_BURST    0x1 /* emitter: white-blue gouraud rings, fire bursts and white / pink glows */
#define FX_LUCKY_SEVEN_LENS_FLARE    0xE /* sprite line: lens-flare set (blue ring, dark red disc, orange rings, violet ring, rainbow halo); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_LUCKY_SEVEN_RAINBOW_STRIPS 0x1B /* model: slanted strips on a rainbow gradient page at run time; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_LUCKY_SEVEN_RAINBOW_TRAIL 0x2A /* emitter: orbiting rainbow trail with glow sprites and rainbow rings */

/* fx_138 choreography (docs/fxdata.md, "Choreography literals"): the reel and rainbow models' textures, the rainbow
 * scroll, op80's and op81's keyframes and lengths, the lens flare, the trail buffer and the table sizes. */
#define FX_LUCKY_SEVEN_REELS      3      /* fx_138_reels / model96 / fx_138_modelFileIds entries */
#define FX_LUCKY_SEVEN_RAND_SETS  6      /* fx_138_randSets rows (one picked at random) */
#define FX_LUCKY_SEVEN_PATHS      6      /* fx_138_posPaths, the op81 trail emitters and their spawn points */
#define FX_LUCKY_SEVEN_MODEL_COUNT 4     /* btlfx_initEffModels slots */
#define FX_LUCKY_SEVEN_REEL_TPAGE 0x99   /* the reel models: 8-bit page (0x240, 0x100) */
#define FX_LUCKY_SEVEN_REEL_CLUT  0x3D40 /* CLUT (0, 0xF5) */
#define FX_LUCKY_SEVEN_REEL_U     0
#define FX_LUCKY_SEVEN_REEL_V     0
#define FX_LUCKY_SEVEN_RAINBOW_STRIPS_TPAGE 0x9A   /* 8-bit page (0x280, 0x100) */
#define FX_LUCKY_SEVEN_RAINBOW_STRIPS_CLUT  0x3E40 /* CLUT (0, 0xF9) */
#define FX_LUCKY_SEVEN_RAINBOW_STRIPS_U     0
#define FX_LUCKY_SEVEN_RAINBOW_STRIPS_V     0x80
#define FX_LUCKY_SEVEN_STRIPS_SCALE 0x800 /* RAINBOW_STRIPS' scale */
/* op80 scrolls the rainbow texture from (SRC_X, SRC_Y) (the reel page at v 0x80) through RAINBOW_STRIPS' rect
 * (its page at v 0x80), 0x10 a frame. */
#define FX_LUCKY_SEVEN_RAINBOW_VRAM_X 0x280
#define FX_LUCKY_SEVEN_RAINBOW_VRAM_Y 0x180
#define FX_LUCKY_SEVEN_RAINBOW_VRAM_W 0x40
#define FX_LUCKY_SEVEN_RAINBOW_VRAM_H 0x80
#define FX_LUCKY_SEVEN_RAINBOW_SRC_X  0x240
#define FX_LUCKY_SEVEN_RAINBOW_SRC_Y  0x180
#define FX_LUCKY_SEVEN_STRIPS_FRAME   1    /* op80: RAINBOW_STRIPS is drawn (additive, darkened) */
#define FX_LUCKY_SEVEN_STRIPS_FRAMES  4    /* work->timer92: for this many frames, then BTLFX_SIG_WAIT is set */
#define FX_LUCKY_SEVEN_REELS_FRAMES   0x28 /* op80's length (the models are cleared) */
#define FX_LUCKY_SEVEN_RAINBOW_TRAIL_TRAIL_SLOTS  0x18 /* RAINBOW_TRAIL's trail buffer fx_138_op81Trails: slots of */
#define FX_LUCKY_SEVEN_RAINBOW_TRAIL_TRAIL_STRIDE 0x40 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_LUCKY_SEVEN_BURST_FRAME    0xC  /* op81: FIRE_BURST starts (the path trails and RAINBOW_TRAIL at frame 0) */
#define FX_LUCKY_SEVEN_FLARE_FRAMES   0xD  /* op81: LENS_FLARE is drawn for this many frames, */
#define FX_LUCKY_SEVEN_FLARE_FADE_FRAME 5  /* fading out from this one */
#define FX_LUCKY_SEVEN_FLARE_ANIM_FRAMES 6 /* its sprite frames (the last one holds) */
#define FX_LUCKY_SEVEN_FRAMES         0x3C /* op81's length */

typedef struct {
    s16 x, y, z;
} Fx138Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* key count */
    Fx138Point *keys4; /* keys */
} Fx138Path;

typedef struct {
    Pool6CEntry *emit0[6];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28;
    u16 frame30; /* frame, up to 5 */
    u16 fade32; /* scale */
} Fx138Work81; /* 0x34 */

typedef struct {
    u8 pad0[0x24];
    VECTOR pos24; /* model position */
    VECTOR unk34; /* write-only: model scale */
    u8 pad44[0x48 - 0x44];
    SVECTOR rot48; /* model rotation */
    VECTOR pos50; /* model position */
    VECTOR scale60; /* model scale */
    SVECTOR pos70; /* caster position */
    SVECTOR rot78; /* caster rotation */
    u8 pad80[0x88 - 0x80];
    s16 unk88; /* unk88..unk8E: only initialised, never read (runtime watch, round 33); unk8A is the fourth value of the fx_138_randSets row */
    s16 unk8A; /* write-only */
    s16 unk8C; /* write-only (-1) */
    s16 unk8E; /* write-only (-1, then 0x1A) */
    s16 active90; /* model shown */
    s16 timer92; /* model countdown */
    s16 frame94; /* frame */
    u16 model96[FX_LUCKY_SEVEN_REELS]; /* ring models */
    u16 model9C; /* model */
} Fx138Work80; /* 0xA0 */

typedef struct {
    SVECTOR rot0; /* rotation */
    s16 unk8; /* write-only (0x200) */
    s16 unkA; /* unused */
    s16 unkC; /* unused */
    u16 unkE; /* write-only */
    u8 pad10[0x18 - 0x10];
} Fx138Ring; /* 0x18 */

extern Fx138Ring fx_138_reels[FX_LUCKY_SEVEN_REELS]; /* set up from a fx_138_randSets row, never read (runtime watch, round 33): slot reels that were never drawn? */
extern u16 fx_138_randSets[FX_LUCKY_SEVEN_RAND_SETS][4];
extern u16 fx_138_modelFileIds[FX_LUCKY_SEVEN_REELS];
extern u16 fx_138_pathFileIds[FX_LUCKY_SEVEN_PATHS];
extern BtlModelBuf fx_138_effModels[];
extern u16 fx_138_emitFileIds[FX_LUCKY_SEVEN_PATHS]; /* trail files */
extern s32 fx_138_op81SpawnPoints[FX_LUCKY_SEVEN_PATHS];
extern Unk800F78A4 *fx_138_env;
extern u8 fx_138_op81Trails[];
extern SVECTOR fx_138_emitPos[FX_LUCKY_SEVEN_PATHS];
extern Fx138Path fx_138_posPaths[FX_LUCKY_SEVEN_PATHS];
extern SVECTOR fx_138_pathOrigin; /* path origin */
extern SVECTOR fx_138_pathRot; /* path rotation */

s32 fx_138_op81SpawnPoint0(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint1(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint2(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint3(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint4(SVECTOR *out, s32 dir, s32 i);
s32 fx_138_op81SpawnPoint5(SVECTOR *out, s32 dir, s32 i);
void fx_138_getPathPoint(SVECTOR *out, Fx138Path *path, s32 key);
static s32 fxm_posPathGet(Fx138Path *path, s32 i, SVECTOR *out);
s32 fx_138_op80(s32 mode, Fx138Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

static void fxm_posPathInit(Fx138Path *path, u16 *data);
static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

#endif
