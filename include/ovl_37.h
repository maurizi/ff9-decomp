#ifndef OVL_37_H
#define OVL_37_H

/* Declarations local to overlay 0x37 (items menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* Items menu state (D_801FAB78). */
typedef struct {
    u8 unk0; /* active list, index into D_801F3040 */
    u8 unk1; /* state, index into D_801F307C */
    u8 unk2; /* party slot */
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
    MenuList unkC;  /* item list */
    MenuList unk1C; /* key item list */
    RECT unk2C;
    RECT unk34;
    WinDef unk3C;
    Window *unk4C;
    u32 unk50; /* timer */
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60[0x50]; /* key item ids */
    s32 unkB0;      /* key item count */
} ItemMenu;
extern ItemMenu D_801FAB78;

typedef struct {
    void (*unk0)(s32, MenuList *);
    void (*unk4)(s32, s32, s32, MenuView *);
    MenuList *unk8;
} ItemList; /* 12 bytes */
extern ItemList D_801F3040[2];
extern s8 D_801F3064[24];
extern void (*D_801F307C[])(void);
extern u8 D_801F3120[2][0x10];
extern RECT D_801F3158;
extern RECT D_801F3160;
extern u8 D_801EDFD8[];

extern s32 D_801F309C[3];
extern u8 D_801F3098[3];
extern RECT D_801F30B8;

/* main exe: candidates for ff9.h */
typedef struct {
    unsigned int unk0 : 4;   /* target kind */
    unsigned int unk0_4 : 1;
    unsigned int unk0_5 : 3;
    unsigned int pad0_8 : 24;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
} Unk8006B1F4; /* 0xC, items 0xE0.. (also in ovl_0b.h) */
extern Unk8006B1F4 D_8006B1F4[];
u8 func_800654F8(Character *, Character *, Unk8006B1F4 *, s32, s32);
extern s32 D_80070408;
/* func_800355DC; this overlay's link only has the D_ name */
void D_800355DC(void);
void func_801F3038(void);
void func_801F2CEC(void);
void func_801F2EDC(void);
void func_801F26E0(void);
extern RECT *D_801F30A8;
extern RECT D_801FA958;

extern u8 D_801F30E0[2][0x10];
extern RECT D_801F3148;
extern u8 D_801F3140[];

/* other overlay */
s8 func_800B8124(void);
void func_800B81A0(void);

extern UnkPair D_801FA968[0x100]; /* inventory sort buffer */
extern UnkPair D_801FAB68;

void func_801F0D18(void);
void func_801F1778(void);
void func_801F2590(s32);
void func_801F2C58(void);
u8 func_800281D0(s32); /* main exe */
void func_80026C34(s32, s32, s32, s32, u8); /* main exe */
void func_801F2B20(Window *);
void func_801F2B58(Window *);
void func_801F2D58(void);
void func_80028128(s32); /* main exe */
void func_80026EA0(s32, s32, s32, s32, s32); /* main exe */
void func_801F187C(void);
void func_801F24DC(void);
void func_801F2E18(void);
void func_801F2E40(Window *);
void func_801F2EBC(Window *);
void func_801F2ECC(Window *);
void func_801F0E10(void);
void func_801F0F50(void);
void func_801F10C4(s32, MenuList *);
void func_801F11E4(void);
void func_801F13F8(void);
void func_801F1B78(void);
void func_801F1D88(void);
void func_801F1E88(void);
void func_801F21E8(void);
void func_801F2240(void);
void func_801F2280(void);
void func_801F22C8(void);
void func_801F23C4(void);
void func_801F2488(void);
void func_801F253C(s32);
void func_801F262C(void);
void func_801F27A0(void);
void func_801F2870(s32, s32, s32, MenuView *);
void func_801F29D0(s32, s32, s32, MenuView *);
void func_801F2BC0(void);
void func_801F2F30(void);
void func_801F2F38(s32);
void func_801F2FA0(void);

#endif
