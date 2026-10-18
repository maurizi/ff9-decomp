#ifndef FX_17D_1_H
#define FX_17D_1_H

/* Effect module fx_17d_1 (dir 13 file 0x17D). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801ED420;

void func_801EC704(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
void func_801EC944(u32 *ot, s32 tpage, s32 x, s32 r, s32 g, s32 b);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx17d1Summon;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d1Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8;
    SVECTOR unk10; /* particle position */
    SVECTOR unk18; /* camera target */
    u8 pad20[8];
    Fx17d1Vec3 unk28; /* model position (28.0, low halves used) */
    Fx17d1Vec3 unk34; /* model scale */
    SVECTOR unk40; /* model rotation */
    SVECTOR unk48; /* particle rotation */
    Fx17d1Vec3 unk50; /* summon position */
    Fx17d1Vec3 unk5C; /* summon scale */
    SVECTOR unk68; /* summon rotation */
    Pool6CEntry *unk70;
    Pool6CEntry *unk74;
    s32 unk78; /* model */
} Fx17d1Work80; /* 0x7C */

extern BtlModel D_801ED430[];
extern s32 D_801ED5C0[];

s32 fx_17d_1_op80(s32 mode, Fx17d1Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
