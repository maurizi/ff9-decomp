#ifndef FX_1A8_H
#define FX_1A8_H

/* Effect module fx_1a8 (dir 13 file 0x1a8): Odin (short); Zantetsuken. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a8 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ODIN_BLUE_RINGS       0x1 /* emitter: white-cyan-violet and white-cyan-blue gouraud rings */
#define FX_ODIN_FLAME_TRAIL      0x2 /* trail: orange flame strip */
#define FX_ODIN_GOLD_RAYS        0x5 /* emitter: two fan elements, a thin pale-gold streak at run time, matching the gold rays of the slash explosion; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_ODIN_SKY_CRYSTAL      0x6 /* 'so' model: polyhedron crystal, blue sky-and-cloud texture */
#define FX_ODIN_FLAME_RIBBON     0x7 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_FLAME_RIBBON_2   0x8 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_FLAME_RIBBON_3   0x9 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_SKY_CRYSTAL_2    0xA /* 'so' model: flattened crystal, blue sky-and-cloud texture */
#define FX_ODIN_CRIMSON_RINGS    0xB /* rings: white-crimson gouraud rings, 16 frames */
#define FX_ODIN_BLACK_DEBRIS     0xC /* emitter: spiral of two speck-sprite elements, dark grey specks at run time: the black debris flying out of the explosion; runtime (docs/runtime-findings.md effect-data pass 2) */
#define FX_ODIN_CRIMSON_RINGS_2  0xD /* emitter: white-crimson and white-violet gouraud rings */
#define FX_ODIN_FLAME_RIBBON_4   0x10 /* 'so' model: twisted ribbon, fire texture */
#define FX_ODIN_STONE_RING       0x11 /* 'so' model: ring band, grey-green stone texture */
#define FX_ODIN_RED_SPARKS       0x12 /* emitter: speck sprites, tiny red sparks at run time (not separable on screen); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_ODIN_PINK_SMOKE       0x13 /* emitter: white-pink smoke burst quads, 22 frames */
#define FX_ODIN_CLOUD_TORUS      0x14 /* 'so' model: torus, dark marbled cloud texture */
#define FX_ODIN_GOLD_RING        0x15 /* 'so' model: ring band, yellow-brown texture */
#define FX_ODIN_VORTEX_CONE      0x16 /* 'so' model: spiralling cone, brown-grey texture */
#define FX_ODIN_CALLBACK_EMITTER 0x17 /* emitter: two callback elements (drawn by the module); runtime (docs/runtime-findings.md effect-data pass 2): started with the slash explosion; ablating it only shifts the random pink smoke puffs (look pending) */
#define FX_ODIN_ZANTETSUKEN      0x19 /* 'so' model: thin sword with a hilt, red-violet texture (the Zantetsuken blade) */

/* fx_1a8 choreography (docs/fxdata.md, "Choreography literals"; deep pass docs/fxchoreo_deep_e.md): op80 throws
 * Zantetsuken from above and behind the target (DESCEND), drives it into the target (STRIKE), then the blast
 * (BLAST): flame ribbons, stone and gold rings, the cloud torus, the crimson rings and the emitters. The blade (with
 * VORTEX_CONE round it) is wrapped in a wind sheath (fxm_drawTexCylinder) until the blast's SHEATH_FADE_FRAMES.
 * Nothing here is shared with the long form fx_105 (its own camera-keyed timeline) except the model slot count's
 * name. */
#define FX_ODIN_MODEL_SLOTS        0xC    /* btlfx_initEffModels slots (fx_105: 0xF) */
#define FX_ODIN_STATE_DESCEND      0      /* op80 states (work->state0) */
#define FX_ODIN_STATE_STRIKE       1
#define FX_ODIN_STATE_BLAST        2
#define FX_ODIN_DROP_HEIGHT        0x2000 /* the blade starts this far above and behind (-z) the target (camera point 3
                                           * looks there); DESCEND covers the first 0x1000, STRIKE the rest */
