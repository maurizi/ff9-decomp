#ifndef FX_016_H
#define FX_016_H

/* Effect module fx_016 (dir 13 file 0x16). */
#include "btlfx_mod.h"

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
