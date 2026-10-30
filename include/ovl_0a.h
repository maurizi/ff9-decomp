#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* texture animation as ModelObj.texAnims shows it (fld_isModelTexAnimActive, fld_drawModels): TexAnim's layout */
typedef struct {
    u8 flags; /* TEXANIM_ACTIVE / TEXANIM_ONESHOT */
    u8 pad1[7];
    s32 pos; /* frame position (20.12) */
    u8 padC[2];
    s16 lastFrame; /* frame last uploaded; 0x1000 forces the next upload */
    u8 pad10[4];
} Unk800AB0B0Entry; /* 0x14 */

typedef struct Unk800AB0B0 {
    u16 count; /* animations (TexAnimSet.count) */
    u8 pad2[2];
    Unk800AB0B0Entry anims[1];
} Unk800AB0B0;

/* 0x24-byte entries at FieldSpriteSet.unk8 (fld_spriteCommand sets them) */
typedef struct {
    u32 flags : 4; /* flags */
    u32 abr : 4;
    s32 spriteId : 8;
    s32 parent : 8;
    u32 bone : 8;
    s16 scale;
    u8 speed;
    u8 brightness; /* colour scale: fld_drawSprite3D gets brightness << 4 (GPF, 4096 = 1.0); 0x80 default, sprite command 0x9B */
    s16 pos[3]; /* position */
    s16 frameEnd;
    s16 rot[3]; /* rotation */
    s16 frame;
    void *sprite;
    s16 offset[3]; /* offset */
    s16 otOffset;
} FieldSprite; /* 0x24 */

/* initialised by fld_initSprites */
typedef struct FieldSpriteSet {
    u16 unk0;
    u8 capacity;
    u8 count; /* count of 0x24-byte entries at unk8 */
    u16 *unk4; /* 16 entries, follows this header */
    FieldSprite *entries;
    u8 *primBufs[2]; /* primitive buffers, per draw buffer */
} FieldSpriteSet; /* 0x14, followed by the buffers it points to */

/* trigger table entry (fld_removeAnimSfx) */
typedef struct FieldAnimSfx {
    u8 uid; /* object uid; low 2 bits also select a channel */
    u8 randomPitch;
    u16 modelId; /* the object's modelId when added (fld_addAnimSfx) */
    u16 anim;
    s16 frame;
    u16 sound[2];
} FieldAnimSfx; /* 0xC */

/* script object (fld_removeAnimSfx) */
typedef struct {
    u8 pad0[0x14];
    struct {
        s32 vx, vy, vz;
    } pos; /* position */
    s16 rotX; /* rotation */
    s16 rotY;
    s16 rotZ;
    u8 pad26[0x34 - 0x26];
    u16 modelId; /* EvtObject.modelId */
    u16 anim;
    u8 pad38[2];
    u8 animFrame;
    u8 uid; /* uid */
    u8 pad3C[0x4C - 0x3C];
} FieldEvtObj; /* 0x4C */

typedef struct {
    u32 flags; /* flags */
    s32 modelId; /* model id (fld_setupActorModel), key of fld_findLightColor */
    void *model; /* model (ModelObj) */
    FieldEvtObj *obj;
} FieldEvtCtx;
/* FieldEvtCtx.flags / FieldObjSlot.flags */
#define FLD_OBJ_TILT 0x2 /* fld_drawModels tilts the model to the floor (bgi_getActorTilt) */
#define FLD_OBJ_DIR_LIGHT 0x4 /* fld_applyLights uses the directional light */
#define FLD_OBJ_NO_SHADOW 0x10 /* fld_drawShadow skips the object (event ops 0x7F/0x80; runtime: removes Zidane's shadow) */
#define FLD_OBJ_SHADOW_FIXED 0x20 /* fld_calcShadowMatrix uses the FieldObjEnt placement (event ops 0x83/0x84) */
#define FLD_OBJ_NO_ANIM_SFX 0x40 /* fld_playAnimSfx skips the object */
#define FLD_OBJ_LIT 0x100 /* light colour applied (fld_applyLights) */
#define FLD_OBJ_COLOR_SET 0x400 /* fld_setSlotColor; fld_applyLights keeps it */
#define FLD_OBJ_SEMITRANS 0x10000 /* fld_modelCommand 0, applied by fld_applyModelCommands */
#define FLD_OBJ_SEMITRANS_AGAIN 0x20000 /* fld_applyModelCommands: repeat the semi-transparency write for the other buffer */
#define FLD_OBJ_CLIP_Y 0x100000 /* fld_modelCommand 4: fld_drawModels uses the ClipY mesh functions */

/* 0x330-byte block, FieldMapObj.unk14 (allocated by fld_allocScene) */
typedef struct {
    u32 flags; /* flags */
    s32 loadEnd; /* end of the load area */
    u8 unk8;
    s8 encounterMusicStep;
    s16 nextId; /* current field, then the destination set by game_setNextScene */
    void *eventScript; /* db item 0x5xxxx (fld_findEventScript), passed to evt_init */
    struct FieldSpriteSet *sprites;
    struct BgFile *bg; /* background (fld_setupBackground) */
    void *walkmesh; /* the background's 0xB0000 link (fld_setupBackground) */
    void *lights; /* light data, file 0x1Fxxxx (fld_applyLights) */
    s32 heapTop;
    void *fieldFile; /* container registered by fld_loadFile */
    s32 *modelBundles; /* = modelBundleBuf, modelBundleCount entries (fld_loadModelBundle) */
    struct FieldObjPlace *shadows; /* shadow entries (fld_initShadows, fld_drawShadow) */
    s16 otBias; /* added to projected depths before indexing the OT (camera unk30, fldbg_setCamera) */
    u8 pad32[0x34 - 0x32];
    u8 *movieWork;
    s16 encounterScreenX;
    s16 encounterScreenY;
    u16 battleMusic;
    u16 battleMusicBank;
    u8 modelBundleCount;
    s8 encounterStep;
    s8 movieState;
    u8 movieRgb24;
    s32 movieFile;
    u8 movieList;
    u8 movieEntry;
    u8 exitReason; /* FLD_EXIT_*, set by fld_main, dispatched by fld_exit */
    u8 animSfxCount; /* animSfx entry count */
    void *vibration; /* file 0x1Exxxx */
    s16 shadowTexX;
    s16 shadowTexY;
    struct FieldActorInfo *actorInfo; /* = actorInfoBuf */
    struct FieldAnimSfx *animSfx; /* = animSfxBuf */
    u8 sndVolume;
    u8 movieSndVolume;
    u8 soundBankSlot;
    u8 unk5F;
    u8 modelQueueCount; /* g_fieldModelQueue count */
    u8 unk61[1];
    s16 unk62;
    s32 unk64;
    u8 unk68[4];
    s32 modelBundleBuf[9]; /* fld_allocScene */
    u8 actorInfoBuf[0x270 - 0x90]; /* FieldActorInfo[24] (fld_allocScene) */
    u8 animSfxBuf[0x330 - 0x270]; /* FieldAnimSfx[16] (fld_allocScene) */
} FieldScene;
/* FieldScene.flags */
#define FLD_SCN_ENCOUNTER_DONE 0x1 /* encounter screen effect (func_80053A10) finished */
#define FLD_SCN_ENCOUNTER_LOADED 0x2 /* fld_updateEncounter uploaded file 0x1000B and played its SEs */
#define FLD_SCN_BANK_SLOT 0x10 /* fld_soundBankReadCallback uploads to soundBankSlot */
#define FLD_SCN_RAIN_ALLOC 0x100 /* fld_enableRain allocated the main rain_init node buffer */
#define FLD_SCN_RAIN_ON 0x200 /* fld_drawRain runs main rain_draw each frame */
#define FLD_SCN_MOVIE_DISPMASK 0x1000 /* fld_movieUpdate set g_padPortMask bit 0x04000000 */
#define FLD_SCN_ACTORS_READY 0x10000 /* set by fld_setupActorModel; fld_preloadHook starts queuing */
#define FLD_SCN_PRELOADS_QUEUED 0x100000 /* fld_preloadHook queued fld_queueFieldPreloads */
#define FLD_SCN_PRELOAD_REQUESTED 0x200000 /* fld_preloadRequest accepted a request */
#define FLD_SCN_PRELOAD_RESTART 0x800000 /* fld_preloadHook re-queues once the queue drains */

/* FieldScene.exitReason: why the field loop ended (fld_main sets it from the event system's result, fld_exit
 * switches on it and sets GameState.mode to the same number; docs/encounters.md, docs/runtime-findings.md) */
