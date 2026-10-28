#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* w_frameSetParameter request ids (event opcode 0xC4). Names follow Square's debug strings
 * "*[ Request from script:... ]" printed for each id (spelling corrected), [code] where silent. */
#define WM_REQ_WEATHER 0 /* "Weather": wm_setWeather(arg, WM_FOG_KEEP, param 0); WM_WEATHER_AUTO turns auto weather on */
#define WM_REQ_ACTOR 1 /* "Actor": w_movementChange */
#define WM_REQ_NAVI_ACTIVE 2 /* "Navi Active" */
#define WM_REQ_NAVI_MODE 4 /* "Navi mode" */
#define WM_REQ_CAMERA_LOOKUP_FIX 5 /* "Camera Lookup Fix mode" */
#define WM_REQ_PARAM0 6 /* silent: g_worldReqParams[0..3] (6-9) */
#define WM_REQ_PARAM1 7
#define WM_REQ_PARAM2 8
#define WM_REQ_PARAM3 9
#define WM_REQ_SPAWN_EFFECT 10 /* silent: wm_spawnEffect(param 0-2, arg, param 3) */
#define WM_REQ_TITLE_DRAW_TIME 11 /* "Title Draw Time" */
#define WM_REQ_WEATHER_SPEED 14 /* "Whather Speed": param 0 */
#define WM_REQ_WEATHER_AUTO 15 /* "Whather Auto" */
#define WM_REQ_SE_PARAM0 16 /* "SE Param0".."SE Param3" (16-19): g_worldReqParams[0..3] */
#define WM_REQ_SE_PARAM1 17
#define WM_REQ_SE_PARAM2 18
#define WM_REQ_SE_PARAM3 19
#define WM_REQ_SE_PLAY 20 /* "SE Play" */
#define WM_REQ_SE_VOL 21 /* "SE Vol" */
#define WM_REQ_SE_VOL_INTPL 22 /* "SE Vol Intpl" */
#define WM_REQ_SE_STOP 23 /* "SE Stop" */
#define WM_REQ_MUSIC_VOL_INTPL 24 /* "Music Vol Intpl" */
#define WM_REQ_CAMERA_SMOOTH 25 /* "Camera Smooth" */
#define WM_REQ_EVENT_BATTLE_PROB 26 /* "EventBattleProbability" */
#define WM_REQ_NAVI_CURSOR 27 /* "Navi Cursol" */
#define WM_REQ_GET_GETOFF_POS 28 /* "Get Getoffpos": wm_findGetoffPos into params 0-2 (10000 each when none) */
#define WM_REQ_CALL_CHOCOBO 29 /* "Call Chocobo" */
#define WM_REQ_GETOFF_SP1 30 /* "Get Getoff SP1" */
#define WM_REQ_GETOFF_SP2 31 /* "Get Getoff SP2" */
#define WM_REQ_MUSIC_FADE 32 /* "Music Fade" */
#define WM_REQ_MUSIC_FADE_FORCE 33 /* "Music Fade Force" */
#define WM_REQ_AUTO_PILOT_ON 34 /* "Auto Pilot ON" */
#define WM_REQ_MOG_CHOICE 35 /* "Mog choice" */
#define WM_REQ_CURSOR_DRAW 36 /* "Cursol Draw" */
#define WM_REQ_ALPHA0 37 /* "Alpha 0" (clears the smoothed speed/climb inputs) */
#define WM_REQ_AUTO_PILOT_OFF 38 /* "Auto Pilot OFF" */
#define WM_REQ_CD_USE_ON 39 /* "CD Use ON" */
#define WM_REQ_CD_USE_OFF 40 /* "CD Use OFF" */
#define WM_REQ_MAP_JUMP 41 /* "Map Jump" */

/* w_frameGetParameter ids: event-script expression variables >= 0xC0 on the world map
 * (main evt_getSysVar's default branch) */
#define WM_PARAM_AREA 0xC0 /* area of the cell under the player ("Now Area" in wm_getEncounterEntry) */
#define WM_PARAM_TOPOGRAPHY 0xC1 /* topography of that cell ("Topograph") */
#define WM_PARAM_REQ0 0xC3 /* g_worldReqParams[0..2], e.g. the WM_REQ_GET_GETOFF_POS result */
#define WM_PARAM_REQ1 0xC4
#define WM_PARAM_REQ2 0xC5
#define WM_PARAM_CAMERA_YAW 0xC7 /* g_worldCamera->unkC >> 4 */
#define WM_PARAM_CD_BUSY 0xC8 /* "Event:CDAccessWait": g_worldCdUser | music request pending */
#define WM_PARAM_PLAYER_X 0xC9
#define WM_PARAM_PLAYER_Y 0xCA
#define WM_PARAM_PLAYER_Z 0xCB
#define WM_PARAM_WEATHER_POINT_DIST 0xCC /* g_worldWeatherPointDist */
#define WM_PARAM_EVENT_BATTLE 0xCD /* 1 in g_worldEventBattleRate + 1 while walking on topography 0x24-0x26 */
#define WM_PARAM_PLAYER_DIR 0xCE /* player rotY >> 4 */
#define WM_PARAM_ZONE 0xCF /* wm_getAreaZone(area) */
#define WM_PARAM_RUNNING 0xD1 /* g_worldState == WM_STATE_RUNNING */
#define WM_PARAM_TITLE_PHASE 0xD2 /* g_worldTitlePhase */
#define WM_PARAM_NAVI_VISIBLE 0xD3 /* g_worldNaviVisible */

/* cell attribute (WmActorType.unk8, wm_getBlockHeight): area in bits 8-13, topography in bits 2-7 (docs/encounters.md) */
#define WM_ATTR_AREA(a) (((a) & 0x3F00) >> 8)
#define WM_ATTR_TOPOGRAPHY(a) (((a) & 0xFC) >> 2)

/* g_worldMapDebugFlags bits: each gates one group of debug prints */
#define WM_DEBUG_MUSIC 0x4 /* w_music */
#define WM_DEBUG_ENCOUNTER 0x8 /* "- Encount Information -" */
#define WM_DEBUG_FORM 0x10 /* "BGI Form", "Scene:%d / Status:%d" */
#define WM_DEBUG_MOVEMENT 0x20 /* w_movementChange */
#define WM_DEBUG_MEMORY 0x40 /* w_memoryman, destructors */
#define WM_DEBUG_REQUEST 0x80 /* "Request from script", "Event:CDAccessWait" */
#define WM_DEBUG_PACK 0x100 /* w_frameExtruct, w_framePackGetPtr */
#define WM_DEBUG_TEXTURE 0x200 /* "Texture ReadDone and Extruct", "Texture Transfer Area" */

/* g_worldState */
#define WM_STATE_START 0 /* only the event script runs */
#define WM_STATE_SETUP 1 /* set by wm_frameMain on frame 4 */
#define WM_STATE_RUNNING 2 /* set by wm_chunksLoaded after the first 16 chunks: draw and pad input */

/* g_worldEventResult: event-script frame result returned by wm_frameMain */
#define WM_EVT_BATTLE 3 /* -> WM_EXIT_BATTLE */
#define WM_EVT_FIELD 4 /* "MAP JUMP" -> WM_EXIT_FIELD */
#define WM_EVT_WORLD 5 /* next world map */

/* WorldMapObj.exitReason, switched on by wm_exit */
#define WM_EXIT_FIELD 1
#define WM_EXIT_BATTLE 2
#define WM_EXIT_WORLD 3

/* g_worldCdUser: who holds the CD */
#define WM_CD_FREE 0
#define WM_CD_CHUNKS 1 /* wm_readChunks */
#define WM_CD_MUSIC 2 /* w_musicUpdate */
#define WM_CD_SCRIPT 3 /* "CD Use ON" */

/* g_worldDataSet ("Read CD Image [Disc-1]" / "[Disc-4]") */
#define WM_DATASET_DISC1 1
#define WM_DATASET_DISC4 4

