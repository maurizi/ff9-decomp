#ifndef FX_1BE_H
#define FX_1BE_H

/* Effect module fx_1be (dir 13 file 0x1be): Atomos, short summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_1be effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ATOMOS_PURPLE_RINGS 0x1 /* emitter: white / magenta / purple and pale yellow / grey gouraud rings (byte-identical to fx_0b8 entry 0x21) */
#define FX_ATOMOS_ORANGE_STREAKS 0x2 /* emitter: two fans, thin orange light streaks (byte-identical to fx_0b8 entry 0x7, same render) */
#define FX_ATOMOS_VIOLET_RING  0x4 /* emitter: white-violet and white-blue gouraud rings */
#define FX_ATOMOS_ROCK_DEBRIS  0x5 /* emitter: orbiting grey-blue rock debris particles, four elements */
#define FX_ATOMOS_BLUE_SPECKS  0x6 /* emitter: tiny blue speck sprites */
#define FX_ATOMOS_FIRE_RINGS   0x7 /* emitter: violet-to-navy and white-crimson gouraud rings + rings textured with a pink / gold / blue fire circle */
#define FX_ATOMOS_NAVY_RINGS   0x8 /* emitter: lilac / navy gouraud rings + small violet sprite */
#define FX_ATOMOS_VORTEX_TORUS 0x9 /* 'so' model: torus, dark violet cloud texture */
#define FX_ATOMOS_ORANGE_RING  0xA /* 'so' model: flat ring, orange cloud texture */
#define FX_ATOMOS_RED_CLOUD_SPHERE 0xC /* 'so' model: segmented sphere; page 0x117 holds a dark red cloudy texture at run time: the dark red cloud sphere inside the violet gate; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_ATOMOS_VORTEX_DOME  0xD /* 'so' model: dome, violet and cyan cloud texture */
#define FX_ATOMOS_RING_FUNNEL  0xE /* 'so' model: gouraud ring funnel, white glow at the hole */
#define FX_ATOMOS_BACKDROP     0xF /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ATOMOS_GREY_RINGS   0x10 /* emitter: white-to-grey gouraud rings, 8 frames */

/* fx_1be choreography (deep pass). op80 is one frame switch that starts emitters and loads countdowns into
 * work->counters0[FX_ATOMOS_T_*]; each countdown (or flag) drives one piece below the switch. */
