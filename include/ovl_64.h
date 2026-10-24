#ifndef OVL_64_H
#define OVL_64_H

/* Declarations local to overlay 0x64 (FMV player, loaded at 0x80198700;
 * other overlays such as ovl_15 call into it). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
void bcopy(void *, void *, s32);
extern s8 g_mbgRgb24;
extern s32 g_mbgFrame;
extern s8 g_mbgPaused;
void func_80056228(void);
char *strcpy(char *, const char *);
extern s8 D_80071E30;
void func_80022F1C(s32, void (*)());
void func_80022FD0(s32);
void func_80023018(void);
void CdSetDebug(s32);
void func_80056448(s32, s32);
void func_80056488(void *);
int SetGraphDebug(int level);
int CdPosToInt(CdlLOC *p);

/* PsyQ libpress */
void DecDCToutCallback(void (*func)());
extern volatile u32 *D_8019E008; /* MDEC1 register pointer */

/* ---- this overlay ---- */

/* movie table entry (0x14 bytes) */
typedef struct {
    s32 unk0; /* file */
    u8 rgb24;
    u8 unk5;
    s16 startFrame;
    s16 lastFrame; /* last frame */
    s16 xOffset; /* x offset */
    u8 padC[0x14 - 0xC];
} Ovl64MovieInfo;

typedef struct {
    s16 count; /* count */
    Ovl64MovieInfo *entries;
} Ovl64Movie; /* 8-byte entries */
extern Ovl64Movie g_mbgMovieLists[6];

typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl64Size;
extern Ovl64Size g_mbgGeomOffset;

/* FMV player state (g_mbgPlayer). */
typedef struct {
    u8 ended;
    u8 rgb24;
    u8 unk2;
    u8 unk3;
    u8 pauseReq;
    u8 unk5;
    s8 unk6;
    u8 pad7;
    s32 unk8; /* current buffer */
    CdlLOC unkC; /* stream start position */
    u8 pad10[0x24 - 0x10];
    CdlLOC unk24; /* stream position */
    s32 unk28;
    s32 width; /* frame width */
    s32 height; /* frame height */
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    void *vlcTable;
    s32 mdecIn0;
    s32 mdecIn1;
    s32 mdecOut0;
    s32 mdecOut1;
    s32 ringBuf;
    s32 audioBuf;
    void (*endCallback)(); /* end callback (called without arguments) */
    s32 unk64;
} Ovl64State;
extern Ovl64State g_mbgPlayer;

/* sector header of the current frame */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u8 pad12[2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} Ovl64Header;
extern Ovl64Header g_mbgStartCamera;
extern u16 D_8019F278;

/* decode / display state (g_mbgDecode) */
typedef struct {
    u32 *mdecIn[2]; /* MDEC input buffers */
    s32 inIdx;     /* current input buffer */
    u32 *mdecOut[2]; /* MDEC output buffers */
    s32 outIdx;    /* current buffer */
    RECT frameRect[2]; /* frame area per draw buffer */
    RECT slice;    /* slice being decoded */
    s32 frameDone;     /* frame done */
    u8 pad34[0x38 - 0x34];
} Ovl64Dec;
extern Ovl64Dec g_mbgDecode;
extern u8 g_mbgVlcTable[];

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl64Vlc;
extern Ovl64Vlc g_mbgVlcPairs[108];
extern s32 g_mbgVlcSize;

/* CD stream ring (g_mbgStream) */
typedef struct {
    u16 unk0; /* flags: 0x1000 free, 0x8000 ready */
    u8 pad2[2];
    u16 unk4;
    u8 pad6[2];
    u8 pad8[0x10 - 8];
    u16 unk10; /* frame width */
    u16 unk12; /* frame height */
    u8 pad14[0x20 - 0x14];
} Ovl64RingHdr; /* 0x20 */

/* sector header at the end of each ring slot */
typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x1C - 0xC];
    union {
        u32 w;
        struct {
            u16 lo;
            u16 hi;
        } h;
    } unk1C;
} Ovl64SectHdr;

/* per-frame info copied out of the stream (g_mbgSlotCameras) */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u16 unk20;
    u16 unk22;
} Ovl64FrameInfo; /* 0x24 */

/* 2 KB stream sector */
typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    Ovl64FrameInfo unk14[2];
    s32 unk5C;
    u32 unk60;
} Ovl64StreamSect;

/* per ring slot: the pad vibration of the frame (frame header +0x5C), copied to the actuator bytes D_800739B0
 * (which ovl_34's vibration test also writes) while the frame shows */
typedef struct {
    u8 on;    /* header byte 1 >= 0x80 */
    u8 level;
} MbgRumble;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    void (*unk4)(u8 *, u8 *, Ovl64FrameInfo *);
    void (*unk8)(void); /* sector callback */
    s32 unkC;
    s32 cameraSel;
    s32 lastReadTime;
    s32 channel;
    s32 baseSector;
    CdlLOC unk20;
    u8 pad24[0x38 - 0x24];
    s32 unk38;
    u32 lastFrame;
    s32 unk40;
    s32 sector;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 frameSectors;
    u32 frame;
    u32 unk5C; /* sectors per slot */
    u32 unk60;
    s32 unk64;
    s32 unk68;
    u32 slotCount; /* slot count */
    u32 readSlot; /* current slot */
    Ovl64RingHdr *unk74;
    u8 *unk78; /* slot data, 0x8F4 bytes per sector */
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    u32 unk88;
    u32 unk8C;
    u8 *unk90;
} Ovl64Stream;
extern s32 g_mbgSectorLogCount;
extern Ovl64Stream g_mbgStream;

