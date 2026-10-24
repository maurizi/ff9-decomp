#ifndef OVL_3A_H
#define OVL_3A_H

/* Declarations local to overlay 0x3A (name entry menu, loaded at 0x801EDF00).
 * Overlay addresses mean different things in different overlays, so they
 * live here, not in ff9.h. */
#define FF9_BUILTIN_MEMCPY /* see memcpy in psyq.h */
#include "ff9.h"

/* Name entry state (D_801F4BBC). */
typedef struct {
    u8 unk0; /* 0 = side menu, 1 = character grid */
    u8 unk1; /* side menu cursor (0-4) */
    u8 unk2; /* character set (0/1) */
    u8 unk3; /* grid cursor, 15 per row */
    u8 unk4; /* name length (max 8) */
    u8 unk5[2]; /* grid size per character set */
    u8 unk7[10];  /* name being edited, 0xFF-terminated */
    u8 unk11[11]; /* default name */
    u8 *unk1C[2]; /* character set strings */
    u8 unk24;     /* 1 = party member, 0 = other */
    u8 pad25[3];
    u8 unk28[0x104]; /* saved VRAM (StoreImage) */
    s32 unk12C; /* name id */
    s32 unk130; /* character index */
    RECT unk134[5]; /* windows */
} NameEntry;
extern NameEntry D_801F4BBC;

/* A RECT, not code: splat starts .data at 0x6C50 instead of 0x6C48, so the
 * symbol keeps the func_ name. */
extern RECT D_801F4B48;
extern u8 D_801F4B50[2];
extern RECT *D_801F4B54; /* window pointers into D_801F4BBC.unk134 */
extern RECT *D_801F4B58;
extern RECT *D_801F4B5C;
extern RECT *D_801F4B60;
extern RECT *D_801F4B64;
extern RECT D_801F4B68;
extern RECT D_801F4B70;
extern RECT D_801F4B78;
extern s32 D_801F4B80[12];
extern u8 D_801F4BB0[12];
extern u8 g_nameMenuText[];
extern u8 D_801EE0B4[][0x6C0];

/* main exe: candidates for ff9.h */
void SetLineF2(LINE_F2 *);
extern s32 D_8007047C;
s32 func_80028F3C(s32, s32, s32, u8, s32);
u8 *func_8002BA90(u32 *, void *);
extern s32 D_80077DEC; /* = g_help.otz, referenced by its own name */

/* x offset of a 5-column group in the name-entry character grid */
static inline s32 grp_x(s32 c) {
    return c * 72 + 0x3E;
}

void func_801F3874(void);
void func_801F3A98(void);
void func_801F3BBC(void);
void func_801F3D9C(void);
void func_801F3FAC(void);
void func_801F4110(void);
void func_801F4238(void);
void func_801F459C(void);
void func_801F47A0(void);
void func_801F47E8(void);
void func_801F48A0(void);
void func_801F490C(void);
void func_801F49A0(s32);
void func_801F4A34(void);
s32 func_801F4A68(s32);
s32 func_801F4A84(s32);
void func_801F4A9C(void);
void func_801F4AE0(void);

#endif