#define FLD_EXIT_FIELD 1 /* next field (fld_depFindField / fld_depFindArea) */
#define FLD_EXIT_BATTLE 2 /* event result 3: battle, mode 2 */
#define FLD_EXIT_WORLD 3 /* event result 5: world map, mode 3 */
#define FLD_EXIT_GAME_OVER 7 /* event result 8 (EVT_OP_GAME_OVER): mode 7, fld_gameOver */
#define FLD_EXIT_TETRA_MASTER 9 /* event result 7: mode 9 loads ovl_0d (Tetra Master, docs/ff9img.md) */

/* 0x38-byte object hanging off FieldMap.unkC */
typedef struct {
    s32 unk0;
    s32 loadEnd;    /* end of the load area */
    void *msgTable;  /* file 0x6xxxx */
    void *areaFile;
    u16 areaId;
    u8 unk12[2];
    FieldScene *scene;
    u8 unk18[0x20];
} FieldMapObj;

/* field context, GameData.unk8E4.field (0x50 bytes) */
typedef struct FieldMap {
    u32 flags; /* flags */
    s32 loadEnd;
    s8 playerObj;
    s8 unk9;
    s16 prevFieldId;
    FieldMapObj *area;
    void *commonFile;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x20 - 0x1C];
    s32 soundBankSize;
    u8 unk24[0x2C];
} FieldMap;
/* FieldMap.flags (g_fieldFlagQueue.mapFlags queues changes one frame ahead) */
#define FLD_MAP_NO_WINDOWS 0x1 /* fld_main skips func_80023A0C (win_updateAll/win_drawAll) */
#define FLD_MAP_NO_BGI_ANIM 0x8 /* fld_main skips bgi_updateAnims/bgi_updateFloorMoves */
#define FLD_MAP_NO_DISPENV 0x20 /* fld_endFrame skips PutDispEnv */
#define FLD_MAP_NO_DRAW 0x40 /* fld_endFrame skips the OT draw */
#define FLD_MAP_SHADOW_MASK 0x200 /* fld_drawShadow wraps the shadow quads in mask-bit packets */
#define FLD_MAP_CLEAR_ONLY 0x400 /* fld_endFrame only does PutDrawEnv */
#define FLD_MAP_NO_PRELOAD 0x1000 /* fld_main skips preload_update (fld_movieStart, main menu_doOpen) */
#define FLD_MAP_REFIND_SECTORS 0x10000 /* fld_exit keeps the area id, fld_refindSectors rescans the directory */
#define FLD_MAP_DISC_CHANGE 0x100000 /* fld_requestDiscChange, fld_isDiscChangePending */

/* walkmesh flag bits (bgi_dumpText "Flags"; code in bgi.c / bgi_move.c) */
#define BGI_FLAG_ACTIVE 0x1 /* triangle/floor walkable, actor entry or animation active */
#define BGI_ANIM_PLAYING 0x4 /* BgiAnim / BgiFloorMove: advancing, cleared at the end */
#define BGI_ANIM_BOUNCE 0x10 /* BgiAnim / BgiFloorMove: reverse at the ends instead of wrapping */
#define BGI_TRI_VISITED 0x80 /* BgiTriangle: marked during a neighbour search, cleared before each */
/* triangle mask byte (flags >> 8, ANDed with g_fieldBgi.maskFilter) */
#define BGI_MASK_NO_PLAYER 0x80 /* blocks the player actor */
#define BGI_MASK_NO_OTHERS 0x40 /* blocks every other actor */
/* bgi_moveActor / bgi_updateActor results */
#define BGI_MOVE_BLOCKED 0x1 /* position reverted (main event.c then calls bgi_revertActor) */
#define BGI_MOVE_OK 0x2
#define BGI_MOVE_STILL 0x4 /* position unchanged */
#define BGI_MOVE_PUSHED 0x8 /* bgi_applyPushes moved the actor */
#define BGI_MOVE_CORRECTED 0x10 /* x/z changed by the collision */
#define BGI_ERR_NO_MESH 0x8000 /* no walkmesh loaded */
#define BGI_ERR_BAD_ACTOR 0x4000 /* actor index >= actorCount */

/* walkmesh actor entry (g_fieldBgi.actors, 0x28 bytes) */
typedef struct BgiActor {
    s16 flags;
    s16 unk2;
    s16 floor;
    s16 triangle;
    s16 lastFloor;
    s16 lastTriangle;
    u8 padC[0x1C - 0xC];
    s32 radius;
    s32 radiusSq;
    EvtObject *actor;
} BgiActor;

typedef struct {
    u8 mode;
    u8 discMask;
    u8 disc;
    u8 cursor;
    s16 fieldId;
    s16 areaId;
    s32 savedFlags[2]; /* saved g_padPortMask */
} FieldDiscChange;

typedef struct {
    void (*enter)(void); /* enter */
    void (*update)(u8);   /* update */
    void (*leave)(void); /* leave */
} FieldDiscMode; /* 0xC */

typedef struct {
    u16 x;
    u16 y;
} __attribute__((aligned(4))) BgPoint;

/* field state (g_fieldBgState) */
typedef struct {
    s32 flags; /* flags */
    BgPoint scroll;
    BgPoint cameraScroll;
    s16 geomOffsetX; /* geometry offset adjust */
    s16 geomOffsetY;
    BgPoint scrollFrom;
    s16 scrollToX;
    s16 scrollToY;
    s16 scrollFrame;
    s16 scrollFrames;
    MATRIX *viewMatrix;
    u16 *projDist;
    u16 savedScrollMinX;
    u16 savedScrollMaxX;
    u16 savedScrollMinY;
    u16 savedScrollMaxY;
    s16 playerScreenX; /* projected origin */
    s16 playerScreenY;
    u8 pad30[0x72 - 0x30];
    u8 unk72;
    u8 unk73;
    u8 unk74;
    u8 cameraId;
    s16 followHeight;
    s32 unk78;
    u8 pad7C[0x9C - 0x7C];
    s16 unk9C;
    u8 pad9E[2];
    u8 unkA0;
    u8 unkA1;
    u8 unkA2;
    u8 unkA3;
    s16 parallaxOriginX;
    s16 parallaxOriginY;
    s16 viewportPos[4][2];
    s16 viewportSize[4][2];
    s16 viewportRateX[4];
    s16 viewportRateY[4];
} FieldBgState;
/* FieldBgState.flags */
#define FLDBG_CAMERA_CHANGED 0x80 /* fldbg_setCamera/fldbg_setOverlayPos; fldbg_updateOverlayScroll clears it */

/* field background file (g_fieldBgData = g_fieldBgCtx.unk10, fld_setupBackground): the header holds
 * offsets (from the file start) of its overlay-animation, overlay and camera tables */
typedef struct BgFile {
    u8 pad0[4];
    u16 overlayAnimCount; /* entries in the unkC table */
    u16 overlayCount; /* entries in the unk10 table */
    u8 pad8[0xC - 8];
    s32 overlayAnimOffset;  /* offset: BgOverlayAnim[] */
    s32 overlayOffset; /* offset: BgOverlay[] */
    u8 pad14[0x18 - 0x14];
    s32 cameraOffset; /* offset: BgCamera[] */
    u16 orgDepth;
    u16 baseDepth;
    u16 orgX;
    u16 orgY;
    u16 posX;
    u16 posY;
    u8 pad28[2];
    u16 scrollMaxX; /* the current camera's (fldbg_setCamera) */
    u8 pad2C[2];
    u16 scrollMaxY;
    u16 screenX; /* screen origin x */
    u16 screenY; /* screen origin y */
} BgFile;

typedef struct {
    u8 overlay;
    s8 delay;
} BgAnimStep;

typedef struct {
    unsigned flags : 8; /* flags */
    unsigned stepCount : 24; /* entries at unkC */
    unsigned cameraId : 8;
    signed pos : 24; /* position (8.8 step index) */
    s16 speed; /* added to pos per step (8.8, 0x100 = one step; fldbg_setAnimSpeed, negated by FLDBG_ANIM_PINGPONG) */
    s16 delayCounter;
    s32 stepOffset; /* offset: BgAnimStep[] */
} BgOverlayAnim; /* 0x10 */
/* BgOverlayAnim.flags */
#define FLDBG_ANIM_ACTIVE 0x2 /* fldbg_updateOverlayAnims runs it */
#define FLDBG_ANIM_PLAY_ONCE 0x4 /* cleared when the end is reached */
#define FLDBG_ANIM_LOOP 0x10 /* fldbg_setAnimFlags: keeps running after the wrap */
#define FLDBG_ANIM_PINGPONG 0x20 /* reverses at either end */

/* background sprite: DR_TPAGE followed by a SPRT_16 (0x18 bytes) */
typedef struct {
    DR_TPAGE tpage;
    SPRT_16 sprt;
} BgSprt;

/* packed tile position: x in the top 10 bits, y below it */
typedef struct {
    unsigned pad : 12;
    unsigned y : 10;
    unsigned x : 10;
} BgPos;