/* g_worldFog / wm_setWeather fog argument */
#define WM_FOG_OFF 0
#define WM_FOG_ON 1
#define WM_FOG_KEEP 2
/* wm_setWeather mode */
#define WM_WEATHER_AUTO 4 /* WM_REQ_WEATHER: turn auto weather on */
#define WM_WEATHER_KEEP 5

/* movement mode control types (WmMoveMode.unk0, wm_controlPlayer) */
#define WM_CTRL_WALK 0
#define WM_CTRL_FLY 1
#define WM_CTRL_STEER 2

/* songs (w_musicRequestSend strings) */
#define WM_SONG_NORMAL 0 /* "Normal Song" */
#define WM_SONG_CHOCOBO 1 /* "Chocobo Song" */
#define WM_SONG_AIRSHIP 2 /* "Air Ship Song" */
#define WM_SONG_DISC4 3 /* "Disc4 Song" */
#define WM_SONG_FIELD1 4 /* "Field Song 1" */
#define WM_SONG_FIELD2 5 /* "Field Song 2" */
/* g_worldMusicState (w_musicUpdate strings) */
#define WM_MUSIC_READY 0
#define WM_MUSIC_REQUESTED 1
#define WM_MUSIC_WAITING 2

/* palette animation entry (wm_setActorFog) */
typedef struct WmPalAnim {
    u16 modelId; /* id */
    u8 pad2[2];
    s16 clutX; /* clut x */
    s16 clutY; /* clut y */
    u16 srcPal[16]; /* source palette */
    u16 workPal[2][16]; /* per draw buffer */
} WmPalAnim; /* 0x68 */

/* per-object entry in WorldMapObj.unk1C (0x28 bytes) */
typedef struct {
    unsigned shade : 8;
    unsigned scaleX : 12; /* scale x */
    unsigned scaleZ : 12; /* scale z */
    unsigned unk4 : 12; /* angle */
    signed offsetX : 10; /* offset x */
    signed offsetZ : 10; /* offset z */
    POLY_FT4 *prims[2][4]; /* shadow quads, per draw buffer */
} WmObjEnt;

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    s32 flags;
    void *heapEnd;
    u8 unk8[4];
    s16 swirlX;
    s16 swirlY;
    void *mapScript; /* file 0x5xxxx */
    void *areaData; /* file 0x6xxxx */
    s8 battleStep;
    s8 swirlFrames;
    u8 palAnimCount;
    u8 exitReason;
    WmObjEnt *shadows;
    s32 primCursor;
    s16 sceneId; /* map id */
    u16 battleMusic;
    struct WmPalAnim *palAnims; /* count: unk1A */
    u8 *loadBase;
    u8 *loadPtr;
    void *files[2]; /* [1] freed by wm_freeFile */
    u8 unk3C[16];
} WorldMapObj;

/* world map context, GameData.unk8E4.world */
typedef struct WorldMap {
    u32 flags; /* flags: 0x100 0x200 0x400 0x800 skip per-frame updates */
    u8 *mapHeapBase; /* end of the load area */
    WorldMapObj *obj; /* = &unk48 */
    void *worldData; /* allocation, freed by wm_freeWorldData */
    s32 parkedSoundSize;
    u8 unk14[0x34];
    WorldMapObj objData;
} WorldMap; /* 0x94 */
/* WorldMap.flags */
#define WM_MAP_LOADING 0x1 /* set at the start of wm_init, cleared at the end of wm_load */
#define WM_MAP_NO_FRAME 0x100 /* wm_main skips wm_frameMain (wm_startBattleTransition) */
#define WM_MAP_NO_TRANSITION 0x400 /* set by wm_load; while clear wm_main runs wm_updateBattleTransition */
#define WM_MAP_NAVI_SCREEN 0x1000 /* w_frame: set while g_worldNaviMode == 2 */

typedef struct {
    u8 pad0[0x11];
    u8 autoTurnOff;
} WmCameraFlags;

/* doubly linked list node; g_worldEffectFree is the list head */
typedef struct WmEffect {
    s32 sprite;
    struct WmEffect *prev;
    struct WmEffect *next;
    s8 abr;
    s8 type;
    s16 scale;
    s32 x;
    s32 y;
    s32 z;
    s16 frame;
    s16 pad1E;
    s32 phase;
    s32 unk24;
} WmEffect; /* 0x28 */

/* frame pack slot list: g_worldReadtxHead (head) -> g_worldReadtxSlots[6] -> g_worldReadtxTail (tail) */
typedef struct WmReadtxSlot {
    struct WmReadtxSlot *prev;
    struct WmReadtxSlot *next;
    s32 *pack; /* frame pack */
    s16 texId; /* low 6 bits: id; -1: none */
    u8 extracted;
    u8 padF;
} WmReadtxSlot; /* 0x10 */

typedef struct {
    s16 stickSide;
    s16 viewBlend;
    s32 viewBlendSpeed;
    s32 viewForceNormal;
    u16 yaw; /* camera angle */
    u16 yawOffset;
} WmCameraState;

typedef struct {
    u8 terrainClass;
    u8 shadowWidth;
    u8 shadowDepth;
    u8 flying;
    u8 unk4;
    u8 polyCache;
    u8 shadowJoint;
    u8 shadowOtBias;
    u32 cellAttr;
    s16 rideOffset;
    s16 floorY;
} WmActorType; /* 0x10 */

typedef struct {
    s16 unk0[5];
} WmCameraParams; /* 10 bytes */

/* movement mode table entry (g_worldMoveModes[], current one at g_worldMoveModeInfo) */
typedef struct {
    u8 controlType; /* mode */
    u8 checkTransitions;
    s16 speed; /* speed */
    u8 turnSpeed; /* turn speed */
    u8 climbSpeed;
    s8 maxRoll;
    u8 pad7;
    u8 airborne;
    u8 canToggleView;
    u8 lookAhead;
    u8 music;
    u8 loopSe;
    u8 countsSteps;
    s16 getoffDist;
    u8 cameraInFront;
    u8 cameraRow;
    u8 pad12[2];
    u32 allowedCells[2]; /* allowed cell types (bit mask) */
} WmMoveMode; /* 0x1C */

/* doubly linked node (wm_frmbufInit) */
typedef struct WmFrmbufSlot {
    struct WmFrmbufSlot *prev;
    struct WmFrmbufSlot *next;
    s16 texId;
    s8 vramArea;
    u8 transferred;
} WmFrmbufSlot; /* 0xC */

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
    u8 boneCount;
    u8 groupCount; /* group count */
    u8 pad4[0xC - 4];
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

/* animation file header: offsets relocated by wm_loadActorModel */
typedef struct WmAnimHdr {
    u8 pad0[4];
    s16 rootPos[3]; /* root position: constant, or offset of s16[frame] */
    u16 constMask;    /* 1, 2, 4: rootPos[i] is a constant */
    s32 rotKeys;    /* -> WmAnimKey[], rotation tracks, one per bone */
    s32 rotKeysLo;   /* -> WmAnimKey[], low rotation bits, two frames per byte */
} WmAnimHdr;

typedef struct ModelObj {
    u16 flags; /* flags; 0x10: behind the camera */
    u8 pad2[2];
    u8 parentBone; /* bone in the parent */
    u8 pad5[3];
    WmModel *model;
    struct WmAnimHdr *anim;
    u8 *packets; /* primitive buffer */
    s32 hiddenGroups;
    WmTexInfo *texInfo;
    struct ModelObj *parent; /* parent */
    u8 *boneMatrices;
    MATRIX worldMatrix; /* t: position */
    WmModelOut *groupOut;
    u8 pad48[0x50 - 0x48];
    s32 texAnims;
    u16 scaleX; /* scale */
    u16 scaleY;
    u16 scaleZ;
    u16 clipY;
    s32 unk5C;
    s32 mirrorSrc;
    u8 pad64[0x74 - 0x64];
    u16 attachX;
    u16 attachY;
    u16 attachZ;
} ModelObj;
/* ModelObj.flags */
#define MDL_BEHIND_CAMERA 0x10 /* wm_cullBehindCamera: origin SZ 0 or parent behind */

