#ifndef FX_0FB_1_H
#define FX_0FB_1_H

/* Effect module fx_0fb_1 (dir 13 file 0x0FB): Madeen, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* unit home position */
    SVECTOR rotC; /* unit rotation */
    s16 spinX14; /* spin speed */
    s16 spinY16;
    s16 spinZ18;
    u8 pad1A[0x1C - 0x1A];
    SVECTOR rot1C; /* current rotation */
    s32 unitCount24; /* unit count */
    s32 lift28; /* lift height */
    s32 unit2C; /* unit */
} Fx0fb1Work82; /* 0x30 */

typedef struct {
    s32 vx, vy, vz;
} Fx0fb1Vec3;

typedef struct {
    Fx0fb1Vec3 scale0[8]; /* unit scales */
    SVECTOR homePos60[8]; /* unit home positions */
    SVECTOR liftPosA0[8]; /* lifted positions */
    SVECTOR rotE0[8]; /* unit rotations */
    SVECTOR spin120[8]; /* spin speeds */
    SVECTOR curRot160[8]; /* current rotations */
    s32 unitCount1A0; /* unit count */
    s32 liftJitter1A4;
} Fx0fb1Work81; /* 0x1A8 */

extern Unk800F78A4 *fx_0fb_1_env;
extern s32 fx_0fb_1_unitsRestored;
extern s32 fx_0fb_1_rockRgb; /* rock model brightness */

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);
s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    s16 kind0; /* kind */
    s16 state2; /* state */
    s16 unk4; /* velocity */
    s16 unk6;
    s16 unk8;
    s16 speedA; /* speed */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spinX1C; /* spin */
    s16 spinY1E;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
} Fx0fb1Rock; /* 0x28 */

typedef struct {
    u8 pad0[0x4C];
    s32 model4C; /* model */
    s32 model50; /* model */
    u8 pad54[0x5C - 0x54];
    s32 modelZ5C; /* pos54.vz of Fx0fb1Op80Work */
    u8 pad60[0x6C - 0x60];
    SVECTOR rot6C; /* model rotation (rot6C of Fx0fb1Op80Work) */
    u8 pad74[0xC0 - 0x74];
    s32 modelC0; /* model */
} Fx0fb1Work80;

extern s32 fx_0fb_1_rocksHeld;


typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    u8 pad8[4];
    SVECTOR centerC; /* position */
    SVECTOR pos14;
    Fx0fb1Vec3 summonPos1C; /* summon position */
    Fx0fb1Vec3 summonScale28; /* summon scale */
    SVECTOR rot34; /* summon rotation */
    SVECTOR camTarget3C; /* camera */
    u8 pad44[8];
    s32 model4C[2]; /* models */
    Fx0fb1Vec3 pos54; /* model position */
    Fx0fb1Vec3 scale60; /* model scale */
    SVECTOR rot6C; /* model rotation */
    Fx0fb1Vec3 pos74;
    Fx0fb1Vec3 scale80;
    SVECTOR rot8C;
    Fx0fb1Vec3 scale94;
    s32 modelA0; /* models */
    s32 modelA4;
    s32 modelA8;
    s32 modelAC;
    s32 modelB0;
    s32 modelB4;
    s32 modelB8;
    s32 modelBC;
    s32 modelC0;
    SVECTOR rotC4[4];
    Pool6CEntry *emitE4;
} Fx0fb1Op80Work; /* 0xE8 */

typedef struct {
    u8 pad0[4];
    SVECTOR unk4[0x40];
    u8 pad204[8];
} Fx0fb1Ring; /* 0x20C */

typedef struct {
    u8 pad0[0x38];
    BtlModelBuf *effModels38;
    u8 pad3C[0x54 - 0x3C];
    struct {
        u8 pad0[0x18C];
        BtlMotion *motion18C;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60;
} Fx0fb1Summon;

extern SVECTOR fx_0fb_1_origin;
extern u8 fx_0fb_1_gradient[];
extern s16 fx_0fb_1_modelBlocks[2];
extern s16 fx_0fb_1_rockOffsets[];
extern Fx0fb1Ring fx_0fb_1_strips[2];
extern BtlModelBuf D_801F0D3C[];

#endif
