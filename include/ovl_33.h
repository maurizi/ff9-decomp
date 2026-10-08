#ifndef OVL_33_H
#define OVL_33_H

/* Declarations local to overlay 0x33 (equipment and ability menus, loaded at
 * 0x801EDF00). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Equipment menu state (D_801FBF84). */
typedef struct {
    u8 unk0; /* active list */
    u8 unk1; /* state, index into D_801F33A8 */
    u8 unk2; /* party slot */
    u8 pad3;
    u8 unk4[2]; /* per list: 0 = greyed title */
    u8 pad6[2];
    MenuList unk8[2];
    RECT unk28;
    RECT unk30;
    RECT unk38;
    RECT unk40;
    RECT unk48;
    u8 pad50[0x58 - 0x50];
    WinDef unk58;
    u8 pad68[0x768 - 0x68];
    u8 unk768;
    u8 pad769[0x76B - 0x769];
    u8 unk76B; /* animation tick (0-6) */
    u8 unk76C; /* animation frame (0-6) */
    u8 pad76D[0x770 - 0x76D];
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
    u8 pad1C[0x34 - 0x1C];
    RECT unk34;
    UnkPair unk3C[0x100]; /* {item id, count} */
    WinDef unk23C;
    UnkBlink unk24C;
    u8 pad258[0x260 - 0x258];
    u8 unk260;
    u8 unk261; /* state saved while switching characters */
} AbilityMenu;
extern AbilityMenu D_801FC840;

typedef struct {
    u8 unk0; /* low nibble: item kind */
    u8 pad1[0xF];
} Unk801F34EC; /* 0x10 */
extern Unk801F34EC D_801F34EC[];

extern void (*D_801F33A8[])(void);
extern void (*D_801F7420[])(void);
extern u8 D_801F3458[];
extern u8 D_801F3468[];
extern RECT D_801F3484;
extern RECT D_801F348C;
extern Icon D_801F3494[];
extern u8 D_801EE164[];
extern u8 D_801FBD54[];
extern u8 D_801FBD74[];

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB; /* bit 2: value x4 when D_8007AF02 */
} Unk801F1018;

void func_801EE978(void);
void func_801F0024(void);
void func_801F2AB4(void);
extern s32 D_801F33C0[2]; /* title x offsets */
extern s32 D_801F7430[3];
void func_801F0934(s32, s32, s32, MenuView *);
void func_801F09CC(s32, s32, s32, Character *);
void func_801F0AB8(s32, s32, s32, Character *);
void func_801F307C(void);
void func_80026C34(s32, s32, s32, s32, u8); /* main exe */
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
void func_801F26C4(void);
void func_801F2C78(void);
void func_801F2CD0(void);
void func_801F2D18(void);
void func_801F2E34(void);
void func_801F2F3C(s32, s32, s32, MenuView *);

#endif
