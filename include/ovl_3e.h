#ifndef OVL_3E_H
#define OVL_3E_H

/* Declarations local to overlay 0x3E: the "insert disc" / disc swap screen.
 * Checks the CD lid and disc type, shows a two-part 160x224 picture and waits
 * for the right disc. Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- overlay ---- */

typedef struct {
    u8 unk0;  /* result of cd_checkShellOpen */
    u8 unk1;  /* state, index into the handler tables */
    u8 unk2;
    u8 unk3;  /* last func_801F6368 result */
    u8 unk4;  /* wanted disc (0-3) */
    u8 pad5;
    s16 unk6;
    s32 unk8[2]; /* saved g_padPortMask[] */
    u32 unk10;   /* frame counter */
} Ovl3eState;
extern Ovl3eState D_801F6BBC;

extern void (*D_801F6B60[])(void);
extern void (*D_801F6B74[])(void);
extern void (*D_801F6B88[])(void);
extern RECT D_801F6B9C;
extern RECT D_801F6BA4;
extern s32 D_801F6BAC[];
extern RECT D_801EDF04;
extern u32 D_801EDF0C[];

void func_801F6124(void);
void func_801F6254(void);
u8 func_801F6368(void);
void func_801F6654(void);
void func_801F66AC(void);
void func_801F66F4(void);
void func_801F673C(void);
void func_801F682C(u32 *, s32);
void func_801F6A3C(void);
void func_801F6A78(void);
void func_801F6AF0(void);
void func_801F6B24(void);

#endif
