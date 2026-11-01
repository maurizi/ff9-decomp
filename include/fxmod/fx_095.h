#ifndef FX_095_H
#define FX_095_H

/* Effect module fx_095 (dir 13 file 0x95): Stop. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    s32 spawnCount4; /* spawned */
    DVECTOR screenXY8; /* caster screen position */
    s32 releaseC; /* release */
} Fx095Work80; /* 0x10 */

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only (cleared at init) */
    u16 pixels8[0x3000]; /* captured screen strip */
} Fx095Work81; /* 0x6008 */

typedef struct {
    s32 angle0; /* angle */
    s32 state4; /* state */
    s32 index8; /* index */
} Fx095Part; /* 0xC */

typedef struct {
    u8 pad0[8];
    s16 screenX8; /* screen x */
    s16 screenYA; /* screen y */
    s32 releaseC; /* release */
} Fx095Ctl;

extern Unk800F78A4 *fx_095_env;
extern SVECTOR fx_095_bodyPos;

static s32 fx_095_op80PoolTask(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool, Fx095Ctl *ctl);
void fx_095_greyScreenCopy(s32 step, s32 x, s32 y, u16 *buf, RECT *area);
void fx_095_drawTexDisc(s32 ot, s32 x, s32 y, s32 outerR, s32 bright, RECT *rect);

extern s32 fx_095_markBlocks[]; /* file ids */
extern s32 fx_095_glowBlocks[]; /* file ids */
extern s32 fx_095_markBlocks2[]; /* file ids */

#endif
