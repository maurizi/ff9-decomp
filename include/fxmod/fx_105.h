#ifndef FX_105_H
#define FX_105_H

/* Effect module fx_105 (dir 13 file 0x105): Odin, full summon. Ribbon trails (fxm_drawRibbonTrail). Evidence:
 * btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_105 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ODIN_SHOT2_CAMERA_KEYS_3 0x1 /* table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[2]) */
#define FX_ODIN_SHOT0_CAMERA_KEYS_1 0x2 /* table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[0]) */
#define FX_ODIN_SPECK_SPIRAL        0x3 /* emitter: white dot specks rising in a spiral */
#define FX_ODIN_PALE_CLOUD          0x4 /* emitter: pale blue cloud puffs spiralling along an axis (later frames: texture not in the file) */
#define FX_ODIN_SHOT0_CAMERA_KEYS_3 0x5 /* table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[2]) */
#define FX_ODIN_SHOT0_CAMERA_KEYS_4 0x6 /* table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[3]) */
#define FX_ODIN_SHOT0_CAMERA_KEYS_9 0x7 /* table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[8]) */
#define FX_ODIN_SHOT0_CAMERA_KEYS_2 0x8 /* table: camera key path (fxm_camKeyAdd from frame 0x0 into env->camera[1]) */
#define FX_ODIN_SHOT1_CAMERA_KEYS_1 0x9 /* table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[0]) */
#define FX_ODIN_SHOT1_CAMERA_KEYS_3 0xA /* table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[2]) */
#define FX_ODIN_PALE_CLOUD_2        0xB /* emitter: pale blue cloud puffs spiralling (later frames: texture not in the file) */
#define FX_ODIN_STONE_RIBBON        0xD /* 'so' model: wavy strip with a dark green stone texture (model138[0]) */
#define FX_ODIN_WHITE_DEBRIS        0xE /* emitter: white crumbling debris + specks */
#define FX_ODIN_STAR                0x10 /* sprite: small dark-grey speck, drawn as the star field (fx_105_stars) */
#define FX_ODIN_MOON                0x11 /* emitter: pale blue full moon sprite (+ light beam) */
#define FX_ODIN_RIBBON_PATH_4       0x14 /* table: world-space ribbon path (fx_105_drawWorldRibbon) */
#define FX_ODIN_STONE_RIBBON_2      0x15 /* 'so' model: thin wavy strip with a dark green stone texture (model138[1]) */
#define FX_ODIN_SHOT1_CAMERA_KEYS_2 0x16 /* table: camera key path (fxm_camKeyAdd from frame 0xA7 into env->camera[1]) */
#define FX_ODIN_SHOT2_CAMERA_KEYS_1 0x17 /* table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[0]) */
#define FX_ODIN_CAM_RIBBON_PATH     0x18 /* table: camera-space ribbon path (fx_105_drawCamRibbon) */
#define FX_ODIN_SHOT2_CAMERA_KEYS_2 0x19 /* table: camera key path (fxm_camKeyAdd from frame 0xFB into env->camera[1]) */
#define FX_ODIN_SHOT3_CAMERA_KEYS_1 0x1A /* table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[0]) */
#define FX_ODIN_SHOT3_CAMERA_KEYS_3 0x1B /* table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[2]) */
#define FX_ODIN_SHOT3_CAMERA_KEYS_2 0x1C /* table: camera key path (fxm_camKeyAdd from frame 0x137 into env->camera[1]) */
#define FX_ODIN_DEBRIS_RINGS        0x1D /* emitter: grey-green gouraud rings + pale debris chips + specks */
#define FX_ODIN_DEBRIS_CONE         0x1E /* emitter: pale debris chips in a cone + specks */
#define FX_ODIN_DEBRIS_SPIRAL       0x1F /* emitter: white crumbling debris in a spiral + specks */
#define FX_ODIN_RIBBON_PATH         0x20 /* table: world-space ribbon path (fx_105_drawWorldRibbon) */
#define FX_ODIN_RIBBON_PATH_2       0x22 /* table: world-space ribbon path (fx_105_drawWorldRibbon) */
#define FX_ODIN_RIBBON_PATH_3       0x23 /* table: world-space ribbon path (fx_105_drawWorldRibbon) */
#define FX_ODIN_RIBBON_PATH_5       0x25 /* table: world-space ribbon path (fx_105_drawWorldRibbon) */
#define FX_ODIN_DEBRIS_BURST        0x28 /* emitter: pale debris burst + specks */
#define FX_ODIN_STONE_RIBBON_3      0x29 /* 'so' model: thin wavy strip with a dark green stone texture (model138[2]) */
#define FX_ODIN_STONE_RIBBON_4      0x2A /* 'so' model: thin wavy strip with a dark green stone texture (model138[3]) */
#define FX_ODIN_STONE_RIBBON_5      0x2B /* 'so' model: thin wavy strip with a dark green stone texture (model138[4]) */
#define FX_ODIN_STONE_RIBBON_6      0x2C /* 'so' model: thin wavy strip with a dark green stone texture (model138[5]) */
#define FX_ODIN_STONE_RIBBON_7      0x2D /* 'so' model: thin wavy strip with a dark green stone texture (model138[6]) */
#define FX_ODIN_STONE_RIBBON_8      0x2E /* 'so' model: thin wavy strip with a dark green stone texture (model138[7]) */
#define FX_ODIN_SMOKE_SPIRAL        0x3B /* emitter: grey smoke wisps in a spiral + specks */
#define FX_ODIN_WHITE_FOLD_0        0x80 /* summon data entry 0: 'so' model, folded white sheet */
#define FX_ODIN_WHITE_FOLD_1        0x81 /* summon data entry 1: 'so' model, folded white sheet */
#define FX_ODIN_WHITE_FOLD_2        0x82 /* summon data entry 2: 'so' model, folded white sheet */
#define FX_ODIN_WHITE_FOLD_3        0x83 /* summon data entry 3: 'so' model, folded white sheet */

