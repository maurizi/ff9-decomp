#ifndef OVL_64_H
#define OVL_64_H

/* Declarations local to overlay 0x64 (FMV player, loaded at 0x80198700;
 * other overlays such as ovl_15 call into it). Overlay addresses mean
 * different things in different overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* main exe: candidates for ff9.h */
extern s8 D_80071E31;
extern s32 D_80071E34;
extern s8 D_80071E38;
void func_80056228(void);
void func_80065238(s32, s32);

/* PsyQ libpress */
void DecDCToutCallback(void (*func)());

/* ---- this overlay ---- */

typedef struct {
    s16 unk0; /* count */
    struct {
        s32 unk0;
        u8 pad4[0x14 - 4];
    } *unk4;
} Ovl64Movie; /* 8-byte entries */
extern Ovl64Movie D_8019DC5C[6];

typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl64Size;
extern Ovl64Size D_8019DC88;

/* FMV player state (D_8019DC94). */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 unk4;
    u8 pad5;
    s8 unk6;
    u8 pad7;
    s32 unk8[12];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    void *unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    void (*unk60)(s8 *);
    s32 unk64;
} Ovl64State;
extern Ovl64State D_8019DC94;

/* sector header of the current frame */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u8 pad12[2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} Ovl64Header;
extern Ovl64Header D_8019F258;
extern u16 D_8019F278;

/* decode / display state (D_8019F4D8) */
typedef struct {
    u8 pad0[0x18];
    s16 unk18; /* width */
    u8 pad1A[0x2C - 0x1A];
    s16 unk2C;
    u8 pad2E[0x38 - 0x2E];
} Ovl64Dec;
extern Ovl64Dec D_8019F4D8;
extern u8 D_8019F288[];

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl64Vlc;
extern Ovl64Vlc D_8019DCFC[108];
extern s32 D_8019DDD4;

void func_8019A7A8(s32, s32);
void func_8019C864(void);
s32 func_8019C99C(s32 *);
s32 func_8019C9B4(u32);
s32 func_8019CBF4(s32 *);
s32 func_8019CC3C(s32);
s32 func_80199F30(s32);

#endif
