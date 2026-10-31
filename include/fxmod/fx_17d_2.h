#ifndef FX_17D_2_H
#define FX_17D_2_H

/* Effect module fx_17d_2 (dir 13 file 0x17d): Ark, full summon (load group 2). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0; /* state */
    s16 spin2; /* spin */
    s32 scale4; /* size */
    SVECTOR pos8; /* position */
    SVECTOR rot10; /* rotation */
} Fx17d2Elem; /* 0x18 */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* counter */
    SVECTOR pos8;
    SVECTOR pos10;
    SVECTOR pos18;
    u8 pad20[0x28 - 0x20];
    s32 pos28[3]; /* model position */
    s32 scale34[3]; /* model scale */
    SVECTOR rot40; /* model rotation */
    s32 pos48[3]; /* model 2 position */
    SVECTOR rot54; /* model 2 rotation */
    s32 scale5C[3]; /* model 2 scale */
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    Pool6CEntry *emit7C;
} Fx17d2Work; /* 0x80 */

extern Unk800F78A4 *fx_17d_2_env;
extern BtlModel fx_17d_2_effModels[];

static s32 fx_17d_2_op80PoolTask(s32 mode, Fx17d2Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d2Work *work);

#endif
