#ifndef OVL_0A_H
#define OVL_0A_H

/* Declarations local to overlay 0x0A (field, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* field context, GameData.unk8E4.field */
typedef struct {
    u8 unk0; /* flags */
    u8 pad1[7];
    s32 unk8;
    u8 padC[2];
    s16 unkE;
    u8 pad10[4];
} Unk800AB0B0Entry; /* 0x14 */

typedef struct Unk800AB0B0 {
    u16 unk0; /* count */
    u8 pad2[2];
    Unk800AB0B0Entry unk4[1];
} Unk800AB0B0;

/* 0x24-byte entries at FieldSpriteSet.unk8 (fld_spriteCommand sets them) */
typedef struct {
    u32 unk0_0 : 4; /* flags */
    u32 unk0_4 : 4;
    s32 unk1 : 8;
    s32 parent : 8;
    u32 bone : 8;
    s16 unk4;
    u8 speed;
    u8 unk7;
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
    u8 unk2;
    u8 unk3; /* count of 0x24-byte entries at unk8 */
    u16 *unk4; /* 16 entries, follows this header */
    FieldSprite *unk8;
    u8 *unkC[2]; /* primitive buffers, per draw buffer */
} FieldSpriteSet; /* 0x14, followed by the buffers it points to */

/* trigger table entry (fld_removeAnimSfx) */
typedef struct FieldAnimSfx {
    u8 unk0; /* object uid; low 2 bits also select a channel */
    u8 randomPitch;
    u16 unk2;
    u16 anim;
    s16 frame;
    u16 sound[2];
} FieldAnimSfx; /* 0xC */

/* script object (fld_removeAnimSfx) */
typedef struct {
    u8 pad0[0x14];
    struct {
        s32 vx, vy, vz;
    } unk14; /* position */
    s16 unk20; /* rotation */
    s16 unk22;
    s16 unk24;
    u8 pad26[0x34 - 0x26];
    u16 unk34;
    u16 unk36;
    u8 pad38[2];
    u8 unk3A;
    u8 unk3B; /* uid */
    u8 pad3C[0x4C - 0x3C];
} FieldEvtObj; /* 0x4C */

typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* light id */
    void *unk8; /* model (ModelObj) */
    FieldEvtObj *unkC;
} FieldEvtCtx;

/* 0x330-byte block, FieldMapObj.unk14 (allocated by fld_allocScene) */
typedef struct {
    u32 unk0; /* flags */
    s32 unk4; /* end of the load area */
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
    s32 *modelBundles; /* = unk6C, modelBundleCount entries (fld_loadModelBundle) */
    struct FieldObjPlace *shadows; /* shadow entries (fld_initShadows, fld_drawShadow) */
    s16 otBias; /* added to projected depths before indexing the OT (camera unk30, fldbg_setCamera) */
    u8 pad32[0x34 - 0x32];
    u8 *movieWork;
    s16 encounterScreenX;
    s16 encounterScreenY;
    u16 battleMusic;
    u16 unk3E;
    u8 modelBundleCount;
    s8 encounterStep;
    s8 movieState;
    u8 movieRgb24;
    s32 movieFile;
    u8 movieList;
    u8 movieEntry;
    u8 exitReason; /* FLD_EXIT_*, set by fld_main, dispatched by fld_exit */
    u8 animSfxCount; /* animSfx entry count */
    void *unk4C; /* file 0x1Exxxx */
    s16 shadowTexX;
    s16 shadowTexY;
    struct FieldActorInfo *actorInfo; /* = unk90 */
    struct FieldAnimSfx *animSfx; /* = unk270 */
    u8 unk5C;
    u8 unk5D;
    u8 soundBankSlot;
    u8 unk5F;
    u8 modelQueueCount; /* g_fieldModelQueue count */
    u8 unk61[1];
    s16 unk62;
    s32 unk64;
    u8 unk68[4];
    s32 unk6C[9];
    u8 unk90[0x270 - 0x90];
    u8 unk270[0x330 - 0x270];
} FieldScene;

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
    s32 unk4;    /* end of the load area */
    void *unk8;  /* file 0x6xxxx */
    void *unkC;
    u16 unk10;
    u8 unk12[2];
    FieldScene *unk14;
    u8 unk18[0x20];
} FieldMapObj;

