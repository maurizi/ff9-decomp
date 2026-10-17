#ifndef OVL_14_H
#define OVL_14_H

/* Declarations local to overlay 0x14 (loaded at 0x800A7000). Addresses in the
 * overlay range mean different things in different overlays, so they live here,
 * not in ff9.h. */
#include "ff9.h"

typedef struct {
    union {
        u32 w; /* uv + clut, copied as one word */
        struct {
            u8 u;
            u8 v;
            u16 clut;
        } s;
    } unk0;
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Ovl14Text; /* 8-byte item drawn by func_80027710 */

/* 8-byte directory entry in the area at D_80067944 + 0x14C700 */
typedef struct {
    u16 unk0; /* id */
    u16 unk2;
    s32 unk4; /* start; the next entry's start ends it */
} Ovl14Dir;

/* main exe: candidates for ff9.h */
void func_8001D910(void *, RECT **, RECT **); /* TIM: clut rect, image rect */
typedef struct {
    u32 unk0;
    u32 unk4; /* flags: bpp in low bits */
} TmTimLike; /* TIM header */
s32 func_80022AC4(char *);
extern s32 g_mbgFrame;
u8 func_80022ED4(s32);
void func_80032100(void);
void func_80023018(void);
void mbg_stop(void);
s32 func_80055290(void *);
void func_800552B8(s32);
void func_800553EC(s32, s32, s32, s32);
void func_80055704(s32);
void func_80055730(s32, s32);
void func_80055764(s32, s32, s32);
void func_800557A8(s32, s32, s32, s32);
void func_800557F0(s32);
void func_8005581C(s32, s32);
void func_80055850(s32, s32, s32);
void func_80055894(s32, s32, s32, s32);
void func_800558DC(s32);
void func_80055908(s32, s32);
void func_8005593C(s32, s32, s32);
void func_80055980(s32, s32, s32, s32);
void func_800559C8(s32, s32);
void func_800559FC(s32, s32, s32);
void func_80055A34(s32, s32, s32, s32);

/* ovl_64 (FMV player) */
void mbg_streamSetBaseSector(s32);
void MBG_init(s32, u8 **);
void mbg_play(s32, s32);
s32 mbg_update(s32);
void mbg_setRgb24(s32);
void mbg_setEndCallback(void (*)(void));
s32 mbg_getLastFrame(void);
extern char **D_8019DC68;

/* other overlay (0x801F....) */
u8 func_801F5330(void);
void mbg_shutdown(void);

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
extern s32 D_800AB2C8;
extern s16 D_800AB3B8[2][4];
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
extern u32 D_800AB300; /* main state */
extern u16 D_800AB2DC[];
extern SPRT D_800AB3D0[2][2];
extern DR_TPAGE D_800AB420[2][2];
extern u8 D_800AB2EC;
s32 func_800AA848(s32);
s32 func_800AA960(s32);
void func_800AA994(void);
void func_800AA9D8(void);
extern SPRT D_800AB448[2];
extern DR_TPAGE D_800AB470[2];
void func_800AAF40(void);
void func_800AB06C(void);
void func_800AB09C(void);
void func_800AB0E0(void);
extern u8 D_800AB2CC; /* 1: FMV skipped */
extern u16 D_800AB2CE; /* FMV frame seen last loop; no splat symbol, see ovl_14.c */
void func_800A9DD4(void);
s32 func_800A82B0(void);
void func_800A808C(void);
s32 func_800A9968(void);
void func_800A9B14(void);
void func_800A9B58(void);
extern s8 D_800AB234; /* sound request kind, -1 = none */
void func_800A7F70(s32, s32);
s32 func_800A9220(void);
s32 func_800A9E14(s32);
void func_800AAADC(void);
void func_800A741C(void);
void func_800A767C(void);
void func_800A7FF0(void);
void func_800A80D4(void);
void func_800A77BC(void);
extern u16 D_800AB2D8[];
extern s32 D_800AB2F4;
extern u32 D_800AB440;
typedef struct {
    u16 unk0; /* flags */
} Ovl14Flags; /* a struct member in the original: loads of it are not moved above OT stores */
extern Ovl14Flags D_800AB2D0;

void func_800A7578(void);
void func_800A75C8(void);
void func_800A7600(void);
void func_800A7768(void);
void func_800A780C(void);
void func_800A78C0(void);
void func_800A796C(void);
s32 func_800A799C(s32, s32, s32, s32, s32);
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
void func_800A8868(s32 *);
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
