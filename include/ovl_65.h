#ifndef OVL_65_H
#define OVL_65_H

/* Declarations local to overlay 0x65 (battle rewards: "EXP Received", loaded
 * at 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */

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
    s32 unk1C;
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
    s32 unkC;
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
extern s32 D_80067944;
extern s32 D_80067948;
extern u16 D_80067950;
extern s16 D_8007AEF0;

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
void func_8003556C(s32);
void func_800E9DBC(void);

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
    s16 unk4[4]; /* rect */
    u8 unkC;
    u8 padD[3];
} WinDef; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad5[3];
    u8 unk8; /* state, index into D_801F3B88 */
    u8 unk9; /* party members receiving exp */
    u8 unkA;
    u8 padB;
    u32 unkC;
    u32 unk10;
    u8 unk14[4]; /* gets exp */
    u8 unk18[4]; /* gets ap */
    u8 unk1C[4];
    u8 unk20[4];
    /* 0x24: cleared as a block (0x88 bytes) */
    u32 unk24; /* exp per member */
    u32 unk28; /* ap */
    s32 unk2C; /* card, 0xFF = none */
    u32 unk30; /* item kinds */
    u8 unk34[6][2]; /* {item id, count} */
    Counter unk40[4]; /* exp */
    Counter unk70[4]; /* ap */
    Counter unkA0;    /* gil */
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
extern void (*D_801F3B88[])(void);
extern s32 D_801F3BA0[];
extern s32 D_801F3BB8[];
extern RECT D_801F525C;
extern u8 D_801F5278[2][0x1400]; /* primitive buffers */
extern u32 D_801F7DF8[0x100];   /* VRAM backup */
extern u8 D_801F8278;

s32 func_801EF6D8(void);
s32 func_801EF814(void);
void func_801F0FC4(void);
void func_801F0FF4(void);
void func_801F1060(void);
void func_801F114C(void);
void func_801F12E0(void);
void func_801F1300(WinSlot *);
void func_801F160C(void);
void func_801F16F4(void);
void func_801F17D4(void);
void func_801F1848(void);
void func_801F2080(void);
void func_801F213C(void);
void func_801F2248(Window *);
void func_801F2538(void);
void func_801F2834(void);
void func_801F28F0(void);
void func_801F2A4C(Popup *);
void func_801F2A6C(Popup *);
void func_801F2AAC(Popup *);
void func_801F2E24(void);
void func_801F3250(void);
void func_801F332C(void);
void func_801F347C(void);

#endif
