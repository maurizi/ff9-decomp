#ifndef FX_17D_1_H
#define FX_17D_1_H

/* Effect module fx_17d_1 (dir 13 file 0x17D): Ark, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_17d_1_env;

void fx_17d_1_drawTexGrid(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr);
void fx_17d_1_drawSideFade(u32 *ot, s32 tpage, s32 x, s32 r, s32 g, s32 b);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[0x60 - 0x58];
    s32 model60; /* summon model */
} Fx17d1Summon;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d1Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR unk8;
    SVECTOR pos10; /* particle position */
    SVECTOR pos18; /* camera target */
    u8 pad20[8];
    Fx17d1Vec3 pos28; /* model position (28.0, low halves used) */
    Fx17d1Vec3 scale34; /* model scale */
    SVECTOR rot40; /* model rotation */
    SVECTOR rot48; /* particle rotation */
    Fx17d1Vec3 pos50; /* summon position */
    Fx17d1Vec3 scale5C; /* summon scale */
    SVECTOR rot68; /* summon rotation */
    Pool6CEntry *emit70;
    Pool6CEntry *emit74;
    s32 model78; /* model */
} Fx17d1Work80; /* 0x7C */

extern BtlModelBuf fx_17d_1_effModels[];
extern s32 fx_17d_1_op80Trails[];

s32 fx_17d_1_op80(s32 mode, Fx17d1Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
