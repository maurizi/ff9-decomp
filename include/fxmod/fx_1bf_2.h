#ifndef FX_1BF_2_H
#define FX_1BF_2_H

/* Effect module fx_1bf_2 (dir 13 file 0x1bf): Ark (short); Eternal Darkness (load group 2). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1bf_2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_ORANGE_BEAM 0x1 /* 'so' model: glowing red-orange beam bar (byte-identical to fx_17d_8 entry 0x3) */
#define FX_ARK_FLAT_PANEL 0x2 /* 'so' model: flat band on page 0x11B, whose texture in the file is a placeholder with the kana labels up / left / right / down (filled at run time) (byte-identical to fx_17d_8 entry 0x6); runtime (docs/runtime-findings.md effect-data pass): fx_17d_8's same entry was not reached within 4600 frames of a full Ark (look pending) */
#define FX_ARK_BACKDROP    0x3 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) (byte-identical to fx_17d_8 entry 0x7) */
#define FX_ARK_GREY_STAR_DISC 0x5 /* 'so' model: shallow dish with a grey radial cloud texture (byte-identical to fx_17d_8 entry 0x9) */
#define FX_ARK_GOLD_SPARKS 0x7 /* emitter: tube of small gold spark sprites (byte-identical to fx_17d_8 entry 0xB) */
#define FX_ARK_STARFIELD   0xB /* sprite: faint starfield drawn on screen (byte-identical to fx_17d_8 entry 0x10) */
#define FX_ARK_PLANET 0xD /* 'so' model: hemisphere with an earth-globe texture (byte-identical to fx_17d_8 entry 0x1) */
#define FX_ARK_RED_GLOW_DOME 0xF /* 'so' model: shallow red-orange gouraud glow dome (byte-identical to fx_17d_8 entry 0x2; type 10 data below the group's slot) */
#define FX_ARK_DARK_BLUE_RING 0x10 /* 'so' model: flat ring with a dark blue texture (byte-identical to fx_17d_8 entry 0x4; type 10 data below the group's slot) */
#define FX_ARK_FIRE_RING 0x11 /* rings: white-yellow-red gouraud rings (byte-identical to fx_17d_8 entry 0x5; type 10 data below the group's slot) */

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 scale2; /* scale */
    s16 posX4; /* position */
    s16 posY6;
    s16 posZ8;
    s16 padA;
    SVECTOR rotC; /* rotation */
} Fx1bf2Obj;

typedef struct {
    u8 pad0[0x78];
    s32 model78; /* model */
} Fx1bf2Inst;

/* copied */
extern s32 fx_1bf_2_particleDrift;
/* end copied */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1bf2Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* home position */
    SVECTOR pos10; /* particle position */
    SVECTOR target18; /* particle target */
    SVECTOR camPos20; /* camera position */
    Fx1bf2Vec3 pos28; /* model position */
    SVECTOR unk34; /* write-only (cleared at init) */
    Fx1bf2Vec3 scale3C; /* model scale */
    Fx1bf2Vec3 pos48; /* spawn position */
    SVECTOR pos54;
    SVECTOR rot5C; /* model rotation */
    s32 model64; /* models */
    s32 model68;
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    MATRIX viewMtx7C; /* saved matrices */
    MATRIX viewMtx9C;
} Fx1bf2Work80; /* 0xBC */

extern SVECTOR fx_1bf_2_basePos;
extern Unk800F78A4 *fx_1bf_2_env;
extern BtlModelBuf fx_1bf_2_effModels[];

s32 fx_1bf_2_op80(s32 mode, Fx1bf2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
