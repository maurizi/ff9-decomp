#ifndef OVL_65_H
#define OVL_65_H

/* Declarations local to overlay 0x65 (battle rewards: "EXP Received", loaded
 * at 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */

/* PsyQ libgpu (candidates for psyq.h) */
typedef struct {
    u32 tag;
    u32 code[15];
} DR_ENV;

typedef struct {
    RECT clip;
    s16 ofs[2];
    RECT tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct {
    RECT disp;
    RECT screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0, pad1;
} DISPENV;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 w, h;
} SPRT;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u32 tag;
    u32 code[1];
} DR_TPAGE;

#define setRECT(r, _x, _y, _w, _h) ((r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h))
#define setXY0(p, _x0, _y0) ((p)->x0 = (_x0), (p)->y0 = (_y0))
#define setUV0(p, _u0, _v0) ((p)->u0 = (_u0), (p)->v0 = (_v0))
#define setWH(p, _w, _h) ((p)->w = (_w), (p)->h = (_h))
#define setRGB0(p, _r0, _g0, _b0) ((p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0))

void ClearImage(RECT *rect, s32 r, s32 g, s32 b);
DRAWENV *SetDefDrawEnv(DRAWENV *env, s32 x, s32 y, s32 w, s32 h);
DISPENV *SetDefDispEnv(DISPENV *env, s32 x, s32 y, s32 w, s32 h);
void SetSemiTrans(void *p, s32 abe);
u16 GetTPage(s32 tp, s32 abr, s32 x, s32 y);

/* File table entry returned by func_8001E218 / func_80021E5C / func_80063088. */
typedef struct {
    s32 unk0;
    s32 unk4; /* start sector */
    s32 unk8;
    s32 unkC; /* end sector / next id */
} FileEntry;

FileEntry *func_8001E218(s32, s32);
FileEntry *func_80063088(s32, s32);
s32 func_800220E8(s32);
void func_8001DAA4(s32, s32, s32, s32);
void func_80022390(s32);
void func_8002177C(void *, s32, s32, s32, s32, s32, s32);
s32 func_800217C4(void *);
void func_80022DF0(u8 *, s32, s32);
void func_8001D8A8(s32);
void func_8001CFB4(void);
void func_80021C60(void);
s32 func_80021CFC(void);
void func_80021CDC(s32);
void func_8004A06C(s32);
void func_8004BFB0(void);
void func_800AD90C(s32, s32, s32, s32, s32);
void func_80063580(DR_TPAGE *, s32);
void func_800635E0(DR_TPAGE *, s32);

/* Party member record (GameData.unk54C[i]); fuller view of ff9.h's Unk54C. */
typedef struct {
    u8 unk0[0xB]; /* name */
    u8 unkB;      /* level (exp table has 99 entries) */
    u32 unkC;     /* exp, capped at 9999999 */
    u16 unk10;    /* hp */
    u8 pad12[0x34 - 0x12];
    u32 unk34; /* low nibble: character id; bits 12-16: ability table row */
    u8 unk38;  /* status; 0x45 = dead/stone/... (can't gain exp) */
    u8 unk39[5]; /* equipment item ids (0xFF = none) */
    u8 pad3E[0x58 - 0x3E];
    u8 unk58[0x30]; /* learned AP per ability slot */
    u8 unk88[1];
} PartyMember;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} Unk1354;

typedef struct {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[0x10 - 7];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    u8 *unk1C; /* load address */
} Unk11C0;

typedef struct {
    u8 pad0[0x11C0];
    Unk11C0 *unk11C0;
    u8 pad11C4[0x1354 - 0x11C4];
    Unk1354 *unk1354;
} Unk8E4;

/* Fuller view of ff9.h's GameData (D_8006794C->unk1C). */
typedef struct {
    u32 unk0; /* flags */
    u8 pad4[4];
    u8 unk8; /* double-buffer index */
    u8 pad9[3];
    u16 unkC;
    u8 padE[2];
    u32 *unk10[2]; /* ordering tables */
    u8 pad18[0x3A - 0x18];
    u16 unk3A;
    u8 pad3C[0x54C - 0x3C];
    PartyMember *unk54C[5];
    u32 unk560; /* gil, capped at 9999999 */
    u8 pad564[0x56E - 0x564];
    u8 unk56E;
    u8 unk56F;
    u8 pad570[0x7E4 - 0x570];
    s32 unk7E4;
    u16 unk7E8;
    u16 unk7EA;
    u8 pad7EC[0x7FC - 0x7EC];
    s32 unk7FC;
    s32 unk800;
    s32 unk804;
    s32 unk808;
    u8 pad80C[0x81C - 0x80C];
    u16 unk81C;
    u8 pad81E[0x854 - 0x81E];
    u8 unk854[0x8AC - 0x854];
    s32 unk8AC;
    u8 pad8B0[0x8BC - 0x8B0];
    s32 unk8BC;
    u8 pad8C0[0x8E4 - 0x8C0];
    Unk8E4 *unk8E4;
} GameDataX;

typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 unk9; /* VSync mode */
    u8 padA[2];
    u8 *unkC;  /* DISPENV[2] (0x14 each) */
    u8 *unk10; /* DRAWENV[2] (0x5C each) */
    u8 pad14[0x1C - 0x14];
    GameDataX *unk1C;
    u8 *unk20; /* file load buffer */
} GameStateX;

#define GSTATE ((GameStateX *)D_8006794C)
#define GDATA (GSTATE->unk1C)

/* Menu window object returned by func_8002DC54. */
typedef struct Window {
    s16 unk0; /* x */
    s16 unk2; /* y */
    s16 unk4; /* w */
    s16 unk6; /* h */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u8 pad10[0x18 - 0x10];
    s16 unk18; /* OT layer */
    u8 pad1A[2];
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50; /* user data */
} Window;

/* Item table entry (D_80069018, 0x12 bytes); fuller view of Unk80069018. */
typedef struct {
    u16 unk0; /* name offset into D_8006A218 */
    u8 pad2[0xC - 2];
    u8 unkC[3]; /* abilities taught */
    u8 unkF;
    u8 pad10[2];
} ItemX;
#define ITEMS ((ItemX *)D_80069018)

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
extern u32 D_8006DF5C[]; /* exp needed for next level, by level */
extern s32 D_80077528;   /* pad buttons pressed */
extern u8 D_80077DB5;
extern u8 *D_80067944; /* end of load area */
extern s32 D_80067948;
extern u16 D_80067950;

void ClearOTagR(u32 *ot, s32 n);
void DrawOTagEnv(u32 *p, void *env);
void PutDispEnv(void *env);
void ResetGraph(s32 mode);
void SetDrawTPage(void *p, s32 dfe, s32 dtd, s32 tpage);
u16 GetClut(s32 x, s32 y);

void func_8001C750(s32, s32);
void func_8002C308(s32);
void func_8002DD00(Window *);
void func_8002F488(s32);
void func_8002F514(s32);
void func_8002F55C(s32);
u8 func_8002F810(void);
s32 func_8002F8E4(s32);
void func_8002F8A8(void (*)(void));
void func_8002FA50(s32);
void func_8002FAAC(s32);
void func_8002DE60(void);
void func_800E9DBC(void);
void func_800239D0(void);
void func_80024BD8(void);
void func_8002C064(s32);
void func_8002DEE4(RECT *, s32, s32);
void func_8002705C(RECT *, s32, s32);
void func_8002B524(s32, s32, s32, s32, s32);
void func_8002F9D0(void);
s32 func_80035774(s32); /* text pointer */
void func_8003575C(u8 *);
void func_800325E0(void);
void func_80063108(u8 *, u8 *);
void func_80063118(void *, void *, s32);
void func_800631AC(void *);
u8 func_800349BC(s32, s32);
u8 func_80034A30(PartyMember *);
void func_80033CCC(s32, s32);
Window *func_8002DC54(RECT *, s32, s32);
void func_8002E2EC(Window *, void (*)(Window *));
void func_8002E2F4(Window *, void (*)(Window *));
void func_8002E2FC(Window *, void (*)(Window *));
void func_8002E304(Window *, void *);
void func_8002E31C(Window *, s32);
void func_8002E334(Window *, s32);
s32 func_8002AE7C(s32);
void func_8002B56C(s32, s32, s32, s32, s32);
void func_8002B5FC(s32, s32, s32, s32, s32);
typedef struct {
    u8 pad0[0x68];
    s32 unk68;
    u8 pad6C[0x94 - 0x6C];
    s32 unk94;
} Unk800703FC; /* text table? */
extern Unk800703FC D_800703FC;
extern s32 D_80070450;
extern s32 D_80070460;

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

typedef struct {
    u8 pad0[4];
    RECT unk4;
    s32 unkC;
} WinDef; /* 0x10 */

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
    u8 pad70[8];
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

static inline u8 func_801F_alive(PartyMember *c) {
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
void func_801F0D98(s32, s32, s32, s32, s32);
void func_801F0DE4(s32, s32, s32, s32, s32);
void func_801F0E30(void);
s32 func_801F0F04(void);
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
u8 func_801F171C(PartyMember *, s32);
void func_801F16F4(void);
void func_801F1774(void);
void func_801F17D4(void);
void func_801F17FC(s32, s32, s32, u8 *, s32);
void func_801F1848(void);
void func_801F1A04(s32, u32);
u8 func_801F1BA4(PartyMember *, s32);
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
s32 func_801F27F0(PartyMember *, s32);
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

#endif