#define FX_ATOMOS_MODEL_COUNT   5    /* btlfx_initEffModels slots (5 registered) */
#define FX_ATOMOS_COUNTERS      0x20 /* work->counters0 slots */
#define FX_ATOMOS_EMITTERS      16   /* work->emit20 slots (cleared when their emitter is done) */
#define FX_ATOMOS_DEBRIS_ENDS   64   /* fx_1be_op80ParticleEnds: random end points of the ROCK_DEBRIS particles (slot & 0x3F) */
#define FX_ATOMOS_EASE_LINEAR      0 /* fxm_easeLerp ratio 1 - t / n (as fx_033's) */
#define FX_ATOMOS_EASE_SQUARED     1 /* (1 - t / n)^2 */
#define FX_ATOMOS_EASE_INV_SQUARED 2 /* 1 - (t / n)^2 */
#define FX_ATOMOS_T_GATE         0    /* flag: BACKDROP layers and RED_CLOUD_SPHERE drawn */
#define FX_ATOMOS_T_GATE_DROP    1    /* countdown: the sphere (and pos8C) sinks from GATE_START_Y to GATE_END_Y */
#define FX_ATOMOS_T_SHRINK       3    /* countdown: the vortex models shrink from VORTEX_SCALE to 0 */
#define FX_ATOMOS_T_ZOOM         4    /* countdown: the opening screen-grid zoom fades */
#define FX_ATOMOS_T_VORTEX       5    /* flag: VORTEX_TORUS drawn facing the camera, ORANGE_RING's texture scrolled */
#define FX_ATOMOS_T_FUNNEL       6    /* flag: RING_FUNNEL and ORANGE_RING drawn with the torus */
#define FX_ATOMOS_T_VORTEX_IN    7    /* countdown: VORTEX_TORUS fades in */
#define FX_ATOMOS_T_GRIDS        9    /* countdown: the two gradient grids drawn (and their texture scrolled) */
#define FX_ATOMOS_T_GRID_IN      0xA  /* countdown: the grids fade in */
#define FX_ATOMOS_T_GRID_OUT     0xB  /* countdown: the grids fade out; counts only while they are drawn, so it overrides GRID_IN */
#define FX_ATOMOS_T_PULL         0xC  /* countdown: the targets are pulled into the hole (each after its random delay) */
#define FX_ATOMOS_T_DOME         0xD  /* countdown: VORTEX_DOME drawn, fading out over its last DOME_FADE_FRAMES */
#define FX_ATOMOS_T_STREAKS_OUT  0xE  /* countdown: ORANGE_STREAKS and VIOLET_RING fade out, then stop */
#define FX_ATOMOS_T_STREAKS_MOVE 0xF  /* countdown: ORANGE_STREAKS and VIOLET_RING follow pos8C */
#define FX_ATOMOS_T_LAND         0x10 /* countdown: the targets drop back home (GREY_RINGS when it ends) */
#define FX_ATOMOS_T_SPEED_UP     0x12 /* countdown: the RING scroll speeds up */
#define FX_ATOMOS_T_DOME_SCROLL  0x14 /* flag: VORTEX_DOME's texture scrolled */
#define FX_ATOMOS_GATE_DROP_FRAMES 0x28
#define FX_ATOMOS_DOME_FRAMES      0x20
#define FX_ATOMOS_DOME_FADE_FRAMES 0x10
#define FX_ATOMOS_ZOOM_FRAMES      0x10
#define FX_ATOMOS_STREAKS_FRAME    0x18 /* ORANGE_STREAKS, VIOLET_RING and ROCK_DEBRIS start; the vortex appears, the pull starts */
#define FX_ATOMOS_STREAKS_MOVE_FRAMES 0x20
#define FX_ATOMOS_VORTEX_IN_FRAMES 0x10
#define FX_ATOMOS_PULL_FRAMES      0x30 /* T_PULL */
#define FX_ATOMOS_UNIT_PULL_FRAMES 0x28 /* a target's own pull (Fx1beUnit timer30): to pos8C, scaled to 0, spinning */
#define FX_ATOMOS_GRID_OUT_FRAME   0x2C /* T_GRID_OUT loaded (before the grids exist) */
#define FX_ATOMOS_GRID_FADE_FRAMES 0x10 /* T_GRID_IN / T_GRID_OUT */
#define FX_ATOMOS_STREAKS_OUT_FRAME 0x30
#define FX_ATOMOS_STREAKS_OUT_FRAMES 0x10
#define FX_ATOMOS_FUNNEL_FRAME     0x40 /* ROCK_DEBRIS stops, NAVY_RINGS start; the vortex moves to the hole at VORTEX_SCALE, the funnel and grids appear */
#define FX_ATOMOS_GRID_FRAMES      0x3C
#define FX_ATOMOS_RINGS_FRAME      0x50 /* FIRE_RINGS, PURPLE_RINGS and BLUE_SPECKS start */
#define FX_ATOMOS_SHRINK_FRAME     0x54
#define FX_ATOMOS_SHRINK_FRAMES    8
#define FX_ATOMOS_CLOSE_FRAME      (FX_ATOMOS_SHRINK_FRAME + FX_ATOMOS_SHRINK_FRAMES) /* the gate, vortex and dome scroll stop */
#define FX_ATOMOS_LAND_FRAME       0x7C /* the targets' scale and rotation restored, T_LAND loaded */
#define FX_ATOMOS_LAND_FRAMES      8
#define FX_ATOMOS_FRAMES           0xAC /* op80 ends */
#define FX_ATOMOS_HOLE_Z           0x418    /* z of the gate / hole (camera point 1, sphere, particles, grids) */
#define FX_ATOMOS_HOLE_Y           (-0x800) /* the hole's height (camera point 1, particle start, the vortex from FUNNEL_FRAME) */
#define FX_ATOMOS_GATE_START_Y     (-0x2000) /* the sphere's height at the start, */
#define FX_ATOMOS_GATE_END_Y       (-0x1000) /* after T_GATE_DROP */
#define FX_ATOMOS_VORTEX_SCALE     0x555 /* the vortex models' scale from FUNNEL_FRAME */
#define FX_ATOMOS_GRID_TPAGE       0x19   /* fx_1be_drawGradientGrid texture: 4-bit page (0x240, 0x100), u 0..0x5F, v 0x40..0x9F */
#define FX_ATOMOS_GRID_CLUT        0x3D03 /* CLUT (0x30, 0xF4) */
#define FX_ATOMOS_GRID_ROWS        8      /* gradient rows (fx_1be_gridColors has one more) */
#define FX_ATOMOS_GRID_COLS        4
#define FX_ATOMOS_GRID_CELL_X      0x1400
#define FX_ATOMOS_GRID_CELL_Z      0xA00
#define FX_ATOMOS_GRID_HALF        0x2800 /* the grid is centred: COLS * CELL_X / 2 = ROWS * CELL_Z / 2 */
#define FX_ATOMOS_RING_VRAM_X  0x2C0 /* btlfx_drawScrollMove rect: ORANGE_RING's 8-bit texels (page 0x9B; fx_0b8's rect) */
#define FX_ATOMOS_RING_VRAM_Y  0x100
#define FX_ATOMOS_RING_VRAM_W  0x38
#define FX_ATOMOS_RING_VRAM_H  0x70
#define FX_ATOMOS_RING_SRC_X   0x2C0
#define FX_ATOMOS_RING_SRC_Y   0x170
#define FX_ATOMOS_DOME_VRAM_X  0x280 /* VORTEX_DOME's and VORTEX_TORUS's 4-bit texels (page 0x1A; fx_0b8's rect) */
#define FX_ATOMOS_DOME_VRAM_Y  0x100
#define FX_ATOMOS_DOME_VRAM_W  0x20
#define FX_ATOMOS_DOME_VRAM_H  0x80
#define FX_ATOMOS_DOME_SRC_X   0x2A0
#define FX_ATOMOS_DOME_SRC_Y   0x100
#define FX_ATOMOS_GRID_VRAM_X  0x240 /* the gradient grids' texels (GRID_TPAGE, u 0..0x5F, v 0x40..0x9F) */
#define FX_ATOMOS_GRID_VRAM_Y  0x140
#define FX_ATOMOS_GRID_VRAM_W  0x18
#define FX_ATOMOS_GRID_VRAM_H  0x60
#define FX_ATOMOS_GRID_SRC_X   0x240
#define FX_ATOMOS_GRID_SRC_Y   0x1A0

