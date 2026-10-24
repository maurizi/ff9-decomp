#ifndef OVL_33_H
#define OVL_33_H

/* Declarations local to overlay 0x33 (equipment and ability menus, loaded at
 * 0x801EDF00). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

typedef struct {
    u8 unk0; /* item id */
    u8 unk1; /* index into D_801F6FEC */
    u8 pad2[2];
} EquipEntry;

typedef struct {
    u16 unk0; /* name offset into D_801F722C */
    u8 pad2[4];
    u8 unk6; /* kind */
    u8 unk7; /* count */
    s32 unk8; /* first index into D_801F734C */
} Unk801F6FEC; /* 0xC */
extern Unk801F6FEC D_801F6FEC[];
extern u8 D_801F722C[];
extern u8 D_801F734C[];
extern u8 D_801F33D0[][2];

/* Equipment menu state (D_801FBF84). */
typedef struct {
    u8 unk0; /* active list */
    u8 unk1; /* state, index into D_801F33A8 */
    u8 unk2; /* party slot */
    u8 unk3; /* party slot cursor; bit 7 toggled */
    u8 unk4[2]; /* per list: 0 = greyed title */
    u8 pad6[2];
    MenuList unk8[2];
    RECT unk28;
    RECT unk30;
    RECT unk38;
    RECT unk40;
    RECT unk48;
    RECT unk50;
    WinDef unk58;
    u8 unk68[0x100]; /* per item id */
    u8 unk168[0x100][5]; /* per ability id: item giving it, per slot */
    EquipEntry unk668[0x30]; /* item list */
    u8 unk728[0x40];         /* ability list */
    u8 unk768;
    u8 unk769; /* item list length */
    u8 unk76A; /* ability list length */
    u8 unk76B; /* animation tick (0-6) */
    u8 unk76C; /* animation frame (0-6) */
    u8 unk76D;
    u8 unk76E; /* party size */
    u8 unk76F;
    UnkBlink unk770;
} EquipMenu;
extern EquipMenu D_801FBF84;

/* Ability menu state (D_801FC840). */
typedef struct {
    u8 unk0;
    u8 unk1; /* state, index into D_801F7420 */
    u8 unk2;
    u8 unk3; /* party slot */
    MenuList unk4;
    RECT unk14;
    RECT unk1C;
    RECT unk24;
    RECT unk2C;
    RECT unk34;
    ItemSlot unk3C[0x100]; /* copy of the item inventory */
    WinDef unk23C;
    UnkBlink unk24C;
    u8 pad258[0x25C - 0x258];
    u8 unk25C[4]; /* party levels */
    u8 unk260;
    u8 unk261; /* state saved while switching characters */
} AbilityMenu;
extern AbilityMenu D_801FC840;

typedef struct {
    unsigned int unk0 : 4; /* item kind */
    unsigned int unk0_4 : 1;
    unsigned int unk0_5 : 3;
    unsigned int pad0_8 : 24;
    u8 unk4;
    u8 pad5[0xA - 5];
    u8 unkA;
    u8 unkB; /* bit 0: usable from the menu; bit 2: value x4 when D_8007AF02 */
    u8 padC[2];
    u16 unkE; /* name offset into D_801F40EC */
} Unk801F34EC; /* 0x10 */
extern Unk801F34EC D_801F34EC[];

extern void (*D_801F33A8[])(void);
extern void (*D_801F7420[])(void);
extern u8 D_801F3458[];
extern u8 D_801FC740[]; /* help text buffer */
extern u8 D_801F8988[]; /* "ABILITY" */
extern u8 D_801F8990[]; /* item help texts */
u8 func_800654F8(Character *, Character *, Unk801F34EC *, s32, s32);
extern u8 D_801F3468[];
extern RECT D_801F3484;
extern RECT D_801F348C;
extern Icon D_801F3494[];
extern u8 D_801EE164[];
extern u8 D_801FBD54[];
extern u8 D_801FBD74[];

