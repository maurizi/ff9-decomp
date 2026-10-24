#ifndef FX_0E0_H
#define FX_0E0_H

/* Effect module fx_0e0 (dir 13 file 0x0E0). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* time */
    SVECTOR unk8; /* caster bone position */
} Fx0e0Work84;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[25];
} Fx0e0Bolt;

extern Unk800F78A4 *D_801EA918;

void func_801E7718(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
void func_801E7910(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip);
void func_801E7B08(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr);
void func_801E7E18(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr);
void func_801E8430(Unk80100C78Entry *entry, Fx0e0Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad);
s32 fx_0e0_op84(s32 mode, Fx0e0Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* sub-state */
    SVECTOR unk4; /* velocity (vx, vy, vz); pad = model scale index */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* rotation speed */
    s16 unk1E;
    Pool6CEntry *unk20; /* particle */
} Fx0e0Obj;

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* target */
    u8 pad10[0x5C - 0x10];
    s32 unk5C; /* model */
    u8 pad60[0x70 - 0x60];
    s32 unk70; /* model */
} Fx0e0Inst;

extern s32 D_801EA8B8[];
extern s32 D_801EB1D8;
extern s32 D_801EB1DC;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e0Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10; /* target */
    SVECTOR unk18; /* camera target */
    u8 pad20[4];
    Fx0e0Vec3 unk24; /* model position */
    Fx0e0Vec3 unk30; /* model scale */
    SVECTOR unk3C; /* model rotation */
    Fx0e0Vec3 unk44; /* scale */
    SVECTOR unk50; /* rotation */
    u8 pad58[4];
    s32 unk5C; /* models */
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    u8 pad78[0xC];
} Fx0e0Work80; /* 0x84 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
} Fx0e0Spark;

typedef union {
    VECTOR v;
    struct {
        CVECTOR col;
        s32 pad;
        SVECTOR rot;
    } s;
} Fx0e0Tmp;

extern SVECTOR D_801EA8B0;
extern CVECTOR D_801EA8C8[];
extern Fx0e0Bolt D_801EA928[];
extern BtlModel D_801EAB98[];

#endif
