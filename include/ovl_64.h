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
void akao_sendCmdE2(void);
char *strcpy(char *, const char *);
extern s8 D_80071E30;
u8 cd_readStreamAsync(s32, void (*)());
u8 func_80022FD0(void (*)(u8 *));
s32 cd_pause(void);
void CdSetDebug(s32);
s32 akao_sendCmdE8(s32, s32);
s32 func_80056488(void *);
int SetGraphDebug(int level);
int CdPosToInt(CdlLOC *p);

/* PsyQ libpress */
void DecDCToutCallback(void (*func)());
extern volatile u32 *g_mdec1Reg; /* MDEC1 register pointer */

/* ---- this overlay ---- */

/* movie table entry (0x14 bytes) */
typedef struct {
    char *path; /* movie file name (cd_findFile, mbg_getEntryName) */
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
    s32 x;
    s32 y;
} Ovl64Size;
extern Ovl64Size g_mbgGeomOffset;

/* FMV player state (g_mbgPlayer). */
typedef struct {
    u8 ended;
    u8 rgb24;
    u8 savedCameraId; /* field background camera at the start (fldbg_getCamera), restored at the end */
    u8 modeSwitch; /* 1: switch rgb24 mode at the next frame end (mbg_setRgb24 while playing), 2: mbg_update resets the graph env */
    u8 pauseReq;
    u8 unk5;
    s8 firstFrame; /* set by mbg_play; mbg_readFrame keeps the first frame's camera in g_mbgStartCamera and clears it */
    u8 pad7;
    s32 shortFrame; /* set by mbg_update on every 50th movie frame, the next frame then waits vsyncMode 3 instead of 4 and clears it */
    CdlLOC startPos; /* stream start position */
    u8 pad10[0x24 - 0x10];
    CdlLOC streamPos; /* stream position (seek target after a bad frame) */
    s32 savedOtBias; /* the field scene's otBias, zeroed while the movie plays */
    s32 width; /* frame width */
    s32 height; /* frame height */
    s32 audioFrames; /* frames with audio pushed (mbg_streamPushAudio) */
    s32 volumeDelay; /* at audioFrames == volumeDelay: akao_sendCmdE5(volumeFade, volume) (mbg_setVolume, default 3) */
    s32 volumeFade; /* default 4 */
    s32 volume; /* default 0x7F; the field passes movieSndVolume */
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

/* a frame's camera, laid out like a MATRIX (mbg_readFrame copies it into GameData.unk18) */
typedef struct {
    u16 m00, m01, m02;
    u16 m10, m11, m12;
    u16 m20, m21, m22;
    u8 pad12[2];
    s32 tx;
    s32 ty;
    s32 tz;
} Ovl64Header;
extern Ovl64Header g_mbgStartCamera;
extern u16 g_mbgStartCameraId; /* id of the start camera (Ovl64FrameInfo.id) */

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
    u16 status; /* low 12 bits kept; 0x1000 free, 0x2000/0x4000 being filled, 0x8000 ready (mbg_streamPeekFrame) */
    u8 pad2[2];
    u16 firstSector; /* sector in the slot where the frame data starts */
    u8 pad6[2];
    u8 pad8[0x10 - 8];
    u16 width; /* frame width */
    u16 height; /* frame height */
    u8 pad14[0x20 - 0x14];
} Ovl64RingHdr; /* 0x20 */

/* sector header at the end of each ring slot */
typedef struct {
    u16 unk0;
    u16 type; /* 4: the frame's first sector, read for the mask info */
    u8 pad4[4];
    s32 frame; /* frame number, must equal g_mbgStream.frame */
    u8 padC[0x1C - 0xC];
    union {
        u32 w;
        struct {
            u16 lo;
            u16 hi;
        } h;
    } maskInfo; /* bit 29: mask data present, low 16 bits its size, hi & 0xFFF its offset (g_mbgSlotMasks) */
} Ovl64SectHdr;

/* per-frame camera record copied out of the stream (g_mbgSlotCameras); mbg_readFrame applies it to the GTE and
 * GameData.unk18 (rotation, translation) and GameData.unkA (id) */
typedef struct {
    u16 id;
    u16 m00, m01, m02;
    u16 m10, m11, m12;
    u16 m20, m21, m22;
    s32 tx;
    s32 ty;
    s32 tz;
    u16 ofsX; /* geometry offset - 160 */
    u16 ofsY; /* geometry offset - 112 */
} Ovl64FrameInfo; /* 0x24 */

/* 2 KB stream sector */
typedef struct {
    u8 pad0[0x12];
    u16 hasCamera; /* 1: cameras[] valid */
    Ovl64FrameInfo cameras[2]; /* selected by g_mbgStream.cameraSel */
    s32 rumble; /* negative: vibration, bits 8-15 >= 0x80 on, low byte the level (g_mbgSlotRumble) */
    u32 lastFrame; /* 0xEEEEEEEE: none */
} Ovl64StreamSect;

/* per ring slot: the pad vibration of the frame (frame header +0x5C), copied to the actuator bytes g_padActuator
 * (which ovl_34's vibration test also writes) while the frame shows */
typedef struct {
    u8 on;    /* header byte 1 >= 0x80 */
    u8 level;
} MbgRumble;

