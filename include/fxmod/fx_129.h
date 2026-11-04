#ifndef FX_129_H
#define FX_129_H

/* Effect module fx_129 (dir 13 file 0x129): Pumpkin Head; enemy Pumpkin Head (Abomination/Hedgehog Pie, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_129 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PUMPKIN_HEAD_SMOKE_BURST_2 0x1 /* emitter: pale smoke burst with a dark red glow, later bounces */
#define FX_PUMPKIN_HEAD_SMOKE_BURST   0x3 /* emitter: pale smoke burst with a dark red glow, first bounce */
#define FX_PUMPKIN_HEAD_PUMPKIN       0x7 /* model: round pumpkin head with a bow tie, drawn with the caster's texture list */

typedef struct {
    s32 vx, vy, vz;
} Fx129Vec3;

typedef struct {
    s32 model0; /* model */
    VECTOR pos4; /* model position */
    SVECTOR rot14; /* model rotation */
    VECTOR scale1C; /* model scale */
    SVECTOR pos2C; /* target body position */
    s32 phase34; /* bounce phase */
    s32 speed38; /* bounce height */
    s32 height3C;
    s32 height40; /* height offset */
    s32 frame44; /* phase time */
    s32 unk48; /* unused */
    s32 fade4C; /* fade time */
    s32 color50; /* brightness */
    s32 timer54; /* squash countdown */
    s32 count58; /* landings */
    s32 unk5C; /* write-only (cleared at init) */
    s32 timer60; /* shake countdown */
    Fx129Vec3 scale64; /* target scale */
    Fx129Vec3 scale70;
} Fx129Work;

extern Unk800F78A4 *fx_129_env;
extern BtlModelBuf fx_129_effModels[];

#endif