/* model slot (game_getModel) */
typedef struct {
    s32 flags; /* flags: 1 = loaded */
    s32 modelId; /* file id */
    ModelObj *obj;
    EvtObject *actor;
} WmObjSlot;
s32 evt_getObjectChrIndex(EvtObject *);
void wm_drawMesh(ModelObj *, u8);
void wm_projectMesh(ModelObj *, u8);
void wm_applyScale(ModelObj *);
void wm_calcBoneMatrices(ModelObj *, u16);
void wm_drawShadow(WmObjEnt *, s32, s32, s32); /* draw a shadow */
void wm_projectMeshClipY(ModelObj *, u8);
void wm_drawMeshClipY(ModelObj *, u8);
u8 *func_800634A8(s32);
u8 *func_800634D8(s32);

/* relocatable block: offsets are turned into pointers by wm_relocatePack */
typedef struct {
    s32 unk0;
    s32 verts;
    s32 polys;
} WmPackEntry;

typedef struct {
    s8 count; /* count */
    u8 pad1[3];
    s32 unk4;
    WmPackEntry *entries[1];
} WmPack;

/* text windows (menuMain_setCursor .. menuMain_updatePanels) */
typedef struct {
    u8 cursor;
    u8 state; /* index into s_menuMainInput */
    u8 orderCursor; /* party slots being swapped */
    u8 orderTarget;
    u8 orderPicked;
    u8 pad5[3];
    WinDef panels[6];
} MenuMain;

/* map cell (4 bytes) */
typedef struct {
    u16 sector;
    u8 packSectors;
    u8 texSectors;
} WmChunkEntry;

typedef struct {
    s16 width; /* width */
    s16 height; /* height */
    WmChunkEntry entries[1];
} WmChunkTable;

/* filled by w_musicGetID */
typedef struct {
    s32 unk0; /* volume mask */
    s32 count; /* count */
    s32 seIds[3]; /* sound ids */
} WmSeIds;

typedef struct {
    s32 pos[4];
} WmBlockPos; /* 0x10 */

/* list nodes (wm_blockListCompact, wm_chunkListCompact): g_worldBlockHead (head) ->
 * g_worldBlockNodes[40] -> g_worldBlockTail (tail) */
typedef struct WmBlockNode {
    struct WmBlockNode *prev;
    struct WmBlockNode *next;
    struct WmBlock *block;
    WmBlockPos *pos;
    s16 cell;
    u8 state;
    s8 slot;
} WmBlockNode; /* 0x14 */

/* list: g_worldChunkHead (head) -> g_worldChunkSlots[32] -> g_worldChunkTail (tail) */
typedef struct {
    u8 v0; /* vertex indices */
    u8 v1;
    u8 v2;
    u8 pad3[0xE - 3];
    u16 attr; /* attribute */
} WmPoly; /* 0x10 */

/* terrain mesh block (4 per map chunk): wm_getBlockHeight walks its triangles, wm_drawBlock draws it */
typedef struct WmBlock {
    u8 unk0;
    u8 texArea;
    u16 polyCount; /* polygon count */
    SVECTOR *verts; /* vertices */
    WmPoly *polys;
    u8 outline[0x14]; /* vertex indices of the on-screen test (wm_updateVisibleBlocks) */
    u8 outlineCount; /* vertex count */
    u8 fxId; /* block effect id (wm_drawBlockFx, wm_updateBlockFx) */
} WmBlock;

/* visible cells sorted by distance (wm_updateVisibleBlocks) */
typedef struct {
    s32 cell; /* cell */
    WmBlock *block;
    s32 dist; /* distance */
} WmNearCell;

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} WmScrRect;

/* chunk pack read by wm_readChunks: the 4 terrain blocks of one map chunk (wm_getCellBlock) */
typedef struct WmChunkPack {
    u8 unk0;
    u8 unk1;
    u8 texArea;
    u8 unk3;
    s32 unk4;
    WmBlock *blocks[4];
} WmChunkPack;

typedef struct WmChunkSlot {
    struct WmChunkSlot *prev;
    struct WmChunkSlot *next;
    struct WmChunkPack *pack;
    s16 chunkId;
    u8 state;
    u8 generation;
    u8 hasData;
    u8 pad11[3];
} WmChunkSlot; /* 0x14 */

/* colour + 2 values (8 bytes) */
typedef struct {
    CVECTOR color;
    u16 fogOffset;
    u16 fogOffset2;
} WmFogParam;

typedef struct {
    u16 fogOffset;
    s16 fogOffset2;
    s16 color[3]; /* rgb */
    s16 pad;
} WmFog; /* 0xC */

typedef union {
    SVECTOR v;
    u8 c[8];
} WmSkyCol;

/* sky / lighting parameter block (0x5E bytes); wm_blendWeather blends them */
typedef struct {
    SVECTOR lightColor[3]; /* light colours: columns of g_worldMapColorMatrix (wm_updateColorMatrices) */
    SVECTOR backdropLightColor;
    WmSkyCol backColor; /* back colour 1 */
    WmSkyCol backdropBackColor; /* back colour 2 */
    WmFog fog[3];
    s16 fxBackColor[3];
    u8 pad5A[2];
    u16 unk5C;
} WmSky; /* 0x5E */

typedef struct {
    WmSky presets[16]; /* presets: [mode * 4 + flag * 2 + 0/1] */
    WmSky work[7]; /* [0] current, [1]/[2] blended from [3]..[6] */
} WmWeatherData;
void wm_blendWeather(WmSky *, WmSky *, WmSky *, s32, s32);
void wm_weatherAuto(void);
void wm_updateFogParams(void);
void wm_updateColorMatrices(void);

typedef struct {
    s16 tpage;
    s16 clut;
    u8 u;
    u8 v;
    u8 pad6[2];
    s16 frmTpage; /* tpage */
    s16 frmClut; /* clut */
} WmTexArea; /* 0xC */

typedef struct {
    s16 mapX;
    s16 mapY;
    s32 worldX; /* x */
    s32 worldZ; /* z */
} WmNaviPlace; /* 0xC */

/* file 0x12xxxx lookup table (wm_findDefaultAnim) */
typedef struct {
    u16 modelId;
    u16 animId;
} WmAnimTableEntry;

typedef struct {
    u8 unk0;
    u8 count; /* count */
    u8 pad2[2];
    WmAnimTableEntry entries[1];
} WmAnimTable;

/* streamed load request (wm_streamSoundChunk) */
typedef struct {
    u8 pad0[8];
    s32 addr; /* address */
    s32 offset; /* offset */
    s32 size; /* size */
} WmStreamReq;

/* POLY_F4 with the screen coordinates as packed words */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s32 xy0;
    s32 xy1;
    s32 xy2;
    s32 xy3;
} WmPolyF4;

/* ---- overlay functions ---- */
s32 wm_getLeftStickX(s32 *);
s32 wm_getLeftStickY(s32 *);
s32 wm_getRightStickY(s32 *);
void wm_spawnEffect(s32, s32, s32, s32, s32);
s32 wm_freeFile(WorldMapObj *, s16);
s32 wm_depFindBattle(s16, s32 *, s32 *);
s32 wm_depFindBackground(s32, s32 *, s32 *);
s32 wm_depFindField(s16, s32 *, s32 *);
s32 wm_depFindArea(s32, s32 *, s32 *);
void w_frameMapDestructor(void);
void menuMain_swapPortraits(s32, s32);
void wm_blockVertsFog(void *);
s32 wm_blockFacesFog(void *, s32, s32);
void wm_blockVertsFar(void *);
s32 wm_blockFacesFar(void *, s32, s32);
void wm_blockVertsNear(void *);
s32 wm_blockFacesNear(void *, s32, s32);
s32 wm_getWeatherByPos(void);
s32 wm_getStoryFog(void);
void w_musicRequestSend(u32);
void wm_seFadeVolume(s32, u16, u8);
void wm_blendCameraPreset(s16, s16);
void wm_autoPilotOff(void);
void menuMain_selectCommand(void);
void wm_seStop(s32);
void wm_sePlay(s32, u8);
void wm_seSetVolume(s32, u8);
void wm_updateActors(void);
s32 w_memorymanHeapAlloc(s32);
void wm_setWeather(s32, s32, s32);
s32 wm_getTerrainHeightOffset(s32, u32, u8 *);
void wm_updateActorHeight(EvtObject *);
void wm_modelVerts(s32);
void wm_modelVertsFlat(s32);
s32 wm_modelFaces(s32, s32, s32);
s32 wm_modelFacesFog(s32, s32, s32);
void menuMain_drawMember(s32);
void menuMain_drawPortraitShade(RECT *, s32);
void wm_controlWalk(void);
void wm_controlSteer(void);
void wm_controlFly(void);
void menuMain_initPanels(void);
void menuMain_updateHelp(void);
void wm_frameTerm(void);
void w_memorymanDestructor(void);
void wm_mapSystemTerm(void);
void wm_musicTerm(void);
void wm_termEffects(void);
void w_movementChange(void);
void w_musicUpdate(void);
/* TIM image blocks */
typedef struct {
    u32 unk0; /* length */
    RECT unk4;
    u16 unk8[1];
} WmTimBlk;

