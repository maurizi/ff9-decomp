#ifndef FX_038_H
#define FX_038_H

/* Effect module fx_038 (dir 13 file 0x38). */
#include "btlfx_mod.h"

/* falling rock, a pool item */
typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* position */
    SVECTOR unkC; /* rotation */
    Pool6CEntry *unk14;
    SVECTOR unk18; /* landing position */
    s32 unk20; /* index */
} Fx038Item; /* 0x24 */

/* trail strip for btlfx_drawTrail */
typedef struct {
    u8 pad0[0x8C];
} Fx038Strip;

extern Fx038Strip D_801E857C[];

typedef struct {
    s32 unk0;
    SVECTOR unk4; /* target position */
    s32 unkC; /* items spawned */
    s32 unk10; /* models */
    s32 unk14;
    s32 unk18;
} Fx038Work; /* 0x1C */

extern u8 D_801E82AC[0x18]; /* spawn frames */
extern Unk800F78A4 *D_801E830C;
extern BtlModel D_801E831C[];

s32 func_801E7704(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work);

#endif
