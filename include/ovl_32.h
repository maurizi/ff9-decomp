#ifndef OVL_32_H
#define OVL_32_H

/* Declarations local to overlay 0x32 (Chocograph menu, loaded at 0x801EDF00).
 * Addresses in the overlay range mean different things in different overlays,
 * so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */

/* GameData fields not yet named in ff9.h */
#define GAMEDATA_UNK8 (((u8 *)D_8006794C->unk1C)[8])
#define GAMEDATA_UNK844 (*(s32 *)((u8 *)D_8006794C->unk1C + 0x844)) /* selected chocograph (1-based, 0 = none) */

extern u32 D_80077528; /* pad buttons pressed this frame */
extern s32 D_80070424;
extern s32 D_80070428;
extern s32 D_800704AC;
extern s32 D_80070454;
extern s32 D_80077DC0; /* frame counter? */



/* menu list/cursor state passed to func_8002EE8C etc. */
typedef struct {
    u8 pad0[5];
    u8 unk5; /* scroll top */
    u8 unk6; /* cursor row */
    u8 pad7;
    u8 unk8;
    u8 pad9[7];
} MenuList;

/* visible window of a MenuList (func_8002F344) */
typedef struct {
    u8 unk0; /* visible rows */
    u8 unk1; /* first index */
    s16 unk2; /* y */
    u8 pad4[4];
} MenuView;

void func_8002705C(RECT *, s32, s32);
void func_80027710(s32, s32, s32, RECT *, s32);
void func_8002B524(s32, s32, s32, s32, s32);
void func_8002B56C(s32, s32, s32, u8 *, s32);
void func_8002B5FC(s32, s32, s32, u8 *, s32);
void func_8002D9B4(RECT *, s32, s32);
void func_8002DEE4(RECT *, s32, s32);
void func_8002E040(RECT *, s32, s32, void *);
void func_8002EB74(MenuList *);
void func_8002ECA0(MenuList *, MenuView *);
void func_8002F40C(MenuList *, RECT *, RECT *);
void func_8002EE8C(MenuList *);
void func_8002F344(MenuList *, MenuView *);
void func_8002F488(s32);
u8 func_8002F810(void);
void func_800325E0(void);
void func_80035674(s32);
u8 *func_80035774(s32); /* text lookup */
int MoveImage(RECT *rect, int x, int y);
u16 GetClut(int x, int y);
#define setRECT(r, _x, _y, _w, _h) ((r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h))
void func_8003575C(const u8 *);
void func_8002EDFC(MenuList *, s32, s32, s32, s32);
void SetDrawTPage(void *p, int dfe, int dtd, int tpage);

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
    u32 wh;
} SprtWords; /* SPRT written as whole words */
void func_80035580(s32);
void func_800355E4(void *);
void func_8002AD38(u8 *, u8 *);
void func_8002BD54(u8 *, u8 *);
void func_8002E040(RECT *, s32, s32, void *);

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

#endif
