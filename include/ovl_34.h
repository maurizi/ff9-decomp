#ifndef OVL_34_H
#define OVL_34_H

/* Declarations local to overlay 0x34 (config menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* Config menu state (D_801F1558). */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2; /* 0 = picking an item, else editing it */
    u8 unk3; /* blinking */
    u8 unk4; /* slider moving: 1 = right, else left */
    u8 unk5; /* slider ticks */
    u8 unk6; /* blink ticks */
    u8 unk7;
    u8 unk8[9];  /* per menu row: text id */
    u8 unk11[9]; /* per menu row: character index */
    u8 pad1A[2];
    WinDef unk1C[2]; /* option windows */
    MenuList unk3C; /* .unk5 = scroll top, .unk6 = cursor row */
    u8 *unk4C;      /* next free primitive */
    u8 *unk50[2];   /* primitive buffers */
    s32 unk58;
} ConfigMenu;

/* visible rows of the item list (func_801F1334) */
typedef struct {
    u8 unk0;  /* row count */
    u8 unk1;  /* first item */
    s16 unk2; /* y offset */
} ScrollPos;
extern ConfigMenu D_801F1558;
extern const u8 D_801EDF38[]; /* menu text block */

/* GameState.unk24 config bits, written through bitfields by func_801EF50C */
typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 1;
    u32 b3 : 1;
    u32 b4 : 1;
    u32 b5 : 1;
    u32 b6 : 1;
    u32 b7 : 1;
    u32 b8 : 3;
    u32 b11 : 3;
    u32 b14 : 3;
    u32 b17 : 1;
    u32 b18 : 1;
    u32 b19 : 1;
    u32 b20 : 12;
} ConfigBits;
#define CFG (*(ConfigBits *)&g_gameState->unk24)
extern RECT D_801F1534[2]; /* option window rects */
extern u8 D_801F1524[];
extern u8 D_801F1464[];
extern u8 D_801F14AC[]; /* window contents for func_80063118 */
extern s32 D_801F1474[8]; /* button mask per pad config slot */
extern s32 D_801F14F0; /* text ids */
extern s32 D_801F14F4;
extern u8 D_801F14F8[];
typedef struct {
    u8 unk0; /* text id */
    u8 unk1; /* icon */
} ConfigPadEntry;
extern ConfigPadEntry D_801F14FC[8];
/* func_800355DC; this overlay's link only has the D_ name */
void D_800355DC(void);

/* main exe: candidates for ff9.h */
extern s32 D_80077E54; /* = D_80077E50.unk4, referenced by its own name */

typedef struct {
    u8 unk0;    /* title text id */
    u8 unk1[3]; /* option text ids */
} ConfigText;
extern ConfigText D_801F13F0[13];
extern void (*D_801F1424[])(void);
extern void (*D_801F14BC[13])(RECT *, s32);

void func_801EECB4(void);
void func_801EED9C(void);
void func_801EEEF8(void);
void func_801EF110(void);
void func_801EF2B4(void);
void func_801EFA54(void);
void func_801EFB38(void);
void func_801F00F8(void);
void func_801F0184(void);
void func_801F025C(void);
void func_801EEB68(void);
void func_801EF3F8(void);
void func_801EF50C(u8);
void func_801EF938(void);
void func_801EFC20(s32, s32, s32, s32, s32);
void func_801F02CC(MenuList *, u32);
void func_801F0380(MenuList *, u32);
void func_801F0550(RECT *, s32, s32);
void func_801F0BE8(void);
void func_801F0C78(void);
void func_801F0D20(RECT *, s32, s32, s32);
void func_801F0DD8(RECT *, s32, s32, s32, s32);
void func_801F0FE8(s32, s32, s32, s32, s32);
void func_801F1074(void);
void func_801F10C8(void);
void func_801F1168(void);
u8 func_801F126C(void);
void func_801F12D0(u8);
ScrollPos *func_801F1334(MenuList *, ScrollPos *);
ScrollPos *func_801EFF58(MenuList *, ScrollPos *);
void func_801F043C(MenuList *, RECT *, RECT *);
void func_801F049C(void);
void func_801F0F48(s32, s32, s32, u8 *, s32);
void func_801F0F94(s32, s32, s32, s32, u8);
void func_801F11C4(s32, s32);
u8 func_801F125C(void);

/* main exe: candidates for ff9.h */
/* slider gauge pieces drawn by func_80027710: left cap, middle (repeated via DR_TWIN), right cap */
typedef struct {
    u8 unk0; /* texture window x */
    u8 unk1; /* texture window y */
    u16 unk2; /* uv */
    u8 unk4; /* width */
    u8 unk5; /* height */
    u8 pad6[2];
} GaugeIcon;
extern GaugeIcon D_80068988[3];
void SetTexWindow(void *p, RECT *r);

/* LINE_F2 written as whole words */
typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineWords;
extern u32 D_801F154C[3]; /* gauge line colours */
extern RECT D_801F1544; /* gauge texture window */


#endif
