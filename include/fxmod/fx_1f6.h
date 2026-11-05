#ifndef FX_1F6_H
#define FX_1F6_H

/* Effect module fx_1f6 (dir 13 file 0x1f6): enemy Aera (Griffin, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1f6 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERA_GREEN_WIND_RING 0x1 /* 'so' model: ring of green wind streaks (grass-fringe cylinder) (byte-identical to fx_0cd entry 0x1) */
#define FX_AERA_GREY_ARC 0x3 /* 'so' model: open grey arc band (byte-identical to fx_0cd entry 0x3) */
#define FX_AERA_GREEN_RING   0x4 /* emitter: green gouraud rings (byte-identical to fx_0cd entry 0x4) */
#define FX_AERA_WIND_RING    0x5 /* emitter: orbiting white wind-ring particle (byte-identical to fx_0cd entry 0x5) */
#define FX_AERA_GREEN_TORNADO 0x6 /* 'so' model: twisted green column with a smoke texture (tornado) (byte-identical to fx_0cd entry 0xD) */
#define FX_AERA_GREEN_BURST  0x7 /* emitter: green gust puffs (byte-identical to fx_0cd entry 0x7) */
#define FX_AERA_GREEN_WIND_BLADES 0x8 /* emitter: spiralling sprites + 3D trail, the translucent green wind blades sweeping round the target (byte-identical to fx_0cd entry 0x8); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_AERA_GUST_RINGS   0xA /* emitter: cyan-green gouraud rings (byte-identical to fx_0cd entry 0xA) */
#define FX_AERA_SPRITE_BURST 0xC /* emitter: sprites (texture garbled in the render) (byte-identical to fx_0cd entry 0xC); runtime (docs/runtime-findings.md effect-data pass 2): as fx_0cd's entry 0xC: a tiny green glint sprite at run time, ablating it changes < 210 px (look pending) */

typedef struct {
    s32 model0; /* model */
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
} Fx1f6Model; /* 0x2C */

typedef struct {
    s32 timer0[8]; /* countdowns */
    Fx1f6Model models20[3];
    SVECTOR posA4; /* target position */
    SVECTOR posAC; /* target body position */
} Fx1f6Work; /* 0xB4 */

extern Unk800F78A4 *fx_1f6_env;
extern BtlModelBuf fx_1f6_effModels[];
extern s32 fx_1f6_op80Trails0[];
extern s32 fx_1f6_op80Trails1[];

#endif
