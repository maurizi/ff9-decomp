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
    u8 pad4[2];
    u8 unk6; /* blink ticks */
    u8 pad7[0x1C - 7];
    WinDef unk1C;
    WinDef unk2C;
    MenuList unk3C; /* .unk5 = scroll top, .unk6 = cursor row */
    u8 *unk4C;      /* next free primitive */
} ConfigMenu;
extern ConfigMenu D_801F1558;

extern u8 D_801F13F0[13][4]; /* per item: {title, option 0, option 1, ...} text ids */
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
void func_801F0D20(RECT *, s32, s32, s32);
void func_801F0DD8(RECT *, s32, s32, s32, s32);
void func_801F0FE8(s32, s32, s32, s32, s32);
void func_801F1074(void);
void func_801F10C8(void);
void func_801F1168(void);
u8 func_801F126C(void);
void func_801F12D0(u8);

#endif