typedef struct {
    u8 unk0;
    u8 stalled; /* the ring was full: reading paused until mbg_streamFreeFrame */
    u8 endState; /* 1 past lastFrame, 2 error (channel / frame mismatch, timeout); mbg_streamGetFrame then returns 5 */
    u8 hasCamera; /* frames carry camera / mask data */
    void (*frameInfoCallback)(u8 *, u8 *, Ovl64FrameInfo *);
    void (*frameCallback)(void); /* called after each complete frame */
    s32 unkC;
    s32 cameraSel;
    s32 lastReadTime;
    s32 channel;
    s32 baseSector;
    CdlLOC startPos;
    u8 pad24[0x38 - 0x24];
    s32 startFrame;
    u32 lastFrame;
    s32 startSector;
    s32 sector;
    u32 frameSector; /* sector within the current frame */
    u32 cycleSector; /* sector within the audio/video interleave; 0 and 1 are audio */
    u32 cycleLength;
    u32 frameSectors;
    u32 frame;
    u32 slotSectors; /* sectors per slot */
    u32 ringSectors; /* ring size in sectors */
    s32 slotSector; /* sector being written within the slot */
    s32 writeSlot; /* slot being written (counts down) */
    u32 slotCount; /* slot count */
    u32 readSlot; /* current slot */
    Ovl64RingHdr *ringHdrs; /* one per slot */
    u8 *ringData; /* slot data, 0x8F4 bytes per sector */
    u32 audioSectors; /* audio buffer size in sectors */
    u32 audioWrite; /* next audio sector */
    u32 audioLast; /* audio sector just written */
    u32 audioPending; /* written, not yet pushed (mbg_streamPushAudio takes 2) */
    u32 audioPendingMax;
    u8 *audioBuf;
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
/* per ring slot: the frame's mask data (applied by mbg_applyMask) */
typedef struct {
    s32 size;
    s32 data;
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

extern Ovl64MovieInfo *g_mbgMovie;
extern s32 g_mbgMaskSize[2];
typedef struct {
    u8 v[0x914];
} Ovl64Sub;
extern Ovl64Sub g_mbgMaskData[2];
typedef struct {
    u32 b0 : 7;
    u32 vibrationOff : 1; /* the Config screen's Vibration OFF (ovl_34 CFG_BIT_VIBRATION) */
    u32 pad : 24;
} Ovl64Cfg; /* view of GameState.unk24, the config word */

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
    u16 runMasked; /* current run: nonzero = pixels get the mask bit */
    u16 runLength; /* pixels left in the current run */
    s32 size; /* mask data size, 0 = none */
    s32 data; /* mask data (g_mbgMaskData) */
    s32 cursor; /* read position (mbg_applyMask) */
} Ovl64Vlc4C8; /* argument block of mbg_applyMask */
extern Ovl64Vlc4C8 g_mbgMaskCursor;
extern u16 g_mbgMaskBit;
extern u8 g_mbgLoadRectIdx;
extern RECT g_mbgLoadRects[4];
void mbg_applyMask(void *, u32 *, u16);
typedef struct {
    u8 pad0[3];
    u8 len; /* packet length (2) */
    u32 code[2]; /* 0xE6000002 (check mask), 0 */
} Ovl64Stp;
extern Ovl64Stp g_mbgMaskStp[2];
s32 mbg_stop(void);
s32 mbg_streamPushAudio(void);
/* PsyQ libpress */
int DecDCTin(u32 *, int);
int DecDCTinSync(int);
int DecDCTout(u32 *, int);
int DecDCTReset(int);
/* ovl_0a's deferred flag queue ([0] set, [1] clear, one frame per entry), here to clear the flags the switch
 * to rgb24 set */
typedef struct {
    u16 gameFlags[2][4]; /* GameData.unk0 */
    u16 mapFlags[2][4]; /* FieldMap.flags */
    u16 movieFlags[2][4];
} Ovl64Field;
extern Ovl64Field g_fieldFlagQueue;

/* GameData.unk8E4 in field mode, as far as the movie player reads it */
typedef struct {
    u8 pad0[0x30];
    s16 otBias;
    u8 pad32[0x44 - 0x32];
    s32 movieFile; /* stream base sector */
} Ovl64FieldC14; /* ovl_0a FieldScene */
typedef struct {
    u8 pad0[0x14];
    Ovl64FieldC14 *scene;
} Ovl64FieldC; /* ovl_0a FieldMapObj */
typedef struct {
    u8 pad0[0xC];
    Ovl64FieldC *area;
} Ovl64Field8E4; /* ovl_0a FieldMap */


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
u32 DecDCTBufSize(u16 *p);
void mbg_mdecOutCallback(void);

/* mbg_streamReadyCallback: the two cross-jumped timeout blocks (one inline, called twice) */
s32 cd_restartAt(s32); /* main exe */
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
        cd_restartAt(g_mbgStream.sector);
    } else {
        g_mbgStream.endState = 2;
        cd_pause();
    }
}

/* Functions called before (or without) a declaration in scope (tools/decl_check.py --protos) */
s32 fldbg_setCamera(s32 arg0); /* ovl_0a */
s32 mbg_streamGetFrame(u8 **data, Ovl64RingHdr **hdr, Ovl64FrameInfo **arg2, Ovl64Pair **arg3, MbgRumble **arg4);

#endif