void func_801EE978(void);
void func_801F0024(void);
void func_801F2AB4(void);
extern s32 D_801F33C0[2]; /* title x offsets */
extern s32 D_801F7430[3];
void func_801F0934(s32, s32, s32, MenuView *);
void func_801F09CC(s32, s32, s32, Character *);
void func_801F0AB8(s32, s32, s32, Character *);
void func_801F307C(void);
void func_801EFCB4(void);
void func_801EFE2C(void);
void func_801F3264(void);
void func_801F3278(s32, s32);
void func_801EF900(s32, s32);
void func_801F0ED4(void);
void func_801F0890(void);
void func_80027110(s32, s32, s32, s32, s32, u8, u8); /* main exe */
s32 func_80034584(s32, s32); /* main exe */
s32 func_800347D8(s32, s32); /* main exe */
extern u8 D_8007AFD3;
u8 func_801F0C10(s32, Character *);
void func_801F1048(void);
void func_801F0410(void);
extern s32 D_8007043C; /* main exe */
extern u8 D_801EE644[];
extern u16 D_801F8968[]; /* character masks */
extern u8 D_801F8980[]; /* slot masks */
s32 chr_getDefaultSet(Character *); /* main exe (returns u8) */
extern u8 D_801FC700[0x40];
extern u8 D_801F8948[5]; /* slot masks */
extern u16 D_801F8950[]; /* character masks */
void func_801F0EE4(void);
void func_801F1148(void);
void func_801F301C(void);
void func_801F31B0(void);
void func_801F2894(void);
void func_801F24EC(void);
void func_80036270(s32); /* main exe */
extern u8 D_801EDF04[];
s32 func_801F0E64(s32);
extern u8 D_801F3418[];
extern u8 D_801F3428[];
extern s16 D_801F893C[3][2]; /* cursor x per column */
extern u8 D_801EE648[];
extern s32 D_801F8918[]; /* text index per party slot */
void help_nullHandler(void);
void func_801EED10(void);
void func_801F0DDC(void);
s32 func_801EFB8C(s32, Character *);
u32 func_801F0CC4(s32, Character *);
s32 func_801F1018(Unk801F34EC *);
void func_801F0F38(s32, s32, s32, s32);
extern s32 D_801F740C[]; /* colour by state */
extern u8 D_801F40EC[];  /* item names */
extern u8 D_801F6540[];  /* ability names */
extern u16 D_801F33C8[][2]; /* cursor x per slot */
/* main exe: candidates for ff9.h */
extern s32 D_80070400;
extern s32 D_800704B8;
extern s32 D_80077E54; /* = D_80077E50.unk4, referenced by its own name */
extern s32 D_80070498;
extern s32 D_8007042C;
void func_80026EA0(s32, s32, s32, s32, s32);
s32 func_80034430(s32, s32);
void ui_drawItemIcon(s32, s32, s32, s32, u8); /* main exe */
void func_801F32E0(Character *);
void func_801F30D8(void);
void func_801EEEA8(void);
void func_801EEFDC(void);
void func_801EF270(void);
void func_801EF3A8(void);
void func_801EF560(void);
void func_801EF644(void);
void func_801F0664(void);
void func_801F06BC(void);
void func_801F07DC(void);
Icon *func_801F0F90(s32);
void func_801F15E0(void);
void func_801F189C(void);
void func_801F1A94(void);
void func_801F1F08(void);
void func_801F239C(void);
void func_801F26C4(Character *);
void func_801F2C78(void);
void func_801F2CD0(void);
void func_801F2D18(void);
void func_801F2E34(void);
void func_801F2F3C(s32, s32, s32, MenuView *);

typedef struct {
    u16 unk0;
    u16 unk2;
} HelpOfs; /* item help text offsets into D_801F4734 */
extern HelpOfs D_801F6240[];
extern u8 D_801F4734[];
extern u8 D_801F67D4[];
extern u8 D_801F3478[];
extern u8 D_801FBD84[];
extern u8 *D_80077DF0; /* = g_help.text, referenced by its own name */
typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[2];
} SupportHelp; /* D_8006D128 entry: +4 = help text offset into D_801F67D4 */

/* support ability help text */
static inline u8 *func_801F0024_suphelp(s32 i) {
    SupportHelp *b = (SupportHelp *)D_8006D128;
    i -= 0xC0;
    return D_801F67D4 + b[i].unk4;
}

void func_801F3344(s32 x, s32 y, s32 z, u8 id, s32 color);
extern u8 D_801F892C[]; /* arrow */
extern u8 D_801F8930[4]; /* stat names */
extern u8 D_801F8934[5]; /* attack/defence names */
void func_801F20A0(Unk80032E84Out *cur, Unk80032E84Out *new);

extern Unk801F34EC D_801F743C[]; /* action abilities (ability menu), same layout as D_801F34EC */
extern u8 D_801F803C[]; /* action ability names (ability menu) */
extern u8 D_801F8684[]; /* support ability names (ability menu) */

#endif