typedef struct {
    u32 unk0; /* id */
    u32 unk4; /* flags: 8 = has CLUT */
    WmTimBlk unk8;
} WmTim;

void wm_loadTim(u32 *, s32, s32, s32, s32, s32, s8);
void menuMain_setExpHelp(s32);
void menuMain_setEquipHelp(u8);
s32 wm_getPadHeld(void);
s32 wm_getPadPressed(void);
s32 w_musicGetID(s32, WmSeIds *);
s32 w_frameExtruct(s32 *);
void wm_backdropVerts(s32);
s32 wm_backdropFaces(s32, s32, s32);
void wm_copyViaVram(s32, s32, s32);
void wm_mapTerm(void);
void wm_stopAllSe(void);
void wm_applyEncounterEvents(void);
void wm_frameSetup(void);
void wm_initClutAnims(void);
void wm_setMountPalette(void);
void wm_findVehicles(void);
void wm_weatherStart(void);
void wm_musicSetup(void);
void wm_main(s32);
void wm_init(void);
void wm_shutdown(void);
void wm_allocContext(void);
void wm_resetContext(void);
s32 wm_loadSoundBanks(void);
s32 wm_loadResidentMusic(void);
s32 wm_loadShadowTexture(void);
s32 wm_loadWorldData(void);
s32 wm_afterLoadSoundBanks(void);
s32 wm_afterLoadWorldData(void);
s32 wm_afterLoadResidentMusic(void);
s32 wm_freeWorldData(void);
s32 wm_afterLoadShadowTexture(void);
void wm_load();
void wm_exit(void);
u32 wm_loadActorModel(s32, EvtObject *);
void wm_clearOT(void);
void wm_endFrame(void);
void wm_startBattleTransition(void);
void wm_updateBattleTransition(void);
void wm_togglePause(void);
s32 wm_preloadRequest(void);
s32 wm_frameMain(void);
void wm_systemInit(void);
void w_frameSystemDestructor(void);
s32 func_800C45D8(void);
s32 wm_soundCommand(s32, s32, s32, s32, s32);
void wm_cameraFollow(void);
void wm_updateCameraPreset(void);
void wm_snapActorsToFloor(void);
void wm_scrollTextures(void);
void wm_animTextures(void);
void wm_animCluts(void);
void wm_initTexPages(void);
void wm_readtxInit(void);
void wm_readtxUpdate(void);
void wm_frmbufInit(void);
void wm_frmbufUpdate(void);
s32 wm_getSongMusicId(u16);
void menuMain_drawTimeGil(void);
void menuMain_drawLocation(void);
void menuMain_drawCursor(void);
void menuMain_input(void);
void menuMain_draw(void);
void menuMain_updatePanels(void);
void menuMain_drawParty(void);

