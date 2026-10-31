#ifndef FX_0F4_H
#define FX_0F4_H

/* Effect module fx_0f4 (dir 13 file 0x0F4). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR unk14; /* model rotation */
    VECTOR unk1C; /* model position */
    VECTOR unk2C; /* model scale */
    s32 unk3C; /* model brightness */
    SVECTOR unk40; /* model 2 rotation */
    VECTOR unk48; /* model 2 scale */
    SVECTOR unk58; /* centre */
    SVECTOR unk60;
    s16 unk68; /* fade-in countdown */
    s16 unk6A; /* fade-out countdown */
    u16 unk6C; /* model */
    u16 unk6E; /* model 2 */
} Fx0f4Work; /* 0x70 */

extern Unk800F78A4 *fx_0f4_env;
extern BtlModel fx_0f4_effModels[];

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