/* field context, GameData.unk8E4.field (0x50 bytes) */
typedef struct FieldMap {
    u32 unk0; /* flags */
    s32 unk4;
    s8 unk8;
    s8 unk9;
    s16 unkA;
    FieldMapObj *unkC;
    void *unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x20 - 0x1C];
    s32 unk20;
    u8 unk24[0x2C];
} FieldMap;

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
    Unk8004AB64 *actor;
} BgiActor;

/* 0xC-byte entries at g_fieldBgCtxEntries */
typedef struct {
    u8 unk0;
    u8 pad1[0xB];
} Unk800C9DCC;

typedef struct {
    u8 mode;
    u8 discMask;
    u8 disc;
    u8 cursor;
    s16 arg;
    s16 unk6;
    s32 savedFlags[2]; /* saved g_padPortMask */
} FieldDiscChange;

typedef struct {
    void (*enter)(void); /* enter */
    void (*update)(u8);   /* update */
    void (*leave)(void); /* leave */
} FieldDiscMode; /* 0xC */

typedef struct {
    u16 unk0;
    u16 unk2;
} __attribute__((aligned(4))) BgPoint;

/* field state (g_fieldBgState) */
typedef struct {
    s32 flags; /* flags */
    BgPoint scroll;
    BgPoint unk8;
    s16 unkC; /* geometry offset adjust */
    s16 unkE;
    BgPoint scrollFrom;
    s16 scrollToX;
    s16 scrollToY;
    s16 scrollFrame;
    s16 scrollFrames;
    MATRIX *viewMatrix;
    u16 *projDist;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    s16 unk2C; /* projected origin */
    s16 unk2E;
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
    s16 unkA4;
    s16 unkA6;
    s16 viewportPos[4][2];
    s16 viewportSize[4][2];
    s16 viewportRateX[4];
    s16 viewportRateY[4];
} FieldBgState;

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
    u16 unk1C;
    u16 baseDepth;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u8 pad28[2];
    u16 unk2A;
    u8 pad2C[2];
    u16 unk2E;
    u16 unk30; /* screen origin x */
    u16 unk32; /* screen origin y */
} BgFile;

typedef struct {
    u8 unk0;
    s8 unk1;
} BgAnimStep;

typedef struct {
    unsigned unk0 : 8; /* flags */
    unsigned unk1 : 24; /* entries at unkC */
    unsigned unk4 : 8;
    signed unk5 : 24; /* position (8.8 step index) */
    s16 unk8;
    s16 unkA;
    s32 unkC; /* offset: BgAnimStep[] */
} BgOverlayAnim; /* 0x10 */

/* background sprite: DR_TPAGE followed by a SPRT_16 (0x18 bytes) */
typedef struct {
    DR_TPAGE unk0;
    SPRT_16 unk8;
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
    unsigned unk0 : 8; /* flags */
    unsigned unk1 : 12; /* depth */
    unsigned unk2 : 12; /* depth */
    u16 unk4; /* scroll wrap x */
    u16 unk6; /* scroll wrap y */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u16 unk18;
    u16 unk1A;
    s16 scrollX;
    s16 scrollY;
    s16 unk20;
    s16 unk22;
    unsigned unk24 : 8;
    unsigned unk25 : 1;
    unsigned unk25_1 : 7;
    u16 unk26;  /* tile count */
    s32 unk28;  /* offset: BgPos[] */
    s32 unk2C;  /* offset: BgTile[] */
    BgSprt *sprites;
    u8 pad34[4];
} BgOverlay; /* 0x38 */

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

typedef struct {
    u32 unk0;
    u8 pad4[0xC];
} BgOverlayAnimTbl; /* 0x10 */

