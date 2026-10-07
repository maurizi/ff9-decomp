#ifndef OVL_3B_H
#define OVL_3B_H

/* Declarations local to overlay 0x3B (party select menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Party select request passed to func_801F457C. */
typedef struct {
    u8 unk0;     /* party size */
    u8 pad1[3];
    u8 unk4[4];  /* current party (character ids, 0xFF = empty) */
    u8 unk8[8];  /* reserve */
    u8 unk10[8]; /* per-character value */
} PartyRequest;

typedef struct {
    u8 unk0; /* column */
    u8 unk1; /* row */
} Cursor;

/* Party select state (D_801F4748). */
typedef struct {
    u8 unk0;       /* 0 = picking, 1 = swapping */
    u8 unk1;       /* party size */
    u8 pad2[2];
    Cursor unk4[2]; /* [0] = current, [1] = swap source */
    u8 unk8;
    u8 unk9;
    u8 unkA[2][8]; /* rows: party, reserve (character ids, 0xFF = empty) */
    u8 unk1A[8];
} PartySelect;
extern PartySelect D_801F4748;

/* RECTs, not code: splat starts .data at 0x6830 instead of 0x6828, so the
 * first one keeps a func_ name. */
extern RECT func_801F4728;
extern RECT D_801F4730;
extern RECT D_801F4738;
extern RECT D_801F4740;

/* main exe: candidates for ff9.h (ovl_3a calls it unprototyped) */
u8 func_80028954(s32);

void func_801F37A8(void);
void func_801F38D8(void);
void func_801F3A60(void);
void func_801F3B3C(void);
void func_801F3CF0(void);
void func_801F3E58(void);
void func_801F40AC(void);
void func_801F4150(void);
void func_801F4190(void);
void func_801F4204(void);
void func_801F42A4(void);
void func_801F4360(s32, s32, s32, s32);
void func_801F43FC(s32, s32, s32, s32, s32, u8);
u8 func_801F4614(s32);
u8 func_801F465C(void);

#endif
