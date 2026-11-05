#ifndef FX_0CD_H
#define FX_0CD_H

/* Effect module fx_0cd (dir 13 file 0xCD): enemy Aera (Gimme Cat, ...). Three growing, spinning models (Fx0cdModel)
 * and particle emitters at the caster. Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0cd effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERA_GREEN_WIND_RING 0x1 /* 'so' model: ring of green wind streaks (grass-fringe cylinder) */
#define FX_AERA_GREY_ARC     0x3 /* 'so' model: open grey arc band */
#define FX_AERA_GREEN_RING   0x4 /* emitter: green gouraud rings */
#define FX_AERA_WIND_RING    0x5 /* emitter: orbiting white wind-ring particle */
#define FX_AERA_GREEN_BURST  0x7 /* emitter: green gust puffs */
#define FX_AERA_GREEN_WIND_BLADES 0x8 /* emitter: spiralling sprites + 3D trail, the translucent green wind blades sweeping round the target; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_AERA_GUST_RINGS   0xA /* emitter: cyan-green gouraud rings */
#define FX_AERA_SPRITE_BURST 0xC /* emitter: sprites; the render still reads garbled yellow texels (texture not resolved); runtime (docs/runtime-findings.md effect-data pass 2): its run-time sprite is a tiny green glint; ablating it changes < 210 px (look pending) */
#define FX_AERA_GREEN_TORNADO 0xD /* 'so' model: twisted green column with a smoke texture (tornado) */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
} Fx0cdModel; /* 0x2C */

typedef struct {
    s32 timer0; /* model C countdown */
    s32 timer4; /* model A countdown */
    s32 timer8; /* model B countdown */
    s32 unkC[5]; /* unused */
    Fx0cdModel model20; /* A */
    Fx0cdModel model4C; /* B */
    Fx0cdModel model78; /* C */
    SVECTOR posA4; /* caster position */
    SVECTOR posAC; /* caster body position */
} Fx0cdWork;

extern Unk800F78A4 *fx_0cd_env;
extern BtlModelBuf fx_0cd_effModels[];
extern s32 fx_0cd_trails[];
extern s32 fx_0cd_trails2[];

#endif