/* packed tile texture info (8 bytes) */
typedef struct {
    unsigned clutY : 9;
    unsigned clutX : 6;
    unsigned tpY : 1;
    unsigned tpX : 4;
    unsigned tp : 2;
    unsigned abr : 2;
    unsigned v : 8;
    unsigned u : 8;
    unsigned pad : 20;
    unsigned trans : 1;
} BgTile;

typedef struct {
    unsigned flags : 8; /* flags */
    unsigned depth : 12; /* depth */
    unsigned baseDepth : 12; /* depth */
    u16 width; /* scroll wrap x */
    u16 height; /* scroll wrap y */
    s16 baseX;
    s16 baseY;
    s16 x;
    s16 y;
    s16 minX;
    s16 maxX;
    s16 minY;
    s16 maxY;
    u16 screenX;
    u16 screenY;
    s16 scrollX;
    s16 scrollY;
    s16 fracX;
    s16 fracY;
    unsigned cameraId : 8;
    unsigned skewVertical : 1;
    unsigned viewport : 7;
    u16 tileCount;  /* tile count */
    s32 posOffset;  /* offset: BgPos[] */
    s32 tileOffset;  /* offset: BgTile[] */
    BgSprt *sprites;
    u8 pad34[4];
} BgOverlay; /* 0x38 */
/* BgOverlay.flags */
#define FLDBG_OVL_SCREEN 0x1 /* fldbg_drawOverlay: placed in its viewport without scroll compensation */
#define FLDBG_OVL_VISIBLE 0x2 /* fldbg_showOverlay; fldbg_draw draws it; flipbook steps move it between layers */
#define FLDBG_OVL_LOOP 0x4 /* fldbg_setOverlayLoop: wraps by width/height, auto-scrolls */
#define FLDBG_OVL_PARALLAX 0x8 /* fldbg_setOverlayParallax */
#define FLDBG_OVL_ANIM_GROUP 0x20 /* flipbook step layer: fldbg_initEffectList skips the group's steps */
#define FLDBG_OVL_ANIMATED 0x40 /* fldbg_initEffectList adds an effect entry */
#define FLDBG_OVL_SKEW_LOOP 0x80 /* fldbg_setOverlaySkewLoop */

/* BgOverlay's unk24/unk25/unk25_1 bitfield word as a plain u32 (fldbg_buildOverlaySprites masks it by hand) */
typedef struct {
    u8 pad0[0x24];
    u32 unk24;
} BgOverlayFlags;

typedef struct {
    u16 projDist; /* projection distance */
    s16 rot[3][3];
    s32 trans[3];
    u16 centerX;
    u16 centerY;
    s16 width;
    s16 height;
    s16 scrollMinX;
    s16 scrollMaxX;
    s16 scrollMinY;
    s16 scrollMaxY;
    u16 otBias;
    u8 pad32[0x34 - 0x32];
} BgCamera; /* 0x34 */

/* the same 0x10-byte table, as animation state (BgOverlayAnim without the bitfields, fldbg_init) */
typedef struct {
    u8 flags;
    u8 pad1[3];
    u32 cameraPos; /* cameraId : 8, pos : 24 */
    s16 speed;
    s16 delayCounter;
    s32 stepOffset; /* offset: BgAnimStep[] (first byte: the first step's overlay) */
} BgOverlayAnimView;

typedef struct BgEntry {
    u8 type;
    u8 overlay;
    u8 pad2[2];
    s32 dataOffset;
    s32 dataSize;
} BgEntry; /* 0xC */

typedef struct {
    u8 *bufIndex; /* -> draw buffer index */
    s32 projDist;
    u32 *ot[2]; /* ordering tables, per draw buffer */
    BgFile *data;
    s32 viewMatrix;
    u8 *heap;
    u8 *overlayRgb; /* 3 bytes per entry */
    u8 *overlayTinted;
    u8 entryCount;
    u8 pad25[3];
    BgEntry *entries;
    s32 work;
    s32 workSize;
    s32 workUsed;
} FieldBgCtx;

/* field light (12 bytes) */
typedef struct {
    s8 type; /* type: 1, 2 */
    u8 pad1;
    s8 shadowShade; /* sprite colour offsets */
    s8 shadowSize;
    u32 color; /* colour */
    s32 floors; /* 4 floor ids (type 0) */
} FieldLight;

/* 8-byte entries following Unk800B3254.unkC[unk6] */
typedef struct {
    u16 modelId; /* id */
    s8 shadowShade;
    s8 shadowSize;
    u32 color; /* colour; top byte: the shadow's OT bias (Unk800B3294Bias) */
} Unk800B3294;

typedef struct {
    u8 pad0[6];
    u8 colorOffset; /* count */
    u8 lightCount; /* count */
    u8 pad8;
    u8 colorCount;
    u8 padA[2];
    FieldLight lights[1];
} Unk800B3254;

/* 6-byte vertex entries at g_fieldBgi.unkC */
typedef struct BgiVertex {
    s16 vx;
    s16 vy;
    s16 vz;
} BgiVertex;

/* the same vertex viewed as an array (bgi_pointInTriangle) */
typedef struct {
    s16 xyz[3];
} BgiVertexArr;

/* header of the block g_fieldBgi.unk8 points to */
typedef struct {
    u16 dataSize; /* data size */
    s16 orgX; /* original position */
    s16 orgY;
    s16 orgZ;
    s16 curPos[3]; /* origin (current position) */
    s16 minX; /* minimum */
    s16 minY;
    s16 minZ;
    s16 maxX; /* maximum */
    s16 maxY;
    s16 maxZ;
    s16 charX; /* default character position */
    s16 charY;
    s16 charZ;
    s16 activeFloor; /* active floor */
    s16 activeTriangle; /* active triangle */
    u16 triangleCount; /* triangle count */
    u16 triangleOffset; /* offset: BgiTriangle[] */
    u16 edgeCount; /* edge count */
    u16 edgeOffset; /* offset: BgiEdgeView[] */
    u16 animCount; /* keyframe animation count (g_fieldBgi.unk20) */
    u16 animOffset; /* offset: g_fieldBgi.unk20 */
    u16 floorCount; /* floor count */
    u16 floorOffset; /* offset: BgiFloor[] */
    u16 normalCount; /* normal count */
    u16 normalOffset; /* offset: g_fieldBgi.unk14 */
    u16 vertexCount; /* vertex count */
    u16 vertexOffset; /* offset: g_fieldBgi.unkC */
} BgiHeader; /* walkmesh ("BGI") header */

typedef struct {
    u16 flags; /* flags */
    s16 clone; /* clone */
} BgiEdgeView; /* edge, 4 bytes */

typedef struct {
    u16 flags; /* flags */
    u16 floorIndex; /* floor index */
    s16 orgPos[3]; /* base position */
    u16 curPos[3]; /* current position */
    s16 min[3]; /* minimum */
    s16 max[3]; /* maximum */
    u16 triangleCount; /* count of unk1E entries */
    u16 triangleOffset; /* offset: s32[] (BgiTriangle indices) */
} BgiFloor; /* 0x20 */

typedef struct {
    s32 nx;
    s32 ny;
    s32 nz;
    s32 invNy;
} BgiNormal; /* 0x10 */

typedef struct {
    u16 flags; /* flags */
    u16 frameCount; /* count */
    s16 frameRate;
    s16 counter;
    s32 curFrame;
    s32 frameOffset; /* offset into g_fieldBgi.unk8 */
} BgiAnim; /* 0x10 */

/* keyframe of a walkmesh animation (BgiAnim.unkC) */
typedef struct {
    u16 flags; /* flags */
    s16 value; /* value */
    u16 triangleCount; /* triangle count */
    u16 triangleOffset; /* offset: s32 triangle indices */
} BgiKeyframe;

typedef struct {
    u16 flags; /* flags */
    u16 data; /* data */
    s16 floor; /* BgiFloor index */
    s16 normal;
    u16 thetaX;
    s16 thetaZ;
    s16 vertices[3]; /* BgiVertex (vertex) indices */
    s16 edges[3]; /* edge indices */
    s16 neighbors[3]; /* neighbour node ids, -1 = none */
    s16 centerX;
    s16 centerY;
    s16 centerZ;
    s32 d; /* plane distance (16.16) */
} BgiTriangle; /* 0x28 */

/* walkmesh edge */
typedef struct {
    s16 flags;
    s16 clone; /* index in the neighbour's edge list, -1 = none */
} BgiEdge;

typedef struct {
    u8 loaded;
    u8 maskFilter;
    u16 actorCount; /* count of unk4 entries */
    struct BgiActor *actors;
    BgiHeader *header;
    struct BgiVertex *vertices;
    BgiTriangle *triangles;
    BgiNormal *normals;
    BgiEdge *edges;
    BgiFloor *floors;
    BgiAnim *anims;
} BgiContext;

