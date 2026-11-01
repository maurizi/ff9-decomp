#ifndef FX_0A0_H
#define FX_0A0_H

/* Effect module fx_0a0 (dir 13 file 0xA0): enemy Web, String (Axe Beak/Carve Spider and others). Particles with
 * trails and a thread line to the caster (fxm_drawTrailLine). Evidence: [data] the party ability / item tables
 * and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR pts4[2];
} Fx0a0Line;

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR posC;
    SVECTOR pos14; /* caster body position */
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
} Fx0a0Work;

extern Fx0a0Line fx_0a0_line;
extern Unk800F78A4 *fx_0a0_env;
extern s32 fx_0a0_trails[];
extern SVECTOR fx_0a0_bonePos;
extern Unk80100C78Entry *fx_0a0_task;
extern s32 fx_0a0_trailAlpha;

#define gte_stsxy01(r0, r1) __asm__ volatile( \
    "swc2\t$12, 0(%0);" \
    "swc2\t$13, 0(%1)" \
    : : "r"(r0), "r"(r1) : "memory")

void fx_0a0_drawGouraudLine(SVECTOR *a, SVECTOR *b, CVECTOR *c0, CVECTOR *c1, s32 p, s32 abr);
static void fxm_drawTrailLine(u8 *file, SVECTOR *a, SVECTOR *b, s32 frame, s32 fade, s32 abr, s32 lineWidth);
static void fx_0a0_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame);

#endif
