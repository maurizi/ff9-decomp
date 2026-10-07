#ifndef OVL_14_H
#define OVL_14_H

/* Declarations local to overlay 0x14 (loaded at 0x800A7000). Addresses in the
 * overlay range mean different things in different overlays, so they live here,
 * not in ff9.h. */
#include "ff9.h"

/* ---- main exe: candidates for ff9.h ---- */

typedef struct {
    RECT disp;
    RECT screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0;
    u8 pad1;
} DISPENV; /* 0x14 */

typedef struct {
    u8 pad0[0x5C];
} DRAWENV; /* opaque, 0x5C */

int ResetGraph(int mode);
void SetDispMask(int mask);
u32 *ClearOTagR(u32 *ot, int n);
void DrawOTag(u32 *p);
void PutDrawEnv(DRAWENV *env);
void DrawOTagEnv(u32 *p, DRAWENV *env);
DISPENV *PutDispEnv(DISPENV *env);
void *memmove(void *dst, const void *src, int n);

/* D_8006794C->unk1C as this overlay sees it (the ff9.h GameData layout does
 * not fit here: offset 0 is a flag word and 0x10 holds the two OT pointers). */
typedef struct {
    u32 unk0; /* flags */
    u8 pad4[4];
    u8 unk8; /* draw buffer index (0/1) */
    u8 unk9;
    u8 padA[2];
    s16 unkC;
    u8 padE[2];
    u32 *unk10[2]; /* ordering tables, 0x1000 entries */
    u8 pad18[0x38 - 0x18];
    u8 unk38;
    u8 unk39;
    u16 unk3A;
    u8 pad3C[0x7DC - 0x3C];
    s16 unk7DC;
    u16 unk7DE;
    u8 pad7E0[0x7E8 - 0x7E0];
    u16 unk7E8;
    u16 unk7EA;
    s32 unk7EC[2];
    s32 unk7F4[2];
    s32 unk7FC;
    s32 unk800;
    s32 unk804;
    s32 unk808;
    u8 pad80C[0x814 - 0x80C];
    void *unk814;
    u8 pad818[0x81C - 0x818];
    u16 unk81C[2];
    u16 unk820[2];
    u8 unk824[2];
    u8 unk826[2];
    u8 pad828[0x83C - 0x828];
    void (*unk83C)();
    u16 unk840;
    u16 unk842;
    u8 pad844[0x84C - 0x844];
    s32 unk84C;
    u8 unk850;
    u8 pad851[0x854 - 0x851];
    u8 unk854[0x8AC - 0x854]; /* CD read context (func_8002177C) */
    s32 unk8AC;
    u16 unk8B0;
    u8 pad8B2[2];
    u16 unk8B4[2];
    u8 pad8B8[0x8C4 - 0x8B8];
    s32 unk8C4; /* VSync(-1) timestamp */
} Ovl14Ctx;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x24 - 0xC];
    s32 unk24;
    s32 unk28;
    u8 pad2C[0x34 - 0x2C];
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x44 - 0x3C];
    s32 unk44;
    s32 unk48;
} Ovl14Unk14;

/* GameState (D_8006794C) as this overlay sees it */
typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 unk9; /* VSync mode */
    u8 unkA;
    u8 unkB; /* pad / connection bits, 0xFF = none */
    DISPENV *unkC;  /* [2] */
    DRAWENV *unk10; /* [2] */
    Ovl14Unk14 *unk14;
    u8 pad18[4];
    Ovl14Ctx *unk1C;
} Ovl14State;

#define G14 ((Ovl14State *)D_8006794C)

extern u8 *D_80067944;

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl14Pair;
extern Ovl14Pair D_800739B0[2];

void func_8001C750(void *, s32);
s32 func_8001C7FC(s32, s32, s32);
void func_8001CCB4(s32, s32, s32);
void func_8001DCB8(void);
void func_8002177C(void *, s32, s32, void *, s32, s32, s32);
s32 func_800217C4(void *);
void func_8002306C(void);
void func_800239D0(void);
void func_8002C0DC(s32);
void func_8002C4B0(s32);
u8 func_8002F810(void);
void func_80032120(s32, s32);
void func_80032574(void);
void func_80032628(void (*)());
void func_80032634(s32);
void func_8003253C(void);
void func_80033D1C(s32);
void func_8004BBF0(void);
void func_80050A84(void);
void func_8005392C(void);
void func_800551F0(void);
void func_80055D40(void);
void func_80055D60(void);
void func_80055DF4(void);
void func_80055E14(s32, s32, s32);
void func_80062344(void);
void func_8006237C(void);
void func_800651D8(void);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
} Ovl14FileEntry;
Ovl14FileEntry *func_8001E218(s32, s32);

/* other overlay (0x801F....) */
u8 func_801F5330(void);

/* ---- this overlay ---- */

typedef struct {
    void (*unk0)(void); /* init */
    void (*unk4)(void); /* enter */
    void (*unk8)(void); /* update */
    void (*unkC)(void); /* leave */
} Ovl14Mode;
extern Ovl14Mode D_800AB238[6];

typedef struct {
    u8 unk0; /* current mode, index into D_800AB238 */
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Ovl14ModeState;
extern Ovl14ModeState D_800AB308;
extern u32 D_800AB310;
extern u8 D_800AB318;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4[2];
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
    s32 unkC;
    s32 unk10;
} Ovl14Unk320;
extern Ovl14Unk320 D_800AB320;

extern void *D_800AB230;

void func_800A7578(void);
void func_800A75C8(void);
void func_800A7600(void);
void func_800A7768(void);
void func_800A780C(void);
void func_800A78C0(void);
void func_800A796C(void);
void func_800A799C(void);
u8 func_800A811C(void);
void func_800A812C(s32);
s32 func_800A81B8(void);
s32 func_800A8260(s32);
s32 func_800A8298(s32, s32);
void func_800A82F4(s8);
void func_800A8378(void);
void func_800A84B0(void);
void func_800A84EC(void);
void func_800A8618(void);
void func_800A880C(void);
void func_800A8D2C(s32);
s32 func_800A8FCC(void);
void func_800A8FDC(void);
void func_800AB1E4(void);
void func_800AB200(void);

#endif