typedef struct {
    u16 flags; /* flags */
    u16 floor; /* walkmesh floor */
    u16 axis; /* axis */
    u16 curve; /* curve: 0 linear, 1 cosine */
    s16 min; /* min */
    s16 max; /* max */
    s16 cur; /* current */
    s16 prev; /* previous */
    u16 range;
    s16 speed; /* speed (8.8) */
    s32 accum;
} BgiFloorMove; /* 0x18 */

/* deferred flag updates: [0] = set, [1] = clear, 4-frame queues for
 * GameData.unk0, FieldMap.unk0 and g_fieldMovieFlags */
typedef struct {
    u16 gameFlags[2][4];
    u16 mapFlags[2][4];
    u16 movieFlags[2][4];
} Unk800C9EF0;

/* text windows (same layout in ovl_0c) */
typedef struct {
    u8 cursor;
    u8 state; /* index into s_menuMainInput */
    u8 orderCursor;
    u8 orderTarget;
    u8 orderPicked;
    u8 pad5[3];
    WinDef panels[6];
} MenuMain;

/* file 0x12xxxx lookup table (fld_getModelAnimId) */
typedef struct {
    u16 modelId;
    u16 animId;          /* default motion 0x3xxxx (0xFFFF none); only entry 0 is ever matched, docs/field-models.md */
    unsigned tableOffset : 24; /* from the entry: texCount {tpage, clut}, then {du, dv}, then ModelTexOfs */
    unsigned texCount : 8;
    u16 texAnimId;          /* texture animations 0x19xxxx (0xFFFF none); only entry 0 is ever matched */
    u8 padA[2];
} WmAnimTableEntry; /* 0xC */

typedef struct {
    u8 unk0;
    u8 count; /* count */
    u8 pad2[2];
    WmAnimTableEntry entries[1];
} WmAnimTable;

/* model object (same layout in ovl_0a, ovl_0b and ovl_0c) */

/* primitive sources of a model group; index fields select vertices (unk0..),
 * uv pairs (WmModelGroup.unk20) and colours (WmModelGroup.unk24) */
typedef struct {
    u16 vertices[4];
    u16 uvs[4]; /* uv */
    u8 rgb[3]; /* rgb */
    u8 texture;    /* texture */
    u8 unk14;
    u8 flags; /* flags: 1 = semi transparent, 0xE0 = mode */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

typedef struct {
    u16 vertices[3];
    u8 texture; /* texture */
    u8 pad7;
    u8 rgb[3]; /* rgb */
    u8 padB;
    u16 uvs[3]; /* uv */
    u8 flags;    /* flags */
    u8 pad13;
} WmSrcFT3; /* 0x14 */

typedef struct {
    u16 vertices[4];
    u16 uvs[4];  /* uv */
    u16 colors[4]; /* colours */
    u8 pad18[4];
    u8 flags; /* flags */
    u8 texture; /* texture */
    u8 pad1E[2];
} WmSrcGT4; /* 0x20 */

typedef struct {
    u16 vertices[3];
    u16 uvs[3]; /* uv */
    u8 padC[3];
    u8 flags;      /* flags */
    u16 colors[3]; /* colours */
    u8 texture;     /* texture */
    u8 pad17;
} WmSrcGT3; /* 0x18 */

typedef struct {
    u16 vertices[4];
    u16 colors[4]; /* colours */
    u8 pad10[4];
    u8 flags; /* flags */
    u8 pad15[3];
} WmSrcG4; /* 0x18 */

typedef struct {
    u16 vertices[3];
    u8 pad6[2];
    u16 colors[3]; /* colours */
    u8 padE[5];
    u8 flags; /* flags */
} WmSrcG3; /* 0x14 */

typedef struct {
    u16 vertices[4];
    u8 rgb[3]; /* rgb */
    u8 padB[2];
    u8 flags; /* flags */
    u8 padE[2];
} WmSrcF4; /* 0x10 */

typedef struct {
    u16 vertices[3];
    u8 pad6[2];
    u8 rgb[3]; /* rgb */
    u8 flags; /* flags */
} WmSrcF3; /* 0xC */

typedef struct {
    u16 packetSize;
    u16 ft4Count;  /* FT4 count */
    u16 ft3Count;  /* FT3 */
    u16 gt4Count;  /* GT4 */
    u16 gt3Count;  /* GT3 */
    u16 g4Count;  /* G4 */
    u16 g3Count;  /* G3 */
    u16 f4Count;  /* F4 */
    u16 f3Count; /* F3 */
    u8 pad12;
    s8 otBias; /* ordering table bias */
    void *boneVertexCounts;
    void *vertices;
    u8 *prims;      /* primitive sources, in the order above */
    u8 (*uvs)[2]; /* uv */
    u8 (*colors)[4]; /* colours */
} WmModelGroup; /* 0x28 */

typedef struct {
    u16 flags; /* flags: 1 = relocated */
    u8 boneCount;  /* bone count */
    u8 groupCount; /* group count (<= 32: ModelObj.hiddenGroups bits) */
    u16 packetSize; /* sum of the groups' packetSize (u16, wraps for model 200; the field code sums the groups) */
    u8 lookAtBone; /* look-at bone (fldmdl_calcBoneMatricesLookAt) */
    u8 pad7[0xC - 7];
    s32 bones;
    WmModelGroup *groups;
} WmModel;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    void *prims; /* first primitive */
    s32 unk8;
} WmModelOut; /* 0xC */

typedef struct {
    u16 tpage; /* tpage */
    u16 clut; /* clut */
} WmTexInfo;

/* EvtObject position (pos) seen as an array, indexed by axis */
typedef struct {
    u8 pad0[0x14];
    s32 pos[3];
} ActorPosView;

/* skeleton bone (WmModel.unkC, 4 bytes) */
typedef struct {
    s16 length; /* length: z offset from the parent */
    u8 unk2;
    u8 parent; /* parent bone */
} WmBone;

/* animation track: three 16-bit offsets/constants (x, y in unk0, z in unk4)
 * and their flags in the high half of unk4 (see gte_ldrotkey) */
typedef struct {
    u32 unk0;
    u32 unk4;
} WmAnimKey;

/* SVECTOR copied as two words */
typedef union {
    struct {
        s16 vx;
        s16 vy;
        s16 vz;
        s16 pad;
    } v;
    s32 w[2];
} SVECTOR4;

/* animation file (ModelObj.unkC) */
typedef struct {
    u8 pad0[4];
    s16 rootPos[3]; /* root position: constant, or offset of s16[frame] */
    u16 constMask;    /* 1, 2, 4: rootPos[i] is a constant */
    WmAnimKey *rotKeys;  /* rotation tracks, one per bone */
    WmAnimKey *rotKeysLo; /* low rotation bits, packed two frames per byte */
} WmAnim;

typedef struct ModelObj {
    u16 flags; /* flags; 0x10: behind the camera */
    u8 pad2[2];
    u8 parentBone; /* bone in the parent */
    u8 pad5[3];
    WmModel *model;
    WmModel *anim; /* file 0x3xxxx */
    u8 *packets; /* primitive buffer */
    s32 hiddenGroups;
    WmTexInfo *texInfo;
    struct ModelObj *parent; /* parent */
    u8 *boneMatrices;
    MATRIX worldMatrix; /* t: position */
    WmModelOut *groupOut;
    s16 lookAtTarget[3]; /* look-at target (fldmdl_calcBoneMatricesLookAt) */
    u8 pad4E[2];
    struct Unk800AB0B0 *texAnims; /* file 0x19xxxx */
    u16 scaleX; /* scale */
    u16 scaleY;
    u16 scaleZ;
    s16 clipY; /* screen y limit (fldmdl_projectMeshClipY) */
    s32 unk5C;
    struct ModelObj *mirrorSrc; /* mirrored model (fldmdl_reflectModel) */
    SVECTOR4 mirrorNormal; /* mirror plane normal */
    SVECTOR4 mirrorPoint; /* point on the mirror plane */
    s16 attachX; /* offset from the parent bone */
    s16 attachY;
    s16 attachZ;
} ModelObj;
/* ModelObj.flags */
#define MDL_LOOK_AT 0x1 /* fld_drawModels uses fldmdl_calcBoneMatricesLookAt */
#define MDL_SCALED 0x2 /* fldmdl_applyScale and the look-at variant apply the scale */
#define MDL_BEHIND_CAMERA 0x10 /* fldmdl_cullBehindCamera: origin SZ 0 or parent behind; colour/abr helpers skip it */
/* WmSrc*.flags */
#define WMSRC_SEMITRANS 0x1 /* fldmdl_buildPackets, fldmdl_setSemiTrans */
#define WMSRC_DOUBLE_SIDED 0x2 /* fldmdl_drawMeshC skips the nclip test */

typedef union {
    u32 w;
    u8 c[4];
} FieldColor;

