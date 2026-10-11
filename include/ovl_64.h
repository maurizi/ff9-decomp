#ifndef OVL_64_H
#define OVL_64_H

/* Declarations local to overlay 0x64 (FMV player, loaded at 0x80198700;
 * other overlays such as ovl_15 call into it). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
void func_80063864(void *, void *, s32);
extern s8 D_80071E31;
extern s32 D_80071E34;
extern s8 D_80071E38;
void func_80056228(void);
void func_80065238(s32, s32);
extern s8 D_80071E30;
void func_80022F1C(s32, void (*)());
void func_80022FD0(s32);
void func_80023018(void);
void func_800236A4(s32);
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
    u8 unk4;
    u8 unk5;
    s16 unk6;
    s16 unk8; /* last frame */
    s16 unkA; /* x offset */
    u8 padC[0x14 - 0xC];
} Ovl64MovieInfo;

typedef struct {
    s16 unk0; /* count */
    Ovl64MovieInfo *unk4;
} Ovl64Movie; /* 8-byte entries */
extern Ovl64Movie D_8019DC5C[6];

typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl64Size;
extern Ovl64Size D_8019DC88;

/* FMV player state (D_8019DC94). */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 pad7;
    s32 unk8; /* current buffer */
    CdlLOC unkC; /* stream start position */
    u8 pad10[0x24 - 0x10];
    CdlLOC unk24; /* stream position */
    s32 unk28;
    s32 unk2C; /* frame width */
    s32 unk30; /* frame height */
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    void *unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    void (*unk60)(); /* end callback (called without arguments) */
    s32 unk64;
} Ovl64State;
extern Ovl64State D_8019DC94;

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
extern Ovl64Header D_8019F258;
extern u16 D_8019F278;

/* decode / display state (D_8019F4D8) */
typedef struct {
    u32 *unk0[2]; /* MDEC input buffers */
    s32 unk8;     /* current input buffer */
    u32 *unkC[2]; /* MDEC output buffers */
    s32 unk14;    /* current buffer */
    RECT unk18[2]; /* frame area per draw buffer */
    RECT unk28;    /* slice being decoded */
    s32 unk30;     /* frame done */
    u8 pad34[0x38 - 0x34];
} Ovl64Dec;
extern Ovl64Dec D_8019F4D8;
extern u8 D_8019F288[];

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl64Vlc;
extern Ovl64Vlc D_8019DCFC[108];
extern s32 D_8019DDD4;

/* CD stream ring (D_8019DE10) */
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

/* per-frame info copied out of the stream (D_8019F510) */
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

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl64Flag;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    void (*unk4)(u8 *, u8 *, Ovl64FrameInfo *);
    void (*unk8)(void); /* sector callback */
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    CdlLOC unk20;
    u8 pad24[0x38 - 0x24];
    s32 unk38;
    u32 unk3C;
    s32 unk40;
    s32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
    u32 unk5C; /* sectors per slot */
    u32 unk60;
    s32 unk64;
    s32 unk68;
    u32 unk6C; /* slot count */
    u32 unk70; /* current slot */
    Ovl64RingHdr *unk74;
    u8 *unk78; /* slot data, 0x8F4 bytes per sector */
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    u32 unk88;
    u32 unk8C;
    u8 *unk90;
} Ovl64Stream;
extern s32 D_8019DE0C;
extern Ovl64Stream D_8019DE10;

/* func_8019B7C4: CD ring debug view, one colour per 8 sectors and a POLY_F4 per sector */
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Ovl64Rgb;
extern Ovl64Rgb D_8019DEAC[];
extern POLY_F4 D_8019F7F0[2][80];
extern s32 D_8019DEA4;
extern s32 D_8019DEA8;
typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl64Pair;
extern Ovl64FrameInfo D_8019F510[];
extern Ovl64Pair D_8019F750[16];
extern Ovl64Flag D_8019F7D0[];

void func_8019C434(u8, u8 *); /* CdReadyCallback */
void func_8019BA70(void);
void func_8019BCE8(void);
void func_8019A7A8(s32, s32);
s32 func_8019C864(void);
s32 func_8019C99C(s32 *);
s32 func_8019C9B4(u32);
s32 func_8019CBF4(s32 *);
s32 func_8019CC3C(s32);
s32 func_8019CC7C(void);
s32 func_80199F30(s32);

typedef struct {
    u8 pad0[0x1C];
    s16 unk1C;
    s16 unk1E;
    u8 pad20[4];
    s16 unk24;
    s16 unk26;
    u8 pad28[6];
    s16 unk2E;
} Ovl64Out; /* func_801990A0 output: frame size in RECTs at 0x18.. */
extern Ovl64MovieInfo *D_8019DCF8;
extern s32 D_8019E018[2];
typedef struct {
    u8 v[0x914];
} Ovl64Sub;
extern Ovl64Sub D_8019E028[2];
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
} Ovl64Vlc4C8; /* argument block of func_8019B3D4 */
extern Ovl64Vlc4C8 D_8019F4C8;
extern u16 D_8019DC84;
extern u8 D_8019DC90;
extern RECT D_8019F488[4];
extern u8 D_8019DC97; /* = D_8019DC94.unk3, referenced by its own name */
void func_8019B3D4(void *, u32 *, u16);
typedef struct {
    u8 pad0[3];
    u8 unk3;
    u32 unk4;
    u32 unk8;
} Ovl64Stp;
extern Ovl64Stp D_8019F4A8[2];
extern s32 D_8019DC9C;
s32 func_8019AAF8(void);
s32 func_8019C9D4(void);
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


/* used by func_80199B28 */
extern char D_8019903C[];
s32 func_8019A5B0(s32);
void func_8019AE44(void *);
void func_8019AEC0(u8 *, u32 *, void *);
s32 func_8019C1B0(s32 unused, u32 arg1, s32 arg2, void (*arg3)(void), s32 arg4);
s32 func_8019C7E4(void);
s32 func_8019C884(Ovl64RingHdr *ring, u32 per, u32 size);
s32 func_8019C954(u8 *arg0, s32 arg1, u32 arg2);
s32 func_8019C98C(s32 arg0);
s32 func_8019CA1C(CdlLOC *loc);
s32 func_8019CAC0(CdlLOC *loc);
s32 func_8019CC0C(void);
s32 func_8019CCBC(u8 *data);
u32 func_8019D3F0(u16 *p);
void func_80199554(void);

#endif
