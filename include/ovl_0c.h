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
 * (main func_80037D6C's default branch) */
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

/* wm_soundCommand (GameData.unk83C) commands named by the request strings */
#define WM_SND_GET_STATE 0xC00 /* "Waiting [ State : %d ]": non-zero while loading */
#define WM_SND_SE_PLAY 0xD000 /* "SE Play" */
#define WM_SND_SE_STOP 0x5100 /* "SE Stop" */
#define WM_SND_SE_VOL 0x9201 /* "SE Vol" */
#define WM_SND_SE_VOL_INTPL 0xD301 /* "SE Vol Intpl" */
#define WM_SND_MUSIC_VOL_INTPL 0x8601 /* "Music Vol Intpl", "Music Fade Force" */


/* palette animation entry (wm_setActorFog) */
typedef struct WmPalAnim {
    u16 unk0; /* id */
    u8 pad2[2];
    s16 unk4; /* clut x */
    s16 unk6; /* clut y */
    u16 unk8[16]; /* source palette */
    u16 unk28[2][16]; /* per draw buffer */
} WmPalAnim; /* 0x68 */

/* per-object entry in WorldMapObj.unk1C (0x28 bytes) */
typedef struct {
    unsigned unk0 : 8;
    unsigned unk1 : 12; /* scale x */
    unsigned unk2 : 12; /* scale z */
    unsigned unk4 : 12; /* angle */
    signed unk4_12 : 10; /* offset x */
    signed unk4_22 : 10; /* offset z */
    POLY_FT4 *unk8[2][4]; /* shadow quads, per draw buffer */
} WmObjEnt;

/* object embedded in WorldMap (0x4C bytes) */
typedef struct {
    s32 unk0;
    void *unk4;
    u8 unk8[4];
    s16 swirlX;
    s16 swirlY;
    void *mapScript; /* file 0x5xxxx */
    void *areaData; /* file 0x6xxxx */
    s8 battleStep;
    s8 swirlFrames;
    u8 unk1A;
    u8 exitReason;
    WmObjEnt *shadows;
    s32 primCursor;
    s16 sceneId; /* map id */
    u16 battleMusic;
    struct WmPalAnim *unk28; /* count: unk1A */
    u8 *unk2C;
    u8 *unk30;
    void *unk34[2]; /* [1] freed by wm_freeFile */
    u8 unk3C[16];
} WorldMapObj;

/* world map context, GameData.unk8E4.world */
typedef struct WorldMap {
    u32 unk0; /* flags: 0x100 0x200 0x400 0x800 skip per-frame updates */
    u8 *unk4; /* end of the load area */
    WorldMapObj *unk8; /* = &unk48 */
    void *worldData; /* allocation, freed by wm_freeWorldData */
    s32 parkedSoundSize;
    u8 unk14[0x34];
    WorldMapObj unk48;
} WorldMap; /* 0x94 */

typedef struct {
    u8 pad0[0x11];
    u8 unk11;
} Unk800CE0E8;

/* doubly linked list node; g_worldEffectFree is the list head */
typedef struct WmNode {
    s32 unk0;
    struct WmNode *unk4;
    struct WmNode *unk8;
    s8 unkC;
    s8 unkD;
    s16 unkE;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s16 unk1C;
    s16 pad1E;
    s32 unk20;
    s32 unk24;
} WmNode; /* 0x28 */

/* frame pack slot list: g_worldReadtxHead (head) -> g_worldReadtxSlots[6] -> g_worldReadtxTail (tail) */
typedef struct WmReadtxSlot {
    struct WmReadtxSlot *unk0; /* prev */
    struct WmReadtxSlot *unk4; /* next */
    s32 *unk8; /* frame pack */
    s16 texId; /* low 6 bits: id; -1: none */
    u8 extracted;
    u8 padF;
} WmReadtxSlot; /* 0x10 */

typedef struct {
    s16 unk0;
    s16 viewBlend;
    s32 viewBlendSpeed;
    s32 viewForceNormal;
    u16 yaw; /* camera angle */
    u16 yawOffset;
} WmCameraState;

typedef struct {
    u8 unk0;
    u8 shadowWidth;
    u8 shadowDepth;
    u8 flying;
    u8 unk4;
    u8 polyCache;
    u8 shadowJoint;
    u8 unk7;
    u32 cellAttr;
    s16 rideOffset;
    s16 floorY;
} WmActorType; /* 0x10 */

typedef struct {
    s16 unk0[5];
} Unk800CE1E8; /* 10 bytes */

/* movement mode table entry (g_worldMoveModes[], current one at g_worldMoveModeInfo) */
typedef struct {
    u8 controlType; /* mode */
    u8 checkTransitions;
    s16 unk2; /* speed */
    u8 turnSpeed; /* turn speed */
    u8 climbSpeed;
    s8 maxRoll;
    u8 pad7;
    u8 airborne;
    u8 canToggleView;
    u8 unkA;
    u8 music;
    u8 loopSe;
    u8 countsSteps;
    s16 getoffDist;
    u8 cameraInFront;
    u8 cameraRow;
    u8 pad12[2];
    u32 unk14[2]; /* allowed cell types (bit mask) */
} WmMoveMode; /* 0x1C */

/* doubly linked node (wm_frmbufInit) */
typedef struct WmFrmbufSlot {
    struct WmFrmbufSlot *unk0; /* prev */
    struct WmFrmbufSlot *unk4; /* next */
    s16 texId;
    s8 vramArea;
    u8 transferred;
} WmFrmbufSlot; /* 0xC */

/* model object (same layout in ovl_0a, ovl_0b and ovl_0c) */

/* primitive sources of a model group; index fields select vertices (unk0..),
 * uv pairs (WmModelGroup.unk20) and colours (WmModelGroup.unk24) */
typedef struct {
    u16 unk0[4];
    u16 unk8[4]; /* uv */
    u8 unk10[3]; /* rgb */
    u8 unk13;    /* texture */
    u8 unk14;
    u8 unk15; /* flags: 1 = semi transparent, 0xE0 = mode */
    u8 pad16[2];
} WmSrcFT4; /* 0x18 */

typedef struct {
    u16 unk0[3];
    u8 unk6; /* texture */
    u8 pad7;
    u8 unk8[3]; /* rgb */
    u8 padB;
    u16 unkC[3]; /* uv */
    u8 unk12;    /* flags */
    u8 pad13;
} WmSrcFT3; /* 0x14 */

typedef struct {
    u16 unk0[4];
    u16 unk8[4];  /* uv */
    u16 unk10[4]; /* colours */
    u8 pad18[4];
    u8 unk1C; /* flags */
    u8 unk1D; /* texture */
    u8 pad1E[2];
} WmSrcGT4; /* 0x20 */

typedef struct {
    u16 unk0[3];
    u16 unk6[3]; /* uv */
    u8 padC[3];
    u8 unkF;      /* flags */
    u16 unk10[3]; /* colours */
    u8 unk16;     /* texture */
    u8 pad17;
} WmSrcGT3; /* 0x18 */

typedef struct {
    u16 unk0[4];
    u16 unk8[4]; /* colours */
    u8 pad10[4];
    u8 unk14; /* flags */
    u8 pad15[3];
} WmSrcG4; /* 0x18 */

typedef struct {
    u16 unk0[3];
    u8 pad6[2];
    u16 unk8[3]; /* colours */
    u8 padE[5];
    u8 unk13; /* flags */
} WmSrcG3; /* 0x14 */

typedef struct {
    u16 unk0[4];
    u8 unk8[3]; /* rgb */
    u8 padB[2];
    u8 unkD; /* flags */
    u8 padE[2];
} WmSrcF4; /* 0x10 */

typedef struct {
    u16 unk0[3];
    u8 pad6[2];
    u8 unk8[3]; /* rgb */
    u8 unkB; /* flags */
} WmSrcF3; /* 0xC */

typedef struct {
    u16 unk0;
    u16 unk2;  /* FT4 count */
    u16 unk4;  /* FT3 */
    u16 unk6;  /* GT4 */
    u16 unk8;  /* GT3 */
    u16 unkA;  /* G4 */
    u16 unkC;  /* G3 */
    u16 unkE;  /* F4 */
    u16 unk10; /* F3 */
    u8 pad12;
    s8 unk13; /* ordering table bias */
    void *unk14;
    void *unk18;
    u8 *unk1C;      /* primitive sources, in the order above */
    u8 (*unk20)[2]; /* uv */
    u8 (*unk24)[4]; /* colours */
} WmModelGroup; /* 0x28 */

typedef struct {
    u16 unk0; /* flags: 1 = relocated */
    u8 unk2;
    u8 unk3; /* group count */
    u8 pad4[0xC - 4];
    s32 unkC;
    WmModelGroup *unk10;
} WmModel;

typedef struct {
    s16 unk0;
    u8 pad2[2];
    void *unk4; /* first primitive */
    s32 unk8;
} WmModelOut; /* 0xC */

typedef struct {
    u16 unk0; /* tpage */
    u16 unk2; /* clut */
} WmTexInfo;

/* skeleton bone (WmModel.unkC, 4 bytes) */
typedef struct {
    s16 unk0; /* length: z offset from the parent */
    u8 unk2;
    u8 unk3; /* parent bone */
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
    s16 unk4[3]; /* root position: constant, or offset of s16[frame] */
    u16 unkA;    /* 1, 2, 4: unk4[i] is a constant */
    s32 unkC;    /* -> WmAnimKey[], rotation tracks, one per bone */
    s32 unk10;   /* -> WmAnimKey[], low rotation bits, two frames per byte */
} WmAnimHdr;

typedef struct ModelObj {
    u16 unk0; /* flags; 0x10: behind the camera */
    u8 pad2[2];
    u8 unk4; /* bone in the parent */
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
    s32 unk50;
    u16 scaleX; /* scale */
    u16 scaleY;
    u16 scaleZ;
    u16 clipY;
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x74 - 0x64];
    u16 unk74;
    u16 unk76;
    u16 unk78;
} ModelObj;