typedef struct {
    u8 u;
    u8 v;
} Fx1beUV;

extern SVECTOR fx_1be_gridColors[FX_ATOMOS_GRID_ROWS + 1];
extern SVECTOR fx_1be_op80ParticleEnds[FX_ATOMOS_DEBRIS_ENDS];

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1be_env;
/* end copied */

typedef struct {
    u8 counters0[FX_ATOMOS_COUNTERS]; /* countdowns and flags, by FX_ATOMOS_T_* index (cleared at init) */
    Pool6CEntry *emit20[FX_ATOMOS_EMITTERS];
    s32 model60; /* model */
    VECTOR pos64; /* position */
    VECTOR scale74; /* scale */
    SVECTOR rot84; /* rotation */
    SVECTOR pos8C;
    s32 model94; /* models */
    s32 model98;
    SVECTOR rot9C;
    s32 modelA4;
    s32 modelA8;
    VECTOR scaleAC;
    SVECTOR posBC;
    SVECTOR posC4;
    SVECTOR unkCC; /* write-only (0, -0x2000, 0x418) */
    SVECTOR posD4;
} Fx1beWork80; /* 0xDC */

typedef struct {
    SVECTOR rot0; /* rotation */
    VECTOR scale8; /* scale */
    SVECTOR pos18; /* home position */
    SVECTOR rot20; /* rotation */
    SVECTOR spin28; /* spin */
    s32 timer30;
    s32 timer34; /* delay */
} Fx1beUnit; /* 0x38 */

extern BtlModelBuf fx_1be_effModels[];
extern s32 fx_1be_scrollInitWord;
extern s32 fx_1be_scrollPos;
extern s32 fx_1be_scrollSpeed;
extern Fx1beUnit fx_1be_units[];
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
void fx_1be_drawGradientGrid(SVECTOR *pos, SVECTOR *rot, s32 bright, s32 tpage, s32 clut, s32 abr);
static s32 fx_1be_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
