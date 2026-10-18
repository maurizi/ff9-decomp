#ifndef FX_0FA_H
#define FX_0FA_H

/* Effect module fx_0fa (dir 13 file 0x0FA). */
#include "btlfx_mod.h"

void func_801E7704(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out);

typedef struct {
    s32 unk0[10]; /* countdowns and flags */
    SVECTOR unk28; /* model home position */
    SVECTOR unk30; /* target position */
    s32 unk38; /* model */
    SVECTOR unk3C; /* model rotation */
    SVECTOR unk44; /* model position */
    VECTOR unk4C; /* model position */
    VECTOR unk5C; /* model scale */
    SVECTOR unk6C; /* angles towards the model */
    Pool6CEntry *unk74;
} Fx0faWork; /* 0x78 */

typedef struct {
    s32 unk0;
    s32 unk4;
} Fx0faFlash;

extern Fx0faFlash D_801E7FAC[];
extern Fx0faFlash D_801E7FCC[];
extern Unk800F78A4 *D_801E8034;
extern BtlModel D_801E8044[];

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
