#ifndef FX_130_H
#define FX_130_H

/* Effect module fx_130 (dir 13 file 0x130). */
#include "btlfx_mod.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);
extern Unk800F78A4 *D_801E7DF4;

typedef struct {
    s32 unk0[8]; /* countdowns */
    SVECTOR unk20; /* centre */
} Fx130Work; /* 0x28 */

typedef struct {
    SVECTOR unk0; /* unit position */
    SVECTOR unk8; /* unit rotation */
    SVECTOR unk10; /* unit rotation at the start */
    s32 unk18; /* start delay */
    s32 unk1C; /* spin time */
    s32 unk20; /* effect delay */
    s32 unk24; /* model */
    SVECTOR unk28; /* model rotation */
    VECTOR unk30; /* model position */
    VECTOR unk40; /* model scale */
} Fx130Unit; /* 0x50 */

extern BtlModel D_801E7E04[];
extern Fx130Unit D_801E8124[4];
extern SVECTOR D_801E812C; /* D_801E8124[0].unk8 */

s32 fx_130_op80(s32 mode, Fx130Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
