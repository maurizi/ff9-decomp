#ifndef OVL_39_H
#define OVL_39_H

/* Declarations local to overlay 0x39 (save / load menu, loaded at
 * 0x801EDF00). Overlay addresses mean different things in different
 * overlays, so they live here, not in ff9.h. */
#include "ff9.h"

/* Save menu state (D_801FB1A8). */
typedef struct {
    u8 unk0;
    u8 unk1; /* state */
    u8 unk2; /* 1 = busy, 2 = done */
    u8 pad3[3];
    u8 unk6;
    u8 unk7;
} SaveMenu;
extern SaveMenu D_801FB1A8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Unk801FBAC8;
extern Unk801FBAC8 D_801FBAC8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
} Unk801FBEE0;
extern Unk801FBEE0 D_801FBEE0;

typedef struct {
    u8 unk0;
    u8 unk1;
} Unk801FBEF8;
extern Unk801FBEF8 D_801FBEF8;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
} Unk801FC348;
extern Unk801FC348 D_801FC348;
extern u8 D_801FC358;

typedef struct {
    u8 unk0;
    u8 pad1;
    u8 unk2;
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
} Unk801FC388;
extern Unk801FC388 D_801FC388;

/* memory card status */
extern s32 D_801FB074;
extern s32 D_801FB078;
extern s32 D_801FB07C;
extern s32 D_801FB080;
extern s32 D_801FB084;
extern s32 D_801FB088;
extern s32 D_801FB08C;
extern s32 D_801FC604;

#endif