/* fx_105 choreography (docs/fxdata.md, "Choreography literals"): op80's keyed timeline (night sky, Odin's
 * motions, the four camera shots, Zantetsuken's cut and flashes), the lengths written into its countdown fields,
 * the helpers' meshes and textures. */
#define FX_ODIN_MODEL_SLOTS        0xF   /* btlfx_initEffModels slots */
#define FX_ODIN_STAR_COUNT         32    /* fx_105_stars: screen-space stars of the night sky */
#define FX_ODIN_WIND_STREAK_COUNT  32    /* fx_105_windStreaks */
#define FX_ODIN_CAM_KEY_SLOTS      16    /* fx_105_camKeys: camera key tracks (fxm_camKeyAdd) */
#define FX_ODIN_STONE_RIBBONS      8     /* model138: the STONE_RIBBON models, each on its own delay (fx_105_modelTimers) */
#define FX_ODIN_MOON_FRAMES        0x71  /* timer8 from frame 0: MOON follows camera point 2, fading in */
#define FX_ODIN_NIGHT_FRAMES       0x93  /* timerC from frame 0: the blue night band over the screen */
#define FX_ODIN_NIGHT_FADE_IN_FRAMES 0xF /* its fade-in (the fade-out is the last 8 frames) */
#define FX_ODIN_STARS_FRAMES       0x78  /* timer30 from frame 0: the stars twinkle */
#define FX_ODIN_STARS_DRIFT_FRAMES 0x28  /* and drift left for the last this many frames */
#define FX_ODIN_APPEAR_FRAME       0xF   /* Odin appears (summon motion 0) */
#define FX_ODIN_BLADE_RIBBON_FRAME 0x1E  /* timer4: the ribbon trail along Odin's bone 0x27 */
#define FX_ODIN_BLADE_RIBBON_FRAMES 0x10
#define FX_ODIN_DEBRIS_FRAME       0x21  /* SPECK_SPIRAL, PALE_CLOUD(_2) and two WHITE_DEBRIS (on bones 0x4C / 0x60) start */
#define FX_ODIN_STONE_FRAME        0x41  /* flag10: the STONE_RIBBON models draw from this frame */
#define FX_ODIN_GALLOP_FRAME       0x51  /* four DEBRIS_SPIRALs on Odin's bones and the wind streaks (timer24) */
#define FX_ODIN_WIND_FRAMES        0x30  /* timer24 */
#define FX_ODIN_MOTION_1_FRAME     0x55  /* summon motion 1 */
#define FX_ODIN_MOTION_2_FRAME     0x7F  /* summon motion 2 */
#define FX_ODIN_SHOT0_END_FRAME    0x93  /* the first shot ends: emitters stop, Odin hides, summon motion 3 */
#define FX_ODIN_SHOT1_FRAME        0xA7  /* camera shot 1 (fxm_camKeyAdd); Odin shows again from 0x9C */
#define FX_ODIN_DEBRIS_RINGS_FRAME 0xAB  /* DEBRIS_RINGS at camera point 2 */
#define FX_ODIN_CUT_FRAME          0xD8  /* timer14: black screen, then the cut models */
#define FX_ODIN_CUT_FRAMES         0x21
#define FX_ODIN_CAM_RIBBON_FRAME   0xE0  /* timer28: the ribbon along CAM_RIBBON_PATH */
#define FX_ODIN_CAM_RIBBON_FRAMES  8
#define FX_ODIN_MOTION_4_FRAME     0xE7  /* summon motion 4 */
#define FX_ODIN_FLASH_FRAME        0xEC  /* timer1C: the white flash band */
#define FX_ODIN_FLASH_FRAMES       0x10
#define FX_ODIN_SHOT2_FRAME        0xFB  /* camera shot 2; the frame after: summon motion 5, the targets' motions held, DEBRIS_CONEs */
#define FX_ODIN_DIM_FRAMES         0x30  /* timer20 from that frame: the dimming band */
#define FX_ODIN_LIGHT_FRAMES       0x3C  /* timer18 from that frame: the light band */
#define FX_ODIN_SHOT3_FRAME        0x137 /* camera shot 3; the frame after: summon motion 6, DEBRIS_BURSTs */
#define FX_ODIN_RELEASE_FRAME      0x146 /* the targets' motions are released */
#define FX_ODIN_SMOKE_FRAME        0x165 /* Odin leaves: emitters stop, SMOKE_SPIRAL, the world ribbons (timer2C) */
#define FX_ODIN_SMOKE_FRAMES       0x1C  /* timer2C */
#define FX_ODIN_CAMERA_RESET_FRAME 0x185 /* camera point 0 is reset */
#define FX_ODIN_FRAMES             0x1B0 /* op80's length (the op ends) */
#define FX_ODIN_WIND_TPAGE         0x3A   /* fx_105_drawWindStreaks texture: 4-bit page (0x280, 0x100), abr 1 */
#define FX_ODIN_WIND_CLUT          0x3D00 /* CLUT at (0x0, 0xF4) */
#define FX_ODIN_RIBBON_TPAGE       0xB8   /* the camera / world ribbons' texture: 8-bit page (0x200, 0x100), abr 1 */
#define FX_ODIN_CAM_RIBBON_CLUT    0x3D80 /* fx_105_drawCamRibbon's CLUT at (0x0, 0xF6) */
#define FX_ODIN_WORLD_RIBBON_CLUT  0x3DC0 /* fx_105_drawWorldRibbon's CLUT at (0x0, 0xF7) */
#define FX_ODIN_CAM_RIBBON_POINTS  0x11  /* fx_105_camRibbonPts */
#define FX_ODIN_WORLD_RIBBON_POINTS 0x40 /* fx_105_worldRibbonPts */

