#ifndef FX_17D_5_H
#define FX_17D_5_H

/* Effect module fx_17d_5 (dir 13 file 0x17d, group 5). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4;
    s32 unk8;
    SVECTOR unkC;
    SVECTOR unk14; /* spark target */
    u8 pad1C[0x84 - 0x1C];
    s32 unk84; /* models */
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
} Fx17d5Work80; /* 0x98 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* speed */
    s16 unk4; /* turn rate */
    s16 unk6; /* index */
    SVECTOR unk8; /* position */
    s16 unk10; /* angles; unk10 also the kind at spawn */
    s16 unk12;
    s16 unk14; /* homing done */
    s16 unk16;
    u8 *unk18; /* trail */
    u8 pad1C[0x22 - 0x1C];
    s16 unk22;
} Fx17d5Spark; /* 0x24 */

s32 func_801EC704(s32 mode, Fx17d5Spark *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent);

#endif