/* per-actor extension (FieldActorInfo.unk10) */
typedef struct FieldActorExt {
    u32 flags; /* flags */
    s32 modelId; /* model file id */
    ModelObj *model; /* = unk14 */
    FieldEvtObj *obj; /* = unkAC */
    s32 unk10;
    u8 modelBuf[0xAC - 0x14]; /* mirrored model */
    u8 objBuf[0xF8 - 0xAC]; /* copy of the object */
    FieldEvtCtx *src;
    SVECTOR4 mirrorPoint; /* mirror plane point */
    SVECTOR4 mirrorNormal; /* mirror plane normal */
    FieldColor tint; /* rgb, [3]: frames left */
} FieldActorExt; /* 0x110 */

/* 0x14-byte per-actor record at FieldScene.unk54, indexed by uid */
/* rgb + flag byte (bit 24 of the word: pending) */

typedef struct FieldActorInfo {
    u32 flags; /* flags */
    s8 litFloor; /* walkmesh floor the light was computed for, -1 = none */
    u8 abr;
    u8 unk6;
    u8 unk7;
    FieldColor color; /* colour */
    FieldColor nextColor; /* next colour */
    FieldActorExt *mirror;
} FieldActorInfo;

/* background VRAM animation (fldmdl_updateTexAnims) */
typedef struct {
    RECT dest;      /* destination area */
    DVECTOR *frames;  /* per-frame source position */
} TexAnimRect; /* 0xC */

typedef struct {
    u8 flags; /* flags: 1 active, 2 one-shot, 4 scroll */
    u8 frameCount; /* frame count */
    s16 speed; /* speed (20.12) */
    u16 delayBase; /* random delay base */
    u16 delayRange; /* random delay range */
    s32 pos; /* position (20.12) */
    u8 rectCount; /* rect count */
    u8 padD;
    u16 lastFrame; /* last frame */
    TexAnimRect *rects;
} TexAnim; /* 0x14 */
/* TexAnim.flags (fldmdl_updateTexAnims) */
#define TEXANIM_ACTIVE 0x1
#define TEXANIM_ONESHOT 0x2 /* cleared with TEXANIM_ACTIVE at the end */
#define TEXANIM_SCROLL 0x4 /* vertical scroll mode, frameCount 0 */

typedef struct {
    u16 count; /* count */
    u8 pad2[2];
    TexAnim anims[1];
} TexAnimSet;

/* screen fade quads (g_fieldBgFollowOverlays, fldbg_addFollowOverlay) */
typedef struct {
    s8 abr; /* flags */
    u8 r; /* rgb */
    u8 g;
    u8 b;
    s16 overlay;
    s16 offsetX;
    s16 offsetY;
    u8 padA[2];
    POLY_F4 quads[2][4]; /* per draw buffer */
    DR_TPAGE tpages[2];
} BgFollowOverlay; /* 0xDC */

extern BgFollowOverlay g_fieldBgFollowOverlays[];
extern u16 g_fieldBgFollowOverlayCount; /* BgFollowOverlay count */

/* palette fade packet (fldbg_startClutFade), in the g_fieldBgCtx.unk2C buffer */
typedef struct {
    u8 active;
    u8 count;  /* colour count */
    u8 first;  /* first */
    u8 last;  /* last */
    s16 unk4; /* mode */
    u8 pad6[2];
    u32 frames; /* frames */
    s32 frame;
    s32 stepR; /* r step (16.16) */
    s32 stepG;
    s32 stepB;
    s32 savedOffset; /* offset: saved colours */
    s32 workOffset; /* offset: work colours */
    RECT rect;
} FieldPalAnim;

/* pause screen (fld_togglePause) */
typedef struct {
    SPRT sprite; /* ui_buildSprite */
    DR_TPAGE tpage;
    u8 pad1C[0x40 - 0x1C];
} FieldPausePkt;
extern DR_TPAGE g_fieldPauseTPage;
extern u32 g_fieldTempOt;
extern FieldPausePkt g_fieldPauseSprite;
extern TILE g_fieldPauseDim;

/* 3D sprite data (FieldSprite.unk18) */
typedef struct {
    u16 frameCount; /* frame count | 0x8000 */
    u16 tpage; /* tpage */
    u16 clut; /* clut */
    u8 height;  /* width */
    u8 width;  /* height */
    u16 frameOffsets[1]; /* frame offsets, then the u16-counted table */
} FieldSprHdr;

/* parameter block for fld_emitSprite3D */
typedef struct {
    u16 *uvs;
    u8 *colors;
    s32 halfW;
    s32 halfH;
    s32 tpage;
    s32 clut;
    s32 fade;
    s32 quadCount;
    s32 primCode;
} FieldSprArgs;

u8 *fld_drawSprite3D(u8 *, FieldSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, u32 *);
u8 *fld_emitSprite3D(u8 *, FieldSprArgs *, u8 *, u32 *);
void math_rotMatrixYXZ(SVECTOR *, MATRIX *);

/* object placement (fld_calcShadowMatrix) */
typedef struct {
    unsigned shade : 8;
    unsigned scaleX : 12; /* scale x */
    unsigned scaleZ : 12; /* scale z */
    unsigned angle : 12; /* angle */
    signed offsetX : 10; /* offset x */
    signed offsetZ : 10; /* offset z */
} FieldObjEnt;

/* object placement entries (FieldScene.unk2C) */
typedef struct FieldObjPlace {
    FieldObjEnt params;
    void *prims[2][4]; /* shadow primitives, per draw buffer */
} FieldObjPlace; /* 0x28 */

/* Unk800B3294 seen bytewise: the top byte of the colour is a depth bias (fld_drawShadow) */
typedef struct {
    u8 pad0[7];
    s8 shadowOtBias; /* added to the shadow quads' OT index (fld_drawShadow) */
} Unk800B3294Bias;

/* texture set tables (fld_loadModelBundle) */
typedef struct {
    s16 unk0;
    s16 unk2;
} FieldTexPair; /* tpage/clut or u/v */

/* ModelTexOfs as fld_loadModelBundle fills it in (signed view, the loads must stay lh): on input srcX is a key,
 * odd = the texture gets its own 128x128 page (dest = the page), even = a 32-row strip (src = the strip) that
 * shares the page of the entry keyed srcX - 1 */
typedef struct {
    s16 srcX;
    s16 srcY;
    s16 destX;
    s16 destY;
} FieldTexEnt;

/* VRAM allocation cursors (file 0x1C0000, fld_loadModelBundle) */
typedef struct {
    s16 pageX; /* next 128x128 page (odd-keyed textures): y 0x180, 0x100, then x += 0x40 */
    s16 pageY;
    u8 pad4[4];
    s16 clutX; /* their palettes, y += 2 */
    s16 clutY;
    u8 padC[4];
    s16 stripX; /* next 32-row strip (even-keyed textures): y += 0x20 up to 0x1E0, then x -= 0x40 */
    s16 stripY;
    u8 pad14[4];
    s16 stripClutX; /* their palettes, y += 2 */
    s16 stripClutY;
    u8 pad1C[4];
    s16 skipX; /* page column the page cursor skips */
    u8 pad22[0x30 - 0x22];
    u16 shadowTexX; /* shadow texture position (fld_getShadowTexPos) */
    u16 shadowTexY;
} FieldVramPos;

s32 fld_allocModelCache(s32, s32);

FieldLight *fld_getLightColorTable(Unk800B3254 *);
s32 bgi_getActorTriangle(s32, u16 *, u16 *);
s32 bgi_linkFrameNeighbors(BgiHeader *, BgiAnim *, BgiKeyframe *);
FieldLight *fld_findGlobalLights(Unk800B3254 *, FieldLight **, FieldLight **);
FieldLight *fld_findFloorLight(Unk800B3254 *, FieldEvtCtx *, s16 *);

/* object model slot (game_getModel) */
typedef struct {
    s32 flags; /* flags: 1 = loaded */
    s32 modelId; /* file id */
    ModelObj *model;
    void *actor; /* actor */
} FieldObjSlot;

/* field directory entry (fld_refindSectors): the next entry's sector ends this one */
typedef struct {
    u16 id;
    u16 areaId; /* field directory: the field's area (fld_refindSectors) */
    s32 sector;
} FieldDirEnt;

/* background sprite sets (fldmdl_initTexAnims); offsets from the header start */
typedef struct {
    u16 destX;
    u16 destY;
    u8 pad4[8 - 4];
    u32 frames; /* offset: u16 (x, y) pairs */
} TexAnimRelocRect; /* 0xC */

typedef struct {
    u8 unk0;
    u8 frameCount; /* frame count (= TexAnim.frameCount) */
    u8 pad2[0xC - 2];
    u8 rectCount; /* part count */
    u8 texOfsIndex; /* ModelTexOfs index */
    u16 lastFrame;
    u32 rects; /* offset: TexAnimRelocRect[] */
} TexAnimReloc; /* 0x14 */

