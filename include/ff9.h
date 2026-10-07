#ifndef FF9_H
#define FF9_H

#include "common.h"
#include "psyq.h"

/* Layouts are partial: only offsets seen in decompiled code are named.
 * Names are placeholders (unkXX = offset) until their purpose is known.
 * Split into per-subsystem headers once subsystems are identified. */

/* ---- game state (D_8006794C) ---- */

typedef struct {
    u8 unk0[0x28];
} Unk80021938; /* element passed to func_80021938 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
    Unk80021938 unk4[7];
    s32 unk11C;
} Unk8C8;

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkPair;

typedef struct {
    u8 pad0[4];
    u16 unk4; /* high byte: first text code that takes an extra byte */
} Unk7D4;

typedef struct {
    u8 unk0[0x10]; /* name? 0xFF-terminated text, read up to 7 chars */
    u16 unk10;
    u8 pad12[0x34 - 0x12];
    u32 unk34; /* low nibble: index into GameData.unk0 */
} Unk54C;

typedef struct {
    u8 pad0[0x70];
    u32 unk70; /* flags; bit 11 tested, bits 4-8 index D_8006BB8C */
    u8 pad74;
    u8 unk75[5]; /* 5 item ids (equipment slots?) */
    u8 pad7A[0x90 - 0x7A];
} Unk0; /* 0x90 bytes, 9 of them: likely characters */

/* D_8006794C->unk1C: looks like the save-data block */
typedef struct {
    Unk0 unk0[9];
    u8 pad510[0x54C - 0x510];
    Unk54C *unk54C[10];
    UnkPair unk574[0x100]; /* {id, count}: likely the item inventory */
    u8 unk774[0x40];       /* 2 bits per item id */
    u8 pad7B4[0x7D4 - 0x7B4];
    Unk7D4 *unk7D4;
    u8 pad7D8[0x8C8 - 0x7D8];
    Unk8C8 *unk8C8;
} GameData;

typedef struct {
    u8 pad0[8];
    u8 unk8;
    u8 pad9[0x1C - 9];
    GameData *unk1C;
    u8 pad20[4];
    u32 unk24;
    u8 pad28[4];
    u32 unk2C; /* frame counter? divided by 60 */
} GameState;

extern GameState *D_8006794C;

/* Inlined copy of func_80027EDC (item search); the original source inlines it. */
static inline UnkPair *func_80027EDC_inline(s32 id) {
    s32 i;
    UnkPair *e = D_8006794C->unk1C->unk574;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 == id) {
            return e;
        }
    }
    return NULL;
}

/* ---- timers (VSync based) ---- */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
} UnkTimer;

typedef struct {
    s32 unk0; /* VSync(-1) at last update */
    u8 pad[0x404];
    UnkTimer unk408;
} Unk80076B60;
extern Unk80076B60 D_80076B60;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Unk80076F68;
extern Unk80076F68 D_80076F68;

/* ---- CD / streaming state near libcd ---- */

typedef struct {
    u8 unk0;
    u8 pad1[0x17];
    s32 unk18;
    u8 pad1C[0x944 - 0x1C];
    u8 unk944;
    u8 pad945[0x958 - 0x945];
    void (*unk958)();
} Unk800761D0;
extern Unk800761D0 D_800761D0;

/* ---- drawing ---- */

extern u32 *D_8007716C; /* ordering table */
extern u8 *D_80077170;  /* next free primitive in the packet buffer */
extern s16 D_80068F70[];
extern u8 D_8006BB8C[];

typedef struct {
    u8 pad0[0xF];
    u8 unkF; /* mask tested against D_8006BB84[] */
    u8 pad10[2];
} Unk80069018; /* 0x12-byte table entries */
extern Unk80069018 D_80069018[];
extern u8 D_8006BB84[];
extern u8 D_8006BB74[];

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 wh;
} TileWords; /* TILE written as whole words */

typedef struct {
    u32 tag;
    u32 code;
} DrTpageWords;

/* 12-tick toggle timer (func_80028CF8/D08/D2C) */
typedef struct {
    u8 unk0; /* active */
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4; /* ticks */
    void (*unk8)(void);
} UnkBlink;

/* ---- misc globals ---- */

extern s32 D_80077140;
extern s32 D_80077144;
extern void (*D_80068F80[])(s32, s32);
extern void (*D_8006BBEC[])(s32);
extern s32 D_8006BBFC[];
extern s32 D_8006BC18[];
extern s32 D_800770AC;
extern s32 D_800770B0[];
extern s32 D_800770F0[];
extern u8 D_80077130[];
extern s32 D_80077150;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
} Unk80077148;
extern Unk80077148 D_80077148;

/* ---- functions (main exe) ---- */

void func_8001D898(s32, s32, s32);
void func_800213A0(void (*)());
void func_80021938(Unk80021938 *);
void func_80023580(s32, s32);
void func_800235A0();
void func_8002423C(s32, s32, s32, Unk54C *);
u8 *func_80024EB4(s32, u32 *, u8, u8, s32, u8 *);
u8 *func_800253D4(s32, u32 *, u8, u8, s32, s32, u8 *);
void func_8002642C(s16, s16, s32, s32, s32, s32);
u8 *func_80026C7C(s32, s32, u32 *, s32, u8, u8 *);
void func_80026F8C(s32, s32, s32, s32);
u8 *func_80027170(s32, s32, u32 *, s32, s32, u8, u8, u8 *);
void func_800275DC(s32, s32, s32, s32, s32);
void func_80028AF0(s32, s32, s32, s32, u8);

void func_8002BAE4(s32, u8 *, s32, s32);
void func_80027D18(void);
void func_80027D4C(void);
void func_80027D84(u8, u8);
void func_80028204(s32, s32, s32, s32, s32, u8);
void func_800283E0(s32, s32, s32, s32, s32, u8);
void func_80028A40(s32, RECT *, RECT *, RECT *);
void func_80028BE8(u8 *, s32);
s32 func_80028DB0(s32, s32, u8);
u8 *func_80029B4C(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_80029D60(s32, s32, u32 *, s32, s32, u8 *);
u8 *func_8002B644(s32, s32, u32 *, u8 *, s32, u8 *);
void func_8002B5B4(s32, s32, s32, u8 *, s32);
u8 *func_8002920C(s32, s32, u32 *, s32, s32, u8 *);
void func_8002AF70(s16 *, s32);
s32 func_80023938(void);
void func_8002C9B0(void);
void func_8002E180(void);

#endif