/* the same 0x10-byte table, as animation state */
typedef struct {
    u8 unk0;
    u8 pad1[3];
    u32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC; /* offset: u8 track index */
} BgOverlayAnimView;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
} BgEntry; /* 0xC */

typedef struct {
    u8 *bufIndex; /* -> draw buffer index */
    s32 projDist;
    u32 *ot[2]; /* ordering tables, per draw buffer */
    BgFile *data;
    s32 viewMatrix;
    u8 *heap;
    u8 *overlayRgb; /* 3 bytes per entry */
    u8 *unk20;
    u8 entryCount;
    u8 pad25[3];
    BgEntry *entries;
    s32 work;
    s32 workSize;
    s32 workUsed;
} FieldBgCtx;

/* field light (12 bytes) */
typedef struct {
    s8 unk0; /* type: 1, 2 */
    u8 pad1;
    s8 unk2; /* sprite colour offsets */
    s8 unk3;
    u32 unk4; /* colour */
    s32 unk8; /* 4 floor ids (type 0) */
} FieldLight;

/* 8-byte entries following Unk800B3254.unkC[unk6] */
typedef struct {
    u16 unk0; /* id */
    s8 unk2;
    s8 unk3;
    u32 unk4; /* colour */
} Unk800B3294;

typedef struct {
    u8 pad0[6];
    u8 unk6; /* count */
    u8 unk7; /* count */
    u8 pad8;
    u8 unk9;
    u8 padA[2];
    FieldLight unkC[1];
} Unk800B3254;

/* 6-byte vertex entries at g_fieldBgi.unkC */
typedef struct BgiVertex {
    s16 vx;
    s16 vy;
    s16 vz;
} BgiVertex;

/* the same vertex viewed as an array (bgi_pointInTriangle) */
typedef struct {
    s16 unk0[3];
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

/* file 0x1C0000 header */
typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u16 unk32;
} Unk800AA01C;

/* deferred flag updates: [0] = set, [1] = clear, 4-frame queues for
 * GameData.unk0, FieldMap.unk0 and g_fieldMovieFlags */
typedef struct {
    u16 unk0[2][4];
    u16 unk10[2][4];
    u16 unk20[2][4];
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
    u16 unk0;
    u16 unk2;          /* file 0x3xxxx */
    unsigned unk4 : 24; /* offset from the entry: 3 tables of unk7 words */
    unsigned unk7 : 8;
    u16 unk8;          /* file 0x19xxxx */
    u8 padA[2];
} Unk800AA39CEntry; /* 0xC */

typedef struct {
    u8 unk0;
    u8 unk1; /* count */
    u8 pad2[2];
    Unk800AA39CEntry unk4[1];
} Unk800AA39C;

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
    u8 unk2;  /* bone count */
    u8 unk3; /* group count */
    u8 pad4[2];
    u8 unk6; /* look-at bone (fldmdl_calcBoneMatricesLookAt) */
    u8 pad7[0xC - 7];
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

/* Unk8004AB64 position (unk14) seen as an array, indexed by axis */
typedef struct {
    u8 pad0[0x14];
    s32 unk14[3];
} ActorPosView;

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
    s16 unk4[3]; /* root position: constant, or offset of s16[frame] */
    u16 unkA;    /* 1, 2, 4: unk4[i] is a constant */
    WmAnimKey *unkC;  /* rotation tracks, one per bone */
    WmAnimKey *unk10; /* low rotation bits, packed two frames per byte */
} WmAnim;

typedef struct ModelObj {
    u16 unk0; /* flags; 0x10: behind the camera */
    u8 pad2[2];
    u8 unk4; /* bone in the parent */
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
    s16 unk74; /* offset from the parent bone */
    s16 unk76;
    s16 unk78;
} ModelObj;

typedef union {
    u32 w;
    u8 c[4];
} FieldColor;

