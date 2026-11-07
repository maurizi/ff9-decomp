#ifndef FX_081_H
#define FX_081_H

/* Effect module fx_081 (dir 13 file 0x81): LV4 Holy (party and enemy). Rings, two models and a sphere of
 * particles (fx_081_ringCounts). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_081 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LV4_HOLY_BLUE_RING       0x4 /* rings: white-cyan-blue gouraud rings */
#define FX_LV4_HOLY_LENS_FLARE 0x5 /* emitter: white star glints */
#define FX_LV4_HOLY_CYAN_RING       0x6 /* rings: white-cyan-blue gouraud rings, 8 segments */
#define FX_LV4_HOLY_LILAC_RING      0x9 /* rings: white-lilac gouraud rings */
#define FX_LV4_HOLY_RAINBOW_ARCH    0xA /* model: half-ring arch, rainbow cloud texture */
#define FX_LV4_HOLY_LENS_FLARE_LINE 0xB /* sprite line: Holy's lens-flare set; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_LV4_HOLY_BLUE_SPARKLE_DOTS 0xC /* particles: blue sparkle frames */
#define FX_LV4_HOLY_BLUE_STREAKS    0xF /* emitter: blue light streaks + sparkles */
#define FX_LV4_HOLY_SPARKLE_STREAK_TEXTURE 0x10 /* layer table: render-to-texture (btlfx_drawLayerTexture): 4 layers into VRAM (0x240,0x1D0) 128x48, a blue streak full of white sparkles (byte-identical to fx_057 entry 0x3, played there); runtime (docs/runtime-findings.md effect-data pass) */
#define FX_LV4_HOLY_BLUE_SPARKLES 0x11 /* emitter: callback element + blue sparkle sprites (byte-identical to fx_057 entry 0x4) */
#define FX_LV4_HOLY_TRAIL           0x12 /* trail: the strip reads unrelated multicoloured VRAM, filled at run time; runtime (docs/runtime-findings.md effect-data pass): as Holy's trail (fx_057 0x6): noise with the run-time pages (look pending) */
#define FX_LV4_HOLY_VIOLET_RAYS     0x13 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_LV4_HOLY_VIOLET_RAYS_2   0x14 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_LV4_HOLY_BLUE_SPARKLES_2 0x15 /* emitter: blue speck sprites */
#define FX_LV4_HOLY_VIOLET_RING     0x16 /* rings: white-grey-violet-blue gouraud rings */
#define FX_LV4_HOLY_GLOW_BAND 0x17 /* model: gouraud flat annulus */
#define FX_LV4_HOLY_DARK_BLUE_RING  0x19 /* emitter: dark blue gouraud rings */
#define FX_LV4_HOLY_BLUE_RAYS       0x1A /* emitter: two fans drawn as thin vertical blue light rays */
#define FX_LV4_HOLY_BLUE_SHIMMER 0x1C /* emitter: blue dot sprites + shimmering particles (byte-identical to fx_057 entry 0x9, same render) */
#define FX_LV4_HOLY_PRISM_BURST     0x1D /* emitter: prism-coloured particles + violet / cyan rings */
#define FX_LV4_HOLY_BLUE_RING_2     0x1F /* rings: blue gouraud rings */
#define FX_LV4_HOLY_BACKDROP_2      0x20 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_LV4_HOLY_LIGHT_SPHERE 0x21 /* model: sphere, white texture */
#define FX_LV4_HOLY_BACKDROP_3      0x22 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_LV4_HOLY_TEXTURED_RING 0x23 /* rings: textured rings; the UV box reads unrelated multicoloured VRAM, filled at run time; runtime (docs/runtime-findings.md effect-data pass): as Holy's 0x1E: noise with the run-time pages (look pending) */
#define FX_LV4_HOLY_WHITE_VIOLET_RINGS 0x24 /* rings: white-blue-violet gouraud rings, 4 segments */
#define FX_LV4_HOLY_WHITE_GLOW 0x25 /* particles: soft white round glow, as Holy's; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_LV4_HOLY_FLASH_RINGS     0x27 /* emitter: white-yellow and white-grey gouraud rings */

/* fx_081 choreography (docs/fxdata.md, "Choreography literals"): op80's and op81's states and keyframes, the
 * lead particle's camera, the model textures. */
