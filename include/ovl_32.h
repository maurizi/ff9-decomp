#ifndef OVL_32_H
#define OVL_32_H

/* Declarations local to overlay 0x32 (Chocograph menu, loaded at 0x801EDF00).
 * Addresses in the overlay range mean different things in different overlays,
 * so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- overlay ---- */

typedef struct {
    u8 unk0; /* top-level cursor (0/1) */
    u8 unk1; /* mode: index into D_801F96E4 */
    u8 unk2;
    u8 unk3;
    u8 unk4; /* count */
    u8 pad5[3];
    u32 unk8; /* chocograph bitmask: owned */
    u32 unkC; /* chocograph bitmask */
    MenuList unk10;
    RECT unk20;
    RECT unk28;
    RECT unk30;
    RECT unk38;
    RECT unk40;
    RECT unk48;
    RECT unk50;
    u8 unk58[0x2000];
    u8 *unk2058;
} Ovl32State;
extern Ovl32State D_801F9850;

extern void (*D_801F96E4[])(void);
extern u8 D_801F9750[]; /* help text buffer */
extern RECT D_801F970C;
extern RECT D_801F9714;
extern RECT D_801F971C;
extern RECT D_801F9724;

/* chocograph picture: 16-colour CLUT + 4bpp image, 24 of them in .rodata */
typedef struct {
    u8 unk0[0x80];
    u8 unk80[0x498];
} Ovl32Image;
extern Ovl32Image D_801EF1E4[];
extern u8 D_801F713C[];
extern u8 D_801F7948[];
extern u8 D_801F96F0[];
extern u8 D_801F96F4[];
typedef struct {
    RECT r[16];
} RectTbl16;
extern RectTbl16 D_801EDF04; /* icon source rects */

void func_801F84F0(void);
void func_801F85EC(void);
void func_801F86D0(void);
void func_801F7EB4(void);
void func_801F8010(void);
void func_801F8130(void);
void func_801F8240(void);
void func_801F8404(void);
void func_801F8800(void);
void func_801F88F4(s32, s32, s32, s32, u8);
void func_801F8AB0(s32, s32, s32, u8);
void func_801F907C(void);
void func_801F90D4(void);
void func_801F912C(void);
void func_801F9164(void);
void func_801F91D8(void);
void func_801F92C0(void);
void func_801F9394(void);
void func_801F93FC(s32, s32, s32, MenuView *);
void func_801F9518(s32, s32, s32, s32, u8);
s32 func_801F95E8(s32);
/* inlined copy of func_801F95E8 */
static inline s32 func_801F95E8_inline(s32 id) {
    return D_801F96F4[id];
}
void func_801F9600(s32);
void func_801F9660(void);
void func_801F9668(s32, RECT *, RECT *);

extern const u8 D_801EDF84[]; /* text bank for this menu */

typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} Ft4W; /* POLY_FT4 with the colour/code written as one word */

typedef struct {
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u8 w;
    u8 h;
} FrameRect; /* window frame piece: position, uv, size */
extern FrameRect D_801F972C[4]; /* frame pieces drawn by func_801F8AB0 */
extern u8 D_801F974C[2]; /* pieces that get a shadow */

#endif
