#ifndef FX_1A2_H
#define FX_1A2_H

/* Effect module fx_1a2 (dir 13 file 0x1a2): Moonlight Slash (Blank, enemy). Ribbon trails (fxm_drawRibbonTrail).
 * Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_1a2 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MOONLIGHT_SLASH_LIGHT_BAND       0x1 /* model: cylinder band, blue light-streak texture */
#define FX_MOONLIGHT_SLASH_LIGHT_BAND_2     0x2 /* model: cylinder band, blue light-streak texture */
#define FX_MOONLIGHT_SLASH_LIGHT_RING       0x3 /* model: flat ring of blue light streaks */
#define FX_MOONLIGHT_SLASH_MOON             0x5 /* emitter: blue full-moon particles, 30 frames + blue gouraud rings */
#define FX_MOONLIGHT_SLASH_SPIKED_SHAPE 0x6 /* model: 20-poly spiked shape (Hi_RegisterTexListModel, texture not drawn); runtime (docs/runtime-findings.md effect-data pass 2): untextured spiky model, fetched early; nothing visible in a forced cast with Zidane as the caster (a real enemy cast may differ) (look pending) */
#define FX_MOONLIGHT_SLASH_CYAN_RINGS       0x7 /* emitter: thin cyan streak fan + white-cyan gouraud rings */
#define FX_MOONLIGHT_SLASH_BLUE_GLOWS       0x8 /* emitter: three sprite elements, 24 frames, soft blue glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL   0x9 /* emitter: spiralling 3D trail, blue crescent strip */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2 0xA /* emitter: 3D trail, blue crescent strip */
#define FX_MOONLIGHT_SLASH_BLADE            0xB /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_BLADE_2          0xC /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_BLADE_3          0xD /* model: curved blade, blue-white feathered streak texture */
#define FX_MOONLIGHT_SLASH_ROT_KEYS         0xE /* table: per-frame rotation keys */
#define FX_MOONLIGHT_SLASH_CYAN_STREAKS     0xF /* emitter: three fans, thin cyan light streaks */
#define FX_MOONLIGHT_SLASH_ARMOR            0x10 /* model: grey-brown armour plating (129 polys) */
#define FX_MOONLIGHT_SLASH_ARMOR_2          0x11 /* model: grey-brown armour plating (24 polys) */
#define FX_MOONLIGHT_SLASH_WHITE_RINGS      0x12 /* rings: white gouraud rings, 10 frames */
#define FX_MOONLIGHT_SLASH_WHITE_RINGS_2    0x13 /* rings: white gouraud rings, 10 frames */
#define FX_MOONLIGHT_SLASH_CYAN_SPARKLES    0x14 /* emitter: cyan sparkle dots, 12 frames */
#define FX_MOONLIGHT_SLASH_CYAN_SPARKLES_2  0x15 /* emitter: cyan sparkle dots + faint green-grey particles */
#define FX_MOONLIGHT_SLASH_COLOR_GLOWS      0x17 /* emitter: three sprite elements, 24 frames, blue, violet and green glows at run time (played by forcing the effect id on Zidane's Attack, Zidane as the caster: a real enemy cast may place it differently); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */

/* fx_1a2 choreography (docs/fxdata.md, "Choreography literals"; partial: textures, pool, trails). */
#define FX_MOONLIGHT_SLASH_BAND_TPAGE  0xB8   /* Hi_RegisterTexEffModel texture of LIGHT_BAND, LIGHT_BAND_2, LIGHT_RING: 8-bit page (0x200, 0x100), abr 1 */
#define FX_MOONLIGHT_SLASH_BAND_CLUT   0x3DC0 /* CLUT (0x0, 0xF7) */
#define FX_MOONLIGHT_SLASH_BAND_U      0x80
#define FX_MOONLIGHT_SLASH_BAND_V      0
#define FX_MOONLIGHT_SLASH_BLADE_TPAGE 0xBA   /* BLADE, BLADE_2, BLADE_3: 8-bit page (0x280, 0x100), abr 1 */
#define FX_MOONLIGHT_SLASH_BLADE_CLUT  0x3E40 /* CLUT (0x0, 0xF9) */
#define FX_MOONLIGHT_SLASH_BLADE_U     0
#define FX_MOONLIGHT_SLASH_BLADE_V     0xE0
#define FX_MOONLIGHT_SLASH_ARMOR_TPAGE 0x98   /* ARMOR, ARMOR_2: 8-bit page (0x200, 0x100), abr 0 */
#define FX_MOONLIGHT_SLASH_ARMOR_CLUT  0x3E00 /* CLUT (0x0, 0xF8) */
#define FX_MOONLIGHT_SLASH_ARMOR_U     0
#define FX_MOONLIGHT_SLASH_ARMOR_V     0x80
#define FX_MOONLIGHT_SLASH_ROCKS       0x20   /* op80: pool items (init->poolCount), bouncing rocks */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_TRAIL_SLOTS  0x10 /* FX_MOONLIGHT_SLASH_CRESCENT_TRAIL's trail buffer fx_1a2_op80Trails0: slots of */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2_TRAIL_SLOTS  8 /* FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2's trail buffer fx_1a2_op80Trails1: slots of */
#define FX_MOONLIGHT_SLASH_CRESCENT_TRAIL_2_TRAIL_STRIDE 0x4C /* this many bytes (Pool6CEntry trailSlots / trailStride) */