extern Unk800F78A4 *fx_105_env;
typedef struct {
    s16 vx, vy, vz;
    s16 pad, unk8, unkA; /* unused */
} Fx105Spark; /* 0xC */

extern Fx105Spark fx_105_windStreaks[FX_ODIN_WIND_STREAK_COUNT];

void fx_105_drawScreenBand(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb);
void fx_105_initWindStreaks(void);
void fx_105_drawWindStreaks(s32 bright);
void fx_105_drawWorldRibbon(s16 *data, s32 bright, s32 unused, s32 n);
void fx_105_drawCamRibbon(s16 *data, s32 bright, s32 halfWidth, s32 n);

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
    s32 z;
} Fx105RibbonPt; /* 0xC */

extern Fx105RibbonPt fx_105_worldRibbonPts[FX_ODIN_WORLD_RIBBON_POINTS];

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
} Fx105BandPt; /* 8 */

extern Fx105BandPt fx_105_camRibbonPts[FX_ODIN_CAM_RIBBON_POINTS];

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
} Fx105Seg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx105Seg *buf, s32 abr);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
    } *summon54;
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx105Summon;

typedef struct {
    s32 visible0; /* summon visible */
    s32 timer4; /* countdowns */
    s32 timer8;
    s32 timerC;
    s32 flag10;
    s32 timer14;
    s32 timer18;
    s32 timer1C;
    s32 timer20;
    s32 timer24;
    s32 timer28;
    s32 timer2C;
    s32 timer30;
    s32 pad34[3];
    Pool6CEntry *emit40;
    Pool6CEntry *emit44;
    Pool6CEntry *emit48;
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    Pool6CEntry *emit54;
    SVECTOR pos58;
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    SVECTOR pos78;
    SVECTOR pos80;
    SVECTOR pos88;
    SVECTOR pos90;
    SVECTOR pos98;
    SVECTOR posA0;
    Pool6CEntry *emitA8;
    Pool6CEntry *emitAC;
    Pool6CEntry *emitB0;
    SVECTOR posB4;
    SVECTOR posBC;
    SVECTOR posC4;
    SVECTOR rotCC;
    Pool6CEntry *emitD4;
    Pool6CEntry *emitD8;
    Pool6CEntry *emitDC;
    Pool6CEntry *emitE0;
    SVECTOR posE4;
    SVECTOR posEC;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR pos104;
    SVECTOR pos10C;
    SVECTOR pos114;
    CVECTOR color11C;
    CVECTOR color120;
    Pool6CEntry *emit124;
    SVECTOR pos128;
    SVECTOR pos130;
    s32 model138[FX_ODIN_STONE_RIBBONS]; /* models */
    s32 pos158[3];
    s32 pad164;
    SVECTOR rot168;
    s32 scale170[3];
    s32 pad17C;
    s32 model180;
    s32 model184;
    s32 model188;
    s32 model18C;
    SVECTOR rot190;
    s32 pos198[3];
    s32 pad1A4;
    s32 scale1A8[3];
    s32 pad1B4;
    s32 hold1B8[8];
} Fx105Work80; /* 0x1D8 */

