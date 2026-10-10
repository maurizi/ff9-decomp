#ifndef OVL_65_H
#define OVL_65_H

/* Declarations local to overlay 0x65 (battle rewards: "EXP Received", loaded
 * at 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* other overlays (battle, 0x800A7000+ / 0x800E....) */
void func_800AD90C(s32, s32, s32, s32, s32);
void func_800E9DBC(void);
void func_800F4E68(void);

/* main exe: candidates for ff9.h */
void func_800240B0(u8);

/* Battle result (D_800FDF30). */
typedef struct {
    u32 unk0; /* gil */
    u32 unk4; /* exp (split among members) */
    u16 unk8; /* ap */
    u8 unkA;  /* bit per party slot: receives exp */
    u8 unkB;
    u8 unkC[0x10]; /* dropped item ids, 0xFF = none */
    u8 unk1C;      /* card */
} BattleResult;
extern BattleResult D_800FDF30;
extern u8 D_800FDF4D;

/* ---- overlay 0x65 ---- */

typedef struct {
    u32 unk0; /* target */
    u32 unk4; /* step per frame */
    u32 unk8; /* current */
} Counter;

typedef struct {
    Window *unk0;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u8 unkA[0x12];
    s32 unk1C;
} WinSlot; /* 0x20 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    s32 unk4;
    u8 pad8[0x14 - 8];
    u8 unk14;
    u8 unk15;
    s16 unk16;
} Popup; /* 0x18: "Level Up!" style label animation */

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 unk11[0x13];
} LearnList; /* 0x24 */

/* 0x88 bytes at D_801F7A78+0x24, cleared as a block */
typedef struct {
    u32 unk0; /* exp per member */
    u32 unk4; /* ap */
    s32 unk8; /* card, 0xFF = none */
    u32 unkC; /* item kinds */
    u8 unk10[6][2]; /* {item id, count} */
    Counter unk1C[4]; /* exp */
    Counter unk4C[4]; /* ap */
    Counter unk7C;    /* gil */
} Rewards;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5[3];
    u8 unk8; /* state, index into D_801F3B88 */
    u8 unk9; /* party members receiving exp */
    u16 unkA;
    u32 unkC;
    u32 unk10;
    u8 unk14[4]; /* gets exp */
    u8 unk18[4]; /* gets ap */
    u8 unk1C[4];
    u8 unk20[4];
    Rewards unk24;
    WinDef unkAC[10];
    u8 unk14C[10];
    u8 pad156[2];
    WinSlot unk158[4];
    WinSlot unk1D8[4];
    Popup unk258[4];
    LearnList unk2B8[4];
    WinSlot unk348;
    Window *unk368;
    s32 unk36C;
    s32 unk370;
    u8 *unk374; /* next free primitive */
    u8 *unk378[2];
} Unk801F7A78;

extern Unk801F7A78 D_801F7A78;

typedef struct {
    u8 pad0[0xE];
    u16 unkE; /* name offset into D_801F494C */
} Ovl65Ability; /* active ability table */
extern Ovl65Ability D_801F3D4C[];
extern u8 D_801F494C[]; /* ability names */
extern u8 D_801F4F94[]; /* support ability names */
extern RECT D_801F81F8;  /* level-up window rect */
extern RECT *D_801F8200; /* window it is placed under */
extern u8 D_801F5228[];  /* level-up text */
extern u8 D_801EDF04[];
extern void (*D_801F3B88[])(void);
extern s32 D_801F3BA0[]; /* sound effect per state */
extern s32 D_801F3BB8[]; /* text id */
extern u8 D_801F3BC0[];
extern u8 D_801F3BC4[];
typedef struct {
    RECT unk0;
    u8 unk8; /* initially shown */
    u8 pad9;
} WinInit;
extern WinInit D_801F3BC8[10];
extern u8 D_801F3C4C[10][0x10];
extern u8 D_801F3CDC[7][0x10];
extern u8 D_801EE03C[];
extern u8 D_801EE19C[];
extern s8 D_801F5230[];
extern RECT D_801F523C;
extern RECT D_801F5244;
extern RECT D_801F524C;
extern RECT D_801F5254;
extern RECT D_801F525C;
extern RECT D_801EF260; /* const {0, 0, 320, 224} */
extern RECT D_801EF27C; /* const {0, 0, 640, 224} */

/* Double-buffered display setup (func_801F3818). */
typedef struct {
    DRAWENV draw;
    DISPENV disp;
    u32 unk70[2]; /* ordering table */
    DR_TPAGE unk78;
    SPRT unk80;
    DR_TPAGE unk94;
    SPRT unk9C;
    DR_TPAGE unkB0;
    TILE unkB8;
} DispBuf; /* 0xC8 */
extern RECT D_801F5264;
extern u8 D_801F5278[2][0x1400]; /* primitive buffers */
extern u32 D_801F7DF8[0x100];   /* VRAM backup */
extern u8 D_801F8278;

