#ifndef OVL_3E_H
#define OVL_3E_H

/* Declarations local to overlay 0x3E: the "insert disc" / disc swap screen.
 * Checks the CD lid and disc type, shows a two-part 160x224 picture and waits
 * for the right disc. Addresses in the overlay range mean different things in
 * different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* ---- main exe: candidates for ff9.h ---- */

/* GameState fields not yet named in ff9.h */
#define GAMESTATE_UNK0(gs) (*(u32 *)(gs)->pad0)
#define GAMESTATE_UNKB(gs) ((gs)->pad9[2])          /* disc bitmask? 0xFF = unknown */
#define GAMESTATE_UNK20(gs) (*(s32 *)(gs)->pad20)   /* base CD sector? */

extern s32 D_80073998[];
extern u32 D_80077528; /* pad buttons pressed */

typedef struct {
    s32 unk0;
    s32 unk4;
} Unk8001E218;

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy;
    u32 uvclut;
    u32 wh;
} SprtWords; /* SPRT written as whole words */

typedef struct {
    u8 minute, second, sector, track;
} CdlLOC;

void SetDrawTPage(void *p, int dfe, int dtd, int tpage);
u16 GetClut(int x, int y);
int CdControlB(u8 com, u8 *param, u8 *result);
int CdDiskReady(int mode);
int CdGetDiskType(void);
CdlLOC *CdIntToPos(int i, CdlLOC *p);

Unk8001E218 *func_8001E218(s32, s32);
void func_8001DCB8(void);
void func_80021640(s32);
void func_80022BD0(void);
void func_80022DF0(s32, s32, void *);
u8 func_8002306C(void);
void func_8002F488(s32);
u8 func_8002F810(void);
void func_8002F8A8(void (*)(void));
void func_8002FA50(s32);
void func_8002FAAC(s32);
void func_800325E0(void);
void func_80032634(s32);
void func_80032640(s32);

/* ---- overlay ---- */

typedef struct {
    u8 unk0;  /* result of func_8002306C */
    u8 unk1;  /* state, index into the handler tables */
    u8 unk2;
    u8 unk3;  /* last func_801F6368 result */
    u8 unk4;  /* wanted disc (0-3) */
    u8 pad5;
    s16 unk6;
    s32 unk8[2]; /* saved D_80073998[] */
    u32 unk10;   /* frame counter */
} Ovl3eState;
extern Ovl3eState D_801F6BBC;

extern void (*D_801F6B60[])(void);
extern void (*D_801F6B74[])(void);
extern void (*D_801F6B88[])(void);
extern RECT D_801F6B9C;
extern RECT D_801F6BA4;
extern s32 D_801F6BAC[];
extern RECT D_801EDF04;
extern u32 D_801EDF0C[];

void func_801F6124(void);
void func_801F6254(void);
u8 func_801F6368(void);
void func_801F6654(void);
void func_801F66AC(void);
void func_801F66F4(void);
void func_801F673C(void);
void func_801F682C(u32 *, s32);
void func_801F6A3C(void);
void func_801F6A78(void);
void func_801F6AF0(void);
void func_801F6B24(void);

#endif