/* ---- overlay data ---- */
extern RECT *s_menuMainTimeGilRect;
extern u8 s_menuMainGilSuffix[];
extern u8 g_worldFog;
extern u8 D_800CCE4D;
extern WmTexArea *g_worldCurTexArea;
extern u8 *s_menuMainEquipLabels[5];
extern char D_800A79D0[];
extern char D_800A7A0C[];
extern RECT *s_menuMainLocationRect;
extern s16 g_worldCameraBlendSpeed;
extern s16 g_worldCameraBlend;
extern s16 g_worldCameraBlendRow;
extern s8 g_worldCameraGroupOverride;
extern s32 g_worldStickDiv;
extern s32 g_worldSpeedSmoothDiv;
extern s32 g_worldSpeedScaleDiv;
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */
extern s32 D_800CCE2C;
extern s32 g_worldAutoTurnDiv;
extern s32 g_worldAutoTurnDivMin;
extern s32 g_worldAutoTurnDivMax;
extern s32 g_worldAutoTurnDeadZone;
extern u8 g_worldAreaCameraGroup[];
extern u8 g_worldAreaZone[];
extern u8 g_worldEncounterZoneStart[];
extern char D_800A7F48[];
extern WmCameraParams g_worldCameraParams;
extern WmCameraParams g_worldCameraParamsFrom;
extern u8 g_worldBackdropOn;
extern s32 g_worldReadyFrameCount;
extern MATRIX g_worldBlockFxLightMatrix;
extern MATRIX g_worldBackdropLightMatrix;
extern MATRIX g_worldMapLightMatrix;
extern u8 g_worldMapLoaded;
extern u8 s_menuMainText[]; /* text */
extern WmActorType g_worldActorTypeInfo[];
extern EvtObject *g_worldPlayer; /* player actor */
extern WmMoveMode *g_worldMoveModeInfo;
extern s32 g_worldWidth; /* map width (wraps) */
extern s32 g_worldDepth; /* map height (wraps) */
extern RECT g_worldVramParkRect;
extern char D_800A720C[]; /* "w_frameSystemDestructor:\n" */
extern WmNaviPlace g_worldNaviPlaces[][64];
extern s32 *g_worldTexBuffers[3];
extern s32 D_800CE3F8;
extern u8 g_worldPointFxBuf[];
extern u8 *g_worldPointFxBufPtr;
extern s32 g_worldPrimBuffers[];
extern MATRIX g_worldBackdropColorMatrix;
extern MATRIX g_worldMapColorMatrix;
extern MATRIX g_worldBlockFxColorMatrix;
extern u16 g_worldClutBuf[2][0x800]; /* palettes, [1] = shifted copy */
/* texture animation entries (wm_stepClutAnims) */
typedef struct {
    s16 x;
    s16 y;
    s16 start;
    s16 count; /* frame count */
    s16 speed; /* speed */
} WmClutAnim; /* 0xA */
extern WmClutAnim g_worldClutAnims[];
extern s16 g_worldClutAnimAcc[];
extern u8 g_worldClutAnimFrame[];
extern u16 g_worldClutAnimOffset[];
extern WmTexArea g_worldTexAreas[17];
extern s8 g_worldNaviPage;
extern char D_800A8098[];
extern s32 g_worldPrimPtr;
extern WmChunkTable *g_worldChunkTable;
extern WmChunkTable *g_worldChunkTableT;
extern u8 *g_worldBufIndex;
extern s32 g_worldOt[];
extern s32 g_worldEventResult;
extern s32 g_worldTimeEvents;
extern s32 g_worldTimeLogic; /* frame timing */
extern s32 g_swirlFrame;
extern s32 g_swirlBuf;
extern s32 g_worldTimeDraw;
extern s32 g_worldPrimUsed;
extern s32 g_worldPrimUsedPercent;
extern s32 g_worldTimeTotal;
extern WmFogParam g_worldBackdropFog;
extern WmFogParam g_worldFogParams[2];
extern s32 g_worldChunksH;
extern WmBlockNode g_worldBlockHead; /* list head */
extern s32 g_worldBlockListDirty;
extern WmBlockNode g_worldBlockNodes[40];
extern WmBlockPos g_worldBlockPositions[20];
extern WmBlockNode g_worldBlockTail; /* list tail */
extern WmChunkSlot g_worldChunkTail;
extern WmChunkSlot g_worldChunkHead;
extern WmChunkSlot g_worldChunkSlots[32];
extern s32 g_worldChunkCenter;
extern s32 g_worldChunksLoadedCount;
extern s32 g_worldChunkListDirty;
extern WmWeatherData g_worldWeatherData;
extern char D_800A71F4[]; /* "w_frameMapDestructor:\n" */
extern char D_800A7A40[]; /* "w_framePackGetPtr : attempt to get %d element with %d max\n" */
extern void (*s_menuMainInput[])(void);
extern u8 g_worldSongMusicIds[];
extern WmCameraState *g_worldCamera;
extern s8 g_worldMoveMode;
extern s32 g_worldHeapUsed;
extern s32 g_worldHeapDepth;
extern s32 g_worldHeapTop;
extern s32 g_worldHeapLastSize;
extern u16 *g_worldStoryCounterPtr;
extern u8 g_worldDataSet;
extern s32 g_worldCellsW;
extern WmChunkPack *g_worldDefaultBlocks;
extern s32 g_worldChunksW;
extern MATRIX g_worldBlockMatrix;
extern s32 g_worldMapDebugFlags; /* debug flags */
extern s16 D_800CCD7C;
extern u8 g_worldPlayerStepped;
extern u16 g_worldEventBattleRate;
extern s32 g_worldReqParams[4]; /* script request parameters */
/* texture transfer requests (wm_frmbufTransfer) */
typedef struct {
    s32 pack;
    u8 area;
    u8 vramArea;
    u8 pad6[2];
} WmTexTransfer;
extern WmTexTransfer g_worldFrmbufTransfers[];
extern s16 g_worldFrmbufAreaX[2];
extern s16 g_worldFrmbufAreaY[2];
extern s16 g_worldFrmbufClutX[2];
extern s16 g_worldFrmbufClutY[2];
extern char D_800A8118[];
extern WmCameraFlags *g_worldCameraFlags;
extern u8 g_worldAutoPilot;
extern u8 g_worldAutoPilotSavedAutoTurn;
extern s32 g_worldAutoPilotDist;
extern s32 g_worldAutoPilotDest;
extern WmReadtxSlot g_worldReadtxTail;
extern WmReadtxSlot g_worldReadtxHead;
extern WmReadtxSlot g_worldReadtxSlots[6];
extern s32 g_worldReadtxReqCount;
extern s32 g_worldReadtxDirty;
extern s8 g_worldMapError;
extern char g_worldMapErrorMsg[];
extern WmEffect g_worldEffectFree;
extern WmEffect g_worldEffectActive;
extern WmEffect g_worldEffectNodes[60]; /* free list nodes */
extern WmEffect g_worldEffectFreeTail; /* list tail */
extern WmEffect g_worldEffectActiveTail; /* list tail */
extern s32 g_worldEffectSpeed;
extern s32 g_worldSprayLevel;
extern s32 g_worldSprayTarget;
extern s32 g_worldEffectSpeedTarget;
extern VECTOR g_worldBlockFx4Pos;
extern SVECTOR g_worldBlockFx6RingRot[10];
extern s16 g_worldBlockFx6RingDist[10];
extern s16 g_worldBlockFx6FlashLen[10];
extern s8 g_worldBlockFx6FlashTimer[10];
extern SVECTOR g_worldBlockFx6FlashRot[10];
extern VECTOR D_800D4960[9];
extern SVECTOR D_800D49F0[9];
extern SVECTOR g_worldBlockFx7Rot;
extern SVECTOR g_worldBlockFx4Rot;
extern SVECTOR g_worldBlockFx6RotA;
extern SVECTOR g_worldBlockFx6RotB;
extern SVECTOR g_worldBlockFx4PartRots[3];
extern void **g_worldDataPack;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk800B8388;
extern MATRIX *g_worldViewMatrix;
void wm_drawBackdrop(s32, s32);
extern u8 g_worldParkedSoundRectIdx;
extern RECT g_worldParkedSoundRects[];
extern u8 g_worldAreaZone[];
extern u8 g_worldCameraSmoothX;
extern u8 g_worldCameraSmoothY;
extern u8 g_worldCameraSmoothZ;
extern s32 g_worldCameraTarget[];
extern s32 g_worldCameraEye[];
extern s32 g_worldCameraDir[];
extern s32 g_worldChunkLookPos[];
extern s32 g_worldCameraRecenter;
extern s32 g_worldChunkBuffer;
extern u8 D_800D30C4;
extern s32 D_800D32FC;
extern u8 g_worldLoadedFrames;
extern u8 g_worldUsePolyCache;
extern u8 g_worldChunkGeneration;
extern u8 g_worldChunkFlags[]; /* map cells, g_worldChunksW per row */
#define WM_CELL(x, y) g_worldChunkFlags[(y) * g_worldChunksW + (x)]
extern u16 g_worldFormStoryThresholds[11]; /* story progress thresholds (wm_applyStoryForms) */
extern u8 g_worldFormStatusTable[][11];
extern u8 D_8007AF55; /* = g_evtGlobalVars.b[0x65], general global flags */
extern u32 D_800CCB18[2]; /* cell type masks */
extern u32 g_worldWaterCellTypes[2];
u8 wm_testCellType(u32 *, u32);
extern s32 g_worldChunkRunGeneration;
s32 wm_chunkRequest(s16);
void wm_depthCueClut(u16 *, u16 *, s32, s16 *);
extern s32 g_worldWeatherAutoMode;
extern s32 g_worldWeatherSpeed;
extern s32 g_worldWeatherBlend;
extern s32 g_worldWeatherMode;
extern u8 g_worldWeatherAuto;
extern s8 g_worldMusicState;
extern s32 g_worldMusicSong;
extern s32 g_worldMusicStarted;
extern s32 g_worldMusicId;
extern s32 g_worldMusicRequest;
extern MenuMain s_menuMain;
extern u8 g_worldState;
extern char D_800A7CCC[];
extern s16 g_worldTerrainHeightOffsets[][9];
extern WmMoveMode g_worldMoveModes[];
extern s16 g_worldClimbSmooth;
extern s16 g_worldSpeedSmooth;
extern s32 g_worldHeapStack[];
extern u8 g_worldCameraFourProbes;
extern s32 g_worldNaviMode; /* navi mode */
extern u8 g_worldNaviVisible;
extern s32 g_worldTitleDelay;
extern POLY_G3 *g_worldNaviConePrim;
extern POLY_FT4 *g_worldTitlePrim;
extern s8 g_worldTitleId;
extern s32 g_worldTitleWide;
extern u8 g_worldTitleFade;
extern u8 g_worldNaviCursorDraw;
extern u8 g_worldNaviCursorOn;
extern u8 g_worldNaviPlaceCycled;
extern u8 g_worldNaviActive;
extern s32 g_worldNaviPrevMode;
extern s8 g_worldTitlePhase;
extern u8 g_worldNaviPlace;
extern s32 g_worldFrmbufReqCount;
extern WmFrmbufSlot g_worldFrmbufTail;
extern WmFrmbufSlot g_worldFrmbufHead;
extern s32 g_worldFrmbufDirty;
extern WmFrmbufSlot g_worldFrmbufSlots[4];

/* scrolling texture strips (wm_scrollTextures) */
typedef struct {
    u16 x; /* x */
    u16 y; /* y */
    u16 w; /* w */
    u16 h; /* h */
    u16 speed; /* speed (8.8) */
} WmTexScroll; /* 0xA */

/* random-encounter table entry (g_worldEncounterTable, docs/encounters.md) */
typedef struct {
    u16 battle[4];
    u8 topoPattern; /* topography << 2 | slot pattern */
    u8 zoneFog; /* zone << 1 | fog */
} WmEncounter; /* 0xA */

