#ifndef FX_0AD_H
#define FX_0AD_H

/* Effect module fx_0ad (dir 13 file 0xAD): enemy Aero Breath (Worm Hydra). Emitters only. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ad effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERO_BREATH_SWIRL       0x1 /* emitter: green wind swirl */
#define FX_AERO_BREATH_WIND_TRAILS 0x6 /* emitter: green wind trails spiralling */

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos10; /* caster body position */
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx0adWork;

extern Unk800F78A4 *fx_0ad_env;
extern s32 fx_0ad_trails[];

#endif