/* per-actor extension (FieldActorInfo.unk10) */
typedef struct FieldActorExt {
    u32 unk0; /* flags */
    s32 unk4; /* model file id */
    ModelObj *unk8; /* = unk14 */
    FieldEvtObj *unkC; /* = unkAC */
    s32 unk10;
    u8 unk14[0xAC - 0x14]; /* mirrored model */
    u8 unkAC[0xF8 - 0xAC]; /* copy of the object */
    FieldEvtCtx *unkF8;
    SVECTOR4 unkFC; /* mirror plane point */
    SVECTOR4 unk104; /* mirror plane normal */
    FieldColor unk10C; /* rgb, [3]: frames left */
} FieldActorExt; /* 0x110 */

/* 0x14-byte per-actor record at FieldScene.unk54, indexed by uid */
/* rgb + flag byte (bit 24 of the word: pending) */

typedef struct FieldActorInfo {
    u32 unk0; /* flags */
    s8 unk4; /* light index, -1 = none */
    u8 unk5;
    u8 unk6;
    u8 unk7;
    FieldColor unk8; /* colour */
    FieldColor unkC; /* next colour */
    FieldActorExt *unk10;
} FieldActorInfo;

/* background VRAM animation (fldmdl_updateTexAnims) */
typedef struct {
    RECT unk0;      /* destination area */
    DVECTOR *unk8;  /* per-frame source position */
} TexAnimRect; /* 0xC */

typedef struct {
    u8 unk0; /* flags: 1 active, 2 one-shot, 4 scroll */
    u8 unk1; /* frame count */
    s16 unk2; /* speed (20.12) */
    u16 unk4; /* random delay base */
    u16 unk6; /* random delay range */
    s32 unk8; /* position (20.12) */
    u8 unkC; /* rect count */
    u8 padD;
    u16 unkE; /* last frame */
    TexAnimRect *unk10;
} TexAnim; /* 0x14 */

typedef struct {
    u16 unk0; /* count */
    u8 pad2[2];
    TexAnim unk4[1];
} TexAnimSet;

/* screen fade quads (g_fieldBgFollowOverlays, fldbg_addFollowOverlay) */
typedef struct {
    s8 unk0; /* flags */
    u8 unk1; /* rgb */
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 padA[2];
    POLY_F4 unkC[2][4]; /* per draw buffer */
    DR_TPAGE unkCC[2];
} BgFollowOverlay; /* 0xDC */

extern BgFollowOverlay g_fieldBgFollowOverlays[];
extern u16 g_fieldBgFollowOverlayCount; /* BgFollowOverlay count */

/* palette fade packet (fldbg_startClutFade), in the g_fieldBgCtx.unk2C buffer */
typedef struct {
    u8 unk0;
    u8 unk1;  /* colour count */
    u8 unk2;  /* first */
    u8 unk3;  /* last */
    s16 unk4; /* mode */
    u8 pad6[2];
    u32 unk8; /* frames */
    s32 unkC;
    s32 unk10; /* r step (16.16) */
    s32 unk14;
    s32 unk18;
    s32 unk1C; /* offset: saved colours */
    s32 unk20; /* offset: work colours */
    RECT unk24;
} FieldPalAnim;

/* pause screen (fld_togglePause) */
typedef struct {
    SPRT unk0;
    DR_TPAGE unk14;
    u8 pad1C[0x40 - 0x1C];
} FieldPausePkt;
extern DR_TPAGE g_fieldPauseTPage;
extern u32 g_fieldTempOt;
extern FieldPausePkt g_fieldPauseSprite;
extern TILE g_fieldPauseDim;

/* 3D sprite data (FieldSprite.unk18) */
typedef struct {
    u16 unk0; /* frame count | 0x8000 */
    u16 unk2; /* tpage */
    u16 unk4; /* clut */
    u8 unk6;  /* width */
    u8 unk7;  /* height */
    u16 unk8[1]; /* frame offsets, then the u16-counted table */
} FieldSprHdr;

/* parameter block for fld_emitSprite3D */
typedef struct {
    u16 *unk0;
    u8 *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} FieldSprArgs;