extern u16 g_worldTexScrollAcc[8];
extern WmTexScroll g_worldTexScrolls[8];
extern u16 D_800CFEF8;
extern u16 D_800CFF02;
extern void *g_worldTexAnimPack;
extern s8 g_worldTexAnimStep[2];
extern s8 g_worldTexAnimSeq[];
extern WmEncounter *g_worldEncounterTable;
extern u16 (*g_worldEncounterEventRows)[12];
extern s32 g_worldAmbientSeDist[3];
extern u8 g_worldAmbientSeOn[3];
extern s32 g_worldAmbientSeX[3]; /* sound source x */
extern s32 g_worldAmbientSeZ[3]; /* sound source z */
extern u8 g_worldAmbientSe[3]; /* sound ids */
extern u8 g_worldFormFlags[]; /* flags (wm_applyStoryForms) */
extern s32 D_800D3300;
extern u8 g_worldBlockFx7Spin;
extern u8 g_worldClutAnimFx2;
extern u8 g_worldCdUser;
void wm_stepClutAnims(s32, s32, s32);
void wm_initTextureLists(void);
void wm_blockListInit(void);
void menuMain_reorder(s32, s32);
void wm_musicPlay(u16);
void wm_musicStop(u16);

/* image pack loaded by wm_loadImagePack; offsets are from the header start */
typedef struct {
    s32 clutOfs; /* offset: WmImagePackClut */
    s32 imageOfs; /* offset: images (RECT + pixels) */
    s32 imageCount; /* image count */
} WmImagePack;

typedef struct {
    s32 dataOfs; /* offset: pixel data */
    s32 count; /* count */
    RECT rects[1];
} WmImagePackClut;

/* text window contents passed to tween_start (as in ovl_38) */
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 *unk4;
    s32 unk8;
    void (*unkC)(void);
} TextWin;

extern RECT s_menuMainPanelRects[6];
extern TextWin s_menuMainSlideIn[4];
extern u8 s_menuMainExpText[16];
extern u8 s_menuMainNextText[16];
extern u8 s_menuMainExpLabel[];
extern u8 s_menuMainNextLabel[];
extern u8 s_menuMainUntilLabel[];
extern u8 s_menuMainStatusLabel[];
extern u8 s_menuMainStatusNormal[];
extern u8 s_menuMainStatusAbnormal[];

/* wm_initMovement init state */
typedef struct {
    u8 lastSlot; /* last written slot */
    u8 pad1[3];
    u16 polys[6]; /* polygon */
    u16 cells[6]; /* cell */
} WmPolyCache; /* 0x1C: recently hit walkmesh polygons */
s32 wm_getGroundHeight(VECTOR *, u32 *, s32 *, WmPolyCache *, u16 *);
u8 wm_tryStep(VECTOR *, s32, s32, s32, u32 *, s32 *);
u8 wm_findGetoffPos(ActorPos *);
extern WmPolyCache g_worldPolyCache[11];
extern EvtObject g_worldDummyActor;
extern u8 g_worldFlyTurning;
extern u8 g_worldCameraTurning;
extern u8 g_worldPlayerPosChanged;
extern s32 g_worldCeilingGap;
extern EvtObject *g_worldMountActor;
extern u8 g_worldCameraFastRise;
extern EvtObject *D_800CE270;
extern u8 g_worldFastFly;
extern s32 g_worldPlayerClimbSpeed;
extern s32 g_worldMountPaletteOverride;
extern s32 D_800CE284;
extern u8 g_worldCameraTurnedByHand;
extern s32 g_worldPlayerSpeed;
extern u16 g_worldPlayerHeading;
extern EvtObject *g_worldVehicleActor;
extern s32 g_worldCameraTurnGain;
extern EvtObject *D_800CE3D8;
extern s32 g_worldPlayerTurn;

extern u8 D_800CCE49;
extern s32 g_worldDebugStep; /* debug: last update step */
extern s32 g_worldDrawActors;
extern u8 g_worldPointFxOn;
extern u8 g_worldStartup;
extern TILE g_worldPauseTile;
extern DR_TPAGE g_worldPauseTpage;
extern u32 g_worldPauseOT;
extern u8 g_worldPauseSprite[];
void game_resumeClock(s32);
void akao_sendCmd99(s32);
void akao_sendCmd98(s32);
extern s32 g_worldFrameCount;
extern u8 g_worldMogChoice;
extern s16 g_worldOtBias;
extern u8 D_800CF47E;
extern u8 g_worldSuspended;
extern u8 g_worldMapJump;
extern u8 g_worldVehiclesFound;
u8 *wrain_init(u8 *);
void wrain_set(s32, u32);
extern s32 g_worldTimeActorDraw;
extern s32 g_worldTimeMapDraw;
void wm_drawActors(void);
void wm_drawMap(void);
void wm_naviDraw(void);
void wm_drawEffects(void);
s32 wm_getCellBlock(s16, u8 *);
s32 wm_getBlockHeight(s32, WmBlock *, VECTOR *, u32 *, s32 *, WmPolyCache *);
void wm_wrapPos(s32 *, s32 *);
void wm_posToCell(s32, s32, s32 *);
void wm_cellToPos(s32, s32 *, s32 *);
void wm_readtxReset(void);
void wm_readtxDone(s32 *);
void wm_relocatePack(WmPack *);
extern u8 g_worldCameraHold;
extern u8 g_worldCameraLookupFix;
extern s32 g_worldChunkRunSlot;
extern s32 g_worldChunkRunCount[];
extern WmChunkSlot *g_worldChunkRun[];
extern WmPack *g_worldChunkRunPacks[];
extern s32 *g_worldChunkRunTex[];
extern s32 g_worldImageNames[5];
extern s32 g_worldImageSectors[5]; /* CD sectors */
extern s32 D_800CCF84[5];
extern s32 D_800CCF98[5];
extern char D_800A7BB8[];
/* sprite table (wm_naviDrawFrame) */
typedef struct {
    s16 x; /* x */
    s16 y; /* y */
    u8 u; /* u */
    u8 pad5;
    u8 v; /* v */
    u8 pad7;
    s16 w; /* w */
    s16 h; /* h */
} WmNaviFramePart; /* 0xC */
extern WmNaviFramePart g_worldNaviFrameParts[4];
void wm_naviDrawFrame();
extern s32 g_worldWeatherPointDist;
void wm_frmbufReset(void);

extern WmCameraParams g_worldCameraPresets[];
extern u8 g_worldCameraPresetIndex[][3][3];
extern VECTOR g_worldCameraUp;
extern SVECTOR g_worldCameraLookAt; /* look-at point passed to wm_lookAt, never written (zero: the scene is drawn relative to g_worldCameraTarget) */
extern SVECTOR g_worldCameraEyeRel;
extern SVECTOR g_worldCameraEyeOffset;
extern s32 g_worldCameraTargetHeight;
extern s32 g_worldCameraDistScale;
extern s16 g_worldCameraRoll;
extern u8 D_800CE1E1;
extern u8 g_worldCameraStartPhase;

