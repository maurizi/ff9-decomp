#ifndef FX_016_H
#define FX_016_H

/* Effect module fx_016 (dir 13 file 0x16). */
#include "btlfx_mod.h"

/* fx_016 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATER_SPLASH_DROPS  0x1 /* emitter: water droplet clusters, blue glow, small bubbles */
#define FX_WATER_GLOBE_1       0x2 /* model: flared water bowl, second morph stage */
#define FX_WATER_GLOBE_2       0x3 /* model: water vase shape, third morph stage */
#define FX_WATER_GLOBE_3       0x4 /* model: bulging water bowl, fourth morph stage */
#define FX_WATER_GLOBE_0       0x5 /* model: flat water dish, first morph stage of the growing water globe */
#define FX_WATER_SPLASH_FUNNEL 0x7 /* model: flared funnel, misty water texture */
#define FX_WATER_SPLASH_COLUMN 0x8 /* model: open cylinder, misty water texture; morphs to the funnel as the globe bursts */
#define FX_WATER_BASE_DISC     0x9 /* model: flat disc (water texture) drawn under the globe */
#define FX_WATER_BLUE_RING     0xA /* emitter: white-blue gouraud ring */
#define FX_WATER_GLOBE         0xE /* model: full water sphere, last morph stage, then fades */

typedef struct {
    SVECTOR home0[8]; /* target home positions */
    s32 count40; /* target count */
    s32 jump44; /* jump height */
    s32 height48[8]; /* target heights */
} Fx016Work81; /* 0x68 */

typedef struct {
    s32 hold0; /* nonzero: the op idles and never ends by itself (runtime); always 0 in the game (see Fx046Work.hold0) */
    SVECTOR pos4; /* target body */
    SVECTOR posC; /* target */
    s32 model14; /* models */
    s32 model18;
    s32 model1C;
    s32 model20;
    s32 model24;
    s32 model28;
    s32 model2C;
    s32 model30;
    s32 scale34; /* scale */
} Fx016Work80; /* 0x38 */

typedef struct {
    u16 x0;
    u16 y2;
} Fx016Pair;

extern BtlModelBuf fx_016_effModels[];
extern Fx016Pair fx_016_scrollDst[];
extern Unk800F78A4 *fx_016_env;
extern s32 fx_016_radiusScale; /* jump scale */

#endif