#define FX_LV4_HOLY_STATE_GATHER 0 /* op80: the ring stack round the lead particle shrinks in, after a white flash */
#define FX_LV4_HOLY_STATE_FADE   1 /* the rings fade as the sphere glow rises */
#define FX_LV4_HOLY_STATE_RAYS   2 /* FX_LV4_HOLY_VIOLET_RAYS and a blue flash, then the op ends */
#define FX_LV4_HOLY_GATHER_FRAMES 0x16 /* FX_LV4_HOLY_STATE_GATHER's length (the (frame << 12) / N ramp) */
#define FX_LV4_HOLY_FLASH_FRAMES  4    /* FX_LV4_HOLY_STATE_GATHER: the white screen flash fades over this many frames */
#define FX_LV4_HOLY_FADE_FRAMES   0x14 /* FX_LV4_HOLY_STATE_FADE's length (the (frame << 12) / N ramp) */
#define FX_LV4_HOLY_RAYS_FRAME    0x13 /* FX_LV4_HOLY_STATE_RAYS: the rays and flash start */
#define FX_LV4_HOLY_RAYS_FRAMES   0x16 /* FX_LV4_HOLY_STATE_RAYS's length (the op ends) */
#define FX_LV4_HOLY_LEAD_CAM_FRAMES 0x1D /* camera point 3 follows the lead particle (slot 1) this many frames, then */
#define FX_LV4_HOLY_CAM_PAN_FRAMES  0x20 /* pans to fx_081_camEndPos over this many (the ramp is elapsed << 5) */
#define FX_LV4_HOLY_TRAIL_FADE_FRAMES 4  /* a particle's trail fades out over its last this many frames */
#define FX_LV4_HOLY_MODEL_COUNT  3     /* btlfx_initEffModels slots */
#define FX_LV4_HOLY_STREAKS_TRAIL_SLOTS  0x10  /* emit2C's trail buffer fx_081_trails: slots of */
#define FX_LV4_HOLY_STREAKS_TRAIL_STRIDE 0x184 /* this many bytes */
#define FX_LV4_HOLY_SPHERE_TPAGE 0x137  /* FX_LV4_HOLY_LIGHT_SPHERE: 15-bit page (0x1C0, 0x100), abr 1 */
#define FX_LV4_HOLY_SPHERE_CLUT  0xFFFF /* none (15-bit page) */
#define FX_LV4_HOLY_SPHERE_V     0x80
#define FX_LV4_HOLY_ARCH_TPAGE   0xB9   /* FX_LV4_HOLY_RAINBOW_ARCH: 8-bit page (0x240, 0x100), abr 1 */
#define FX_LV4_HOLY_ARCH_CLUT    0x3E00 /* CLUT (0, 0xF8) */
#define FX_LV4_HOLY_ARCH_U       0x80
#define FX_LV4_HOLY_OP81_STATE_FLARE 0 /* op81: lilac rings and the lens flare grow at the target */
#define FX_LV4_HOLY_OP81_STATE_ARCH  1 /* the rainbow arch, rings and blue tints; a darkening tint at its end */
#define FX_LV4_HOLY_OP81_STATE_ORBIT 2 /* sparkles orbit above the targets, white glow rings */
#define FX_LV4_HOLY_OP81_STATE_END   3 /* wait, then the op ends */
#define FX_LV4_HOLY_FLARE_FRAMES 7    /* FX_LV4_HOLY_OP81_STATE_FLARE's length (the (frame << 12) / N ramp) */
#define FX_LV4_HOLY_ARCH_FRAMES  7    /* FX_LV4_HOLY_OP81_STATE_ARCH's length (the (frame << 12) / N ramp) */
#define FX_LV4_HOLY_ORBIT_FRAMES 0x20 /* FX_LV4_HOLY_OP81_STATE_ORBIT's length */
#define FX_LV4_HOLY_END_FRAMES   15   /* FX_LV4_HOLY_OP81_STATE_END's length */
#define FX_LV4_HOLY_ORBIT_HEIGHT 0x200 /* the orbit centre is this far above the targets' centre */
#define FX_LV4_HOLY_ORBIT_SPARKLES 12  /* sparkles per orbit ring */
#define FX_LV4_HOLY_GLOW_RINGS   10    /* white glow rings (fx_081_ringCounts) */
#define FX_LV4_HOLY_DARKEN_RGB   0xFF, 0xFF, 0 /* subtractive screen tint as the arch ends */

extern Unk800F78A4 *fx_081_env;
extern SVECTOR fx_081_camEndPos;
extern SVECTOR fx_081_leadPos;
extern Pool6CEntry *fx_081_emitter0;
extern Unk80100C78Entry *fx_081_task;
extern s32 fx_081_glow; /* glow level */
extern Pool6CEntry *fx_081_emitter1;
extern SVECTOR fx_081_bodyPos; /* caster body position */
extern BtlModelBuf fx_081_effModels[];
extern u8 fx_081_trails[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR pos10;
    Fx081Vec3 modelPos18;
    SVECTOR pos24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx081Work80; /* 0x38 */

static void fx_081_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame,
                   s32 n);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 otZ8; /* priority base */
    SVECTOR posC; /* target position */
    SVECTOR pos14;
    Fx081Vec3 unk1C; /* write-only (copy of posC) */
    s32 model28; /* model */
} Fx081Work81; /* 0x2C */

extern s16 fx_081_ringCounts[FX_LV4_HOLY_GLOW_RINGS]; /* ring point counts */
s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
