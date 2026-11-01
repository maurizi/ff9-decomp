#ifndef FX_05F_H
#define FX_05F_H

/* Effect module fx_05f (dir 13 file 0x5F). */
#include "btlfx_mod.h"

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
