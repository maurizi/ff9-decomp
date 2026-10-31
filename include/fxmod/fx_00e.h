#ifndef FX_00E_H
#define FX_00E_H

/* Effect module fx_00e (dir 13 file 0x0E: Berserk, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0;
    s32 unk4; /* background frame */
    SVECTOR unk8; /* target position */
    SVECTOR unk10; /* target body position */
    s32 unk18[3]; /* model position (a VECTOR without its pad word) */
    s32 model24; /* model */
} Fx00eWork; /* 0x28 */

extern Unk800F78A4 *fx_00e_env;
extern BtlModel fx_00e_effModels[];

#endif