typedef struct {
    u16 count; /* set count */
    u8 pad2[2];
    TexAnimReloc anims[1];
} TexAnimRelocSet;

typedef struct {
    u16 srcX;
    u16 srcY;
    u16 destX;
    u16 destY;
} ModelTexOfs;

/* sprite vertex (TexAnimRelocRect.unk8) */
typedef struct {
    u16 x;
    u16 y;
} TexAnimRelocPos;

/* ---- overlay functions ---- */
void fld_dropSoundBankCache(s32);
s32 fld_freeFieldFile(FieldScene *, s16);
s32 fld_freeVibration(FieldScene *, s16);
void fld_stopSceneSfx(FieldScene *, s16);
s32 fld_movieCommand(s32, s32, s32);
s32 fld_freeCommon(void);
s32 bgi_stepFloorMove();
s32 vib_init(void *);
s32 fld_soundCommand(s32, s32, s32, s32, s32);
void fld_getSoundPanVolume(s16, s16, s16, u8 *, u8 *);
s32 fld_preloadRequest(s32, s32);
s32 fld_preloadHook(void);
void fld_clearPreloadLists(void);
s32 game_getModel(s32);
s32 fld_setSlotColor(FieldEvtCtx *, s32, s32, s32);
void fld_discChangeFinish(void);
void fld_invalidateModelCache(s32);
s32 fldbg_scrollToPlayer(s32, s32);
void fld_init(void);
void fld_quit(void);
void fld_runArea(s32);
void fld_main(s32);
void fld_loadArea(s32);
void fld_unloadArea(void);
void fld_discChangeEnter(void);
void fld_discChangeLeave(void);
void fld_discChangeBeginFrame(void);
void fld_discChangeUpdate(void);
void fld_discChangeEndFrame(void);
void fld_discPromptHelp(void);
void fld_stopResidentSfx(void);
s32 fld_freeAreaFile(FieldMapObj *, s16);
void fld_discChangeSetMode(s32);
s32 *fld_findPreloadRequest(s32);
s32 *fld_findPreloadExclude(s32);
void menuMain_input(void);
void menuMain_updatePanels(void);
void menuMain_draw(void);
void fld_discPromptInput(void);
void fld_discPromptDraw(void);
void fld_discChangeBackToPrompt(void);
void menuMain_drawTimeGil(void);
void menuMain_drawLocation(void);
void menuMain_drawCursor(void);
void menuMain_drawParty(void);
/* party/menu windows shared with ovl_0c (ovl_0c has its own copy of menu_main.c) */
void menuMain_initPanels(void);
void menuMain_swapPortraits(s32, s32);
void menuMain_updateHelp(void);
void menuMain_setExpHelp(s32);
void menuMain_selectCommand(void);
void menuMain_setEquipHelp(u8);

/* ---- overlay data ---- */
extern u8 g_fieldDiscChangeText[]; /* text */
extern s32 g_fieldPreloadCursor;
extern FieldDiscMode g_fieldDiscChangeModes[];
extern FieldBgCtx g_fieldBgCtx;
extern BgFile *g_fieldBgData;
extern u8 g_fieldBgClutRows; /* palette rows used from 0xE0 up (fldbg_buildOverlaySprites), never read */
extern s32 g_fieldBgCtxWork;
extern u16 g_fieldBgiMoveTriCount;
extern u16 g_fieldBgiMoveEdgeCount;
extern BgiContext g_fieldBgi;
extern struct BgEntry *g_fieldBgCtxEntries; /* = g_fieldBgCtx.entries */
extern s32 g_fieldModelQueue[16];
extern s8 g_fieldSoundBankSlots[5];
extern s32 g_fieldSoundBankIds[5];
extern s32 D_800CA0E0;
extern s32 g_fieldPreloadRequests[8];
extern s32 g_fieldPreloadExcludes[8];
extern u8 g_fieldPreloadRequestCount; /* g_fieldPreloadRequests count */
extern u8 g_fieldPreloadExcludeCount; /* g_fieldPreloadExcludes count */
extern FieldDiscChange g_fieldDiscChange;
extern FieldBgState g_fieldBgState;
extern s16 g_fieldBgiMoveTris[];
extern s16 g_fieldBgiMoveEdges[];
extern BgiFloorMove g_fieldBgiFloorMoves[];
extern MenuMain s_menuMain;
extern void (*s_menuMainInput[])(void);
extern Unk800C9EF0 g_fieldFlagQueue;
extern u16 g_fieldMovieFlags;
extern s16 g_fieldBgiPushCount;
extern char s_bgiStrInvNy[]; /* "          1/ny = %lx\n" (walkmesh dump) */
extern s16 g_fieldBgiPushVecs[][3]; /* collision push vectors */
extern s16 g_fieldBgiPushOnEdge[];
extern s32 g_fieldBgiPushDist[];
/* mask-bit primitives, one per draw buffer (fld_beginFrame) */
typedef struct {
    u32 tag;
    u32 code[5];
} Unk800C9F60; /* 0x18 */
extern Unk800C9F60 g_fieldMaskPkt[2];
extern BgiActor g_fieldBgiActors[24];
BgiTriangle *bgi_getPassableNeighbor(BgiActor *, BgiTriangle *, s32);
s32 bgi_pointInTriangle(BgiHeader *, BgiFloor *, BgiTriangle *, s16 *);
s32 bgi_pointInTriangleXZ(BgiHeader *, BgiFloor *, BgiTriangle *, SVECTOR *);
PreloadEntry *preload_waitCurrent(void);
s32 preload_overlaps(s32, s32);
s32 preload_discardRange(u32, s32);
u8 *rain_init(void *);

extern RECT *s_menuMainTimeGilRect;
extern u8 s_menuMainGilSuffix[];
extern RECT *s_menuMainLocationRect;
extern u8 *s_menuMainEquipLabels[5];
extern u8 s_menuMainText[]; /* text */
extern RECT g_fieldDiscChangeRect;

extern RECT s_menuMainPanelRects[6];
extern TweenDef s_menuMainSlideIn[4];
extern u8 s_menuMainExpText[16];
extern u8 s_menuMainNextText[16];
extern u8 s_menuMainExpLabel[];
extern u8 s_menuMainNextLabel[];
extern u8 s_menuMainUntilLabel[];
extern u8 s_menuMainStatusLabel[];
extern u8 s_menuMainStatusNormal[];
extern u8 s_menuMainStatusAbnormal[];

/* 8-byte entries following Unk800B3254.unkC[unk6] */


/* 10 VRAM cache slots at 0x800C9CCC. splat puts the first word at the end of
 * .text (as "g_fieldModelCache"), so the array is reached through that label. */
typedef struct {
    s16 unk0;
    s16 id; /* id */
    u32 addr; /* address (0 = free) */
    s32 size; /* size */
} Unk800C9CCC; /* 0xC */
extern Unk800C9CCC g_fieldModelCache[10];
Unk800C9CCC *fld_findModelCache(s32);
u32 fld_getModelCacheBottom(Unk800C9CCC **);
void fld_allocArea(void);
void fld_initArea(void);
s32 bgi_placeActor(s32, s16, s16, s16);
s32 bgi_moveActor(BgiActor *);
s32 bgi_slide(BgiActor *, BgiTriangle **, s16 *, SVECTOR *, s16, s16, s32, s32);
s32 bgi_applyPushes(BgiActor *, SVECTOR *, BgiTriangle **, s16 *);
s32 fld_fixMul(s32, s32); /* 16.16 multiply */
s32 fldbg_drawOverlay(BgOverlay *, u8 *); /* adds a background layer's tiles to the OT */
s32 fldbg_followPlayer(void); /* camera follow */
extern BgiTriangle *g_fieldBgiTriangles; /* = g_fieldBgi.unk10, referenced by its own name */
s32 bgi_initAnim(u32);
s32 bgi_resetFloorMoves(void);
s32 fld_readAreaFile(FieldMapObj *, s32);
void fldmdl_drawMesh(ModelObj *, u8);
void fldmdl_projectMeshClipY(ModelObj *, u8);
void fldmdl_drawMeshClipY(ModelObj *, u8);

/* run on a 12-byte frame at the top of the scratchpad (0x1F8003FC) and switch back */
#define SetSpadStack12(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -12\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack12() __asm__ volatile("addiu $29, $29, 12\n\tlw $29, 0($29)" : : : "memory")

