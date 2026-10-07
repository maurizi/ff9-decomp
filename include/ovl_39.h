#ifndef OVL_39_H
#define OVL_39_H

/* Declarations local to overlay 0x39 (save / load menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Save menu state (D_801FB1A8). */
typedef struct {
    u8 unk0;
    u8 unk1; /* state */
    u8 unk2; /* 1 = busy, 2 = done */
    u8 pad3;
    u8 unk4[2]; /* card status per slot */
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[0x14 - 9];
    s32 unk14; /* saved func_8002F8E4 mode */
    s32 unk18[2]; /* saved D_80073998[] */
    u8 *unk20; /* help text */
    u8 pad24[0x29 - 0x24];
    u8 unk29; /* list scroll */
    u8 unk2A; /* list cursor */
    u8 pad2B[0x4E - 0x2B];
    u8 unk4E; /* slot 0 state; 15 slots of 0x98 bytes */
} SaveMenu;
extern SaveMenu D_801FB1A8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk801FBAC8;
extern Unk801FBAC8 D_801FBAC8;

typedef struct Unk801FBEE0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    u8 pad8[4];
    void (*unkC)(struct Unk801FBEE0 *);
    u16 unk10; /* text ids */
    u16 unk12;
} Unk801FBEE0;
extern Unk801FBEE0 D_801FBEE0;

typedef struct {
    u8 unk0;
    u8 unk1; /* state, index into D_801FB014 */
    u8 pad2[0xC - 2];
    s32 unkC;  /* used slots */
    s32 unk10; /* file count */
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    Window *unk1C;
} Unk801FBEF8;
extern Unk801FBEF8 D_801FBEF8;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    Window *unk8;
} Unk801FC348;
extern Unk801FC348 D_801FC348;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    Window *unk4;
} Unk801FC358;
extern Unk801FC358 D_801FC358;

typedef struct {
    u8 unk0;
    u8 pad1;
    u8 unk2;
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0x1C - 7];
    Window *unk1C;
    s32 unk20; /* card channel */
} Unk801FC388;
extern Unk801FC388 D_801FC388;

/* memory card status */
extern s32 D_801FB074;
extern s32 D_801FB078;
extern s32 D_801FB07C;
extern s32 D_801FB080;
extern s32 D_801FB084;
extern s32 D_801FB088;
extern s32 D_801FB08C;
extern s32 D_801FC604;
extern void (*D_801FB098)(void); /* memory card completion callback */
extern s32 D_801FB09C;
extern s32 D_801FB0A0;
extern u8 *D_801FC360;
extern u8 D_801EE4D4[];
extern u16 D_801FC3E0[256]; /* CRC-16 table */
extern s32 D_801FC5E0;
extern s32 D_801FC5E4;
extern s32 D_801FC5E8;
extern s32 D_801FC5EC;
extern s32 D_801FC5F4;
extern s32 D_801FC5F8;
extern s32 D_801FC5FC;
extern s32 D_801FC600;
extern s32 D_801FC608;
extern s32 D_801FC60C;
extern s32 D_801FC610;
extern s32 D_801FC614;

typedef struct {
    s32 unk0;
    Unk880 unk4;
    u8 unk14;
} Unk801FA078;

typedef struct {
    u8 pad0[0x12];
    u8 unk12; /* ASCII digits */
    u8 unk13;
} Unk801F6D24;

void func_801F4538(void);
void func_801F53E0(void);
void func_801F5460(void);
void func_801F54C8(void);
void func_801F5550(void);
void func_801F5A24(void);
void func_801F5A68(s32, s32, s32, s32);
void func_801F75C8(s32, s32);
void func_801F7084(void);
void func_801F5E68(s32, s32, s32);
void func_801F6A84(void);
void func_801F6B48(Unk801FBEF8 *);
void func_801F72B4(s32);
u8 func_801F77E4(void);
void func_801F9788(s32, s32);
void func_801F94D0(s32, s32);
s32 func_801FA53C(void);
void func_801FAE74(void);
s32 func_801FA2D8(void);
s32 func_801FA350(s32, void (*)(void));
s32 func_801FA754(s32);
void func_801F65C4(void);
void func_801F7300(s32);
void func_801F7364(SaveMenu *);
void func_801F77A8(s32);
void func_801F7918(void);
void func_801F889C(s32);
s32 func_801FAB08(void);
void func_801FAE1C(void);
void func_801F7780(s32);
u8 func_801F79F4(s8);
void func_801F9C58(void *, void *);
s32 func_801FA37C(s32);
s32 func_801FA4E4(s32, s32, s32);
s32 func_801FA5D0(s32, s32, s32);
void func_801FAAF4(void);
void func_801F4E00(void);
void func_801F5D00(void);
u8 func_801F6694(void);
s32 func_801F6CD0(s32);
void func_801F7108(s32);
void func_801F7950(void);
void func_801F89A0(Window *);
s32 func_801FA478(s32);
s32 func_801FA184(s32);
s32 func_801FABB8(void);
void func_801FAECC(void);
u8 func_801F5340(s32, u8);
extern void (*D_801FAFA8[])(void);
extern void (*D_801FB014[])(void);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x20 - 2];
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u8 unk26;
    u8 pad27;
    u16 unk28;
    u16 unk2A;
} Unk801F9BEC;

#endif
