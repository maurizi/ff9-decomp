#ifndef FX_058_H
#define FX_058_H

/* Effect module fx_058 (dir 13 file 0x58). */
#include "btlfx_mod.h"

/* homing particle, one pool item */
typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* speed */
    s32 unk8; /* turn rate */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* direction */
    u8 *unk1C; /* trail buffer (D_801E7EF0) */
    SVECTOR unk20; /* target; pad = model z scale */
} Fx058Item; /* 0x28 */

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* target body position */
    SVECTOR unkC; /* caster body position */
    SVECTOR unk14; /* caster rotation */
    s32 unk1C; /* particles spawned */
    s32 unk20; /* frame */
    s32 unk24; /* model */
} Fx058Work; /* 0x28 */

extern Unk800F78A4 *D_801E7E64;
extern BtlModel D_801E7E74[];
extern u8 D_801E7EF0[][0x54];

s32 func_801E7704(s32 mode, Fx058Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx058Work *work);

#endif
