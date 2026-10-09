#ifndef OVL_3C_H
#define OVL_3C_H

/* Declarations local to overlay 0x3C (shop menus: a top-level hub that runs
 * one of several sub-menus, loaded at 0x801EDF00). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Shop hub state (D_801FC2D0). */
typedef struct {
    u8 unk0; /* shop kind */
    u8 unk1; /* cursor */
    u8 unk2; /* option count */
    u8 unk3; /* sub-menu running */
    u8 unk4; /* sub-menu has been used */
    u8 pad5[3];
    s32 unk8;  /* shop id (D_80077DBC) */
    s32 unkC;  /* saved func_8002F8E4 mode */
    u8 *unk10; /* title text */
    s32 unk14; /* party member count */
    s8 unk18[8]; /* party member indices */
    s32 unk20;   /* OT layer */
} ShopHub;
extern ShopHub D_801FC2D0;

/* Buy menu (D_801FC2F8). */
typedef struct {
    u8 unk0; /* state, index into D_801FC044 */
    u8 unk1; /* item count */
    u8 unk2; /* quantity */
    u8 unk3; /* max quantity */
    u8 unk4; /* sells equipment */
    u8 pad5[3];
    RECT unk8;
    RECT unk10;
    RECT unk18;
    RECT unk20;
    MenuList unk28;
    Window *unk38;
    u8 *unk3C; /* item list */
} ShopBuy;
extern ShopBuy D_801FC2F8;

typedef struct {
    u16 unk0; /* price */
    u8 unk2[2]; /* ingredients */
    u8 unk4;  /* result */
    u8 unk5;  /* shop mask */
} SynthRecipe;
extern SynthRecipe D_801FC0FC[64];

/* Synthesis menu (D_801FC338). */
typedef struct {
    u8 unk0; /* state, index into D_801FC09C */
    u8 unk1;
    u8 unk2; /* quantity */
    u8 unk3; /* max quantity */
    u8 unk4;
    u8 pad5[3];
    RECT unk8;
    RECT unk10;
    RECT unk18;
    RECT unk20;
    MenuList unk28;
    Window *unk38;
    SynthRecipe unk3C[32];
    u8 padFC[4];
} ShopSynth;
extern ShopSynth D_801FC338;

/* Sell menu (D_801FC438). */
typedef struct {
    u8 unk0; /* state, index into D_801FC27C */
    u8 unk1; /* quantity */
    u8 unk2; /* max quantity */
    u8 pad3;
    MenuList unk4;
    RECT unk14;
    RECT unk1C;
    RECT unk24;
    Window *unk2C[2];
} ShopSell;
extern ShopSell D_801FC438;

extern void (*D_801FC044[])(void);
extern void (*D_801FC09C[])(void);
extern void (*D_801FC27C[])(void);
/* item names: data, splat starts .data at 0xA8F0 instead of 0xA8EC */
extern u8 func_801F87EC[];

/* {init, update, exit} per shop kind and option */
extern void (*D_801FBBA0[][3][3])(void);
extern RECT D_801FBC0C;
extern RECT D_801FBC14;
extern RECT D_801FBC1C;
extern u8 D_801FBC24[];
extern RECT D_801FBC28;
extern u8 D_801FBC30[];
extern RECT D_801FBC34;
extern RECT D_801FBC3C;
extern u8 D_801FBC44[][0x20]; /* item list per shop, 0xFF = none */
extern u8 D_801F36FC[];
extern u8 D_801F3C9C[];
extern u8 D_801FC098[]; /* gil suffix */
extern u8 D_801FC0F8[]; /* gil suffix */
extern RECT *D_801FC288;
extern u8 D_801FC28C[]; /* gil suffix */
void func_801F84D8(void);
typedef struct {
    RECT r[7];
} RectTbl7;
extern RectTbl7 D_801EDF04; /* icon source rects */
extern u8 D_801EDF3C[];

extern u8 D_801F3E98[];
/* main exe: candidates for ff9.h */
s32 func_80027F34(s32); /* main.c defines it returning u8 */
s32 func_80027F94(s32);
extern s32 D_801FC050[3]; /* row y offsets */
extern u8 D_801FC05C[4];
extern u8 D_801FC060[]; /* gil suffix */
extern s32 D_801FC064[3]; /* row label text ids */
extern s32 D_801FC0A8[4];
extern u8 D_801FC0B8[4];
extern u8 D_801FC0BC[]; /* gil suffix */
extern s32 D_801FC0C0[4];
void func_80026C34(s32, s32, s32, s32, u8);
extern u8 D_801F3F08[];
extern u8 D_801F3F88[];
void func_801F7250(void);
void func_801F5EEC(void);
void func_801F76C8(void);
void func_801F8728(void);
u8 func_801F42A8(s32, u8 *);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Unk8006D328; /* equipment stat bonuses, from item 0x58 */
extern Unk8006D328 D_8006D328[];

void func_801F4000(void);
void func_801F40AC(void);
void func_801F4198(void);
void func_801F442C(s32);
void func_801F4748(void);
void func_801F47C8(void);
void func_801F47F0(void);
void func_801F47F8(void);
void func_801F4854(void);
s32 func_801F48F4(s32);
void func_801F4960(void);
void func_801F49C4(void);
void func_801F4A14(void);
void func_801F4A44(void);
s32 func_801F4A90(s32);
void func_801F4AC0(void);
void func_801F4B38(void);
void func_801F4E1C(void);
void func_801F506C(void);
void func_801F51B8(void);
void func_801F56E4(void);
s32 func_801F59A8(s32);
void func_801F5A1C(void);
void func_801F5A74(void);
void func_801F57C8(void);
void func_801F5BCC(s32, s32, s32, MenuView *);
void func_801F5D14(void);
void func_801F5B64(void);
void func_801F551C(Window *);
void func_801F5D40(Window *);
void func_801F5D6C(Window *);
extern s32 D_800704A4;
void func_801F7614(void);
void func_801F8680(void);
void func_801F7528(void);
void func_801F625C(void);
void func_801F65C8(void);
void func_801F6714(void);
void func_801F6C50(void);
void func_801F6D34(s32, s32, s32, MenuView *);
void func_801F6F00(void);
void func_801F73E0(void);
void func_801F7438(void);
void func_801F7470(void);
void func_801F7590(void);
void func_801F79FC(void);
void func_801F7FE0(void);
void func_801F80D0(void);
void func_801F837C(void);
void func_801F83D4(void);
void func_801F83FC(void);
void func_801F856C(s32, s32, s32, MenuView *);
s32 func_801F4458(void);
void func_801F43F8(void);
void func_801F5AAC(void);

#endif