/* STATE_DESCEND: BLUE_RINGS on its first frame, the blade eases in (cos), growing */
#define FX_ODIN_DESCEND_FRAMES     10
#define FX_ODIN_FLAME_TRAIL_FRAMES 0x10   /* FLAME_TRAIL at the start point while frame < this, fading on cos(frame /
                                           * this); DESCEND ends first, at DESCEND_FRAMES */
/* the background capture (btlfx_drawBgOffscreen, when DESCEND ends) lands in the 15-bit page 0x119 (0x240, 0x100)
 * at v 0xC0: the rect where the file uploads the cloud picture the SKY_CRYSTAL models sample (callback elements of
 * CALLBACK_EMITTER), so the crystals show the battle background */
#define FX_ODIN_BG_VRAM_X          0x240
#define FX_ODIN_BG_VRAM_Y          0x1C0
#define FX_ODIN_BG_VRAM_W          0x40
#define FX_ODIN_BG_VRAM_H          0x40
/* STATE_STRIKE: the blade, aimed at the target, falls the last 0x1000 */
#define FX_ODIN_STRIKE_FRAMES      5      /* the ramp ((frame + 1) / this) reaches the target on frame this - 1, which
                                           * ends the state */
#define FX_ODIN_BLADE_SCALE        0x5000 /* the blade's scale from STRIKE on (VORTEX_CONE at half of it) */
#define FX_ODIN_CAMERA_HEIGHT      0x140  /* camera point 3 looks this far above the target from STRIKE on */
#define FX_ODIN_STRIKE_DARKEN_RGB  0x46, 0x46, 0x46 /* STRIKE's end: subtractive, then GOLD_RAYS, CRIMSON_RINGS_2,
                                           * BLACK_DEBRIS, RED_SPARKS, CALLBACK_EMITTER at the target */
/* STATE_BLAST (t = frame / BLAST_FRAMES in 1.12): the blade fades (additive) over the last quarter */
#define FX_ODIN_BLAST_FRAMES       0x20   /* the op ends */
#define FX_ODIN_SHAKE_FRAMES       8      /* camera point 3 shakes (x and y, alternating sides), */
#define FX_ODIN_SHAKE_AMPLITUDE    0x20   /* by this, 4 less each frame */
#define FX_ODIN_BOUNCE_FRAMES      6      /* the blade's y bounces (alternating, cos over this many frames) */
#define FX_ODIN_BLAST_FLASH_FRAMES 6      /* additive red flash (at, at / 2, at / 2) fading from 0xFF */
#define FX_ODIN_SHEATH_FADE_FRAMES 0x10   /* the wind sheath widens and fades out */
#define FX_ODIN_FLAME_RIBBONS      4      /* model4C: FLAME_RIBBON_4, FLAME_RIBBON, _2, _3, drawn at the target */
#define FX_ODIN_FLAME_FRAMES       0x10   /* the flame ribbons draw (and their texture scrolls) while frame < this, */
#define FX_ODIN_FLAME_FADE_FRAMES  12     /* stretching up and fading on cos(frame / this) */
#define FX_ODIN_STONE_RING_FRAMES  0xC    /* STONE_RING (twice, the second wider and flatter) rises and fades */
#define FX_ODIN_GOLD_RING_FRAMES   0x14   /* GOLD_RING copies spin up the column and fade */
#define FX_ODIN_GOLD_RING_COPIES   4      /* each wider, flatter, higher and spinning faster than the one below */
#define FX_ODIN_CRIMSON_RINGS_FRAMES 0x10 /* CRIMSON_RINGS at the target (the data's own 16 frames) */
#define FX_ODIN_PINK_SMOKE_FRAME   0xC    /* PINK_SMOKE starts */
/* FX_ODIN_FLAME_RIBBON* (8-bit page 0x97 (0x1C0, 0x100), u / v 0) sample the rect the flames' btlfx_drawScrollMove
 * fills from (SRC_X, SRC_Y), scrolled 16 lines a frame */
