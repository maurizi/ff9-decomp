#ifndef FX_0B9_H
#define FX_0B9_H

/* Effect module fx_0b9 (dir 13 file 0xB9): enemy Sandstorm (Core/Sand Golem, ...). Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0b9 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SANDSTORM_SAND_STREAK   0x1 /* emitter: grey sand streak (screen trail) */
#define FX_SANDSTORM_SAND_STREAK_2 0x2 /* emitter: grey sand streak (homing 3D trail) */
#define FX_SANDSTORM_SAND_CLOUD    0x3 /* emitter: grainy grey sand clouds */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* position: env->targets->centerX..Z */
    SVECTOR pos10; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR rot20; /* angles from pos10 to pos8 */
} Fx0b9Work80;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* unit position, raised by 0xC8 */
    SVECTOR rotC; /* angles from fx_0b9_srcPos to pos4 */
    s32 unit14; /* unit */
} Fx0b9Work81;

extern Unk800F78A4 *fx_0b9_env;
extern SVECTOR fx_0b9_srcPos;
extern s32 fx_0b9_trails[];
extern s32 fx_0b9_trails2[];

#endif
