#ifndef OVL_0C_H
#define OVL_0C_H

/* Declarations local to overlay 0x0C (world map, loaded at 0x800A7000).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* ---- overlay functions ---- */
s32 func_800AB0C0(s32, s32, s32, s32, s32);
void func_800AD738(void);
void func_800AE280(void);
void func_800B30C0(void);
void func_800B905C(void);
void func_800B91A0(void);
void func_800B93B8(void);
void func_800BA1E4(void);
void func_800C1C98(void);
void func_800C2048(void);
void func_800C2134(void);
void func_800C26E4(void);
s32 func_800C497C(s32);
void func_800CBF18(void);
void func_800CC050(void);
void func_800CC13C(void);
void func_800CC82C(void);
void func_800CC890(void);
void func_800CC8C8(void);
void func_800CC940(void);

/* ---- overlay data ---- */
extern u8 D_800CCFB4[];
extern u8 D_800CE0EC;
extern u8 D_800CE0ED;
extern u8 D_800CE0EE;
extern s32 D_800CE108[];
extern s32 D_800CE198[];
extern s32 D_800CE27C;
extern s32 D_800CF408;
extern u8 D_800D30C4;
extern u8 D_800D30C5;
extern u8 D_800D30C6;
extern u8 D_800D3338;
extern s32 D_800D3F8C;
extern s32 D_800D3F90;
extern s32 D_800D3F94;
extern s32 D_800D3F98;
extern u8 D_800D3F9C;
extern u8 D_800D4818;
extern s32 D_800D481C;
extern s32 D_800D4820;
extern s32 D_800D4824;
extern s32 D_800D4828;
extern u8 D_800D5438;
extern u8 D_800CF46C;

#endif