/* model slot (func_8001E350) */
typedef struct {
    s32 unk0; /* flags: 1 = loaded */
    s32 unk4; /* file id */
    ModelObj *unk8;
    Unk8004AB64 *unkC;
} WmObjSlot;
s32 func_8004AB64(Unk8004AB64 *);
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
    s32 unk4;
    s32 unk8;
} WmPackEntry;

typedef struct {
    s8 unk0; /* count */
    u8 pad1[3];
    s32 unk4;
    WmPackEntry *unk8[1];
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
    u16 unk0;
    u8 unk2;
    u8 unk3;
} WmCell;

typedef struct {
    s16 unk0; /* width */
    s16 unk2; /* height */
    WmCell unk4[1];
} WmCellTable;

/* filled by w_musicGetID */
typedef struct {
    s32 unk0; /* volume mask */
    s32 count; /* count */
    s32 seIds[3]; /* sound ids */
} WmSeIds;

typedef struct {
    s32 unk0[4];
} Unk800D3AB8; /* 0x10 */

/* list nodes (wm_blockListCompact, wm_chunkListCompact): g_worldBlockHead (head) ->
 * g_worldBlockNodes[40] -> g_worldBlockTail (tail) */
typedef struct Unk800D375C {
    struct Unk800D375C *unk0; /* prev */
    struct Unk800D375C *unk4; /* next */
    struct WmTexEnt *unk8;
    Unk800D3AB8 *unkC;
    s16 unk10;
    u8 unk12;
    s8 unk13;
} Unk800D375C; /* 0x14 */

