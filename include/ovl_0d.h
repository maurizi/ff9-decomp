#ifndef OVL_0D_H
#define OVL_0D_H

/* Declarations local to overlay 0x0D (Tetra Master card game, loaded at
 * 0x800A7000). Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- main exe: candidates for ff9.h ---- */

/* argument of window draw callbacks (func_8002B5FC callers) */
typedef struct {
    s16 unk0; /* x */
    s16 unk2; /* y */
    u8 pad4[0x18 - 4];
    s16 unk18;
} WinCtx;

/* Fields of D_8006794C and its ->unk1C block used here; GameState/GameData in
 * ff9.h place other members at these offsets, so this overlay uses views. */
typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9;
    u8 unkA;
    u8 padB[0x1C - 0xB];
    void *unk1C;
    s32 unk20;
} GameStateView;

typedef struct {
    u8 pad0[8];
    u8 unk8; /* current buffer */
    u8 pad9[0x10 - 9];
    u32 *unk10[2]; /* ordering tables */
    u8 pad18[0x814 - 0x18];
    void *unk814;
    u8 pad818[0x83C - 0x818];
    void (*unk83C)(s32, s32, s32, s32, s32);
    u8 pad840[0x854 - 0x840];
    u8 unk854[1];
} GameDataView;

#define GSV ((GameStateView *)D_8006794C)
#define GDV ((GameDataView *)D_8006794C->unk1C)

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
} Unk8001E218;

extern s32 D_80067944;
extern s32 D_80077528; /* pad buttons (pressed) */

void ClearOTagR(u32 *, s32);
void *memmove(void *, void *, s32);
void func_8001C750(s32, s32);
Unk8001E218 *func_8001E218(s32, s32);
void func_80021C60(void);
void func_8002177C(void *, s32, s32, s32, s32, s32, s32);
s32 func_800217C4(void *);
void func_80022DF0(s32, s32, void *);
void func_8002EB64(s32, s32);
void func_800320B8(s32, s32, s32, s32);
void func_80055DF4(void);
void func_80055E14(s32, s32, s32);
s32 func_8001C7FC(s32, s32, s32);
s32 func_8001C8B0(s32, s32, s32, s32);
void func_8002B5FC(s32, s32, s32, u8 *, s32);
void func_8002C308(s32);
void func_8002DD00(s32);
void func_8002E2E4(s32, void (*)(void));
void func_8002F93C(s32);
void func_80035580(RECT *);
u8 *func_80035774(s32);
s32 func_800625F8(void);
s32 func_80062638(void);
s32 func_80062B3C(void);
void func_80062420(u8, u8, u8, u8, s32, s32);

/* ---- Tetra Master ---- */

typedef struct {
    u8 unk0; /* card id? */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;  /* x */
    s32 unk10; /* y */
    s32 unk14; /* owner: 0 player, 1 enemy */
    s32 unk18; /* 1: on the board */
    u8 pad1C[0x20 - 0x1C];
    s32 unk20; /* x << 8 */
    s32 unk24; /* y << 8 */
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3C - 0x38];
} TmCard; /* 0x3C */

typedef struct {
    s32 unk0;
    TmCard *unk4;
} TmCell; /* board square */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x14 - 0xC];
} TmUnk350;

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    TmCard unk10[10]; /* 0-4 player hand, 5-9 enemy hand */
    TmCell unk268[4][4]; /* board [y][x] */
    s32 unk2E8; /* frame counter in the current state */
    s32 unk2EC;
    u8 pad2F0[0x350 - 0x2F0];
    TmUnk350 unk350[10];
    s32 unk418;
    u8 pad41C[0x424 - 0x41C];
    s32 unk424;
    s32 unk428;
    s32 unk42C;
    s32 unk430;
    s32 unk434;
    u8 pad438[0x458 - 0x438];
    s32 unk458;
    s32 unk45C;
    s32 unk460;
    s32 unk464;
    s32 unk468;
    s32 unk46C;
    TmCard *unk470;
    s32 unk474;
    s32 unk478;
    s32 unk47C;
    u8 pad480[0x484 - 0x480];
    s32 unk484;
    u8 pad488[0x4B4 - 0x488];
    s32 unk4B4;
    s32 unk4B8;
    s32 unk4BC;
    s32 unk4C0;
    s32 unk4C4;
    u8 pad4C8[0x4F0 - 0x4C8];
    s32 unk4F0;
    u8 pad4F4[0x550 - 0x4F4];
    s32 unk550;
    s32 unk554;
    u8 pad558[0x564 - 0x558];
    s32 unk564;
    s32 unk568;
    s32 unk56C;
    s32 unk570;
    u8 pad574[0x57C - 0x574];
    s32 unk57C;
    u8 pad580[0x5A4 - 0x580];
    s32 unk5A4;
    s32 unk5A8;
    s32 unk5AC;
} TmState;

extern s32 D_800B365C;
extern s32 D_800B3660;
extern s32 D_800B3C8C[];
extern s32 D_800B3CD0;
extern TmCell D_800B3CE8; /* returned for out-of-range board squares */
extern s32 D_800B3CF0;
extern s32 D_800B3CF4;

s32 func_800A7634(void);
s32 func_800A76F8(void);
s32 func_800A7764(void);
s32 func_800A7818(void);
s32 func_800A7884(void);
s32 func_800A78B8(void);
u8 *func_800A93A0(s32);
void func_800ABEC4(TmState *);
void func_800AF2CC(TmState *, TmCard *, s32, s32);
void func_800AF388(TmState *, TmCard *);
void func_800AF3E8(TmState *, TmCard *, s32, s32);
void func_800B0618(TmState *);
s32 func_800B0EB8(TmState *, s32, s32, s32, s32);
void func_800B1110(TmState *, s32, s32, s32, s32, s32);
void func_800B35F8(TmState *);
void func_800A755C(void);
void func_800A75E0(void);
void func_800A78EC(s32, s32, s32, s32, s32);
void func_800A7E10(void);
void func_800AB8C8(void);
void func_800AB8E8(s32);
void func_800AE54C(TmState *);
void func_800B01E8(s32);
TmCell *func_800B19D4(TmState *, s32, s32);

#endif
