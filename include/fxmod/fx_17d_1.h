#ifndef FX_17D_1_H
#define FX_17D_1_H

/* Effect module fx_17d_1 (dir 13 file 0x17D): Ark, full summon (load group 1). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_17d_1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ARK_FIRE_RING       0x5 /* rings: white-yellow-red gouraud rings */
#define FX_ARK_ORANGE_SPARKLES 0x8 /* emitter: orange square sparkles */
#define FX_ARK_SPARKLE_TRAIL 0x9D /* summon data: emitter, spiralling screen trail with orange square sparkles */
#define FX_ARK_STREAK_SHELL 0x9E /* summon data: 'so' model, barrel-shaped shell with a grey streak texture */
#define FX_ARK_ORANGE_SPARKLES_2 0x9F /* summon data: emitter, orange square sparkles */

/* fx_17d_1 choreography (docs/fxdata.md, "Choreography literals"): op80's states (work->state0; the frame restarts at
 * each), lengths and keyframes, the trail buffer, the backdrop grid's texture, the scrolling shell texture, the
 * primitive helpers' words. */
#define FX_ARK_STATE_FLARE   0    /* STREAK_SHELL, the screen fades from black, FIRE_RING, sparkles, a red tint */
#define FX_ARK_FLARE_FRAMES  24
#define FX_ARK_UNFADE_FRAMES 12   /* STATE_FLARE: the black fill fades over this many frames, then the red tint grows */
#define FX_ARK_SPARKLE_FRAME 8    /* STATE_FLARE: SPARKLE_TRAIL and ORANGE_SPARKLES start */
#define FX_ARK_SPARKLE_TRAIL_TRAIL_SLOTS  0xE0 /* SPARKLE_TRAIL's trail buffer: slots of */
#define FX_ARK_SPARKLE_TRAIL_TRAIL_STRIDE 0x1C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_ARK_SIDE_FADE_FRAME 18 /* STATE_FLARE: the side fade bands from here */
#define FX_ARK_STATE_RISE    1    /* the backdrop grid, the shell rises with the summon */
#define FX_ARK_RISE_FRAMES   0x50 /* the op ends (the models are cleared) */
#define FX_ARK_MODEL_COUNT   2    /* btlfx_initEffModels slots */
#define FX_ARK_GRID_TPAGE    0x9B   /* STATE_RISE's backdrop grid (fx_17d_1_drawTexGrid): 8-bit page (0x2C0, 0x100), additive */
#define FX_ARK_GRID_CLUT     0x3EC0 /* CLUT (0, 0xFB) */
/* both states scroll (SRC_X, SRC_Y) through this rect (8 / 12 texels a frame; not checked against the model) */
#define FX_ARK_SHELL_VRAM_X  0x340
#define FX_ARK_SHELL_VRAM_Y  0x100
#define FX_ARK_SHELL_VRAM_W  0x40
#define FX_ARK_SHELL_VRAM_H  0x80
#define FX_ARK_SHELL_SRC_X   0x340
#define FX_ARK_SHELL_SRC_Y   0x180

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
    SVECTOR unk8; /* read once (copied into pos18), never written */
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

s32 fx_17d_1_op80(s32 mode, Fx17d1Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
