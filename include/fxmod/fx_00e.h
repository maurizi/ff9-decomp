#ifndef FX_00E_H
#define FX_00E_H

/* Effect module fx_00e (dir 13 file 0x0E: Berserk, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    s32 frame4; /* background frame */
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
    s32 pos18[3]; /* model position (a VECTOR without its pad word) */
    s32 model24; /* model */
} Fx00eWork; /* 0x28 */

extern Unk800F78A4 *fx_00e_env;
extern BtlModel fx_00e_effModels[];

#endif
