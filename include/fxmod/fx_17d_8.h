#ifndef FX_17D_8_H
#define FX_17D_8_H

/* Effect module fx_17d_8 (dir 13 file 0x17d): Ark, full summon (load group 8). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    s16 unk4; /* position */
    s16 unk6;
    s16 unk8;
    s16 padA;
    SVECTOR rotC; /* rotation */
} Fx17d8Obj;

typedef struct {
    u8 pad0[0x78];
    s32 unk78; /* model */
} Fx17d8Inst;

extern s32 D_801E8FCC;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d8Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR pos10; /* particle position */
    SVECTOR unk18; /* particle target */
    SVECTOR unk20; /* camera position */
    Fx17d8Vec3 unk28; /* model position */
    SVECTOR unk34;
    Fx17d8Vec3 unk3C; /* model scale */
    Fx17d8Vec3 unk48; /* spawn position */
    SVECTOR unk54;
    SVECTOR rot5C; /* model rotation */
    s32 model64; /* models */
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    MATRIX unk7C; /* saved matrices */
    MATRIX unk9C;
} Fx17d8Work80; /* 0xBC */

extern SVECTOR D_801E89F4;
extern Unk800F78A4 *fx_17d_8_env;
extern BtlModel fx_17d_8_effModels[];

static s32 fx_17d_8_op80PoolTask(s32 mode, Fx17d8Obj *w, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx17d8Inst *inst);
s32 fx_17d_8_op80(s32 mode, Fx17d8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