u8 *fld_drawSprite3D(u8 *, FieldSprHdr *, SVECTOR *, SVECTOR *, s32, s32, s32, s32, u32 *);
u8 *fld_emitSprite3D(u8 *, FieldSprArgs *, u8 *, u32 *);
void math_rotMatrixYXZ(SVECTOR *, MATRIX *);

/* object placement (fld_calcShadowMatrix) */
typedef struct {
    unsigned unk0 : 8;
    unsigned unk1 : 12; /* scale x */
    unsigned unk2 : 12; /* scale z */
    unsigned unk4 : 12; /* angle */
    signed unk4_12 : 10; /* offset x */
    signed unk4_22 : 10; /* offset z */
} FieldObjEnt;

/* object placement entries (FieldScene.unk2C) */
typedef struct FieldObjPlace {
    FieldObjEnt unk0;
    void *unk8[2][4]; /* shadow primitives, per draw buffer */
} FieldObjPlace; /* 0x28 */

/* Unk800B3294 seen bytewise: the top byte of the colour is a depth bias (fld_drawShadow) */
typedef struct {
    u8 pad0[7];
    s8 unk7;
} Unk800B3294Bias;

/* texture set tables (fld_loadModelBundle) */
typedef struct {
    s16 unk0;
    s16 unk2;
} FieldTexPair; /* tpage/clut or u/v */

typedef struct {
    s16 unk0; /* flags: bit 0 = 8-bit */
    s16 unk2;
    s16 unk4; /* vram x */
    s16 unk6; /* vram y */
} FieldTexEnt;

/* VRAM allocation cursor (file 0x1C0000) */
typedef struct {
    s16 unk0; /* 8-bit page x */
    s16 unk2; /* 8-bit page y */
    u8 pad4[4];
    s16 unk8; /* 8-bit clut x */
    s16 unkA; /* 8-bit clut y */
    u8 padC[4];
    s16 unk10; /* 4-bit page x */
    s16 unk12; /* 4-bit page y */
    u8 pad14[4];
    s16 unk18; /* 4-bit clut x */
    s16 unk1A; /* 4-bit clut y */
    u8 pad1C[4];
    s16 unk20; /* x to skip */
} FieldVramPos;

s32 fld_allocModelCache(s32, s32);

FieldLight *fld_getLightColorTable(Unk800B3254 *);
/* fld_findLightColor: called before its definition without a prototype */
s32 bgi_getActorTriangle(s32, u16 *, u16 *);
s32 bgi_linkFrameNeighbors(BgiHeader *, BgiAnim *, BgiKeyframe *);
FieldLight *fld_findGlobalLights(Unk800B3254 *, FieldLight **, FieldLight **);
FieldLight *fld_findFloorLight(Unk800B3254 *, FieldEvtCtx *, s16 *);

/* object model slot (func_8001E350) */
typedef struct {
    s32 unk0; /* flags: 1 = loaded */
    s32 unk4; /* file id */
    ModelObj *unk8;
    void *unkC; /* actor */
} FieldObjSlot;

/* field directory entry (fld_refindSectors): the next entry's unk4 ends this one */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4; /* sector */
} FieldDirEnt;

/* background sprite sets (fldmdl_initTexAnims); offsets from the header start */
typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[8 - 4];
    u32 unk8; /* offset: u16 (x, y) pairs */
} TexAnimRelocRect; /* 0xC */

typedef struct {
    u8 unk0;
    u8 unk1; /* vertex count */
    u8 pad2[0xC - 2];
    u8 unkC; /* part count */
    u8 unkD; /* ModelTexOfs index */
    u16 unkE;
    u32 unk10; /* offset: TexAnimRelocRect[] */
} TexAnimReloc; /* 0x14 */

typedef struct {
    u16 unk0; /* set count */
    u8 pad2[2];
    TexAnimReloc unk4[1];
} TexAnimRelocSet;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} ModelTexOfs;

/* sprite vertex (TexAnimRelocRect.unk8) */
typedef struct {
    u16 unk0;
    u16 unk2;
} TexAnimRelocPos;

