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
    u8 padC[0x14 - 0xC];
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
    u8 pad41C[0x424 - 0x41C];
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
    u8 pad454[0x458 - 0x454];
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
    u8 pad480[0x484 - 0x480];
    s32 unk484;
    u8 pad488[0x48C - 0x488];
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
    u8 pad4F4[0x50C - 0x4F4];
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
    u8 pad558[0x564 - 0x558];
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
} TmState;

typedef struct {
    u8 v[8];
} TmDirMask; /* card arrow bit for each direction */
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
    u32 unk0; /* id */
    u32 unk4; /* flags: bpp in low bits */
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
void func_800AF288(TmState *);
void func_800B3488(TmState *);
void func_800AA3C0(Window *);
void func_800ADFFC(Window *);
void func_800AB0A8(Window *);
void *func_800B16A4(u32 *, void *, TmState *, s32, s32, s32);
void *func_800B33CC(u32 *, void *, s32, s32, s32);
void func_800B1110(TmState *, s32, s32, s32, s32, s32);
void func_800B35F8(TmState *);
void func_800A755C(void);
void func_800A75E0(void);
void func_800A78EC(s32, s32, s32, s32, s32);
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
s32 func_800AE94C(TmState *, s32);
TmCard *func_800AE9D0(TmState *, s32, s32);
void func_800AEA24(TmState *);
void func_800B1A24(TmState *);
void func_800B295C(TmState *);
void func_800B29DC(TmState *);
void func_800AB89C(Window *);
void func_800B355C(Window *);

#endif
