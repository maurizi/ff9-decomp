#ifndef FX_17A_H
#define FX_17A_H

/* Effect module fx_17a (dir 13 file 0x17A): Madeen (short). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    s16 state0; /* state */
    s16 unk2;
    s16 scale4; /* size */
    s16 unk6;
    s16 unk8;
    s16 speedA; /* drift */
    SVECTOR posC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C;
    s16 unk1E;
    s32 unk20;
    s32 unk24;
} Fx17aPart;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17aVec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[4];
    SVECTOR posC; /* position */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
    u8 pad2C[8];
    Fx17aVec3 pos34; /* model position */
    Fx17aVec3 scale40; /* model scale */
    SVECTOR rot4C; /* model rotation */
    u8 pad54[0xC];
    Fx17aVec3 scale60;
    SVECTOR rot6C;
    u8 pad74[0xC];
    s32 model80; /* models */
    s32 model84;
    s32 model88;
    s32 model8C;
    SVECTOR rot90[4];
    Pool6CEntry *emitB0;
} Fx17aWork80; /* 0xB4 */

typedef struct {
    u8 pad0[4];
    SVECTOR points4[0x40]; /* points; vy decays */
    u8 pad204[8];
} Fx17aRing; /* 0x20C */

extern SVECTOR fx_17a_centerPos;
extern u8 fx_17a_cylColors[];
extern Fx17aRing fx_17a_trails[2];
extern BtlModelBuf fx_17a_effModels[];

static s32 fx_17a_op80PoolTask(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* propagated */

extern Unk800F78A4 *fx_17a_env;
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);

#endif