/* streamed sound bank request (fld_soundBankReadCallback, as ovl_0c wm_streamSoundChunk) */
typedef struct {
    u8 pad0[8];
    s32 unk8; /* address */
    s32 unkC; /* offset */
    s32 unk10; /* size */
} Unk800B29F8;

/* ---- overlay functions ---- */
void fld_dropSoundBankCache(s32);
s32 fld_freeFieldFile(FieldScene *, s16);
s32 func_800AA0A8(FieldScene *, s16);
void fld_stopSceneSfx(FieldScene *, s16);
s32 fld_movieCommand(s32, s32, s32);
s32 fld_freeCommon(void);
s32 bgi_stepFloorMove();
void func_80054A28(void *);
s32 fld_soundCommand(s32, s32, s32, s32, s32);
void fld_getSoundPanVolume(s16, s16, s16, u8 *, u8 *);
s32 fld_preloadRequest(s32, s32);
s32 fld_preloadHook(void);
void fld_clearPreloadLists(void);
s32 func_8001E350(s32);
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
/* fld_getBgKey: some callers pass two arguments; no prototype */
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
extern u8 D_800CA108; /* background sprite x extent */
extern s32 g_fieldBgCtxWork;
extern u16 D_800C9DDE;
extern u16 D_800C9DE0;
extern BgiContext g_fieldBgi;
extern Unk800C9DCC *g_fieldBgCtxEntries;
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
extern s16 D_800CAA98[];
extern s16 D_800CAAB8[];
extern BgiFloorMove g_fieldBgiFloorMoves[];
extern MenuMain s_menuMain;
extern void (*s_menuMainInput[])(void);
extern Unk800C9EF0 g_fieldFlagQueue;
extern u16 g_fieldMovieFlags;
extern s16 g_fieldBgiPushCount;
extern char D_800A7814[]; /* "          1/ny = %lx\n" (walkmesh dump) */
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
typedef struct {
    s32 unk0;
    s32 unk4;
} Unk8002208C;
Unk8002208C *preload_waitCurrent(void);
s32 preload_overlaps(s32, s32);
void preload_discardRange(s32, s32);
s32 func_80050BE0(s32);

extern RECT *s_menuMainTimeGilRect;
extern u8 s_menuMainGilSuffix[];
extern RECT *s_menuMainLocationRect;
extern u8 *s_menuMainEquipLabels[5];
extern u8 s_menuMainText[]; /* text */
extern RECT g_fieldDiscChangeRect;

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
extern u8 D_800C9EC0[];
extern u8 D_800C9EC4[];
extern u8 D_800C9ECC[];
extern u8 D_800C9ED4[];
extern u8 D_800C9EDC[];
extern u8 D_800C9EE4[];

/* 8-byte entries following Unk800B3254.unkC[unk6] */


/* 10 VRAM cache slots at 0x800C9CCC. splat puts the first word at the end of
 * .text (as "g_fieldModelCache"), so the array is reached through that label. */
typedef struct {
    s16 unk0;
    s16 unk2; /* id */
    u32 unk4; /* address (0 = free) */
    s32 unk8; /* size */
} Unk800C9CCC; /* 0xC */
extern Unk800C9CCC g_fieldModelCache[10];
Unk800C9CCC *fld_findModelCache(s32);
u32 fld_getModelCacheBottom(Unk800C9CCC **);
void fld_allocArea(void);
void fld_initArea(void);
/* fld_findAreaItem: called before its definition without a prototype */
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
MATRIX *math_rotMatrixZXY(SVECTOR *, MATRIX *);
extern s32 g_vtxScreenXY[]; /* projected screen xy per model vertex */
extern s32 g_vtxScreenZ[]; /* projected z per model vertex */
s32 func_80056048(s32, s32, s32);
extern u32 g_portraitSavedVram[]; /* VRAM save buffer */
extern u8 D_80077E51;
extern u8 *D_80077DF0; /* = g_menuHelp.text, referenced by its own name */
extern s32 D_80077DEC; /* = g_menuHelp.otz, referenced by its own name */
s32 text_measureStyle(u8 *, s32);
extern s32 D_80077E54; /* overlaps Unk80077E50.unk4 in ff9.h */
void menuMain_reorder(s32, s32);
void fld_discPromptSelect(void);
s32 fld_setupActorModel(s32, void *);
void menuMain_drawMember(s32);
void menuMain_drawPortraitShade(RECT *, s32);
extern s32 g_mbgFrame;
void gameover_main(void *); /* overlay 0x65 (loaded at 0x801F0000) */
void mbg_setRgb24(s32); /* overlay loaded at 0x80198000 */
s32 mbg_getFrame(void);
void menu_update(void);
Unk8004AB64 *evt_getPlayer(void);
u32 func_8004AB64(void *); /* actor -> model slot */
void game_updateWindows(void);
void preload_reset(void);
void math_rotMatrixZYX_gte(void *, void *); /* rotation (SVECTOR) -> bone matrix */

