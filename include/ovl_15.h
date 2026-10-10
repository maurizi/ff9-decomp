#ifndef OVL_15_H
#define OVL_15_H

/* Declarations local to overlay 0x15 (Blackjack minigame, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
void func_800552B8(s32);
void func_8001D9CC(s32, s32, s32, s32, s32);
s32 func_80022AC4(s32);
void func_80022ED4(s32);
s32 func_80055290(void *);
void func_800553EC(s32, s32, s32, s32);
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

/* ovl_64 (FMV player, 0x80198700) */
void func_801999B4(s32, u8 **);
void func_80199B28(s32, s32);
s32 func_80199F30(s32);
void func_8019A5B0(s32);
void func_8019A9D8(void);
void func_8019AC3C(void (*)(void));
void func_8019AD0C(void);
void func_8019C98C(s32);
typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 4];
} Unk8019DC5CSub;
typedef struct {
    s32 unk0;
    Unk8019DC5CSub *unk4;
} Unk8019DC5C;
extern Unk8019DC5C D_8019DC5C[];

/* ---- this overlay ---- */

/* Card shoe: 6 decks of 52 cards. Card ids are rank * 4 + suit. */
typedef struct {
    u16 unk0; /* flags: 2 = shuffled, 4 = needs reshuffle */
    u16 unk2; /* next card */
    u8 unk4[6 * 52];
} Ovl15Shoe;

/* A blackjack hand. */
typedef struct {
    u16 unk0; /* flags: 1 = soft ace, 2 = bust, 8 = ace, 0x10 = hole card */
    u16 unk2; /* card count */
    u16 unk4; /* hand value */
    u16 unk6; /* value with aces as 1 */
    s16 unk8; /* screen x */
    s16 unkA; /* screen y */
    s32 unkC[22]; /* cards */
} Ovl15Hand;

typedef struct {
    void (*unk0)(void); /* init */
    void (*unk4)(void); /* enter */
    void (*unk8)(void); /* update */
    void (*unkC)(void); /* leave */
} Ovl15Mode;
extern Ovl15Mode D_800AE444[5];

typedef struct {
    u8 unk0; /* current mode, index into D_800AE444 */
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Ovl15ModeState;
extern Ovl15ModeState D_800B0FD8;

/* Per-mode screen state (D_800B0F50, D_800B0FE0, D_800B0FF0). */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3; /* done */
    u8 pad4[2];
    u8 unk6;
    u8 unk7;
    s32 unk8; /* frame counter */
    s32 unkC;
} Ovl15Screen;
extern Ovl15Screen D_800B0F50;
extern Ovl15Screen D_800B0FE0;
extern Ovl15Screen D_800B0FF0;

typedef struct {
    s16 x;
    s16 y;
} Ovl15Pos;
extern Ovl15Pos D_800AE3DC[4];
extern Ovl15Pos D_800AE494[1];
extern Ovl15Pos D_800AE498[4];
extern void *D_800AE440;

extern s8 D_800AE43C; /* sound request kind, -1 = none */
extern u16 D_800AE0E0; /* flags */
extern s32 D_800AE0E4; /* frame counter */
extern u16 D_800AE4C8[2][4]; /* queued flag sets / clears */
extern s32 D_800AE0E8; /* bankroll */
extern s16 D_800AE430;
extern u16 D_800AE432;
extern u8 D_800B0F44; /* double allowed */
extern s32 D_800B0F48; /* bet */
extern u8 D_800B0F4C; /* split allowed */
extern u32 D_800B0FD0;
extern SPRT D_800B0F60[2][2];
extern DR_TPAGE D_800B0FB0[2][2];
extern s32 D_800AE3EC;
extern u32 D_800AE3F0[16];
s32 func_800AC374(void);
s32 func_800AC448(void);
void func_800AC450(void);
void func_800AC494(void);
extern s16 D_800AE4A8;
extern s32 D_800AE4AC;
extern u8 *D_800AE4B0; /* next free primitive */
extern u32 D_800AE4C0; /* FMV state */
extern u32 D_800AE4B8; /* main state */
extern s32 D_800AE4D8;
extern s32 D_800AE4DC;
extern u8 D_800B1000[2][0x2000]; /* primitive buffers */
extern u32 D_800B5000;
extern RECT D_800AE4E0;
extern u32 D_800AE0EC[8]; /* CLUT */
extern DrTpageWords D_800AE4E8;
extern s32 D_800B0DF8;
extern SPRT D_800AE5F8[2][256];
/* Pip positions on a card face, [rank][pip]; rank 0-8 (cards 2-10) has rank + 2 pips. */
typedef struct {
    s32 x;
    s32 y;
} Ovl15Pip;
extern Ovl15Pip D_800AE10C[][10];

s32 func_800A8528(s32, s32);
s32 func_800A91F0(Ovl15Hand *, s16, s16);
s32 func_800A8630(s32, s32, s32, s32);
void func_800AB1C8(void);
void func_800A7838(void);
void func_800A787C(void);
s32 func_800A766C(s32, s32);
void func_800A7A84(void);
s32 func_800A7378(s32, s32);
void func_800A9488(void);
s32 func_800AB490(void);
void func_800ACB98(void);
void func_800ACC08(void);
void func_800ACE1C(void);
void func_800ACF00(void);
s32 func_800AD7DC(void);
void func_800AB34C(void);
void func_800AB37C(void);
void func_800AB3C4(void);
void func_800AB3CC(void);
void func_800AB3D4(void);
void func_800AB39C(void);
void func_800AD358(void);
void func_800AD644(void);
void func_800AB45C(void);
s32 func_800AC598(s32, s32, s32, s32, s32);
void func_800ACCB4(void);
void func_800ACD70(void);
void func_800ACF58(s32);
s32 func_800AD08C(s32);
u8 func_800AD0C4(s32, s32);
u8 func_800AD128(void);
void func_800AD1FC(void);
void func_800AD308(void);
void func_800AD338(void);
void func_800AD380(void);
void func_800AD388(void);
void func_800AD390(void);
void func_800AD418(void);
void func_800AD514(void);
void func_800AD5EC(void);
void func_800AD624(void);
void func_800AD678(void);
void func_800AD680(void);
void func_800AD688(void);
void func_800AD690(void);
void func_800AD728(void);
void func_800AD75C(void);
void func_800AD77C(void);
extern u8 D_800A70EC[]; /* message table */
s32 func_800ADC3C(void);
void func_800ADC98(void);
void func_800ADD04(void);
u8 *func_800ADE14(u8 *, s32);
void *func_800ADE30(s32, s32, u32 *, u8 *, void *);
s32 func_800ADF90(u8 *);
void *func_800AE028(s32, s32, u32 *, u32, s32 *, SprtWords *);
s32 func_800A9228(Ovl15Hand *, Ovl15Hand *);
s32 func_800A945C(Ovl15Hand *);

#endif