typedef struct {
    s32 posX0; /* x */
    s32 posY4; /* y */
    s32 speed8; /* speed */
    s32 padC;
    s32 flicker10;
} Fx105Drop; /* 0x14 */

typedef struct {
    s32 timer0; /* delay */
    s32 timer4; /* frames */
} Fx105Flash; /* 8 */

extern Fx105Flash fx_105_modelTimers[8];
extern Fx105Seg fx_105_ribbonSegs[];
extern SVECTOR fx_105_modelRot;
extern VECTOR fx_105_modelPos;
extern VECTOR fx_105_modelScale;
extern BtlModelBuf fx_105_effModels[];
extern Fx105Drop fx_105_stars[FX_ODIN_STAR_COUNT];
static void fxm_camKeyUpdate(void);

#define SUMMON105(env) ((Fx105Summon *)(env)->loader)

/* propagated */

typedef struct {
    s32 frame0; /* start frame */
    s32 timer4; /* frames left, -1 = free */
    u32 dstSlots8; /* destination slots, 4 bits each (1-based) */
    u32 srcSlotsC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *keys10; /* data */
} Fx105Track; /* 0x14 */

extern s32 fx_105_camFrame; /* frame */
extern Fx105Track fx_105_camKeys[FX_ODIN_CAM_KEY_SLOTS];
#define FX_105_KEY_FREE (-1) /* timer4: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_105_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
