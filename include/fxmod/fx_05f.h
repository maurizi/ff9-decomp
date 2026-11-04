#ifndef FX_05F_H
#define FX_05F_H

/* Effect module fx_05f (dir 13 file 0x5F). */
#include "btlfx_mod.h"

/* fx_05f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_WATER_BLUE_RING     0x1 /* emitter: white-blue gouraud ring (same call site and look as fx_016 entry 0xa) */
#define FX_WATER_GLOBE         0x2 /* model: full water sphere, last morph stage, then fades (byte-identical to fx_016 entry 0xe) */
#define FX_WATER_GLOBE_3       0x3 /* model: bulging water bowl, fourth morph stage (same morph slot as fx_016 entry 0x4) */
#define FX_WATER_GLOBE_1       0x4 /* model: flared water bowl, second morph stage (byte-identical to fx_016 entry 0x2) */
#define FX_WATER_GLOBE_2       0x5 /* model: water vase shape, third morph stage (same morph slot as fx_016 entry 0x3) */
#define FX_WATER_GLOBE_0       0x6 /* model: flat water dish, first morph stage of the growing water globe (byte-identical to fx_016 entry 0x5) */
#define FX_WATER_SPLASH_FUNNEL 0x7 /* model: flared funnel, misty water texture (byte-identical to fx_016 entry 0x7) */
#define FX_WATER_SPLASH_COLUMN 0x8 /* model: open cylinder, misty water texture; morphs to the funnel as the globe bursts (byte-identical to fx_016 entry 0x8) */
#define FX_WATER_BASE_DISC     0x9 /* model: flat disc (water texture) drawn under the globe (byte-identical to fx_016 entry 0x9) */
#define FX_WATER_SPLASH_DROPS  0xE /* emitter: water droplet clusters, blue glow, small bubbles (same call site and look as fx_016 entry 0x1) */

typedef struct {
    SVECTOR home0[8]; /* target home positions */
    s32 count40; /* target count */
    s32 jump44; /* jump height */
    s32 height48[8]; /* target heights */
} Fx05fWork81; /* 0x68 */

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
} Fx05fWork80; /* 0x34 */

typedef struct {
    u16 x0;
    u16 y2;
} Fx05fPair;

extern BtlModelBuf fx_05f_effModels[];
extern Fx05fPair fx_05f_scrollDst[];
extern Unk800F78A4 *fx_05f_env;

#endif