/* mbg_drawStreamDebug: CD ring debug view, one colour per 8 sectors and a POLY_F4 per sector */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Ovl64Rgb;
extern Ovl64Rgb g_mbgDebugRingColors[];
extern POLY_F4 g_mbgDebugRingPolys[2][80];
extern s32 g_mbgStreamBusy;
extern s32 g_mbgStreamDeferred;
typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl64Pair;
extern Ovl64FrameInfo g_mbgSlotCameras[];
extern Ovl64Pair g_mbgSlotMasks[16];
extern MbgRumble g_mbgSlotRumble[];

void mbg_streamReadyCallback(u8, u8 *); /* CdReadyCallback */
void mbg_streamOnVideoSector(void);
void mbg_streamOnAudioSector(void);
void mbg_setGraphEnv(s32, s32);
s32 mbg_streamStop(void);
s32 mbg_streamGetCameraSelect(s32 *);
s32 mbg_streamSetCameraSelect(u32);
s32 mbg_streamGetLastFrame(s32 *);
s32 mbg_streamSetPaused(s32);
s32 mbg_streamClearRing(void);
s32 mbg_update(s32);

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    s16 unk1E;
    u8 pad20[4];
    s16 unk24;
    s16 unk26;
    u8 pad28[6];
    s16 unk2E;
} Ovl64Out; /* mbg_readFrame output: frame size in RECTs at 0x18.. */
extern Ovl64MovieInfo *g_mbgMovie;
extern s32 g_mbgMaskSize[2];
typedef struct {
    u8 v[0x914];
} Ovl64Sub;
extern Ovl64Sub g_mbgMaskData[2];
typedef struct {
    u32 b0 : 7;
    u32 b7 : 1;
    u32 pad : 24;
} Ovl64Cfg; /* view of GameState.unk24 */

/* PsyQ inline_c.h; candidate for psyq.h */
#define gte_SetGeomOffset(r0, r1) __asm__ volatile( \
    "sll\t$12, %0, 16;"                              \
    "sll\t$13, %1, 16;"                              \
    "ctc2\t$12, $24;"                                \
    "ctc2\t$13, $25"                                 \
    :                                                \
    : "r"(r0), "r"(r1)                               \
    : "$12", "$13")

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Ovl64Vlc4C8; /* argument block of mbg_applyMask */
extern Ovl64Vlc4C8 g_mbgMaskCursor;
extern u16 g_mbgMaskBit;
extern u8 g_mbgLoadRectIdx;
extern RECT g_mbgLoadRects[4];
extern u8 D_8019DC97; /* = g_mbgPlayer.unk3, referenced by its own name */
void mbg_applyMask(void *, u32 *, u16);
typedef struct {
    u8 pad0[3];
    u8 unk3;
    u32 unk4;
    u32 unk8;
} Ovl64Stp;
extern Ovl64Stp D_8019F4A8[2];
extern s32 D_8019DC9C;
s32 mbg_stop(void);
s32 mbg_streamPushAudio(void);
/* PsyQ libpress */
int DecDCTin(u32 *, int);
int DecDCTinSync(int);
int DecDCTout(u32 *, int);
int DecDCTReset(int);
typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[0x18 - 0xA];
    u16 unk18;
} Ovl64Field; /* field overlay object touched when the movie switches modes */
extern Ovl64Field D_800C9EF0;

/* GameData.unk8E4 in field mode, as far as the movie player reads it */
typedef struct {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x44 - 0x32];
    s32 unk44;
} Ovl64FieldC14;
typedef struct {
    u8 pad0[0x14];
    Ovl64FieldC14 *unk14;
} Ovl64FieldC;
typedef struct {
    u8 pad0[0xC];
    Ovl64FieldC *unkC;
} Ovl64Field8E4;


/* used by mbg_play */
extern char g_mbgScratchpadMsg[];
s32 mbg_setRgb24(s32);
void mbg_vlcBuildTable(void *);
void mbg_vlcDecode(u8 *, u32 *, void *);
s32 mbg_streamStart(s32 unused, u32 arg1, s32 arg2, void (*arg3)(void), s32 arg4);
s32 mbg_streamReset(void);
s32 mbg_streamSetRing(Ovl64RingHdr *ring, u32 per, u32 size);
s32 mbg_streamSetAudioBuffer(u8 *arg0, s32 arg1, u32 arg2);
s32 mbg_streamSetBaseSector(s32 arg0);
s32 mbg_streamSeek(CdlLOC *loc);
s32 mbg_streamGetPos(CdlLOC *loc);
s32 mbg_streamUngetFrame(void);
s32 mbg_streamFreeFrame(u8 *data);
u32 func_8019D3F0(u16 *p);
void mbg_mdecOutCallback(void);

/* mbg_streamReadyCallback: the two cross-jumped timeout blocks (one inline, called twice) */
void func_80022F64(s32); /* main exe */
s32 DecDCToutSync(s32);
extern s32 g_mbgSectorLogFrame[];
extern s32 g_mbgSectorLogSlot[];
static inline void mbg_streamReadyCallback_retry(void) {
    s32 limit;

    limit = 0xB4;
    if (g_mbgStream.frame == 1) {
        limit = 0x438;
    }
    if (VSync(-1) - g_mbgStream.lastReadTime < limit) {
        func_80022F64(g_mbgStream.sector);
    } else {
        g_mbgStream.unk2 = 2;
        func_80023018();
    }
}

#endif