/* main exe: candidates for ff9.h */
void math_rotMatrixZXY(SVECTOR *, MATRIX *);
void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
void func_80064CDC(s32, s32);
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */
void math_rotMatrixYXZ(SVECTOR *, MATRIX *); /* RotMatrix */
void math_rotMatrixXYZ(SVECTOR *, MATRIX *);
s32 math_cos(s32); /* cos */
s32 math_sin(s32); /* sin */
void SetGeomScreen(u16);
s32 wrain_draw(s32);
extern u32 g_portraitSavedVram[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *g_menuHelpText;
s32 text_measureStyle(u8 *, s32);
extern s32 g_menuScreenFrame; /* overlaps Unk80077E50.unk4 in ff9.h */
EvtObject *evt_getPlayer(void);
void evt_resetIdleTimer(s32);

/* run on the scratchpad stack (top of the 1 KB scratchpad) */
#define SetSpadStack(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -4\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack() __asm__ volatile("addiu $29, $29, 4\n\tlw $29, 0($29)" : : : "memory")

/* world effect emitter table entry (wm_updateBlockFx) */
typedef struct {
    s32 x; /* position */
    s32 y;
    s32 z;
    s32 rangeX; /* random range */
    s32 rangeY;
    s32 rangeZ;
    u8 pad18[0x24 - 0x18];
    s16 effect; /* effect, -1 = end */
    s16 scale;
    s16 chance; /* chance (of 4096) */
    u8 pad2A[0x40 - 0x2A];
} WmEmitter; /* 0x40 */

/* map-marker colour (wm_naviDrawPlaces) */
extern s32 g_worldNaviCursorX; /* navi cursor */
extern s32 g_worldNaviSnapDist; /* marker snap distance */
extern s32 g_worldNaviCursorY;
typedef struct { SVECTOR v[4]; } WmQuad; /* shadow quad template */
extern WmQuad g_worldBlockBackQuad;
extern WmQuad g_worldNaviMiniMapArrow; /* radar arrow */
extern WmQuad g_worldNaviFullMapArrow; /* big-map arrow */
extern WmScrRect g_worldBlockCullRects[]; /* on-screen test rects */
extern char D_800A8178[]; /* "Illegal Weather Mode" */
extern VECTOR g_worldBlockFx7Pos;
extern VECTOR g_worldBlockFx6Pos;
extern SVECTOR g_worldBlockFx6CoreRot;
extern s32 g_worldBlockFx6CoreOtOfs;
extern s32 g_worldBlockFx6SpriteSize;
extern MATRIX g_worldIdentityMatrix;
extern VECTOR g_worldBlockFx4Scale;
extern VECTOR g_worldBlockFx6RingScale;

/* first entry of data pack `id` */
#define WM_PACK0(id) ((s32)((WmPack *)w_framePackGetPtr(g_worldDataPack, id))->entries[0])

/* inline CompMatrix: c = a * b, c->t = a * b->t + a->t */
#define gte_CompMatrix(a, b, c)    \
    gte_MulMatrix0(a, b, c);    \
    gte_SetTransMatrix(a);      \
    gte_ldlv0((b)->t);          \
    gte_rtv0tr();               \
    gte_stir((c)->t);

extern WmNearCell g_worldNearCells[];
s32 wm_getCameraViewState();
void wm_frmbufCompact(void);
void wm_frmbufTransfer(void);
void wm_blockListCompact(void);
void wm_blockListResolve(void);
s32 wm_blockListRequest(s16);
s32 w_frmbufRequest(s16);
void wm_updateBlockFx(s32);
extern u8 g_worldNaviConeR; /* big-map cone colour */
extern u8 g_worldNaviConeG;
extern u8 g_worldNaviConeB;
extern u8 g_worldNaviTileR; /* big-map tile colour */
extern u8 g_worldNaviTileG;
extern u8 g_worldNaviTileB;
extern RECT g_worldVramSaveRect;
extern char D_800A7F48[]; /* encounter fallback message */ /* palette area (wm_systemInit) */
extern CVECTOR g_worldNaviVehicleColorA; /* marker colours */
extern CVECTOR g_worldNaviVehicleColorB;
extern CVECTOR g_worldNaviMountColorA;
extern CVECTOR g_worldNaviMountColorB;
extern u8 g_worldNaviPlaceR;
extern u8 g_worldNaviPlaceG;
extern u8 g_worldNaviPlaceB;
void wm_naviDrawMarker(s32, s32, s32, s32, CVECTOR, CVECTOR, s32, s32, s32, s32);
void wm_naviWorldToMap(s32, s32, s32 *, s32 *, u8);
void wm_naviUpdate(void);
s32 wm_naviIsPlaceKnown(u32);

static inline CVECTOR wm_naviDrawPlaces_color(void) {
    CVECTOR c;

    memset(&c, 0, 4);
    c.r = g_worldNaviPlaceR;
    c.g = g_worldNaviPlaceG;
    c.b = g_worldNaviPlaceB;
    return c;
}

/* Inlined copy of wm_wrapPos (wrap map coordinates). */
static inline void wm_wrapPos_inline(s32 *x, s32 *z) {
    if (*x < 0) {
        *x += g_worldWidth;
    }
    if (*x >= g_worldWidth) {
        *x -= g_worldWidth;
    }
    if (*z > 0) {
        *z -= g_worldDepth;
    }
    if (*z <= -g_worldDepth) {
        *z += g_worldDepth;
    }
}

/* pad state, only read in mode 2 (wm_debugAdjustScreen) */
static inline u32 wm_debugAdjustScreen_pad(void) {
    return g_worldState == WM_STATE_RUNNING ? g_padPortHeld[0] : 0;
}

static inline u32 wm_debugAdjustScreen_trg(void) {
    if (g_worldState != WM_STATE_RUNNING) {
        return 0;
    }
    return g_padPortPressed;
}

void wm_freeEffect(WmEffect *);
s32 wm_getSpriteFrameCount(u16 *);

/* random byte from the main exe's table (rand_refill refills it) */
static inline s32 wm_rand8(void) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return *g_randPtr++;
}

/* Inlined copy of wm_testCellType (cell type bit test). */
static inline u8 wm_testCellType_inline(u32 *a, u32 k) {
    u32 w;

    if (k >= 32) {
        w = a[0];
        k -= 32;
    } else {
        w = a[1];
    }
    if ((w >> k) & 1) {
        return 1;
    }
    return 0;
}

/* Inlined copy of wm_testCellType with the index already shifted (wm_controlFly). */
static inline u8 wm_testCellType_inline2(u32 *a, u32 c) {
    if (c >= 32) {
        if ((a[0] >> (c - 32)) & 1) {
            return 1;
        }
    } else if ((a[1] >> c) & 1) {
        return 1;
    }
    return 0;
}

typedef struct {
    u16 frameCount; /* frame count (low 15 bits) */
    u16 tpage;
    u16 clut;
    u8 height;
    u8 width;
    u16 frameOffsets[1]; /* per-frame data offsets, then a counted u16 table */
} WmSprHdr; /* sprite animation header (wm_drawSprite3D) */

typedef struct {
    u16 *uvs;
    u16 *colors;
    s32 halfW;
    s32 halfH;
    s32 tpage;
    s32 clut;
    s32 fade;
    s32 quadCount;
    s32 primCode;
    s32 sizeShift;
    s32 otBias;
} WmSprCtx; /* draw context passed to wm_emitSprite3D */

s32 wm_emitSprite3D(u8 *, WmSprCtx *, s32, s32);
s32 wm_drawSprite3D(s32, WmSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    s32 size; /* size */
    void *dest; /* destination */
} WmLoadReq; /* CD read request (wm_readChunks, cd_readChunks) */

extern WmLoadReq g_worldChunkReadReqs[];
extern WmLoadReq g_worldChunkReadList[];
extern s32 g_worldChunkRunDir[];
extern s32 g_worldChunkReadCount;
extern char D_800A7F94[];
extern SVECTOR g_worldEffectFlatRot; /* rotation used by wm_drawEffects */
s32 wm_readtxFind(s32, s32);
u8 wm_findChunkRun(WmChunkSlot **, s32 *, s32 *);
void wm_getChunkEntry(s32, s32, WmChunkEntry *);
void wm_getChunkEntryT(s32, s32, WmChunkEntry *);
void wm_chunksLoaded(void);

typedef struct {
    u8 b[0x1E0];
} WmCellMap; /* g_worldChunkFlags as one block */
typedef struct {
    WmTexScroll e[30];
} WmTexScrollTbl; /* g_worldTexScrolls and the entries after it */
extern u8 g_worldZoneEncounterCount[];
extern s32 g_worldCellsH;
void wm_applyStoryForms(void);
void wm_chunkListInit(void);
void wm_initBlockMatrix(void);
void wm_loadImagePack(WmImagePack *);

#define gte_stir(r0) __asm__ volatile( \
    "swc2\t$9, 0(%0);" \
    "swc2\t$10, 4(%0);" \
    "swc2\t$11, 8(%0)" \
    : : "r"(r0) : "memory")

void wm_calcShadowMatrix(WmObjEnt *, WmObjSlot *, MATRIX *, s32, s32);

