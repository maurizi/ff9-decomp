#ifndef FX_1FC_H
#define FX_1FC_H

/* Effect module fx_1fc (dir 13 file 0x1fc): Fenrir (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1fc effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FENRIR_MORPH_PATH       0x2 /* table: rotation / position path of the morph models (fx_0d2_morphPath) (byte-identical to fx_0d2 entry 0x9) */
#define FX_FENRIR_UNIT_PATHS       0x6 /* table: offset table of per-unit position paths (op83) (byte-identical to fx_0d2 entry 0x8) */
#define FX_FENRIR_BLUE_RINGS       0x9 /* emitter: white-blue gouraud rings (byte-identical to fx_0d2 entry 0x3) */
#define FX_FENRIR_CAMERA_KEYS_3    0xA /* table: camera channel 3 keys (fxm_camChanAdd) */
#define FX_FENRIR_DUST_CLOUD       0xB /* emitter: tan dust clouds and an orange gouraud ring */
#define FX_FENRIR_CAMERA_KEYS_2    0xC /* table: camera channel 2 keys (fxm_camChanAdd) */
#define FX_FENRIR_DUST_DEBRIS      0xD /* emitter: tan dust clouds and gold rock debris quads */
#define FX_FENRIR_CAMERA_KEYS_1    0xE /* table: camera channel 1 keys (fxm_camChanAdd) */
#define FX_FENRIR_SLICE_PATH_2     0x13 /* table: rotation / position path (fx_0d2_slicePath2) (byte-identical to fx_0d2 entry 0x28) */
#define FX_FENRIR_UNIT_OFFSET_PATH 0x17 /* table: position path (fx_1fc_posPath; as fx_0d2 entry 0x14) */
#define FX_FENRIR_UNIT_SCALE_PATH  0x18 /* table: vector path (fx_1fc_vecPath; as fx_0d2 entry 0x15) */
#define FX_FENRIR_MORPH_CURVE      0x1A /* table: value path (fx_0d2_morphCurve2) (byte-identical to fx_0d2 entry 0x19) */
#define FX_FENRIR_MORPH_CURVE_2    0x1B /* table: alternative value path (fx_0d2_morphCurve2) (byte-identical to fx_0d2 entry 0x1A) */

/* fx_1fc choreography (docs/fxdata.md, "Choreography literals"): the short form's ops with Fenrir's (fx_0d2) names:
 * op80 the camera, op81 / op82 one-frame waits, op83 the shattered ground and the white-out (fx_0d2's op83, a
 * frame longer here), op84 the units' drop; and the counts of their pieces. */
#define FX_FENRIR_MODEL_COUNT     0x20 /* op80: btlfx_initEffModels slots */
#define FX_FENRIR_CAMERA_FRAMES   0x78 /* op80 (the three camera key tracks): the task ends at this frame */
#define FX_FENRIR_CAM_CHANNELS    4    /* fxm_camChanReset: frame counters per channel set (fxm_camChanUpdate's ch) */
#define FX_FENRIR_BG_VRAM_X       0x240 /* op80: btlfx_drawBgOffscreen renders the battle background into this VRAM rect */
#define FX_FENRIR_BG_VRAM_Y       0x100 /*   (fx_0d2 uses x 0x2C0) */
#define FX_FENRIR_BG_VRAM_W       0x80
#define FX_FENRIR_BG_VRAM_H       0x80
#define FX_FENRIR_CAM_KEY_SLOTS   64   /* fx_1fc_camChans: camera key tracks (fxm_camChanAdd / fxm_camChanUpdate) */
#define FX_FENRIR_DEBRIS_FRAME    3    /* op83: FX_FENRIR_DUST_DEBRIS starts (turned to rot128 until DEBRIS_STOP_FRAME) */
#define FX_FENRIR_BLUE_RINGS_FRAME 12  /* op83: FX_FENRIR_BLUE_RINGS start at the targets' average position */
#define FX_FENRIR_WHITE_IN_FRAME  22   /* op83: the additive white screen fade-in starts */
#define FX_FENRIR_DEBRIS_STOP_FRAME 38 /* op83: FX_FENRIR_DUST_DEBRIS stops */
#define FX_FENRIR_WHITE_OUT_FRAME 42   /* op83: the units get their scales back, the white fades out */
#define FX_FENRIR_WHITE_FADE_FRAMES 0x10 /* op83: each white fade (fxm_tintScreenLerp) takes this many frames */
#define FX_FENRIR_WHITE_RGB       0xFF, 0xFF, 0xFF /* op83: additive screen tint held between the fades */
#define FX_FENRIR_GROUND_SLICE_FRAMES 0x18 /* op83: fx_1fc_sliceModel follows SLICE_PATH_2 from frame 1 this long */
#define FX_FENRIR_MORPH_FRAMES    0x28 /* op83: the ground morph models are drawn from frame 1 this long, */
#define FX_FENRIR_MORPH_2_FRAME   4    /* morphing 1 -> 2 on MORPH_CURVE, from this frame 2 -> 3 on MORPH_CURVE_2 */
#define FX_FENRIR_MORPH_MODELS    3    /* op83: ground morph models (fx_1fc_morphBlocks, model136[1..3]) */
#define FX_FENRIR_PIECE_MODELS    5    /* op83: rock piece models (fx_1fc_pieceModels, model136[4..8]) */
#define FX_FENRIR_PIECES          10   /* op83: rock pieces flying along fx_1fc_piecePaths (the models in turn) */
#define FX_FENRIR_PIECES_FRAME    1    /* op83: the pieces fly from this frame */
#define FX_FENRIR_PIECES_FRAMES   0x18 /* for this many frames */
#define FX_FENRIR_UNIT_PATH_FRAMES 0x1C /* op83: the units follow UNIT_OFFSET_PATH / UNIT_SCALE_PATH this long */
#define FX_FENRIR_GROUND_FRAMES   0x3D /* op83: the task ends at this frame */
#define FX_FENRIR_DROP_HEIGHT     0xBB8 /* op84: the units start this high above their home positions */
#define FX_FENRIR_DROP_FRAMES     4    /* op84: and drop home in this many frames (FX_FENRIR_DUST_CLOUD at each) */
#define FX_FENRIR_LAND_FRAMES     0x28 /* op84: the task ends at this frame */

