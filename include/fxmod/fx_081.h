#ifndef FX_081_H
#define FX_081_H

/* Effect module fx_081 (dir 13 file 0x81): LV4 Holy (party and enemy). Rings, two models and a sphere of
 * particles (fx_081_ringCounts). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_081_env;
extern SVECTOR fx_081_camEndPos;
extern SVECTOR fx_081_leadPos;
extern Pool6CEntry *fx_081_emitter0;
extern Unk80100C78Entry *fx_081_task;
extern s32 fx_081_glow; /* glow level */
extern Pool6CEntry *fx_081_emitter1;
extern SVECTOR fx_081_bodyPos; /* caster body position */
extern BtlModelBuf fx_081_effModels[];
extern u8 fx_081_trails[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR pos10;
    Fx081Vec3 modelPos18;
    SVECTOR pos24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx081Work80; /* 0x38 */

static void fx_081_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame,
                   s32 n);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 otZ8; /* priority base */
    SVECTOR posC; /* target position */
    SVECTOR pos14;
    Fx081Vec3 unk1C; /* write-only (copy of posC) */
    s32 model28; /* model */
} Fx081Work81; /* 0x2C */

extern s16 fx_081_ringCounts[10]; /* ring point counts */
s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
