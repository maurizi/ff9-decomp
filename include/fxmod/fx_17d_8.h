#ifndef FX_17D_8_H
#define FX_17D_8_H

/* Effect module fx_17d_8 (dir 13 file 0x17d): Ark, full summon (load group 8). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_PLANET      0x1 /* 'so' model: globe with an Earth-like planet texture, work model slot 0 */
#define FX_ARK_RED_GLOW_DOME 0x2 /* 'so' model: shallow red-orange gouraud glow dome, work model slot 1 */
#define FX_ARK_ORANGE_BEAM 0x3 /* 'so' model: long orange gouraud beam (same size as fx_17d_7's), work model slot 3 */
#define FX_ARK_DARK_BLUE_RING 0x4 /* 'so' model: flat ring with a dark blue texture, work model slot 5 */
#define FX_ARK_FIRE_RING   0x5 /* rings: white-yellow-red gouraud rings */
#define FX_ARK_FLAT_PANEL  0x6 /* 'so' model: flat panel on page 0x11B, which holds a handwritten placeholder in the file (filled at run time), work model slot 4 (look pending) */
#define FX_ARK_BACKDROP    0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ARK_GREY_STAR_DISC 0x9 /* 'so' model: shallow disc with a grey radial texture, work model slot 2 */
#define FX_ARK_GOLD_SPARKS 0xB /* emitter: tube of small gold spark sprites */
#define FX_ARK_STARFIELD   0x10 /* sprite: faint starfield drawn on screen */

typedef struct {
    s16 unk0; /* write-only (cleared at spawn) */
    s16 scale2; /* scale */
    s16 posX4; /* position */
    s16 posY6;
    s16 posZ8;
    s16 padA;
    SVECTOR rotC; /* rotation */
} Fx17d8Obj;

typedef struct {
    u8 pad0[0x78];
    s32 model78; /* model */
} Fx17d8Inst;

extern s32 fx_17d_8_particleDrift;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d8Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* home position */
    SVECTOR pos10; /* particle position */
    SVECTOR target18; /* particle target */
    SVECTOR camPos20; /* camera position */
    Fx17d8Vec3 pos28; /* model position */
    SVECTOR unk34; /* write-only (cleared at init) */
    Fx17d8Vec3 scale3C; /* model scale */
    Fx17d8Vec3 pos48; /* spawn position */
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
} Fx17d8Work80; /* 0xBC */

extern SVECTOR fx_17d_8_basePos;
extern Unk800F78A4 *fx_17d_8_env;
extern BtlModelBuf fx_17d_8_effModels[];

static s32 fx_17d_8_op80PoolTask(s32 mode, Fx17d8Obj *puff, Unk800F78A4 *env, s32 entry, s32 frame, s32 pool, Fx17d8Inst *inst);
s32 fx_17d_8_op80(s32 mode, Fx17d8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