/* list: g_worldChunkHead (head) -> g_worldChunkSlots[32] -> g_worldChunkTail (tail) */
typedef struct {
    u8 unk0; /* vertex indices */
    u8 unk1;
    u8 unk2;
    u8 pad3[0xE - 3];
    u16 unkE; /* attribute */
} WmPoly; /* 0x10 */

typedef struct WmTexEnt {
    u8 unk0;
    u8 unk1;
    u16 unk2; /* polygon count */
    SVECTOR *unk4; /* vertices */
    WmPoly *unk8;
    u8 unkC[0x14]; /* vertex indices */
    u8 unk20; /* vertex count */
    u8 unk21; /* sound id */
} WmTexEnt;

/* visible cells sorted by distance (wm_updateVisibleBlocks) */
typedef struct {
    s32 unk0; /* cell */
    WmTexEnt *unk4;
    s32 unk8; /* distance */
} WmNearCell;

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} WmScrRect;

typedef struct WmTexPack {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    WmTexEnt *unk8[4];
} WmTexPack;

typedef struct WmChunkSlot {
    struct WmChunkSlot *unk0; /* prev */
    struct WmChunkSlot *unk4; /* next */
    struct WmTexPack *unk8;
    s16 chunkId;
    u8 state;
    u8 generation;
    u8 unk10;
    u8 pad11[3];
} WmChunkSlot; /* 0x14 */

/* colour + 2 values (8 bytes) */
typedef struct {
    CVECTOR unk0;
    u16 unk4;
    u16 unk6;
} Unk800D1FD8;

typedef struct {
    u16 unk0;
    s16 unk2;
    s16 unk4[3]; /* rgb */
    s16 pad;
} Unk800D3FA0Sub; /* 0xC */

typedef union {
    SVECTOR v;
    u8 c[8];
} WmSkyCol;

/* sky / lighting parameter block (0x5E bytes); wm_blendWeather blends them */
typedef struct {
    SVECTOR unk0[3]; /* light directions */
    SVECTOR unk18;
    WmSkyCol unk20; /* back colour 1 */
    WmSkyCol unk28; /* back colour 2 */
    Unk800D3FA0Sub unk30[3];
    s16 unk54[3];
    u8 pad5A[2];
    u16 unk5C;
} WmSky; /* 0x5E */

typedef struct {
    WmSky unk0[16]; /* presets: [mode * 4 + flag * 2 + 0/1] */
    WmSky unk5E0[7]; /* [0] current, [1]/[2] blended from [3]..[6] */
} Unk800D3FA0;
void wm_blendWeather(WmSky *, WmSky *, WmSky *, s32, s32);
void wm_weatherAuto(void);
void func_800B9774(void);
void wm_updateColorMatrices(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s16 unk8; /* tpage */
    s16 unkA; /* clut */
} Unk800D2FF8; /* 0xC */

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4; /* x */
    s32 unk8; /* z */
} Unk800CD10C; /* 0xC */

/* file 0x12xxxx lookup table (wm_findDefaultAnim) */
typedef struct {
    u16 unk0;
    u16 unk2;
} Unk800AA39CEntry;

typedef struct {
    u8 unk0;
    u8 unk1; /* count */
    u8 pad2[2];
    Unk800AA39CEntry unk4[1];
} Unk800AA39C;

