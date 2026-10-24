#ifndef OVL_0D_H
#define OVL_0D_H

/* Declarations local to overlay 0x0D (Tetra Master card game, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- Tetra Master ---- */

typedef struct {
    u8 unk0; /* card id? */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;  /* x */
    s32 unk10; /* y */
    s32 unk14; /* owner: 0 player, 1 enemy */
    s32 unk18; /* 1: on the board */
    s32 unk1C;
    s32 unk20; /* x << 8 */
    s32 unk24; /* y << 8 */
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3C - 0x38];
} TmCard; /* 0x3C */

typedef struct {
    s32 unk0;
    TmCard *unk4;
} TmCell; /* board square */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    TmCard *unk10;
} TmUnk350;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} TmUnk2F0;

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* y */
} TmPos;

typedef struct {
    s32 unk0; /* offset << 11 of entry */
    s32 unk4;
} TmFileEntry; /* file table loaded at 0x80104A00 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} TmCardData; /* card stats table loaded at 0x80103800 */

typedef struct {
    u8 v[16];
} TmDeckOdds; /* card id by rarity roll, loaded at 0x80103C00 */

typedef struct {
    u8 unk0;
    u8 unk1;
} TmLevel; /* opponent level table, loaded at 0x80104000 */

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    TmCard unk10[10]; /* 0-4 player hand, 5-9 enemy hand */
    TmCell unk268[4][4]; /* board [y][x] */
    s32 unk2E8; /* frame counter in the current state */
    u8 unk2EC; /* brightness */
    u8 pad2ED[3];
    TmUnk2F0 unk2F0[6];
    TmUnk350 unk350[10];
    s32 unk418;
    s32 unk41C;
    s32 unk420;
    s32 unk424;
    s32 unk428;
    s32 unk42C;
    s32 unk430;
    s32 unk434;
    s32 unk438;
    s32 unk43C; /* wins */
    s32 unk440; /* losses */
    s32 unk444; /* draws */
    s32 unk448;
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    Window *unk458;
    s32 unk45C;
    s32 unk460;
    s32 unk464;
    s32 unk468;
    s32 unk46C;
    TmCard *unk470;
    s32 unk474;
    s32 unk478;
    s32 unk47C;
    s32 unk480; /* cursor animation frame */
    Window *unk484;
    s32 unk488;
    TmCard *unk48C[8];
    s32 unk4AC;
    TmCard *unk4B0;
    s32 unk4B4;
    Window *unk4B8;
    s32 unk4BC;
    s32 unk4C0;
    Window *unk4C4;
    s32 unk4C8[9];
    s32 unk4EC;
    s32 unk4F0;
    s32 unk4F4;
    s32 unk4F8;
    s32 unk4FC;
    s32 unk500;
    s32 unk504;
    TmCard *unk508;
    u8 unk50C;
    u8 pad50D[0x510 - 0x50D];
    u8 unk510;
    u8 unk511;
    u8 unk512;
    u8 unk513;
    u8 unk514;
    u8 unk515;
    u8 pad516[0x54C - 0x516];
    s32 unk54C;
    s32 unk550;
    s32 unk554;
    u8 unk558[12];
    s32 unk564;
    s32 unk568;
    s32 unk56C;
    s32 unk570;
    s32 unk574;
    s32 unk578;
    s32 unk57C;
    u8 pad580[0x590 - 0x580];
    s32 unk590[5];
    s32 unk5A4;
    s32 unk5A8;
    s32 unk5AC;
    s32 unk5B0;
    s32 unk5B4;
    s32 unk5B8;
    s32 unk5BC;
    s32 unk5C0;
    s32 unk5C4;
    RECT unk5C8;
} TmState;

typedef struct {
    u8 v[8];
} TmDirMask; /* card arrow bit for each direction */
extern TmDirMask D_800A73B4; /* single-bit masks, one per arrow */
extern TmDirMask D_800A73BC;

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* y */
    s32 unk8; /* x when flipped */
    s32 unkC; /* y when flipped */
    s32 unk10; /* u */
    s32 unk14; /* v */
} TmArrow;

typedef struct {
    TmArrow v[8];
} TmArrowTbl; /* arrow sprite for each direction */
extern TmArrowTbl D_800A73C4;

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* u */
    s32 unk8; /* v */
    s32 unkC; /* w */
} TmCoinFrame;