/* main exe: candidates for ff9.h */
void math_rotMatrixZYX(SVECTOR *, MATRIX *);
void math_rotMatrixZXY(SVECTOR *, MATRIX *);
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */
s32 func_80056048(s32 *, s32, s32);
extern u32 g_portraitSavedVram[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *g_menuHelpText; /* = g_menuHelp.text, referenced by its own name */
extern s32 D_80077DEC; /* = g_menuHelp.otz, referenced by its own name */
s32 text_measureStyle(u8 *, s32);
extern s32 g_menuScreenFrame; /* overlaps MenuScreen.unk4 in ff9.h */
void menuMain_reorder(s32, s32);
void fld_discPromptSelect(void);
s32 fld_setupActorModel(s32, void *);
void menuMain_drawMember(s32);
void menuMain_drawPortraitShade(RECT *, s32);
extern s32 g_mbgFrame;
void gameover_main(s32); /* overlay 0x65 (loaded at 0x801F0000) */
s32 mbg_setRgb24(s32); /* overlay loaded at 0x80198000 */
s32 mbg_getFrame(void);
void menu_update(void);
EvtObject *evt_getPlayer(void);
u32 evt_getObjectChrIndex(void *); /* actor -> model slot */
void game_updateWindows(void);
void preload_reset(void);
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */

/* field state for fld_movieCommand command 3: 0x11, 1 or 0x10/0 */
static inline s32 fld_movieCommand_state(void) {
    s8 v;

    if (g_gameState->disc != 0xFF) {
        v = g_gameState->gameData->ctx.field->area->scene->movieState;
        if (v != 0x11) {
            goto other;
        }
    }
    return 0x11;
other:
    if (v == 8) {
        return 1;
    }
    return (v < 5) << 4;
}

/* Inlined copy of bgi_getFloorMove. */
static inline BgiFloorMove *bgi_getFloorMove_inline(s32 i) {
    return &g_fieldBgiFloorMoves[i];
}

/* tables inside the g_fieldBgData block (stored as offsets from its start) */
static inline BgOverlayAnim *fldbg_overlayAnimPtr(s32 i) {
    return &((BgOverlayAnim *)((u8 *)g_fieldBgData + g_fieldBgData->overlayAnimOffset))[i];
}

static inline BgOverlay *fldbg_overlayPtr(s32 i) {
    return &((BgOverlay *)((u8 *)g_fieldBgData + g_fieldBgData->overlayOffset))[i];
}

static inline BgCamera *fldbg_cameraPtr(s32 i) {
    return &((BgCamera *)((u8 *)g_fieldBgData + g_fieldBgData->cameraOffset))[i];
}

static inline BgAnimStep *fldbg_animSteps(BgOverlayAnim *p) {
    return (BgAnimStep *)((u8 *)g_fieldBgData + p->stepOffset);
}

/* tables inside the g_fieldBgi.unk8 block (offsets from its start) */
static inline BgiTriangle *bgi_trianglePtr(s32 i) {
    return &((BgiTriangle *)((u8 *)g_fieldBgi.header + g_fieldBgi.header->triangleOffset))[i];
}

static inline BgiEdgeView *bgi_edgePtr(s32 i) {
    return &((BgiEdgeView *)((u8 *)g_fieldBgi.header + g_fieldBgi.header->edgeOffset))[i];
}

static inline BgiKeyframe *bgi_keyframes(BgiAnim *e) {
    return (BgiKeyframe *)((u8 *)g_fieldBgi.header + e->frameOffset);
}

static inline BgiFloor *bgi_floorPtr(s32 i) {
    return &((BgiFloor *)((u8 *)g_fieldBgi.header + g_fieldBgi.header->floorOffset))[i];
}

s32 fldbg_drawFollowMask(BgFile *bg, BgOverlay *o, FieldBgState *st, BgFollowOverlay *f);
s32 fldbg_applyGeomOffset(void);


/* field object drawing (fld_drawModels) */
s32 bgi_getActorTilt(s32, s16 *, u16 *);
void fldmdl_applyScale(ModelObj *);
void fldmdl_calcBoneMatrices(ModelObj *, u16);
s32 fld_isModelTexAnimActive(ModelObj *);
void fldmdl_updateTexAnims(TexAnimSet *);
void fld_applyLights(FieldEvtCtx **, s32);
void fld_playAnimSfx(FieldEvtCtx **, s32);
void fld_applyModelCommands(FieldEvtCtx **, s32);
void fldmdl_calcBoneMatricesLookAt(ModelObj *, s32);
void fld_drawShadow(struct FieldObjPlace *, s32);
s32 mbg_getGeomOffset(s32 *, s32 *); /* ovl_64 */

#define gte_stir(r0) __asm__ volatile( \
    "swc2\t$9, 0(%0);" \
    "swc2\t$10, 4(%0);" \
    "swc2\t$11, 8(%0)" \
    : : "r"(r0) : "memory")

/* fld_drawShadow: project one corner of the 3x3 shadow grid */
#define FIELD_SHADOW_PT(x, z) \
    v.vx = (x); \
    v.vy = 0; \
    v.vz = (z); \
    gte_ldv0(&v); \
    gte_rtps()

/* fld_drawShadow: link shadow quad q at its deepest corner */
#define FIELD_SHADOW_ADD(k, q) \
    k = (k >> 2) + bias; \
    if (-g_gameState->gameData->ctx.field->area->scene->otBias < k \
        && k < 0x1000 - g_gameState->gameData->ctx.field->area->scene->otBias) { \
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k], e->prims[g_gameState->gameData->drawBuf][q]); \
    }

void fld_calcShadowMatrix(FieldObjEnt *, FieldObjSlot *, MATRIX *);


/* field movie playback (fld_movieUpdate) */
typedef struct {
    u32 tag;
    u32 code[5];
} Unk800C9F6C; /* 0x18: mask-bit draw mode packet, per draw buffer */
/* movie table (ovl_64's g_mbgMovieLists, as ovl_15 declares it) */
typedef struct {
    char *path; /* movie file name */
    s32 pad4[4];
} MbgMovie; /* 0x14 */
typedef struct {
    s16 count;
    s16 pad2;
    MbgMovie *movies;
} MbgMovieList;
extern MbgMovieList g_mbgMovieLists[]; /* overlay loaded at 0x80198000 */
extern u16 g_fieldMovieLastFrame;
extern Unk800C9F6C g_fieldMovieMaskPkt[2];
void fld_movieStop(void);
void fld_movieFinish(void);
s32 MBG_init(s32, void *); /* overlay loaded at 0x80198000 */
void mbg_setEndCallback(void (*)());
void mbg_setVolume(s32, s32, s32);
s32 mbg_play(u32, s32);
s32 mbg_getStartCamera(void *, u16 *);
s32 mbg_update(s32);
s32 mbg_getLastFrame(void);
/* main exe: candidates for ff9.h */
s32 cd_findFile(char *);
void file_getTimRects(void *, RECT **, RECT **); /* TIM: clut rect, image rect */
u8 cd_seekStreamAsync(s32);
void akao_sendCmdE4(s32);

/* field setup (fld_updateEncounter) */
void fld_clearModelCache(void);
void fld_findBattleMusic(void);
void fld_suspendResidentSfx(void);
/* main exe: candidates for ff9.h */
extern s32 g_swirlFrame;
extern u8 *g_swirlBuf;
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_setMusicVolume(s32, s32);
void preload_setVramRect(PreloadEntry *, s16, s16, s16, s16);
u8 swirl_update(s32, s32);

/* bgi_slide (walkmesh slide along the triangle edges) */
extern VECTOR g_fieldBgiDefaultNormal; /* normal used when a triangle has none */
extern const u8 g_fieldDiscChangeLabel[]; /* "DISC CHANGE" label */
s32 bgi_findEdgeCrossing(BgiHeader *, BgiFloor *, BgiTriangle *, s16 *, s16 *, SVECTOR *, s16 *, s32);

/* clears the visited flag (0x80) of every walkmesh triangle */
static inline void bgi_slide_clear(void) {
    u16 k;
    BgiTriangle *n;
    s32 cnt;
    u16 m = ~BGI_TRI_VISITED;

    n = g_fieldBgi.triangles;
    cnt = g_fieldBgi.header->triangleCount;
    for (k = 0; k < cnt; k++) {
        n->flags &= m;
        n++;
    }
}

/* scales the x/z offset of pos from org (16.16, fractions in *fx/*fz) by 1.05, 1.10, ...
 * until its integer part changes */
static inline void bgi_slide_push(SVECTOR *pos, s16 *org, s32 *fx, s32 *fz) {
    s32 x, z, k, i, tx, tz;

    x = ((pos->vx - org[0]) << 16) | (u16)*fx;
    z = ((pos->vz - org[2]) << 16) | (u16)*fz;
    k = 0x10000;
    i = 0;
    do {
        k += 0xCCC;
        tx = (x >> 8) * (k >> 8);
        tz = (z >> 8) * (k >> 8);
        i++;
        if ((tx >> 16) != (x >> 16) || (tz >> 16) != (z >> 16)) {
            break;
        }
    } while (i < 50);
    pos->vx = org[0] + (tx >> 16);
    pos->vz = org[2] + (tz >> 16);
    *fx = tx & 0xFFFF;
    *fz = tz & 0xFFFF;
}