/* streamed load request (wm_streamSoundChunk) */
typedef struct {
    u8 pad0[8];
    s32 unk8; /* address */
    s32 unkC; /* offset */
    s32 unk10; /* size */
} Unk800AC424;

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
void wm_updateActorHeight(Unk8004AB64 *);
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
void func_800B69AC(void);
void w_memorymanDestructor(void);
void func_800BC0A0(void);
void func_800C4654(void);
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
void func_800B3A80(s32);
s32 func_800B3FEC(s32, s32, s32);
void wm_copyViaVram(s32, s32, s32);
void func_800BC098(void);
void wm_stopAllSe(void);
void wm_applyEncounterEvents(void);
void func_800B8724(void);
void wm_initClutAnims(void);
void wm_setMountPalette(void);
void wm_findVehicles(void);
void wm_weatherStart(void);
void func_800C464C(void);
void wm_main(s32);
void wm_init(void);
void wm_shutdown(void);
void wm_allocContext(void);
void wm_resetContext(void);
s32 wm_loadSoundBanks(void);
s32 wm_loadResidentMusic(void);
s32 wm_loadShadowTexture(void);
s32 wm_loadWorldData(void);
s32 func_800A919C(void);
s32 func_800A91A4(void);
s32 func_800A91AC(void);
s32 wm_freeWorldData(void);
s32 func_800A9220(void);
void wm_load();
void wm_exit(void);
u32 wm_loadActorModel(s32, Unk8004AB64 *);
void wm_clearOT(void);
void wm_endFrame(void);
void wm_startBattleTransition(void);
void wm_updateBattleTransition(void);
void wm_togglePause(void);
s32 func_800AD10C(void);
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
void func_800C2048(void);
void wm_frmbufInit(void);
void func_800C26E4(void);
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
extern Unk800D2FF8 *D_800D27F0;
extern u8 *s_menuMainEquipLabels[5];
extern char D_800A79D0[];
extern char D_800A7A0C[];
extern RECT *s_menuMainLocationRect;
extern s16 g_worldCameraBlendSpeed;
extern s16 g_worldCameraBlend;
extern s16 g_worldCameraBlendRow;
extern s8 g_worldCameraGroupOverride;
extern s32 D_800CCE20;
extern s32 D_800CCE24;
extern s32 D_800CCE28;
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */
extern s32 D_800CCE2C;
extern s32 D_800CCE30;
extern s32 D_800CCE34;
extern s32 D_800CCE38;
extern s32 D_800CCE3C;
extern u8 g_worldAreaCameraGroup[];
extern u8 g_worldAreaZone[];
extern u8 g_worldEncounterZoneStart[];
extern char D_800A7F48[];
extern Unk800CE1E8 g_worldCameraParams;
extern Unk800CE1E8 g_worldCameraParamsFrom;
extern u8 D_800CF46D;
extern s32 g_worldReadyFrameCount;
extern MATRIX D_800CF4C0;
extern MATRIX D_800CF520;
extern MATRIX D_800CF540;
extern u8 g_worldMapLoaded;
extern u8 s_menuMainText[]; /* text */
extern WmActorType g_worldActorTypeInfo[];
extern Unk8004AB64 *g_worldPlayer; /* player actor */
extern WmMoveMode *g_worldMoveModeInfo;
extern s32 g_worldWidth; /* map width (wraps) */
extern s32 g_worldDepth; /* map height (wraps) */
extern RECT D_800A71E0;
extern char D_800A720C[]; /* "w_frameSystemDestructor:\n" */
extern Unk800CD10C g_worldNaviPlaces[][64];
extern s32 *g_worldTexBuffers[3];
extern s32 D_800CE3F8;
extern u8 D_800CE400[];
extern u8 *D_800CF400;
extern s32 g_worldPrimBuffers[];
extern MATRIX D_800CF4E0;
extern MATRIX D_800CF500;
extern MATRIX D_800CF560;
extern u16 g_worldClutBuf[2][0x800]; /* palettes, [1] = shifted copy */
/* texture animation entries (wm_stepClutAnims) */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6; /* frame count */
    s16 unk8; /* speed */
} Unk800CFF18; /* 0xA */
extern Unk800CFF18 g_worldClutAnims[];
extern s16 D_800CFE48[];
extern u8 D_800CFE70[];
extern u16 D_800CFE20[];
extern Unk800D2FF8 D_800D2FF8[17];
extern s8 g_worldNaviPage;
extern char D_800A8098[];
extern s32 g_worldPrimPtr;
extern WmCellTable *g_worldChunkTable;
extern WmCellTable *g_worldChunkTableT;
extern u8 *g_worldBufIndex;
extern s32 g_worldOt[];
extern s32 g_worldEventResult;
extern s32 g_worldTimeEvents;
extern s32 g_worldTimeLogic; /* frame timing */
extern s32 D_8007BD28;
extern s32 D_8007BD30;
extern s32 g_worldTimeDraw;
extern s32 g_worldPrimUsed;
extern s32 g_worldPrimUsedPercent;
extern s32 g_worldTimeTotal;
extern Unk800D1FD8 D_800D1FD8;
extern Unk800D1FD8 D_800D1FE0[2];
extern s32 g_worldChunksH;
extern Unk800D375C g_worldBlockHead; /* list head */
extern s32 g_worldBlockListDirty;
extern Unk800D375C g_worldBlockNodes[40];
extern Unk800D3AB8 D_800D3AB8[20];
extern Unk800D375C g_worldBlockTail; /* list tail */
extern WmChunkSlot g_worldChunkTail;
extern WmChunkSlot g_worldChunkHead;
extern WmChunkSlot g_worldChunkSlots[32];
extern s32 g_worldChunkCenter;
extern s32 g_worldChunksLoadedCount;
extern s32 g_worldChunkListDirty;
extern Unk800D3FA0 g_worldWeatherData;
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
extern WmTexPack *g_worldDefaultBlocks;
extern s32 g_worldChunksW;
extern MATRIX D_800D3770;
extern s32 g_worldMapDebugFlags; /* debug flags */
extern s16 D_800CCD7C;
extern u8 g_worldPlayerStepped;
extern u16 g_worldEventBattleRate;
extern s32 g_worldReqParams[4]; /* script request parameters */
extern s32 D_800CF48C;
extern s32 D_800CF490;
/* texture transfer requests (wm_frmbufTransfer) */
typedef struct {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
} Unk800D3F78;
extern Unk800D3F78 D_800D3F78[];
extern s16 D_800CDF14[2];
extern s16 D_800CDF18[2];
extern s16 D_800CDF1C[2];
extern s16 D_800CDF20[2];
extern char D_800A8118[];
extern Unk800CE0E8 *g_worldCameraFlags;
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
extern WmNode g_worldEffectFree;
extern WmNode g_worldEffectActive;
extern WmNode g_worldEffectNodes[60]; /* free list nodes */
extern WmNode g_worldEffectFreeTail; /* list tail */
extern WmNode g_worldEffectActiveTail; /* list tail */
extern s32 g_worldEffectSpeed;
extern s32 D_800D53CC;
extern s32 D_800D53E0;
extern s32 g_worldEffectSpeedTarget;
extern VECTOR D_800CDF50;
extern SVECTOR D_800D4870[10];
extern s16 D_800D4830[10];
extern s16 D_800D48F8[10];
extern s8 D_800D48E8[10];
extern SVECTOR D_800D4910[10];
extern VECTOR D_800D4960[9];
extern SVECTOR D_800D49F0[9];
extern SVECTOR D_800CDF48;
extern SVECTOR D_800CDF80;
extern SVECTOR D_800CDF88;
extern SVECTOR D_800CDF90;
extern SVECTOR D_800CDFA0[3];
extern void **g_worldDataPack;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Unk800B8388;
extern MATRIX *D_800CF448;
extern s32 D_800CE19C;
void func_800BA3F4(s32, s32);
extern u8 D_800CCA1C;
extern RECT D_800CE0B8[];
extern u8 g_worldAreaZone[];
extern u8 g_worldCameraSmoothX;
extern u8 g_worldCameraSmoothY;
extern u8 g_worldCameraSmoothZ;
extern s32 g_worldCameraTarget[];
extern s32 g_worldCameraEye[];
extern s32 g_worldCameraDir[];
extern s32 D_800D3730[];
extern s32 g_worldCameraRecenter;
extern s32 g_worldChunkBuffer;
extern u8 D_800D30C4;
extern s32 D_800D32FC;
extern u8 D_800D30C5;
extern u8 g_worldUsePolyCache;
extern u8 D_800D3338;
extern u8 g_worldChunkFlags[]; /* map cells, g_worldChunksW per row */
#define WM_CELL(x, y) g_worldChunkFlags[(y) * g_worldChunksW + (x)]
extern u16 D_800CD060[11]; /* story progress thresholds (wm_applyStoryForms) */
extern u8 D_800CD010[][11];
extern u8 D_8007AF55; /* = g_evtGlobalVars.b[0x65], general global flags */
extern u32 D_800CCB18[2]; /* cell type masks */
extern u32 g_worldWaterCellTypes[2];
u8 wm_testCellType(u32 *, u32);
extern s32 D_800CD098;
s32 wm_chunkRequest(s16);
extern s16 D_800D45D4[3];
void func_800B3998(u16 *, u16 *, s32, s16 *);
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
extern POLY_G3 *D_800D3E7C;
extern POLY_FT4 *D_800D3E68;
extern s8 g_worldTitleId;
extern s32 D_800CCE40;
extern u8 g_worldTitleFade;
extern u8 g_worldNaviCursorDraw;
extern u8 g_worldNaviCursorOn;
extern u8 D_800D3E79;
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
    u16 unk0; /* x */
    u16 unk2; /* y */
    u16 unk4; /* w */
    u16 unk6; /* h */
    u16 unk8; /* speed (8.8) */
} Unk800CFEA0; /* 0xA */