typedef struct {
    TmCoinFrame v[8];
} TmCoinFrames; /* coin toss animation frames */
extern TmCoinFrames D_800A74AC;

typedef struct {
    u32 unk0; /* id */
    s32 unk4; /* flags: bpp in low bits */
    u32 unk8; /* clut block length */
    s16 unkC[4]; /* clut rect */
    u32 clut[1];
} TmTim; /* TIM image */

typedef struct {
    u32 len;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u32 data[1];
} TmTimImage; /* TIM pixel block */

extern s32 D_800B365C;
extern s8 D_800B3664; /* sound request kind, -1 = none */
extern u8 g_cardGameText[];
extern s32 D_800B3660;
extern s32 D_800B3C8C[];
extern s32 D_800B3CD0;
extern s32 D_800B3CD8;
extern s32 D_800B3CDC;
extern s32 D_800B3CE0;
extern TmCell D_800B3CE8; /* returned for out-of-range board squares */
extern s32 D_800B3CF0;
extern s32 D_800B3CF4;

s32 func_800A7634(void);
s32 func_800A76F8(void);
s32 func_800A7764(void);
s32 func_800A7818(void);
s32 func_800A7884(void);
s32 func_800A78B8(void);
u8 *func_800A93A0(s32);
void func_800ABEC4(TmState *);
void func_800AF2CC(TmState *, TmCard *, s32);
void func_800AF388(TmState *, TmCard *);
void func_800AF3E8(TmState *, TmCard *, s32, s32);
void func_800B0618(TmState *);
s32 func_800B0EB8(TmState *, s32, s32, s32, TmCard *);
TmCard *func_800AEE08(TmState *, s32, s32);
void func_800AE6EC(TmState *, s32);
void func_800ADE5C(TmState *);
void func_800AD974(TmState *);
void func_800AF170(TmState *);
void func_800AF100(TmState *);
void func_800AF288(TmState *);
void func_800B3488(TmState *);
void func_800AA3C0(Window *);
void func_800ADFFC(Window *);
void func_800AB0A8(Window *);
void *func_800B16A4(u32 *, void *, TmState *, s32, s32, s32);
void *func_800B33CC(u32 *, SPRT *, s32, s32, s32);
s32 func_800B1110(TmState *, s32, s32, s32, s32, s32);
void func_800B35F8(TmState *);
void func_800A755C(void);
void func_800A75E0(void);
s32 func_800A78EC(s32, s32, s32, s32, s32);
void func_800A7E10(void);
void func_800AB8C8(void);
void func_800AB8E8(s32);
void func_800AE54C(TmState *);
void func_800B01E8(s32);
TmCell *func_800B19D4(TmState *, s32, s32);
TmCard *func_800B12F0(TmState *, TmPos *, s32, s32);
void func_800AEBB4(TmState *);
void func_800B13C0(TmPos *, s32);
s32 func_800AE830(TmState *, s32);
s32 func_800AE878(TmState *, s32);
s32 func_800AE8D0(TmState *);
TmCard *func_800AE908(TmState *, s32);
void func_800ACFC8(TmState *);
s32 func_800AE998(TmState *, s32);
s32 func_800ADE14(TmState *, s32);
s32 func_800AE94C(TmState *, s32);
TmCard *func_800AE9D0(TmState *, s32, s32);
void func_800AEA24(TmState *);
void func_800B1A24(TmState *);
void func_800B295C(TmState *);
void func_800B29DC(TmState *);
void func_800AB89C(Window *);
void func_800B355C(Window *);
SPRT *func_800AFB60(void *, SPRT *, s32, s32, s32, s32, s32);
void func_800AE494(Window *);