/* fx_1a2 choreography (deep pass) */
#define FX_MOONLIGHT_SLASH_MODEL_COUNT    10   /* btlfx_initEffModels slots */
#define FX_MOONLIGHT_SLASH_RIBBON_CLEAR   999  /* fxm_drawRibbonTrail: n > this clears n - this points */
#define FX_MOONLIGHT_SLASH_RIBBON_POINTS  0x20 /* points per ribbon (fx_1a2_ribbonEdges0..2), cleared at frame 0, n - 1 drawn */
#define FX_MOONLIGHT_SLASH_SHATTER_PIECES 130  /* fxm_shatterModel: triangles of the split model (fx_1a2_shatterTris / Pieces) */
#define FX_MOONLIGHT_SLASH_SHATTER_HOLD_FRAMES 3 /* a piece stays in place this long, then flies (btlfx_stepModuleParticle) */
#define FX_MOONLIGHT_SLASH_SHATTER_FRAMES 0x40 /* then it is removed */
#define FX_MOONLIGHT_SLASH_ROCK_STATE_HOP  0   /* op80 pool task (Fx1a2Rock.state8): a hop forward, */
#define FX_MOONLIGHT_SLASH_ROCK_STATE_REST 1   /* a short rest on the ground */
#define FX_MOONLIGHT_SLASH_ROCK_HOPS       10  /* the task ends after this many hops */
#define FX_MOONLIGHT_SLASH_ROCK_HOP_FRAMES 8   /* a hop (its sine arc is frame << 9) */
#define FX_MOONLIGHT_SLASH_BIG_ROCK_SCALE  0x1800 /* the one big rock (spawned at SHATTER_FRAME, resting first); smaller rocks turn between hops */
#define FX_MOONLIGHT_SLASH_STATE_CHARGE  0 /* op80 state: ribbons trail from the blade (bone 0x10), WHITE_RINGS and CYAN_SPARKLES at its tip */
#define FX_MOONLIGHT_SLASH_STATE_SLASH   1 /* the BLADE crescents sweep round the target (6 afterimages), MOON and BLUE_GLOWS */
#define FX_MOONLIGHT_SLASH_STATE_PAUSE   2
#define FX_MOONLIGHT_SLASH_STATE_PILLAR  3 /* the light pillar (LIGHT_RING morphing into LIGHT_BAND_2, LIGHT_BAND), crescent trails; the ARMOR shatters */
#define FX_MOONLIGHT_SLASH_STATE_DEBRIS  4 /* the pieces fall, ARMOR_2 bounces and fades (op80 ends) */
#define FX_MOONLIGHT_SLASH_CHARGE_FRAMES 93
#define FX_MOONLIGHT_SLASH_SLASH_FRAMES  0xB
#define FX_MOONLIGHT_SLASH_MOON_FRAME    4    /* SLASH: MOON and BLUE_GLOWS start */
#define FX_MOONLIGHT_SLASH_SLASH_FADE_FRAME 7 /* SLASH: the crescents fade from here to the end */
#define FX_MOONLIGHT_SLASH_BLADE_MODELS  3    /* SLASH: BLADE_3, BLADE_2, BLADE (model30), one per frame, then the last */
#define FX_MOONLIGHT_SLASH_AFTERIMAGES   6    /* SLASH: crescent copies, the rotation keys of the last frames */
#define FX_MOONLIGHT_SLASH_PAUSE_FRAMES  0xC
#define FX_MOONLIGHT_SLASH_PILLAR_FRAMES 29
#define FX_MOONLIGHT_SLASH_BAND_GROW_FRAMES 4 /* PILLAR: LIGHT_BAND grows, then shrinks with the pillar's cosine */
#define FX_MOONLIGHT_SLASH_SHATTER_FRAME 10   /* PILLAR: tint, ARMOR split into pieces, ARMOR_2 thrown, the target hidden; rocks from here */
#define FX_MOONLIGHT_SLASH_SHATTER_TINT_RGB 0x10, 0x3C, 0x46
#define FX_MOONLIGHT_SLASH_ROCK_SPAWN_FRAMES 4 /* PILLAR: rocks spawned for this many frames, */
#define FX_MOONLIGHT_SLASH_ROCKS_PER_FRAME   6 /* this many a frame */
#define FX_MOONLIGHT_SLASH_DEBRIS_FRAMES 0x64
#define FX_MOONLIGHT_SLASH_PLATE_FRAMES  0x30 /* DEBRIS: ARMOR_2 drawn until this frame, */
#define FX_MOONLIGHT_SLASH_PLATE_FADE_FRAME  0x20 /* fading (additive) from here */
#define FX_MOONLIGHT_SLASH_PLATE_FADE_FRAMES 0x10
#define FX_MOONLIGHT_SLASH_CUE_FRAME     0x4B /* DEBRIS: btlfx_signalScriptCue */
#define FX_MOONLIGHT_SLASH_OP81_FADE_FRAME 0x24 /* op81: unit 0x10 drifts back, fading out from here */
#define FX_MOONLIGHT_SLASH_OP81_FRAMES   0x2C
#define FX_MOONLIGHT_SLASH_BLADE_VRAM_X  0x280 /* btlfx_drawScrollMove rect: the BLADEs' 0x80 x 0x20 8-bit texels (BLADE_TPAGE, v 0xE0) */
#define FX_MOONLIGHT_SLASH_BLADE_VRAM_Y  0x1E0
#define FX_MOONLIGHT_SLASH_BLADE_VRAM_W  0x40
#define FX_MOONLIGHT_SLASH_BLADE_VRAM_H  0x20
#define FX_MOONLIGHT_SLASH_BLADE_SRC_X   0x2C0 /* from a strip of 8 frames, one a frame ((frame & 7) * H) */
#define FX_MOONLIGHT_SLASH_BLADE_SRC_Y   0x100
#define FX_MOONLIGHT_SLASH_BAND_VRAM_X   0x240 /* the bands' 0x60 x 0x80 8-bit texels (BAND_TPAGE, u 0x80) */
#define FX_MOONLIGHT_SLASH_BAND_VRAM_Y   0x100
#define FX_MOONLIGHT_SLASH_BAND_VRAM_W   0x30
#define FX_MOONLIGHT_SLASH_BAND_VRAM_H   0x80
#define FX_MOONLIGHT_SLASH_BAND_SRC_X    0x240
#define FX_MOONLIGHT_SLASH_BAND_SRC_Y    0x180

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
} Fx1a2Seg; /* 0x10 */