/* random-encounter table entry (g_worldEncounterTable, docs/encounters.md) */
typedef struct {
    u16 battle[4];
    u8 topoPattern; /* topography << 2 | slot pattern */
    u8 zoneFog; /* zone << 1 | fog */
} WmEncounter; /* 0xA */

extern u16 D_800CFE88[8];
extern Unk800CFEA0 g_worldTexScrolls[8];
extern u16 D_800CFEF8;
extern u16 D_800CFF02;
extern void *D_800CE3F4;
extern s8 D_800CFE98[2];
extern s8 D_800CCFAC[];
extern WmEncounter *g_worldEncounterTable;
extern u16 (*g_worldEncounterEventRows)[12];
extern s32 g_worldAmbientSeDist[3];
extern u8 g_worldAmbientSeOn[3];
extern s32 g_worldAmbientSeX[3]; /* sound source x */
extern s32 g_worldAmbientSeZ[3]; /* sound source z */
extern u8 g_worldAmbientSe[3]; /* sound ids */
extern u8 D_800D30CD;
extern u8 g_worldFormFlags[]; /* flags (wm_applyStoryForms); [5] is D_800D30CD */
extern s32 D_800D3300;
extern u8 D_800D5410;
extern u8 D_800D1FD0;
extern u8 D_800D30D2;
extern u8 D_800D30CB;
extern u8 g_worldCdUser;
void wm_stepClutAnims(s32, s32, s32);
void wm_initTextureLists(void);
void wm_blockListInit(void);
void menuMain_reorder(s32, s32);
void wm_musicPlay(u16);
void wm_musicStop(u16);