/* Main loop (func_800A7E10) and its per-state handlers. */
void func_800A8B80(void);
void func_800A8BC4(TmState *);
void func_800A8D74(TmState *);
void func_800A932C(TmState *, char *); /* debug print, stubbed */
void func_800A9B0C(TmState *);
void func_800A9298(TmState *);
void func_800A9334(void);
void func_800A93C8(u8 *, s32, s32);
void func_800A986C(TmTim *, s32, s32, s32, s32);
void *func_800A993C(u32 *, SPRT *, TmState *);
void *func_800A9424(u32 *, SPRT_16 *, TmState *);
void *func_800A96B0(u32 *, SPRT_16 *, TmState *);
void *func_800A9B2C(u32 *, void *, TmState *);
void *func_800A9F10(u32 *, void *, TmState *);
void *func_800AB574(u32 *, void *, TmState *);
void *func_800AB928(u32 *, void *, TmState *);
void *func_800ABEBC(u32 *, void *, TmState *);
void *func_800AC060(u32 *, void *, TmState *);
void *func_800AC708(u32 *, TILE *, TmState *);
void *func_800ACDA0(u32 *, TILE *, TmState *);
void *func_800AD400(u32 *, void *, TmState *);
void *func_800AD9F8(u32 *, void *, TmState *);
void *func_800AE084(u32 *, void *, TmState *);
void *func_800AE398(u32 *, SPRT *, TmState *);
void *func_800AE5A0(u32 *, void *, TmState *);
void *func_800AFCE4(u32 *, SPRT *, TmState *);
void *func_800B030C(u32 *, void *, TmState *);
void *func_800B07DC(u32 *, void *, TmState *);
void *func_800B1A84(u32 *, SPRT *, TmState *);
void *func_800AF95C(u32 *, SPRT *, TmState *, TmCard *, u8);
void *func_800B14AC(u32 *, SPRT_16 *, TmState *, s32, s32, u8, s32, u8, s32);
void *func_800B29F0(u32 *, void *, TmState *, TmCard *, u8, s32);
void *func_800B2C5C(u32 *, SPRT *, TmState *);
void *func_800B2F60(u32 *, SPRT *, TmState *);
void *func_800B3198(u32 *, void *, TmState *);
void *func_800B3480(u32 *, void *, TmState *);
void func_800AA1FC(TmState *);
void func_800AA368(TmState *);
void func_800AA408(TmState *);
void func_800AA504(TmState *);
void func_800AA548(TmState *);
void func_800AA790(TmState *);
void func_800AAEB4(TmState *);
void func_800AB0F0(TmState *);
void func_800AB2E0(TmState *);
void func_800AB338(TmState *);
void func_800AB854(TmState *);
void func_800ABC74(TmState *);
void func_800AC5AC(TmState *);
void func_800AC624(TmState *);
void func_800AC6A4(TmState *);
void func_800ACF8C(TmState *);
void func_800ACFFC(TmState *);
void func_800AEE2C(TmState *);
void func_800AF760(TmState *);
void func_800AF890(TmState *);
void func_800AFBE4(TmState *);
void func_800B026C(TmState *);
void func_800B08CC(TmState *);
void func_800B35A4(TmState *);

void func_800AC4C8(s32, TmPos *, TmCard *, TmCard *);
TmCard *func_800B1010(TmState *, s32, s32, TmCard *);
void func_800AB060(Window *);
void func_800AB52C(Window *);
void func_800ABE74(Window *);
s32 func_800AF268(TmState *);
void func_800AB8F4(s16);
extern u8 D_800A74A8[];

/* main exe: candidates for ff9.h */
s32 func_8002BA10(u8 *);
void game_updateWindows(void);
void SetSprt16(SPRT_16 *);
s32 func_80062794(s32); /* width of a number string? */
s32 func_80062498(void);
s32 func_800624D8(void);
s32 func_8006251C(void);
u8 pad_isRepeatEnabled(s32, s32);
void help_nullHandler(void);
void func_80062724();
extern u8 *D_80077DF0; /* = g_menuHelp.text, referenced by its own name */
s32 func_80055290(void *);
void func_800552B8(s32);
void func_800553EC(s32, s32, s32, s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);


/* Capture flash bar pieces (func_800AC708), one per direction. */
typedef struct {
    s32 unk0; /* 0: horizontal bar, 1: corner, 2: vertical bar */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14; /* x offset */
    s32 unk18; /* y offset */
} TmFlash;

typedef struct {
    TmFlash v[8];
} TmFlashTbl;
extern TmFlashTbl D_800A72B8; /* INCLUDE_RODATA */

extern char D_800A7398[]; /* "NO" label (INCLUDE_RODATA) */

#endif
