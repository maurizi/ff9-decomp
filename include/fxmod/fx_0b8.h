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
static s32 fx_0b8_op80ParticleMotion(Pool30Entry *pt, s32 a1, SVECTOR *pos, s32 a3, s32 frame, s32 n);
extern Unk800F78A4 *fx_0b8_env;
extern SVECTOR D_801EA680[]; /* colour table */
void func_801E9D80(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr);
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

typedef struct {
    u8 unk0[0x28]; /* flags and timers */
    s32 model28; /* model */
    SVECTOR rot2C; /* rotation */
    VECTOR scale34; /* scale */
    VECTOR pos44; /* position */
    s32 model54; /* models */
    s32 model58;
    SVECTOR rot5C;
    SVECTOR rot64;
    VECTOR pos6C;
    VECTOR pos7C;
    VECTOR scale8C;
    VECTOR pos9C;
    VECTOR scaleAC;
    SVECTOR rotBC;
    SVECTOR posC4;
    s32 modelCC; /* models */
    s32 modelD0;
    s32 modelD4;
    VECTOR scaleD8;
    s32 modelE8; /* models */
    s32 modelEC;
    s32 modelF0;
    s32 modelF4;
    s32 modelF8;
    VECTOR posFC;
    VECTOR scale10C;
    SVECTOR rot11C;
    s32 unk124;
    s32 model128; /* models */
    s32 model12C;
    s32 model130;
    s32 model134;
    s32 model138;
    VECTOR pos13C;
    VECTOR scale14C;
    SVECTOR rot15C;
    SVECTOR pos164;
    s32 model16C; /* models */
    s32 model170;
    SVECTOR rot174;
    s32 model17C;
    s32 model180;
    Pool6CEntry *emit184;
    SVECTOR pos188;
    Pool6CEntry *emit190;
    SVECTOR pos194;
    Pool6CEntry *emit19C;
    SVECTOR pos1A0;
    SVECTOR pos1A8;
    Pool6CEntry *emit1B0;
    s32 unk1B4;
    Pool6CEntry *emit1B8;
    SVECTOR pos1BC;
    SVECTOR unk1C4;
    SVECTOR pos1CC;
    SVECTOR pos1D4;
    SVECTOR pos1DC;
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
extern BtlModel fx_0b8_effModels[];
extern s32 D_801EC1B0;
extern Fx0b8Bolt D_801EC1C0[8];
extern Fx0b8Unit D_801EC8C0[];
extern s32 D_801ECA60[];
extern s32 D_801ED8C0;

#endif
