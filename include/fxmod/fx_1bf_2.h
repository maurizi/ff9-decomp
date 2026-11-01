#ifndef FX_1BF_2_H
#define FX_1BF_2_H

/* Effect module fx_1bf_2 (dir 13 file 0x1bf): Ark (short); Eternal Darkness (load group 2). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 scale2; /* scale */
    s16 posX4; /* position */
    s16 posY6;
    s16 posZ8;
    s16 padA;
    SVECTOR rotC; /* rotation */
} Fx1bf2Obj;

typedef struct {
    u8 pad0[0x78];
    s32 model78; /* model */
} Fx1bf2Inst;

/* copied */
extern s32 fx_1bf_2_particleDrift;
/* end copied */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1bf2Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* home position */
    SVECTOR pos10; /* particle position */
    SVECTOR target18; /* particle target */
    SVECTOR camPos20; /* camera position */
    Fx1bf2Vec3 pos28; /* model position */
    SVECTOR unk34; /* write-only (cleared at init) */
    Fx1bf2Vec3 scale3C; /* model scale */
    Fx1bf2Vec3 pos48; /* spawn position */
    SVECTOR pos54;
    SVECTOR rot5C; /* model rotation */
    s32 model64; /* models */
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    MATRIX viewMtx7C; /* saved matrices */
    MATRIX viewMtx9C;
} Fx1bf2Work80; /* 0xBC */

extern SVECTOR fx_1bf_2_basePos;
extern Unk800F78A4 *fx_1bf_2_env;
extern BtlModelBuf fx_1bf_2_effModels[];

s32 fx_1bf_2_op80(s32 mode, Fx1bf2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
