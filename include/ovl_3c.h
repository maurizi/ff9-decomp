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
    u8 unk14;  /* party member count */
    u8 pad15[3];
    s8 unk18[8]; /* party member indices */
    s32 unk20;   /* OT layer */
} ShopHub;
extern ShopHub D_801FC2D0;

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
extern u8 D_801EDF3C[];

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
void func_801F5AAC(void);

#endif
