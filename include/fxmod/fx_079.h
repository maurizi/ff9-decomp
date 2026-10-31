#ifndef FX_079_H
#define FX_079_H

/* Effect module fx_079 (dir 13 file 0x79): Demi Shock. Three models over the target, a lift of the target and 5
 * jagged btlfx_drawTrail strips from a bone of unit 0x10. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 halfWidth6; /* half-width of the ribbon at this point (scales the edge normal) */
} Fx079Pt;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    Fx079Pt pts4[11];
    s32 endX5C; /* end offset */
    s32 endY60;
    s32 endZ64;
    s32 pad68;
    s32 offsetY6C[11]; /* y offset */
    s32 unk98[11];
    s32 speedYC4[11]; /* y speed */
    s32 unkF0[11];
    s32 timer11C; /* timer */
} Fx079Strip; /* 0x120 */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR scaleC; /* scale */
} Fx079Model; /* 0x1C */

typedef struct {
    s32 timers0[16]; /* timers */
    Fx079Model models40[3];
    VECTOR pos94; /* position */
} Fx079Work; /* 0xA4 */

extern Unk800F78A4 *fx_079_env;
extern BtlModel fx_079_effModels[];
extern SVECTOR fx_079_bonePos;
extern SVECTOR fx_079_bodyPos;
extern SVECTOR fx_079_homePos;
extern VECTOR fx_079_unitScale;
extern SVECTOR fx_079_curBodyPos;
extern s32 fx_079_unitHeight;
extern Pool6CEntry *fx_079_emitter;
extern SVECTOR fx_079_emitStartPos;
extern SVECTOR fx_079_emitPos;
extern Fx079Strip fx_079_strips[5];

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);

#endif
