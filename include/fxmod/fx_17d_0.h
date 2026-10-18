#ifndef FX_17D_0_H
#define FX_17D_0_H

/* Effect module fx_17d_0 (dir 13 file 0x17D). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4; /* line index */
    SVECTOR unk8;
    s16 unk10;
} Fx17d0Part;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4; /* key file */
    s16 unk6; /* morph key file */
} Fx17d0Piece;

extern Fx17d0Piece D_801E9370[21];
extern Unk800F78A4 *D_801E9470;
extern u8 D_801EBC90[][0x24];

void func_801E7704(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b);
void func_801E7894(s32 n, s32 r, SVECTOR *pts);
void func_801E7D18(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s16 *models, s16 *morph, s32 scale);
s32 func_801E7FE0(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