/* music/sound request handler (fld_soundCommand) */
extern s8 g_fieldMusicRequest;
extern s32 g_fieldSoundLoadId;
extern u8 g_fieldSoundLoadKind;
extern s32 g_fieldSoundLoadSector;
s32 fld_soundBankReadCallback(CdReadCtx *arg0);
/* main exe: candidates for ff9.h */
s32 akao_playMusic(void *);
void akao_stopMusic(s32);
void akao_saveMusic(void);
s32 akao_playMusicCount(void *, s32);
s32 akao_playSe(s32, s32, s32, s32);
void akao_sendCmd92(s32);
void akao_setAllSeVolume(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_setSeVolume(s32, s32, s32);
void akao_sendCmdAA(s32);
void akao_sendCmdAB(s32, s32);
void akao_sendCmdA2(s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_sendCmdAC(s32);
void akao_sendCmdAD(s32, s32);
void akao_sendCmdA4(s32, s32, s32);
void akao_sendCmdA5(s32, s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);
void akao_sendCmdD0(s32);
void akao_sendCmdD1(s32, s32);
void akao_sendCmdD2(s32, s32, s32);
void akao_sendCmdD4(s32);
void akao_sendCmdD5(s32, s32);
void akao_sendCmdD6(s32, s32, s32);
void akao_sendCmdD8(s32);
void akao_sendCmdD9(s32, s32);
void akao_sendCmdDA(s32, s32, s32);
s32 akao_getSpuTransferState(void);
void akao_sendCmdE0(s32 *, s32, s32);
void akao_sendCmdE6(s32);

/* GameData.unk828/unk82C as an array (the slot index needs `base + slot*4`) until ff9.h has it */
typedef struct {
    u8 pad[0x828];
    s32 unk828[2];
} GameDataSfx;
#define SFXH(i) (((GameDataSfx *)g_gameState->gameData)->unk828[i])
#define SCENE (g_gameState->gameData->ctx.field->area->scene)

s32 fld_initShadows(s32, s32, s32, s32); /* returns func_80062E3C's result (the next free address) */

FieldAnimSfx *fld_addAnimSfx(FieldEvtCtx *, s32, s32); /* returns the new entry */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void akao_sendCmd98(u32 x); /* main */
void akao_sendCmd99(u32 x); /* main */
s32 bgi_addActor(FieldObjSlot *arg0);
s32 bgi_bindActor(s32 arg0, FieldObjSlot *arg1);
s32 bgi_clear(void);
s32 bgi_getTriangleFlags(s16 arg0, u16 *arg1);
s32 bgi_init(BgiHeader *arg0);
s32 bgi_remapActors(u8 *arg0, BgiAnim *arg1, BgiKeyframe *arg2, s16 arg3, s32 unused);
s32 bgi_segmentsCross(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 x, u8 z);
s32 bgi_setActorActive(s32 arg0, s32 arg1);
s32 bgi_setAnimRate(u32 arg0, s16 arg1);
s32 bgi_updateAnims(void);
s32 bgi_updateFloorMoves(void);
void evt_createModels(void); /* main */
s32 evt_getModelSlot(s32 id); /* main */
u8 *evt_loadScripts(u8 *p); /* main */
Unk4 fade_getColor(void); /* main */
s32 fade_getType(void); /* main */
void fade_getWipeColors(void *a); /* main */
s32 *fld_addPreloadExclude(s32 arg0);
s32 *fld_addPreloadRequest(s32 arg0);
void fld_allocMap(void);
void fld_allocScene(void);
void fld_beginFrame(void);
void fld_clearModelQueue(void);
void fld_clearSoundBankCache(void);
void fld_compactModelCache(void);
s32 fld_depFindArea(s32 arg0, s32 *arg1, s32 *arg2);
s32 fld_depFindBackground(s32 arg0, s32 *arg1, s32 *arg2);
s32 fld_depFindBattle(); /* (u16, s32 *, s32 *); unprototyped: callers pass the argument unconverted */
s32 fld_depFindByLink(s32 arg0);
s32 fld_depFindField(); /* (u16, s32 *, s32 *); unprototyped: callers pass the argument unconverted */
s32 fld_depGetFileSector(s32 arg0, s32 *arg1);
s32 fld_depGetLink(s32 arg0);
void fld_drawModels(void);
void fld_drawSprites(void);
void fld_endFrame(void);
void fld_exit(void);
FieldAnimSfx *fld_findAnimSfx(); /* (FieldEvtCtx *, u16, s16); unprototyped: callers pass the argument unconverted */
s32 fld_findAreaItem(); /* (FieldMapObj *, u16); unprototyped: callers pass the argument unconverted */
s32 fld_findEventScript(); /* (FieldScene *, u16); unprototyped: callers pass the argument unconverted */
Unk800B3294 *fld_findLightColor(); /* (Unk800B3254 *, u16); unprototyped: callers pass the argument unconverted */
s32 fld_findLights(FieldScene *arg0, s32 unused);
s32 fld_freeModelBundles(void);
s32 fld_getBgKey(); /* (FieldScene *); unprototyped: some callers pass a second argument it ignores */
s32 fld_getModelAnimId(s32 arg0);
s32 fld_getModelTables(s32 arg0, s32 **arg1, s32 **arg2, s32 **arg3);
s32 fld_getShadowTexPos(u16 *arg0, u16 *arg1);
s32 fld_getSpriteFrameCount(u16 *arg0);
void fld_initMap(void);
void fld_initPreload(void);
void fld_initScene(void);
u8 *fld_initSprites(u8 *arg0);
s32 fld_isDiscChangePending(void);
s32 fld_isMoviePlaying(void);
s32 fld_isMusicFading(void);
void fld_load(s32 arg0);
s32 fld_loadCommon(void);
s32 fld_loadFile(FieldScene *sc, s32 arg1);
void fld_loadImagePack(s32 *arg0);
s32 fld_loadModelBundle(s32 arg0);
s32 fld_loadModelBundlesInCdOrder(void);
s32 fld_loadSoundBanks(FieldScene *unused, s32 unused2);
s32 fld_loadVibration(); /* (FieldScene *, u16); unprototyped: callers pass the argument unconverted */
void fld_movieStart(s32 arg0, s32 arg1, s32 arg2);
void fld_movieUpdate(void);
s32 fld_needsSplashImage(void);
s32 fld_readSoundBank(void);
void fld_refindSectors(void);
s32 fld_remapModelId(s32 arg0);
s32 fld_removeAnimSfx(); /* (FieldEvtCtx *, u16, s16); unprototyped: callers pass the argument unconverted */
void fld_resetSprite(FieldSprite *arg0);
void fld_resumeResidentSfx(void);
void fld_runDiscChange(void);
void fld_setMusicVolumeFade(s32 arg0, s32 arg1);
s32 fld_setupBackground(FieldScene *arg0, s32 arg1);
void fld_showSplashImage(void);
void fld_startAudio(void);
void fld_startEncounter(void);
void fld_togglePause(void);
void fld_trimModelCache(void);
s32 fld_unpackMusicSeq(s32 arg0);
void fld_updateEncounter(void);
s32 fldbg_draw(void);
BgSprt *fldbg_init(void);
s32 fldbg_setAnimSpeed(s32 arg0, s32 arg1);
s32 fldbg_setCamera(s32 arg0);
void fldbg_setData(BgFile *arg0);
void fldbg_setRenderContext(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
void fldbg_setWorkBuffer(s32 arg0, s32 arg1);
u32 fldmdl_addRgbClamped(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void fldmdl_buildPackets(ModelObj *arg0, WmTexInfo *arg1, s16 (*arg2)[2]);
void fldmdl_drawMeshMirrored(ModelObj *arg0, u8 arg1);
void fldmdl_initModel(ModelObj *arg0, u8 *arg1);
void fldmdl_initTexAnims(TexAnimRelocSet *hdr, ModelTexOfs *ofs);
void fldmdl_projectMesh(ModelObj *arg0, u8 arg1);
void fldmdl_reflectModel(ModelObj *arg0);
void fldmdl_setAbr(ModelObj *arg0, u16 arg1);
void fldmdl_setGroupRgb(ModelObj *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4);
void fldmdl_setSemiTrans(ModelObj *arg0, u16 arg1);
void func_8002FC30(void); /* main */
s32 evt_countNpcActors(s32 unused); /* main */
void fld_drawRain(void);
void game_resumeClock(u8 arg0); /* main */
s32 mbg_shutdown(void); /* ovl_64 */
s32 preload_update(void); /* main */
s32 tpage_getX(s32 a); /* main */
s32 tpage_getY(s16 arg0); /* main */
s32 vib_update(void); /* main */

#endif
