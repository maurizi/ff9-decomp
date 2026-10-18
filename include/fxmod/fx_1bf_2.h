#ifndef FX_1BF_2_H
#define FX_1BF_2_H

/* Effect module fx_1bf_2 (dir 13 file 0x1bf). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    s16 unk4; /* position */
    s16 unk6;
    s16 unk8;
    s16 padA;
    SVECTOR unkC; /* rotation */
} Fx1bf2Obj;

typedef struct {
    u8 pad0[0x78];
    s32 unk78; /* model */
} Fx1bf2Inst;

/* copied */
extern s32 D_801E8FCC;
/* end copied */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1bf2Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR unk10; /* particle position */
    SVECTOR unk18; /* particle target */
    SVECTOR unk20; /* camera position */
    Fx1bf2Vec3 unk28; /* model position */
    SVECTOR unk34;
    Fx1bf2Vec3 unk3C; /* model scale */
    Fx1bf2Vec3 unk48; /* spawn position */
    SVECTOR unk54;
    SVECTOR unk5C; /* model rotation */
    s32 unk64; /* models */
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    MATRIX unk7C; /* saved matrices */
    MATRIX unk9C;
} Fx1bf2Work80; /* 0xBC */

extern SVECTOR D_801E89F4;
extern Unk800F78A4 *D_801E8A44;
extern BtlModel D_801E8A54[];

s32 fx_1bf_2_op80(s32 mode, Fx1bf2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
