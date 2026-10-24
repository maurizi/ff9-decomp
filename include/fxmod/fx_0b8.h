#ifndef FX_0B8_H
#define FX_0B8_H

/* Effect module fx_0b8 (dir 13 file 0xB8). */
#include "btlfx_mod.h"

typedef struct {
    u8 u;
    u8 v;
} Fx0b8UV;

extern SVECTOR D_801EA7B0[32];

void func_801EA1E0(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr);
void func_801EA424(s32 ot);
void func_801EA4E0(s32 a, s32 b, s32 t, s32 n, s32 mode, s32 ot);
s32 func_801EA558(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n);
extern Unk800F78A4 *D_801EA7A0;
extern SVECTOR D_801EA680[]; /* colour table */
void func_801E9D80(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr);
s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 unk0[0x28]; /* flags and timers */
    s32 unk28; /* model */
    SVECTOR unk2C; /* rotation */
    VECTOR unk34; /* scale */
    VECTOR unk44; /* position */
    s32 unk54; /* models */
    s32 unk58;
    SVECTOR unk5C;
    SVECTOR unk64;
    VECTOR unk6C;
    VECTOR unk7C;
    VECTOR unk8C;
    VECTOR unk9C;
    VECTOR unkAC;
    SVECTOR unkBC;
    SVECTOR unkC4;
    s32 unkCC; /* models */
    s32 unkD0;
    s32 unkD4;
    VECTOR unkD8;
    s32 unkE8; /* models */
    s32 unkEC;
    s32 unkF0;
    s32 unkF4;
    s32 unkF8;
    VECTOR unkFC;
    VECTOR unk10C;
    SVECTOR unk11C;
    s32 unk124;
    s32 unk128; /* models */
    s32 unk12C;
    s32 unk130;
    s32 unk134;
    s32 unk138;
    VECTOR unk13C;
    VECTOR unk14C;
    SVECTOR unk15C;
    SVECTOR unk164;
    s32 unk16C; /* models */
    s32 unk170;
    SVECTOR unk174;
    s32 unk17C;
    s32 unk180;
    Pool6CEntry *unk184;
    SVECTOR unk188;
    Pool6CEntry *unk190;
    SVECTOR unk194;
    Pool6CEntry *unk19C;
    SVECTOR unk1A0;
    SVECTOR unk1A8;
    Pool6CEntry *unk1B0;
    s32 unk1B4;
    Pool6CEntry *unk1B8;
    SVECTOR unk1BC;
    SVECTOR unk1C4;
    SVECTOR unk1CC;
    SVECTOR unk1D4;
    SVECTOR unk1DC;
    s32 unk1E4;
    s32 unk1E8;
} Fx0b8Work80; /* 0x1EC */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9]; /* trail points */
    s32 unk4C[9]; /* x */
    s32 unk70[9]; /* y */
    s32 unk94[9]; /* x speed */
    s32 unkB8[9]; /* y speed */
    s32 unkDC; /* timer */
} Fx0b8Bolt; /* 0xE0 */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0b8Vec3;

typedef struct {
    SVECTOR unk0; /* rotation */
    Fx0b8Vec3 unk8; /* scale */
    SVECTOR unk14; /* home position */
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 pad22;
    SVECTOR unk24; /* rotation */
    s16 unk2C; /* rotation speed */
    s16 unk2E;
    s16 unk30;
    s16 pad32;
} Fx0b8Unit; /* 0x34 */

typedef struct {
    s32 unk0; /* y */
    s32 unk4; /* h */
    s32 unk8; /* x */
    s32 unkC; /* colours */
    s32 unk10;
    s32 unk14; /* x speed */
    s32 unk18; /* y speed */
    s32 unk1C;
} Fx0b8Band; /* 0x20 */

typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx0b8Summon;

#define SUMMON(env) ((Fx0b8Summon *)(env)->loader)

extern Fx0b8Band D_801EA6D8[4];
extern BtlModel D_801EA8B0[];
extern s32 D_801EC1B0;
extern Fx0b8Bolt D_801EC1C0[8];
extern Fx0b8Unit D_801EC8C0[];
extern s32 D_801ECA60[];
extern s32 D_801ED8C0;

#endif