/* image pack loaded by wm_loadImagePack; offsets are from the header start */
typedef struct {
    s32 unk0; /* offset: Unk800CB09CClut */
    s32 unk4; /* offset: images (RECT + pixels) */
    s32 unk8; /* image count */
} Unk800CB09C;

typedef struct {
    s32 unk0; /* offset: pixel data */
    s32 unk4; /* count */
    RECT unk8[1];
} Unk800CB09CClut;

/* text window contents passed to func_80063118 (as in ovl_38) */
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
extern u8 D_800CE06C[];
extern u8 D_800CE070[];
extern u8 D_800CE078[];
extern u8 D_800CE080[];
extern u8 D_800CE088[];
extern u8 D_800CE090[];

/* wm_initMovement init state */
typedef struct {
    u8 unk0; /* last written slot */
    u8 pad1[3];
    u16 unk4[6]; /* polygon */
    u16 unk10[6]; /* cell */
} Unk800CE288; /* 0x1C: recently hit walkmesh polygons */
s32 wm_getGroundHeight(VECTOR *, u32 *, s32 *, Unk800CE288 *, u16 *);
u8 wm_tryStep(VECTOR *, s32, s32, s32, u32 *, s32 *);
u8 wm_findGetoffPos(ActorPos *);
extern Unk800CE288 g_worldPolyCache[11];
extern Unk8004AB64 g_worldDummyActor;
extern u8 D_800CE209;
extern u8 D_800CE25D;
extern u8 g_worldPlayerPosChanged;
extern s32 D_800CE260;
extern Unk8004AB64 *g_worldMountActor;
extern u8 D_800CE26D;
extern Unk8004AB64 *D_800CE270;
extern u8 D_800CE274;
extern s32 g_worldPlayerClimbSpeed;
extern s32 g_worldMountPaletteOverride;
extern s32 D_800CE284;
extern u8 g_worldCameraTurnedByHand;
extern s32 g_worldPlayerSpeed;
extern u16 g_worldPlayerHeading;
extern Unk8004AB64 *g_worldVehicleActor;
extern s32 g_worldCameraTurnGain;
extern Unk8004AB64 *D_800CE3D8;
extern s32 g_worldPlayerTurn;

extern u8 D_800CCE49;
extern s32 g_worldDebugStep; /* debug: last update step */
extern s32 g_worldDrawActors;
extern u8 D_800CF44C;
extern u8 g_worldStartup;
extern TILE g_worldPauseTile;
extern DR_TPAGE g_worldPauseTpage;
extern u32 g_worldPauseOT;
extern u8 D_800CE098[];
void func_80023F60(s32);
void func_8005562C(s32);
void func_80055698(s32);
extern s32 g_worldFrameCount;
extern u8 g_worldMogChoice;
extern s16 D_800CF442;
extern u16 D_800D45DC;
extern u16 D_800D4754;
extern u8 D_800CF47E;
extern u8 D_800CF434;
extern u8 g_worldMapJump;
extern u8 D_800CF47C;
void func_80050E34(u8 *);
void func_80050EFC(s32, s32);
extern s32 D_800CF468;
extern s32 g_worldTimeMapDraw;
void wm_drawActors(void);
void wm_drawMap(void);
void wm_naviDraw(void);
void wm_drawEffects(void);
s32 wm_getCellBlock(s16, u8 *);
s32 wm_getBlockHeight(s32, WmTexEnt *, VECTOR *, u32 *, s32 *, Unk800CE288 *);
void wm_wrapPos(s32 *, s32 *);
void wm_posToCell(s32, s32, s32 *);
void wm_cellToPos(s32, s32 *, s32 *);
void func_800C2040(void);
void wm_readtxDone(s32 *);
void wm_relocatePack(WmPack *);
extern u8 g_worldCameraHold;
extern u8 g_worldCameraLookupFix;
extern s32 D_800CD094;
extern s32 D_800D3320[];
extern WmChunkSlot *D_800D33F8[];
extern WmPack *D_800D3740[];
extern s32 *D_800D3310[];
extern s32 D_800CF580[5];
extern s32 g_worldImageSectors[5]; /* CD sectors */
extern s32 D_800CCF84[5];
extern s32 D_800CCF98[5];
extern char D_800A7BB8[];
/* sprite table (func_800C1824) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* y */
    u8 unk4; /* u */
    u8 pad5;
    u8 unk6; /* v */
    u8 pad7;
    s16 unk8; /* w */
    s16 unkA; /* h */
} Unk800CD744; /* 0xC */
extern Unk800CD744 D_800CD744[4];
void func_800C1824();
extern s32 g_worldWeatherPointDist;
void func_800C26DC(void);