/* field state for fld_movieCommand command 3: 0x11, 1 or 0x10/0 */
static inline s32 fld_movieCommand_state(void) {
    s8 v;

    if (g_gameState->disc != 0xFF) {
        v = g_gameState->unk1C->unk8E4.field->unkC->unk14->movieState;
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
    return (BgAnimStep *)((u8 *)g_fieldBgData + p->unkC);
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
    if (-g_gameState->unk1C->unk8E4.field->unkC->unk14->otBias < k \
        && k < 0x1000 - g_gameState->unk1C->unk8E4.field->unkC->unk14->otBias) { \
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][k], e->unk8[g_gameState->unk1C->unk8][q]); \
    }

void fld_calcShadowMatrix(FieldObjEnt *, FieldObjSlot *, MATRIX *);


/* field movie playback (fld_movieUpdate) */
typedef struct {
    u32 tag;
    u32 code[5];
} Unk800C9F6C; /* 0x18: mask-bit draw mode packet, per draw buffer */
typedef struct {
    char *unk0; /* movie file name */
    s32 pad4[4];
} Unk8019DC5CEnt; /* 0x14 */
typedef struct {
    s32 unk0;
    Unk8019DC5CEnt *unk4;
} Unk8019DC5C;
extern Unk8019DC5C g_mbgMovieLists[]; /* overlay loaded at 0x80198000 */
extern u16 g_fieldMovieLastFrame;
extern Unk800C9F6C g_fieldMovieMaskPkt[2];
void fld_movieStop(void);
void fld_movieFinish(void);
void MBG_init(s32, void *); /* overlay loaded at 0x80198000 */
void mbg_setEndCallback(void (*)());
void func_8019AB98(s32, s32, s32);
void mbg_play(s32, s32);
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
extern u8 *D_8007BD30;
void akao_fadeMusicVolume(s32, s32, s32);
void akao_fadeSeVolume(void *, s32, s32, s32);
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
s32 fld_soundBankReadCallback(Unk800B29F8 *arg0);
/* main exe: candidates for ff9.h */
s32 akao_playMusic(void *);
void akao_stopMusic(s32);
void akao_saveMusic(void);
s32 akao_playMusicCount(void *, s32);
void akao_playSe(s32, s32, s32, s32);
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
void akao_sendCmdE0(s32, s32, s32);
void akao_sendCmdE6(s32);

/* GameData.unk828/unk82C as an array (the slot index needs `base + slot*4`) until ff9.h has it */
typedef struct {
    u8 pad[0x828];
    s32 unk828[2];
} GameDataSfx;
#define SFXH(i) (((GameDataSfx *)g_gameState->unk1C)->unk828[i])
#define SCENE (g_gameState->unk1C->unk8E4.field->unkC->unk14)

s32 fld_initShadows(s32, s32, s32, s32); /* returns func_80062E3C's result (the next free address) */

FieldAnimSfx *fld_addAnimSfx(FieldEvtCtx *, s32, s32); /* returns the new entry */

#endif
