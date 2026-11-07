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
#define FX_ARK_BEAM_HAZE   0x6 /* 'so' model: flat panel on page 0x11B (a placeholder in the file), work model slot 4; group 8 was not reached in a full Ark; at run time the short Ark's byte-identical fx_1bf_2 entry 0x2 draws the page as a red-violet nebula, the wide red glow band along Ark's beam hitting the planet; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_ARK_BACKDROP    0x7 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ARK_GREY_STAR_DISC 0x9 /* 'so' model: shallow disc with a grey radial texture, work model slot 2 */
#define FX_ARK_GOLD_SPARKS 0xB /* emitter: tube of small gold spark sprites */
#define FX_ARK_STARFIELD   0x10 /* sprite: faint starfield drawn on screen */

/* fx_17d_8 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each) and lengths, the puff pool, the scrolling textures. */
#define FX_ARK_STATE_STARS   0    /* STARFIELD fades in */
#define FX_ARK_STARS_FRAMES  0x20
#define FX_ARK_STATE_PLANET  1    /* PLANET, FIRE_RING, the starfield slides */
#define FX_ARK_PLANET_FRAMES 0x20
#define FX_ARK_STATE_GLOW    2    /* RED_GLOW_DOME, GOLD_SPARKS (frame 1) */
#define FX_ARK_GLOW_FRAMES   0x18
#define FX_ARK_STATE_BEAM    3    /* ORANGE_BEAM and BEAM_HAZE, a puff every 4 frames */
#define FX_ARK_BEAM_FRAMES   0x18
#define FX_ARK_STATE_END     4    /* the starfield fades (the op ends) */
#define FX_ARK_END_FRAMES    0x14
#define FX_ARK_MODEL_COUNT   7    /* btlfx_initEffModels slots */
#define FX_ARK_PUFF_POOL     0x10 /* op80 pool items (Fx17d8Obj) */
#define FX_ARK_PUFF_FRAMES   0x20 /* a puff's life */
/* scrolling textures (not checked against the models): SCROLL forward, SCROLL_2 back, 4 texels a frame */
#define FX_ARK_SCROLL_VRAM_X   0x2C0
#define FX_ARK_SCROLL_VRAM_Y   0x100
#define FX_ARK_SCROLL_VRAM_W   0x20
#define FX_ARK_SCROLL_VRAM_H   0x80
#define FX_ARK_SCROLL_SRC_X    0x2E0
#define FX_ARK_SCROLL_SRC_Y    0x100
#define FX_ARK_SCROLL_2_VRAM_X 0x300
#define FX_ARK_SCROLL_2_VRAM_Y 0x100
#define FX_ARK_SCROLL_2_VRAM_W 0x20
#define FX_ARK_SCROLL_2_VRAM_H 0x80
#define FX_ARK_SCROLL_2_SRC_X  0x320
#define FX_ARK_SCROLL_2_SRC_Y  0x100

/* fx_17d_8 choreography, deep pass (docs/fxchoreo_deep_c.md): keys inside op80's states, the starfield sprite */
#define FX_ARK_SCROLL_SPEED     4     /* SCROLL / SCROLL_2 texels a frame (on work->frame4) */
#define FX_ARK_STARFIELD_SCALE  0xC00 /* STARFIELD sprite's scale, */
#define FX_ARK_STARFIELD_X      (-0x2C) /* resting screen position (STARS / PLANET slide it in from the right) */
#define FX_ARK_STARFIELD_Y      0x78
#define FX_ARK_PUFF_INTERVAL    3     /* PLANET / GLOW: a puff every 3 frames */
#define FX_ARK_BEAM_PUFF_INTERVAL 4   /* STATE_BEAM: every 4 */
#define FX_ARK_GOLD_SPARKS_FRAME 1    /* STATE_GLOW: GOLD_SPARKS starts, aimed along the beam */
#define FX_ARK_BEAM_WIDTH       0xF5  /* ORANGE_BEAM / BEAM_HAZE's x/y scale (GLOW: narrowing to it; BEAM: shrinking from it) */
#define FX_ARK_BEAM_LENGTH      0x800 /* their z scale (fx_17d_7's beam the same) */
#define FX_ARK_GRID_BRIGHT      0x84  /* GLOW / BEAM / END: the screen grid's brightness, */
#define FX_ARK_GRID_SCALE       0x1020 /* and scale */
#define FX_ARK_VIEW_SAVE_FRAME  0x16  /* STATE_BEAM: the view matrix is saved here; STATE_END draws PLANET with it (frozen) */

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