extern Unk800CE1E8 g_worldCameraPresets[];
extern u8 g_worldCameraPresetIndex[][3][3];
extern VECTOR g_worldCameraUp;
extern SVECTOR D_800CE0F8; /* camera up vector */
extern SVECTOR D_800CE190;
extern SVECTOR g_worldCameraEyeOffset;
extern s32 g_worldCameraTargetHeight;
extern s32 g_worldCameraDistScale;
extern s16 g_worldCameraRoll;
extern u8 D_800CE1E1;
extern u8 D_800CE1F3;

/* main exe: candidates for ff9.h */
MATRIX *func_80030EA4(SVECTOR *, MATRIX *);
void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
void func_80064CDC(s32, s32);
void func_80031264(void *, void *); /* rotation (SVECTOR) -> bone matrix */
void func_800316D8(SVECTOR *, MATRIX *); /* RotMatrix */
void func_80030AF4(SVECTOR *, MATRIX *);
s32 math_cos(s32); /* cos */
s32 math_sin(s32); /* sin */
void SetGeomScreen(u16);
s32 func_80050D38(s32);
extern u32 D_80076F80[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *D_80077DF0;
s32 func_8002B980(u8 *, s32);
extern s32 D_80077E54; /* overlaps Unk80077E50.unk4 in ff9.h */
Unk8004AB64 *func_8004AE00(void);
void func_8004B20C(s32);

/* run on the scratchpad stack (top of the 1 KB scratchpad) */
#define SetSpadStack(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -4\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack() __asm__ volatile("addiu $29, $29, 4\n\tlw $29, 0($29)" : : : "memory")

/* world effect emitter table entry (wm_updateBlockFx) */
typedef struct {
    s32 unk0; /* position */
    s32 unk4;
    s32 unk8;
    s32 unkC; /* random range */
    s32 unk10;
    s32 unk14;
    u8 pad18[0x24 - 0x18];
    s16 unk24; /* effect, -1 = end */
    s16 unk26;
    s16 unk28; /* chance (of 4096) */
    u8 pad2A[0x40 - 0x2A];
} WmEmitter; /* 0x40 */

/* map-marker colour (wm_naviDrawPlaces) */
extern s32 g_worldNaviCursorX; /* navi cursor */
extern s32 D_800CD70C; /* marker snap distance */
extern s32 g_worldNaviCursorY;
typedef struct { SVECTOR v[4]; } Unk800A7FB8; /* shadow quad template */
extern Unk800A7FB8 D_800A7FB8;
extern Unk800A7FB8 D_800A8020; /* radar arrow */
extern Unk800A7FB8 D_800A8000; /* big-map arrow */
extern WmScrRect D_800CD09C[]; /* on-screen test rects */
extern char D_800A8178[]; /* "Illegal Weather Mode" */
extern VECTOR D_800CDF38;
extern VECTOR D_800CDF60;
extern SVECTOR D_800CDF98;
extern s32 D_800CDFB8;
extern s32 D_800CDFBC;
extern MATRIX D_800CD774;
extern VECTOR D_800A8588;
extern VECTOR D_800A8598;

/* first entry of data pack `id` */
#define WM_PACK0(id) ((s32)((WmPack *)w_framePackGetPtr(g_worldDataPack, id))->unk8[0])

/* inline CompMatrix: c = a * b, c->t = a * b->t + a->t */
#define gte_CompMatrix(a, b, c)    \
    gte_MulMatrix0(a, b, c);    \
    gte_SetTransMatrix(a);      \
    gte_ldlv0((b)->t);          \
    gte_rtv0tr();               \
    gte_stir((c)->t);

extern WmNearCell D_800D3C10[];
s32 wm_getCameraViewState();
void wm_frmbufCompact(void);
void wm_frmbufTransfer(void);
void wm_blockListCompact(void);
void wm_blockListResolve(void);
s32 wm_blockListRequest(s16);
s32 w_frmbufRequest(s16);
void wm_updateBlockFx(s32);
extern u8 D_800CD728; /* big-map cone colour */
extern u8 D_800CD72C;
extern u8 D_800CD730;
extern u8 D_800CD734; /* big-map tile colour */
extern u8 D_800CD738;
extern u8 D_800CD73C;
extern RECT D_800A71D8;
extern char D_800A7F48[]; /* encounter fallback message */ /* palette area (wm_systemInit) */
extern CVECTOR D_800A7FF0; /* marker colours */
extern CVECTOR D_800A7FF4;
extern CVECTOR D_800A7FF8;
extern CVECTOR D_800A7FFC;
extern u8 D_800CD710;
extern u8 D_800CD714;
extern u8 D_800CD718;
void wm_naviDrawMarker(s32, s32, s32, s32, CVECTOR, CVECTOR, s32, s32, s32, s32);
void wm_naviWorldToMap(s32, s32, s32 *, s32 *, u8);
void wm_naviUpdate(void);
s32 wm_naviIsPlaceKnown(u32);

static inline CVECTOR wm_naviDrawPlaces_color(void) {
    CVECTOR c;

    memset(&c, 0, 4);
    c.r = D_800CD710;
    c.g = D_800CD714;
    c.b = D_800CD718;
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
    return g_worldState == 2 ? g_padPortHeld[0] : 0;
}

static inline u32 wm_debugAdjustScreen_trg(void) {
    if (g_worldState != 2) {
        return 0;
    }
    return g_padPortPressed;
}

void wm_freeEffect(WmNode *);
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
    u16 unk0; /* frame count (low 15 bits) */
    u16 unk2;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u16 unk8[1]; /* per-frame data offsets, then a counted u16 table */
} WmSprHdr; /* sprite animation header (wm_drawSprite3D) */

