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
    u8 pad2[6];
    u8 unk8;
    u8 pad9[3];
    MenuList unkC;  /* item list */
    MenuList unk1C; /* key item list */
    RECT unk2C;
    RECT unk34;
    WinDef unk3C;
    Window *unk4C;
    u8 pad50[4];
    u8 unk54;
    u8 unk55;
    s8 unk56;
    u8 unk57;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60[0x50]; /* key item ids */
    s32 unkB0;      /* key item count */
} ItemMenu;
extern ItemMenu D_801FAB78;

typedef struct {
    void (*unk0)(s32, MenuList *);
    void (*unk4)(s32);
    MenuList *unk8;
} ItemList; /* 12 bytes */
extern ItemList D_801F3040[2];
extern s8 D_801F3064[24];
extern void (*D_801F307C[])(void);
extern u8 D_801F3120[2][0x10];
extern RECT D_801F3158;
extern RECT D_801F3160;
extern u8 D_801EDFD8[];

void func_801F0D18(void);
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
void func_801F2870(s32);
void func_801F29D0(s32);
void func_801F2BC0(void);
void func_801F2F30(void);
void func_801F2F38(s32);
void func_801F2FA0(void);

#endif