static inline u8 func_801F_alive(Character *c) {
    if (c == NULL) {
        return 0;
    }
    if (c->unk10 == 0) {
        return 0;
    }
    if (c->unk38 & 0x45) {
        return 0;
    }
    return 1;
}

/* Member can still gain exp (alive, exp below the cap). */
static inline u8 func_801F_canExp(Character *c) {
    if (!func_801F_alive(c)) {
        return 0;
    }
    if (c->unkC > 9999998) {
        return 0;
    }
    return 1;
}

/* Member can learn abilities (alive, func_80034A30). */
static inline u8 func_801F_canAp(Character *c) {
    if (!func_801F_alive(c)) {
        return 0;
    }
    if (!func_80034A30(c)) {
        return 0;
    }
    return 1;
}

/* Amount to add to a counter this frame. */
static inline u32 Counter_step(Counter *c) {
    u32 step;
    if (c->unk0 < c->unk8 + c->unk4) {
        step = c->unk0 - c->unk8;
    } else {
        step = c->unk4;
    }
    return step;
}

static inline void WinSlot_clear(WinSlot *s) {
    s->unk4 = 0;
    s->unk6 = 0;
    s->unk1C = 0;
    s->unk8 = 0;
    s->unk0 = 0;
}

extern u8 D_801F8208[0x30]; /* per ability slot: equipped items teaching it */
s32 func_80034430(s32, s32); /* main exe */

void func_801EF28C(void);
void func_801F3818(DispBuf *);
void func_801F3A20(s32);
s32 func_801F3A7C(void);
void func_801F3488(s32);
void func_801EF45C(void);
void func_801EF5C8(void);
s32 func_801EF6D8(void);
s32 func_801EF6F8(void);
s32 func_801EF814(void);
void func_801EF890(void);
void func_801EFB98(void);
void func_801EFC68(void);
void func_801EFF50(void);
void func_801F0044(void);
void func_801F0138(void);
void func_801F0264(void);
void func_801F0394(RECT *);
void func_801F04BC(void);
void func_801F0594(void);
void func_801F076C(void);
void func_801F0878(void);
void func_801F0978(void);
void func_801F0A44(void);
void func_801F0C0C(void);
void func_801F0D98(s32, s32, s32, u8 *, s32);
void func_801F0DE4(s32, s32, s32, u8 *, s32);
void func_801F0E30(void);
u8 func_801F0F04(void);
void func_801F0FC4(void);
void func_801F0FF4(void);
s32 func_801F1004(void);
void func_801F1060(void);
void func_801F107C(void);
void func_801F114C(void);
void func_801F1160(void);
void func_801F11B8(void);
void func_801F1228(void);
void func_801F12E0(void);
void func_801F1300(WinSlot *);
void func_801F1318(void);
void func_801F13B4(void);
void func_801F143C(void);
void func_801F14C0(void);
void func_801F1518(void);
void func_801F1560(void);
void func_801F160C(void);
void func_801F1688(void);
u8 func_801F171C(Character *, s32);
void func_801F16F4(void);
void func_801F1774(void);
void func_801F17D4(void);
void func_801F17FC(s32, s32, s32, u8 *, s32);
void func_801F1848(void);
void func_801F1A04(s32, u32);
u8 func_801F1BA4(Character *, s32);
void func_801F1D94(Window *);
void func_801F1FEC(void);
void func_801F2080(void);
s32 func_801F20A8(s32);
void func_801F213C(void);
void func_801F21DC(s32, u8);
void func_801F2248(Window *);
void func_801F2264(s32, s32, s32, Popup *);
void func_801F2490(void);
void func_801F2538(void);
void func_801F2560(s32, u32);
void func_801F269C(s32);
u32 func_801F26F8(s32);
s32 func_801F27F0(Character *, s32);
void func_801F2834(void);
void func_801F28F0(void);
void func_801F29BC(s32);
void func_801F2A4C(Popup *);
void func_801F2A6C(Popup *);
void func_801F2AAC(Popup *);
s32 func_801F2AC8(Popup *);
void func_801F2BBC(void);
void func_801F2C7C(void);
void func_801F2D08(s32);
void func_801F2D74(void);
void func_801F2E24(void);
u8 func_801F2E58(void);
void func_801F2FE8(Window *);
void func_801F3250(void);
u8 func_801F3264(void);
void func_801F325C(void);
void func_801F332C(void);
void func_801F33A0(Window *);
void func_801F3434(Window *);
void func_801F347C(void);

extern u8 D_801F8238[]; /* text buffer */
extern Icon D_801F526C; /* card icon */
/* main exe: candidates for ff9.h */
s32 func_8003579C(s32); /* text size of a menu string */

#endif