typedef struct {
    s16 x, y, z;
} Fx1fcKey6;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fcKey6 *keys4;
} Fx1fcTrack6;

typedef struct {
    s16 x, y, z;
} Fx1fcPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1fcPoint *keys4;
} Fx1fcPath;

extern BtlModelBuf fx_1fc_effModels[];
extern u32 fx_1fc_bgOt[];
extern SVECTOR fx_1fc_unitPos[8]; /* unit positions */
extern VECTOR fx_1fc_unitScale[8]; /* unit scales */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR pos20[8]; /* unit positions */
    SVECTOR emitPos60[8];
    s16 doneA0[8]; /* landed */
} Fx1fcWork84; /* 0xB0 */


static inline void fx1fc_copy(SVECTOR *d, SVECTOR *s) {
    d->vx = s->vx;
    d->vy = s->vy;
    d->vz = s->vz;
}

/* copied */
extern Unk800F78A4 *fx_1fc_env;
/* end copied */

/* propagated */

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx1fcKey; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx1fcKey *keys4;
} Fx1fcPath_0b4;
static s32 fxm_rotPosPathGet(Fx1fcPath_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_offsetTableEntry(u8 *base, u16 idx);

typedef struct {
    u16 unk0; /* not read by the module */
    u16 count2; /* count */
    s16 *keys4;
} Fx1fcKeys;
static s32 fxm_valuePathGet(Fx1fcKeys *keys, s32 t, s32 *out);
static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start);

typedef struct {
    s16 *keys0;
    s16 startFrame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 channel8; /* channel */
    u16 slotsA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx1fcTrack; /* 0x10 */
extern Fx1fcTrack fx_1fc_camKeys[];
#define FX_1FC_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_1fc_camChanFrames[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1fcVec3;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* slice rotation */
    Fx1fcVec3 pos10; /* slice position */
    s32 pad1C;
    Fx1fcVec3 scale20; /* slice scale */
    s32 pad2C;
    SVECTOR rot30;
    Fx1fcVec3 pos38;
    s32 pad44;
    Fx1fcVec3 scale48;
    s32 pad54;
    SVECTOR pos58[8]; /* unit positions */
    VECTOR scale98[8]; /* unit scales */
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR rot128;
    s16 timer130; /* fade counters */
    s16 timer132;
    s16 active134;
    u16 model136[9]; /* models */
} Fx1fcWork83; /* 0x148 */

extern u16 fx_1fc_modelFileIds[3];
extern s16 fx_1fc_texPages[];
extern s16 fx_1fc_texUv[];
extern u8 fx_1fc_effModelTable[];
extern Unk800E8F50 fx_1fc_sliceModelData;
extern Fx1fcKeys fx_1fc_rotPosPath0;
extern Fx1fcKeys fx_1fc_rotPosPath1;
extern Fx1fcKeys fx_1fc_valuePath;
extern Fx1fcKeys fx_1fc_rotPosPath2[10];
extern Fx1fcKeys fx_1fc_posPath;
extern Fx1fcKeys fx_1fc_vecPath;
static void fxm_posPathInit(Fx1fcPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1fcPath *path, s32 t, SVECTOR *out);
static void fxm_rotPosPathInit(Fx1fcPath *path, u16 *data);
static void fxm_valuePathInit(Fx1fcPath *path, u16 *data);
static void fxm_vecPathInit(Fx1fcPath *path, u16 *data);
static s32 fxm_vecPathGet(Fx1fcTrack6 *track, s32 t, VECTOR *out);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
