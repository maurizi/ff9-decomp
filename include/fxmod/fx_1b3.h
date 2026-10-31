#ifndef FX_1B3_H
#define FX_1B3_H

/* Effect module fx_1b3 (dir 13 file 0x1b3). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* velocity */
    s16 unk2;
    s16 unk4;
    s16 unk6; /* frames left */
    s16 unk8; /* spin */
    s16 unkA;
    s16 unkC;
    s16 unkE; /* scale step */
    s16 unk10; /* center */
    s16 unk12;
    s16 unk14;
    s16 unk16; /* scale */
} Fx1b3Piece; /* 0x18 */

typedef struct {
    s32 unk0; /* model */
    s32 unk4; /* frames left */
    s32 unk8[8]; /* primitive counts */
    SVECTOR *unk28[8]; /* vertices */
    Fx1b3Piece *unk48[8]; /* pieces */
} Fx1b3Split;

extern u8 *D_801EB0C0;

typedef struct {
    s32 unk0; /* mode */
    s16 unk4; /* target */
    s16 unk6;
    s16 unk8;
    s16 padA;
    s32 unkC; /* radius */
    s32 unk10; /* sweep angle */
    s32 unk14; /* sweep step */
} Fx1b3Blast;

extern Fx1b3Blast D_801EB0D4;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9]; /* drawn points */
    s32 unk4C; /* frames left */
    SVECTOR unk50[9]; /* points */
    SVECTOR unk98[9]; /* point velocities */
} Fx1b3Trail; /* 0xE0 */

typedef struct {
    s32 unk0[32]; /* timers: 0 tint, 1 flash, 2 trails, 3 trail kind, 4 sweep, 5 sweep split, 6..0xD */
    Pool6CEntry *emit80[32];
    SVECTOR pos100; /* body position */
    SVECTOR pos108; /* unit position */
    SVECTOR pos110;
    SVECTOR pos118;
    SVECTOR pos120;
    SVECTOR unk128;
    SVECTOR unk130;
    SVECTOR unk138;
    SVECTOR unk140;
    SVECTOR unk148;
    SVECTOR unk150;
    s32 unk158; /* model */
    SVECTOR rot15C;
    SVECTOR rot164;
    VECTOR pos16C;
    VECTOR scale17C;
    SVECTOR rot18C;
    SVECTOR rot194;
    VECTOR pos19C;
    VECTOR pos1AC;
    VECTOR scale1BC;
    SVECTOR rot1CC;
    VECTOR pos1D4;
    VECTOR scale1E4;
    SVECTOR pos1F4;
    SVECTOR rot1FC;
    SVECTOR rot204;
    VECTOR pos20C;
    VECTOR pos21C;
    VECTOR scale22C;
    SVECTOR rot23C;
    VECTOR pos244;
    VECTOR scale254;
} Fx1b3Work80; /* 0x264 */

extern s32 D_801EAFC4[];
extern s32 D_801EB004[];
extern s32 D_801EB018[];
extern BtlTexInfo D_801EB084;
extern BtlTexInfo D_801EB0A4;
extern u8 *D_801EB0BC;
extern Fx1b3Trail *D_801EB0C4;
extern s32 D_801EB0C8;
extern s32 D_801EB0CC;
extern Fx1b3Split D_801EB0F4[9];
extern Fx1b3Split D_801EB4A4[2];
extern Fx1b3Split D_801EB574;
extern Fx1b3Split D_801EB5E4[2];
extern Fx1b3Split D_801EB6B4[3];
extern SVECTOR D_801EB7EC;
extern BtlModel fx_1b3_effModels[];

void func_801EA39C(Fx1b3Split *sp);
void func_801EA7E4(Fx1b3Split *sp);

/* copied */
extern Unk800F78A4 *fx_1b3_env;
/* end copied */

#endif