typedef struct {
    SVECTOR verts0[3];
} Fx1a2Tri; /* 0x18 */

typedef struct {
    s32 pos0; /* position, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12; /* write-only (0x1000) */
    s16 rot14;
    s16 rot16;
    s16 frame18;
    s16 delay1A;
} Fx1a2Piece; /* 0x1C */

extern Fx1a2Tri fx_1a2_shatterTris[FX_MOONLIGHT_SLASH_SHATTER_PIECES];
extern Fx1a2Piece fx_1a2_shatterPieces[FX_MOONLIGHT_SLASH_SHATTER_PIECES];
typedef struct {
    s16 count0; /* bounce count */
    s16 height2; /* sink speed */
    s16 height4; /* bounce height */
    s16 wait6; /* wait frames */
    s32 state8; /* state */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s32 scale1C; /* scale */
} Fx1a2Rock; /* 0x20 */

typedef struct {
    u8 pad0[0x48];
    s32 model48; /* model */
} Fx1a2Inst;

typedef struct {
    u32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* unit position */
    SVECTOR pos10; /* body position */
    SVECTOR pos18; /* particle position */
    s16 pos20; /* bouncing rock position */
    s16 pos22;
    s16 pos24;
    s16 unk26; /* unused */
    s32 vel28; /* bouncing rock velocity */
    s32 scale2C; /* scale */
    s32 model30[3]; /* models */
    s32 model3C;
    s32 model40;
    s32 model44;
    s32 model48;
    s32 model4C;
    s32 model50;
} Fx1a2Work80; /* 0x54 */

extern BtlModelBuf fx_1a2_effModels[];
extern CVECTOR fx_1a2_colors0;
extern CVECTOR fx_1a2_colors1;
extern CVECTOR fx_1a2_colors2;
extern Fx1a2Seg fx_1a2_ribbonEdges0[];
extern Fx1a2Seg fx_1a2_ribbonEdges1[];
extern Fx1a2Seg fx_1a2_ribbonEdges2[];
extern s32 fx_1a2_op80Trails0[];
extern s32 fx_1a2_op80Trails1[];

/* copied */
extern Unk800F78A4 *fx_1a2_env;
/* end copied */

#endif
