#ifndef FX_17D_3_H
#define FX_17D_3_H

/* Effect module fx_17d_3 (dir 13 file 0x17d): Ark, full summon (load group 3). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_3 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_SMOKE_TRAIL   0x1 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_CLOUD_TRAIL   0x2 /* emitter: two spiralling screen trails with a grey cloud texture */
#define FX_ARK_SMOKE_TRAIL_2 0x4 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_SMOKE_TRAIL_3 0x6 /* emitter: spiralling screen trail with a blue-grey smoke texture, orange embers */
#define FX_ARK_DUST_CLOUD    0x9 /* emitter: grey dust clouds and grey sparks */
#define FX_ARK_SMOKE_CLOUDS  0xE /* emitter: two fans of grey-purple smoke clouds fading out */
#define FX_ARK_WING_PLATE 0x80 /* summon data: 'so' model, V-shaped plate with a grey metal texture */
#define FX_ARK_GREY_CYLINDER 0x81 /* summon data: 'so' model, cylinder with a grey patterned texture */
#define FX_ARK_WING_PLATE_2 0x82 /* summon data: 'so' model, flat plate with a grey metal texture and a dark centre bar */
#define FX_ARK_TEAL_CYLINDER 0x83 /* summon data: 'so' model, cylinder with a dark teal patterned texture */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
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
    SVECTOR unk60; /* write-only (cleared at init) */
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    Pool6CEntry *emit78;
    Pool6CEntry *emit7C;
} Fx17d3Work80; /* 0x80 */

extern Unk800F78A4 *fx_17d_3_env;
extern BtlModelBuf fx_17d_3_effModels[];
extern u8 fx_17d_3_bones0[];
extern u8 fx_17d_3_bones1[];
extern u8 fx_17d_3_bones2[];
extern u8 fx_17d_3_bones3[];
extern u8 fx_17d_3_op80Trails0[];
extern u8 fx_17d_3_op80Trails1[];
extern u8 fx_17d_3_op80Trails2[];

#define SUMMON(env) ((Fx17d3Summon *)(env)->loader)

#endif
