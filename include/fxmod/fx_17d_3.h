#ifndef FX_17D_3_H
#define FX_17D_3_H

/* Effect module fx_17d_3 (dir 13 file 0x17d): Ark, full summon (load group 3). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[8]; /* summon motions */
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d3Summon;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* counter */
    u8 pad8[8];
    SVECTOR pos10;
    SVECTOR pos18;
    SVECTOR pos20; /* camera */
    s32 pos28[3]; /* model position */
    SVECTOR rot34; /* model rotation */
    s32 scale3C[3]; /* model scale */
    s32 pos48[3]; /* summon position */
    s32 scale54[3]; /* summon scale */
    SVECTOR unk60;
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    Pool6CEntry *emit78;
    Pool6CEntry *emit7C;
} Fx17d3Work80; /* 0x80 */

extern Unk800F78A4 *fx_17d_3_env;
extern BtlModel fx_17d_3_effModels[];
extern u8 fx_17d_3_bones0[];
extern u8 fx_17d_3_bones1[];
extern u8 fx_17d_3_bones2[];
extern u8 fx_17d_3_bones3[];
extern u8 fx_17d_3_op80Trails0[];
extern u8 fx_17d_3_op80Trails1[];
extern u8 fx_17d_3_op80Trails2[];

#define SUMMON(env) ((Fx17d3Summon *)(env)->loader)

#endif
