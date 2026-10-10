#ifndef OVL_38_H
#define OVL_38_H

/* Declarations local to overlay 0x38 (status menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#include "ff9.h"

/* Status menu state (D_801F120C). */
typedef struct {
    u8 unk0; /* state, index into D_801EFBA4 */
    u8 unk1; /* page */
    u8 unk2; /* party slot */
    u8 unk3; /* page count */
    u8 unk4; /* learnable ability count */
    u8 unk5;
    u8 pad6[2];
    RECT unk8;     /* main window */
    u8 unk10[6];   /* page window open */
    u8 unk16[6];
    WinDef unk1C[6];
    u8 *unk7C;     /* next free primitive */
    u8 *unk80[2];  /* primitive buffers */
    s32 unk88;
    UnkBlink unk8C;
} StatusMenu;
extern StatusMenu D_801F120C;

/* text window contents passed to func_80063118 */
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 *unk4;
    s32 unk8;
    void (*unkC)(void);
} TextWin;
extern TextWin D_801F114C[8];
extern RECT D_801F10E4[];
extern TextWin D_801F1134;

typedef struct {
    u8 *unk0;
    s32 unk4;
} Unk801F11CC;
extern Unk801F11CC D_801F11CC[7];
extern u8 D_801F1204[];
extern u8 D_801EE2A0[];
extern void (*D_801EFBA4[])(void);

typedef struct {
    u8 pad0[0xB];
    u8 unkB; /* bit 1 tested */
    u8 padC[2];
    u16 unkE; /* name offset into D_801F0808 */
} ActiveAbility; /* 0x10 */
extern ActiveAbility D_801EFC08[];
extern u8 D_801F0808[];
extern u8 D_801F0E50[];
extern u8 D_801EFC04[];

/* status page tables in .data */
extern u8 D_801EFBB4[4];       /* [1], [2] patched per character */
extern u8 D_801EFBB8[][2];     /* by ability table row */
extern s32 D_801EFBE0[4];      /* text ids */
extern s32 D_801EFBF0[5];      /* text ids */

/* local copy of the status label table (built on the stack from .rodata) */
typedef struct {
    u16 unk0; /* offset into the string pool */
    u16 unk2;
    s32 unk4;
    s32 unk8;
} StatusEntry;
typedef struct {
    StatusEntry v[48];
} StatusTable;
typedef struct {
    u8 s[0x11E];
} StatusPool; /* label strings */
extern StatusTable D_801EDF04;
extern StatusPool D_801EE144;

/* main exe: candidates for ff9.h */
extern s32 D_8007042C;
void func_80026C34(s32, s32, s32, s32, u8);
typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u8 pad4;
    u8 unk5; /* attack power */
    u8 pad6[0xC - 6];
} Unk8006E0E8; /* weapon stats, 0xC (also in ovl_0b.h) */
extern Unk8006E0E8 D_8006E0E8[];

void func_801EE568(void);
void func_801EE650(void);
void func_801EE740(void);
void func_801EE9E8(void);
void func_801EED18(void);
void func_801EEF00(void);
void func_801EF0A0(void);
void func_801EF258(s32, s32, s32, s32, u8);
void func_801EF494(void);
void func_801EF568(s32, s32);
void func_801EF708(void);
void func_801EF760(void);
void func_801EF80C(void);
void func_801EF8A0(void);
void func_801EF904(void);
void func_801EF9B4(void);
void func_801EF9D4(void);
void func_801EFAF8(void);
void func_801EFB08(void);

extern const u8 D_801EE264[]; /* name strings */

#endif
