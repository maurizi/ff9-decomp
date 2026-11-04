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

extern Unk800F78A4 *fx_105_env;
typedef struct {
    s16 vx, vy, vz;
    s16 pad, unk8, unkA; /* unused */
} Fx105Spark; /* 0xC */

extern Fx105Spark fx_105_windStreaks[];

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

extern Fx105RibbonPt fx_105_worldRibbonPts[];

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
} Fx105BandPt; /* 8 */

extern Fx105BandPt fx_105_camRibbonPts[];

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
    s32 model138[8]; /* models */
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
extern Fx105Drop fx_105_stars[32];
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
extern Fx105Track fx_105_camKeys[];
#define FX_105_KEY_FREE (-1) /* timer4: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_105_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
