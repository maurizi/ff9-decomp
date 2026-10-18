#ifndef FX_17D_6_H
#define FX_17D_6_H

/* Effect module fx_17d_6 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx17d6Trail;

extern Unk800F78A4 *D_801E93B4;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[17];
} Fx17d6Trail17; /* 0x8C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[5];
} Fx17d6Trail5; /* 0x2C */

typedef struct {
    SVECTOR unk0; /* to */
    SVECTOR unk8; /* from */
    u8 unk10; /* trail index */
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14; /* angle */
    u8 unk15;
    u8 pad16[2];
    s32 *unk18; /* noise */
} Fx17d6Bolt;

extern Fx17d6Trail17 D_801EB124[];
extern Fx17d6Trail5 D_801EB474[];

typedef struct {
    s32 unk0; /* frame */
    u8 pad4[0xC];
    s16 *unk10; /* pool */
} Fx17d6Inst;

typedef struct {
    s32 *unk0; /* noise */
    s32 unk4; /* spawn count */
    s32 unk8;
} Fx17d6Work83;

extern SVECTOR D_801EB104; /* to */
extern SVECTOR D_801EB10C; /* from */
extern s32 D_801EB114; /* state */
extern s32 D_801EB584[64][3]; /* noise */

s32 func_801E8ED4(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

void func_801E8B10(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d6Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* bolt target */
    SVECTOR unk10; /* particle position */
    SVECTOR unk18; /* camera position */
    SVECTOR unk20; /* camera target */
    SVECTOR unk28; /* bone positions */
    SVECTOR unk30;
    Fx17d6Vec3 unk38; /* model position */
    SVECTOR unk44; /* model rotation */
    Fx17d6Vec3 unk4C; /* model scale */
    Fx17d6Vec3 unk58; /* summon position */
    Fx17d6Vec3 unk64; /* summon scale */
    SVECTOR unk70; /* summon rotation */
    Pool6CEntry *unk78;
    s32 unk7C; /* models */
    s32 unk80;
} Fx17d6Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A8];
        BtlUnit9CC *unk1A8;
        BtlUnit9CC *unk1AC;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx17d6Summon;

extern SVECTOR D_801E9360;
extern u8 D_801E9368[];
extern BtlModel D_801E93C4[];
extern s32 D_801E9624[];
extern SVECTOR D_801EB104;
extern SVECTOR D_801EB10C;

#define SUMMON(env) ((Fx17d6Summon *)(env)->unk4)

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
