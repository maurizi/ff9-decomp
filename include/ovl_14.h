#ifndef OVL_14_H
#define OVL_14_H

/* Declarations local to overlay 0x14 (loaded at 0x800A7000). Addresses in the
 * overlay range mean different things in different overlays, so they live here,
 * not in ff9.h. */
#include "ff9.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl14Text; /* 8-byte item drawn by func_80027710 */

/* other overlay (0x801F....) */
u8 func_801F5330(void);
void func_8019A9D8(void);

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
extern s32 D_800AB2B4[4];
extern u32 D_800AB310;
extern u8 D_800AB318;

typedef struct {
    s16 x;
    s16 y;
} Ovl14Pos;
extern Ovl14Pos D_800AB298;
extern Ovl14Pos D_800AB29C;
extern Ovl14Pos D_800AB2A0;
extern Ovl14Pos D_800AB2A4;
extern Ovl14Pos D_800AB2A8[3];
extern u16 D_800AB2C4;
extern void *D_800AB2D4;
extern void *D_800AB2F0;
extern s32 D_800AB3C8;

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
    s32 unk14[4]; /* sound/voice handles */
    Ovl14Text unk24[14];
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
s32 func_800A811C(void);
void func_800A812C(s32);
s32 func_800A81B8(void);
s32 func_800A8260(s32);
u8 func_800A8298(s32, s32);
void func_800A82F4(s32);
void func_800A8378(void);
void func_800A84B0(void);
void func_800A84EC(void);
void func_800A8618(void);
void func_800A86D8(void);
void func_800A87AC(void);
void func_800A87E4(void);
void func_800A8868(void *);
void func_800A8A0C(void);
void func_800A8A8C(void);
void func_800A8A94(void);
void func_800A8B7C(void);
void func_800A8C60(void);
void func_800A8CF8(void);
void func_800A8DB0(void);
void func_800A8E10(void);
u8 func_800A8EA4(void);
void func_800A8FBC(void);
void func_800A9180(void);
void func_800A8ED8(void);
s32 func_800A8F44(void);
void func_800A903C(s32, s32, s32, Ovl14Text *, s32);
u8 func_800A832C(void);
void func_800A880C(void);
void func_800A8D2C(void);
s32 func_800A8FCC(void);
void func_800A8FDC(void);
void func_800AB1E4(void);
void func_800AB200(void);

#endif
