#ifndef FX_09A_H
#define FX_09A_H

/* Effect module fx_09a (dir 13 file 0x9A): LV5 Death (party and enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx09aSeg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx09aSeg *buf, s32 abr);
extern Unk800F78A4 *fx_09a_env;
static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 unit8; /* unit */
    s32 unitsDoneC; /* units done */
    s32 unitCount10; /* unit count */
    SVECTOR pos14; /* unit position */
    SVECTOR rot1C; /* unit rotation */
    SVECTOR pos24; /* summon position */
    s32 ribbonHead2C; /* ribbon head */
    s32 unk30;
    SVECTOR rot34; /* summon rotation */
    s32 scale3C[3]; /* summon scale */
} Fx09aWork; /* 0x48 */

/* the summon slot behind env->loader */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx09aSummon;

extern s32 fx_09a_hitBlocks[2];
extern SVECTOR fx_09a_ribbonColA;
extern SVECTOR fx_09a_ribbonColB;
extern SVECTOR fx_09a_ribbonColC;
extern SVECTOR fx_09a_bonePtA;
extern SVECTOR fx_09a_bonePtB;
extern SVECTOR fx_09a_bonePtC;
extern Unk80100DC054 fx_09a_summonModel;
extern u8 fx_09a_trails[];
extern Fx09aSeg fx_09a_ribbon0[];
extern Fx09aSeg fx_09a_ribbon1[];

#endif