#define FX_ODIN_FLAME_VRAM_X       0x1C0
#define FX_ODIN_FLAME_VRAM_Y       0x100
#define FX_ODIN_FLAME_VRAM_W       0x38
#define FX_ODIN_FLAME_VRAM_H       0x70
#define FX_ODIN_FLAME_SRC_X        0x1C0
#define FX_ODIN_FLAME_SRC_Y        0x170
/* FX_ODIN_CLOUD_TORUS (4-bit page 0x19 (0x240, 0x100)) samples the rect at v 0x40 the whole blast scrolls (8 lines a
 * frame, backwards) from (SRC_X, SRC_Y) */
#define FX_ODIN_CLOUD_VRAM_X       0x240
#define FX_ODIN_CLOUD_VRAM_Y       0x140
#define FX_ODIN_CLOUD_VRAM_W       0x20
#define FX_ODIN_CLOUD_VRAM_H       0x80
#define FX_ODIN_CLOUD_SRC_X        0x260
#define FX_ODIN_CLOUD_SRC_Y        0x140
/* the wind sheath: four fxm_drawTexCylinder tubes round the blade (two wide, two narrow; two spinning each way) of
 * SHEATH_SEGMENTS quads (the end ones gouraud-faded), textured from the box (U, V) + TEX_W x TEX_H that its
 * btlfx_drawScrollMove fills from (SRC_X, SRC_Y), scrolled 16 lines a frame backwards */
#define FX_ODIN_SHEATH_SEGMENTS    16     /* quads along a tube: 2 gouraud ends + 14 flat */
#define FX_ODIN_SHEATH_RADIUS      0x100  /* radius at the ends, */
#define FX_ODIN_SHEATH_RADIUS_MID  0xA0   /* narrowing to this at the middle */
#define FX_ODIN_SHEATH_WIDTH       0x200  /* band width of the wide tubes */
#define FX_ODIN_SHEATH_NARROW_WIDTH 0x100 /* and of the narrow ones */
#define FX_ODIN_SHEATH_LENGTH      0xC00  /* along the blade */
#define FX_ODIN_SHEATH_TPAGE       0x19   /* 4-bit page (0x240, 0x100) */
#define FX_ODIN_SHEATH_CLUT        0x3D02 /* CLUT (0x20, 0xF4) */
#define FX_ODIN_SHEATH_U           0
#define FX_ODIN_SHEATH_V           0
#define FX_ODIN_SHEATH_TEX_W       0x80   /* texels: the whole VRAM_W x VRAM_H rect */
#define FX_ODIN_SHEATH_TEX_H       0x40
#define FX_ODIN_SHEATH_VRAM_X      0x240
#define FX_ODIN_SHEATH_VRAM_Y      0x100
#define FX_ODIN_SHEATH_VRAM_W      0x20
#define FX_ODIN_SHEATH_VRAM_H      0x40
#define FX_ODIN_SHEATH_SRC_X       0x260
#define FX_ODIN_SHEATH_SRC_Y       0x100

extern Unk800F78A4 *fx_1a8_env;
extern s32 fx_1a8_elemModels[2]; /* models */

static void fx_1a8_op80ElemDraw0(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 frame, s32 n);
static void fx_1a8_op80ElemDraw1(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 frame, s32 n);
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1a8Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    SVECTOR pos8;
    SVECTOR pos10;
    u8 pad18[0x20 - 0x18];
    SVECTOR pos20; /* ring position */
    s32 model28; /* model */
    SVECTOR rot2C; /* rotation */
    Fx1a8Vec3 scale34; /* scale */
    Fx1a8Vec3 pos40; /* position */
    s32 model4C[FX_ODIN_FLAME_RIBBONS]; /* models */
    s32 model5C;
    s32 model60;
    s32 model64;
    s32 model68;
    u8 pad6C[0x74 - 0x6C];
} Fx1a8Work80; /* 0x74 */

extern SVECTOR fx_1a8_targetPos;
extern u32 fx_1a8_bgOt[];
extern BtlModelBuf fx_1a8_effModels[];
extern u8 fx_1a8_trailPoints[];

#endif