typedef struct {
    u16 *unk0;
    u16 *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} WmSprCtx; /* draw context passed to func_800CB1EC */

s32 func_800CB1EC(u8 *, WmSprCtx *, s32, s32);
s32 wm_drawSprite3D(s32, WmSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    s32 unk0; /* size */
    void *unk4; /* destination */
} WmLoadReq; /* CD read request (wm_readChunks, cd_readChunks) */

extern WmLoadReq D_800D3410[];
extern WmLoadReq D_800D3370[];
extern s32 D_800D3490[];
extern s32 D_800D3728;
extern s32 D_800CF5A4;
extern s32 D_800CF5A8;
extern char D_800A7F94[];
extern SVECTOR D_800A849C; /* rotation used by wm_drawEffects */
s32 wm_readtxFind(s32, s32);
u8 wm_findChunkRun(WmChunkSlot **, s32 *, s32 *);
void wm_getChunkEntry(s32, s32, WmCell *);
void wm_getChunkEntryT(s32, s32, WmCell *);
void wm_chunksLoaded(void);

typedef struct {
    u8 b[0x1E0];
} WmCellMap; /* g_worldChunkFlags as one block */
typedef struct {
    Unk800CFEA0 e[30];
} Unk800CFEA0Tbl; /* g_worldTexScrolls and the entries after it */
extern u8 g_worldZoneEncounterCount[];
extern s32 g_worldCellsH;
void wm_applyStoryForms(void);
void wm_chunkListInit(void);
void wm_initBlockMatrix(void);
void wm_loadImagePack(Unk800CB09C *);

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
    if (-D_800CF442 < k && k < 0x1000 - D_800CF442) { \
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][k], e->unk8[g_gameState->unk1C->unk8][q]); \
    }

/* main exe: candidates for ff9.h */
void cd_readChunks(s32, void *, s32, s32);
s32 func_8004B1EC(void);
s32 func_8004B1FC(void);
void func_8002FA10(void);
void OuterProduct0(VECTOR *v0, VECTOR *v1, VECTOR *v2);

/* wm_getBlockHeight (walkmesh height lookup) */
extern s16 D_800D27F8[];
extern u16 D_800D2DF8[];
extern char D_800A7D20[];

/* wm_movePlayer (walkmesh movement) */
void wm_controlPlayer(void);
extern u8 D_800CCE4A;
extern u8 D_800CCE55;
extern u8 D_800CCF6C;
extern u8 D_800CE208;

/* wm_cameraFollow (camera follow) */
extern u8 D_800CE1E0;
extern Unk800CE288 g_worldCameraPolyCache[4];
extern s32 D_800CCAFC;
extern s32 D_800CCB04;
extern s32 D_800CCB08;
extern s32 D_800CCB0C;
extern s32 D_800CCB10;
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
void wm_streamSoundChunk(Unk800AC424 *arg0);
s32 func_800B8108(void);

/* main exe: candidates for ff9.h (sound) */
s32 func_80055290(void *);
void func_800552B8(s32);
void func_80055310(void);
void func_800553EC(s32, s32, s32, s32);
void func_80055604(s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800558DC(s32);
void func_80055908(s32, s32);
void func_8005593C(s32, s32, s32);
void func_80055980(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);
s32 func_80055DCC(void);

s32 wm_initShadowPrims(s32, s32); /* returns func_80062E3C's result (the next free address) */

#endif
