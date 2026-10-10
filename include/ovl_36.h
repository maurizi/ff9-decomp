#ifndef OVL_36_H
#define OVL_36_H

/* Declarations local to overlay 0x36 (Tetra Master card list / collector
 * rank menu, loaded at 0x801EDF00). Overlay addresses mean different things
 * in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

typedef struct {
    u8 unk0; /* in use */
    u8 unk1; /* card id */
    u8 unk2; /* VRAM slot */
    u8 unk3;
} CardSlot;

/* Card picture cache (D_801F347C.unk1900 = D_801F4D7C). */
typedef struct {
    u8 unk0; /* oldest slot */
    u8 unk1; /* next slot */
    u8 unk2; /* slots used */
    u8 unk3; /* card being loaded */
    u8 unk4; /* card to load, 0xFF = none */
    u8 unk5; /* card under the cursor */
    u8 unk6; /* previous card */
    u8 unk7; /* card in the big picture */
    u8 unk8; /* fade ticks */
    u8 unk9; /* fading */
    u8 unkA; /* CD read in progress */
    u8 padB[2];
    CardSlot unkD[12];
} CardCache; /* 0x3D */

/* Card turn animation (D_801F347C.unk1941 = D_801F4DBD). */
typedef struct {
    u8 unk0; /* active */
    u8 unk1; /* direction */
    u8 unk2; /* ticks */
    u8 unk3; /* card */
} CardTurn;

typedef struct {
    s32 unk0;
    s32 unk4;
} Unk801F4DC4;

/* Run-length packed block (func_801F3034). */
typedef struct {
    u8 unk0;
    u8 unk1; /* ~key: bytes above key start a run */
    u16 unk2; /* packed size */
    u8 unk4[1];
} RleHeader;

/* Card menu state (D_801F347C). */
typedef struct {
    u8 unk0; /* state, index into D_801F30F4 */
    u8 unk1; /* cursor column */
    u8 unk2; /* cursor row */
    u8 unk3;
    u8 unk4;
    u8 unk5; /* yes/no cursor */
    u8 unk6;
    u8 unk7; /* collector rank */
    u8 unk8[100];  /* cards owned per id */
    u8 unk6C[100]; /* shown variant per id */
    u8 unkD0[0x200]; /* decompressed card CLUT */
    u8 unk2D0[0xE00]; /* decompressed card image */
    u8 unk10D0[0x800]; /* CD sector buffer */
    RECT unk18D0; /* card grid window */
    RECT unk18D8;
    RECT unk18E0; /* card picture window */
    Window *unk18E8; /* yes/no window */
    FileEntry *unk18EC;
    s32 unk18F0[4]; /* collector points: kinds, arrows, types, total */
    CardCache unk1900;
    u8 pad193D[4];
    CardTurn unk1941;
    u8 pad1945[3];
    Unk801F4DC4 unk1948[4];
} CardMenu;
extern CardMenu D_801F347C;
void func_801F2860(s32, s32, s32, s32);

typedef struct {
    s16 unk0; /* icon */
    s8 unk2;  /* x */
    s8 unk3;  /* y */
} Unk801F3108; /* card arrow marker */

extern s32 D_801F30E4[4];
extern void (*D_801F30F4[])(); /* state handlers, called without arguments */
extern RECT D_801F3100;
extern Unk801F3108 D_801F3108[8];
extern s16 D_801F3128[32]; /* points needed per rank */
extern RECT D_801F3168;
extern RECT D_801F3170;
extern RECT D_801F3178;
extern RECT D_801F3180;
extern u8 D_801F3378[4];
extern u8 D_801F337C[0x100];
extern u8 D_801EE1B4[];
extern u8 D_801EEFB4[];
extern s32 D_801F0274[];