/* wm_drawShadow: project one corner of the 3x3 shadow grid */
#define SHADOW_PT(x, z) \
    v.vx = (x); \
    v.vy = 0; \
    v.vz = (z); \
    gte_ldv0(&v); \
    gte_rtps()

/* wm_drawShadow: link shadow quad q at its deepest corner */
#define SHADOW_ADD(k, q) \
    k >>= 3; \
    if (-g_worldOtBias < k && k < 0x1000 - g_worldOtBias) { \
        addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][k], e->prims[g_gameState->gameData->unk8][q]); \
    }

/* main exe: candidates for ff9.h */
u8 cd_readChunks(s32, void *, s32, s32);
s32 evt_getWorldCamEyeObj(void);
s32 evt_getWorldCamTargetObj(void);
void fade_inAdditiveDefault(void);
void OuterProduct0(VECTOR *v0, VECTOR *v1, VECTOR *v2);

/* wm_getBlockHeight (walkmesh height lookup) */
extern s16 g_worldHitHeights[];
extern u16 g_worldHitPolys[];
extern char D_800A7D20[];

/* wm_movePlayer (walkmesh movement) */
void wm_controlPlayer(void);
extern u8 D_800CCE4A;
extern u8 D_800CCE55;
extern u8 g_worldFastWalk;
extern u8 g_worldPlayerFloorPoly;

/* wm_cameraFollow (camera follow) */
extern u8 g_worldCameraSmoothing;
extern WmPolyCache g_worldCameraPolyCache[4];
extern s32 g_worldCameraEaseDiv;
extern s32 g_worldCameraEaseShift;
extern s32 g_worldCameraEaseDivMin;
extern s32 g_worldCameraEaseDivMax;
extern s32 g_worldCameraEaseScale;
extern s32 D_800CCB14;

/* wm_soundCommand (sound/music request handler) */
extern s32 g_worldMusicReqId; /* requested music id */
extern u8 D_800CCA12;  /* byte 2 of g_worldMusicReqId: id kind */
extern s8 g_worldMusicReqKind;  /* request kind, -1 = none */
extern s32 g_worldMusicSector; /* CD sector of the music file */
s32 wm_isMusicFadingOut(void);
s32 wm_findSoundFile(); /* old-style: one caller passes a third argument */
s32 wm_depGetLink(s32 arg0);
s32 wm_unpackMusic(s32 arg0);
void wm_streamSoundChunk(WmStreamReq *arg0);
s32 wm_getModelCacheBottom(void);

/* main exe: candidates for ff9.h (sound) */
s32 akao_playMusic(void *);
void akao_stopMusic(s32);
void akao_saveMusic(void);
s32 akao_playSe(s32, s32, s32, s32);
void akao_sendCmd92(s32);
void akao_setAllSeVolume(s32);
void akao_fadeAllSeVolume(s32, s32);
void akao_setSeVolume(s32, s32, s32);
void akao_fadeSeVolume(s32, s32, s32, s32);
void akao_sendCmdAA(s32);
void akao_sendCmdAB(s32, s32);
void akao_sendCmdA2(s32, s32, s32);
void akao_sendCmdA3(s32, s32, s32, s32);
void akao_sendCmdAC(s32);
void akao_sendCmdAD(s32, s32);
void akao_sendCmdA4(s32, s32, s32);
void akao_sendCmdA5(s32, s32, s32, s32);
void akao_setMusicVolume(s32, s32);
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeMusicVolumeFrom(s32, s32, s32, s32);
s32 akao_getSpuTransferState(void);

s32 wm_initShadowPrims(s32, s32); /* returns func_80062E3C's result (the next free address) */

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
void evt_createModels(void); /* main */
s32 evt_isControlEnabled(void); /* main */
u8 *evt_loadScripts(u8 *p); /* main */
s32 evt_countNpcActors(s32 unused); /* main */
void evt_callWorldCellTrigger(s32 a, s32 b, s32 c); /* main */
void func_800B36AC(void);
s32 func_800B8548(void);
void func_800C16C0(void);
void game_updateWindows(void); /* main */
void menu_update(void); /* main */
void msg_setOtz(s32 a); /* main */
s32 swirl_update(s16 arg0, s16 arg1); /* main */
void *w_framePackGetPtr(void **arg0, s32 arg1);
void w_movementChangeSP1(void);
void w_movementChangeSP2(void);
s32 w_readtxRequest(); /* (s16); unprototyped: callers pass the argument unconverted */
void wm_autoPilotOn(void);
void wm_buildPackets(ModelObj *arg0, WmTexInfo *arg1, s16 (*arg2)[2]);
void wm_calcCameraPreset(s32 arg0, s32 arg1, s32 arg2, s32 arg3, WmCameraParams *arg4);
void wm_callChocobo(void);
s32 wm_cameraAltitudeBlend(s32 arg0);
void wm_cameraAutoTurnVehicle(void);
void wm_cameraAutoTurnWalk(void);
void wm_cameraSmooth(void);
void wm_chunkListCompact(void);
s32 wm_drawActorModel(EvtObject *arg0, SVECTOR *arg1, s32 unused);
void wm_drawActorShadow(EvtObject *arg0, SVECTOR *arg1, s32 arg2);
void wm_drawBlock(u8 *arg0, s32 arg1, s32 arg2);
void wm_drawBlockFx(u8 arg0);
void wm_drawBlocks(void);
void wm_drawModel(s32 arg0, s32 arg1, u32 arg2);
void wm_drawTitle(void);
void wm_fetchParkedSound(void);
s32 wm_findAreaData(); /* (WorldMapObj *, u16); unprototyped: callers pass the argument unconverted */
void wm_findBattleMusic(void);
s32 wm_findDefaultAnim(s32 arg0);
s32 wm_findMapScript(); /* (WorldMapObj *, u16); unprototyped: callers pass the argument unconverted */
void wm_finishLoad(void);
s32 wm_getAreaZone(s32 arg0);
s32 wm_getChunkBuffer(void);
s32 wm_getWorldDataFileId(void);
void wm_heapInit(void);
void wm_initCamera(void);
void wm_initDiscImage(void);
void wm_initEffects(void);
void wm_initLighting(void);
void wm_initMapState(void);
void wm_initModel(ModelObj *arg0, u8 *arg1);
void wm_initMovement(void);
s32 wm_isMapSongPlaying(void);
s32 wm_lerp(s32 arg0, s32 arg1, s32 arg2);
s32 wm_lerpShift(s32 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 wm_loadFile(WorldMapObj *arg0, s32 unused);
void wm_loadTimPack(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void wm_lookAt(MATRIX *arg0, SVECTOR *arg1, SVECTOR *arg2, VECTOR *arg3);
void wm_movePlayer(WmActorType *cc);
void wm_musicFade(); /* (void); unprototyped: w_frame.c passes an argument it ignores (splat writes the empty body itself) */
void wm_musicInit(void);
void wm_naviDrawFullMap(void);
void wm_naviDrawMiniMap(void);
void wm_naviDrawPlaces(void);
void wm_naviInit(void);
void wm_postDraw(void);
void wm_readChunks(void);
void wm_readtxCompact(void);
void wm_requestChunks(s32 arg0);
void wm_resetMapObj(void);
void wm_runEvents(void);
void wm_saveDepTable(void);
void wm_setActorFog(EvtObject *arg0, s32 arg1);
void wm_setMusicVolumeFade(s32 arg0, s32 arg1);
void wm_setupMapObj(void);
void wm_setupWorldData(void);
void wm_updateCamera(void);
void wm_updateCameraNoView(void);
void wm_updateChunks(void);
void wm_updateDraw(void);
void wm_updateEffects(void);
void wm_updateLogic(void);
void wm_updateMap(void);
void wm_updateTextureLists(void);
void wm_updateVisibleBlocks(void);
void wm_updateVramAnims(void);
void wm_uploadParkedSound(void);
void wm_weatherInit(void);
void wm_weatherUpdate(void);
s32 wm_worldToView(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2, s32 *arg3);

#endif