typedef struct {
    u32 unk0; /* uv + clut */
    u8 unk4;  /* w */
    u8 unk5;  /* h */
    u16 unk6; /* tpage */
} Ovl36Sprite;
extern Ovl36Sprite D_801F3188[];
extern RECT D_801EDF08;
extern RECT D_801EDF10;
extern RECT D_801F3348;
extern RECT D_801F3350;
extern RECT D_801F3358;
extern RECT D_801F3360;
extern RECT D_801F3368;
extern RECT D_801F3370;
void func_801F2A04(s32, s32, s32, Unk6 *);
void func_801F2AB8(s32, s32, s32, Unk6 *);
void func_801F1AE8(s32, s32, s32, s32);
void func_801F0CC8(void);
void func_801F1C88(void);
void func_801F2CD4(void);
void func_801F2E60(void);
void func_801F2FCC(void);
extern s32 D_801F30D4[4];
void func_801F0DB0(void);
void func_801F0F9C(void);
void func_801F12B8(s32);
void func_801F1508(void);
void func_801F166C(void);
void func_801F1778(void);
void func_801F1A48(void);
void func_801F1D6C(s32, s32, s32, s32, s32);
void func_801F1EA0(u8 *);
void func_801F1FE8(u8 *);
void func_801F2138(s32 *);
void func_801F22DC(void);
void func_801F24A0(void);
void func_801F2508(void);
void func_801F2548(void);
void func_801F2664(void);
void func_801F2B94(void);
void func_801F2C14(void);
void func_801F2C5C(s32);
CardSlot *func_801F2D30(s32);
void func_801F2D88(CardSlot *);
void func_801F2EE8(void);
void func_801F2F14(Window *);
void func_801F2F24(Window *);
void func_801F2F34(Window *);
void func_801F2FE0(s32, s32);
s32 func_801F3034(u8 *, u8 *);
u32 func_801F30C8(void);

extern u8 D_801EDF04[];
/* main exe: candidates for ff9.h */
s32 func_80062498(void); /* collector points */
s32 func_800624D8(void);
s32 func_8006251C(void);
/* defined as returning u16 in main.c, but called here without masking */
s32 func_8002C7F0(s32, s32);
s32 func_8002C830(s32, s32);
/* text width; the call result passes through an inline return value */
static inline s32 func_801F0F9C_width(u8 *s) {
    return func_8002BA68((s32)s);
}

/* Inlined copy of func_801F2D88 (show a cached card picture), as expanded in func_801F1778. The
   x, y, w, h statement order matters there: the w/h constants live in s-registers and their stores
   go through the &a argument register, so they get output dependences on the x/y stores. */
static inline void func_801F2D88_inline(CardSlot *s) {
    RECT a, b;
    u32 n = s->unk2;

    a.x = 0;
    a.y = n + 0x1F4;
    a.w = 0x100;
    a.h = 1;
    b.x = (u8)(n % 3) * 0x13;
    b.y = (u8)(n / 3) * 0x2F + 0x100;
    b.w = 0x13;
    b.h = 0x2F;
    MoveImage(&a, 0, 0x1F2);
    MoveImage(&b, 0x3C0, 0x1B0);
    DrawSync(0);
    D_801F347C.unk1900.unk7 = s->unk1;
}

/* Start the CD read of the next requested card picture (D_801F347C.unk1900.unk4).
   func_801F1A48 expands this in both arms of its if/else; the original's two copies of
   the call are what make gcc give the argument loads no launch priority (REG_N_SETS == 2),
   after which jump2 cross-jumps the tails. */
static inline void func_801F1A48_read(CardCache *c) {
    s32 id;
    u32 x;

    id = c->unk4;
    if (id != 0xFF) {
        c->unk4 = 0xFF;
        c->unk3 = id;
        D_801F347C.unk1900.unkA = 1;
        x = c->unk3;
        D_801F347C.unk4 = x;
        func_80022C60(D_8006794C->unk20 + D_801F347C.unk18EC->unk4 + x, 0x800, D_801F347C.unk10D0, NULL);
    }
}

#endif
