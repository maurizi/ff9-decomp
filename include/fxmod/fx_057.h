#ifndef FX_057_H
#define FX_057_H

/* Effect module fx_057 (dir 13 file 0x57). */
#include "btlfx_mod.h"

/* fx_057 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_HOLY_VIOLET_RINGS       0x1 /* rings: white-grey-violet-blue gouraud rings */
#define FX_HOLY_LENS_FLARE         0x2 /* emitter: white lens-flare glints */
#define FX_HOLY_SPARKLE_STREAK_TEXTURE 0x3 /* layer table: render-to-texture (btlfx_drawLayerTexture): 4 layers into VRAM (0x240,0x1D0) 128x48, a blue streak full of white sparkles, sampled by the sweeping light bands; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_HOLY_BLUE_SPARKLES      0x4 /* emitter: callback element + blue sparkle sprites */
#define FX_HOLY_VIOLET_RAYS        0x5 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_HOLY_TRAIL              0x6 /* trail drawn by the emitter callback; the strip reads unrelated multicoloured VRAM, filled at run time; runtime (docs/runtime-findings.md effect-data pass): its strip still reads noise with the run-time pages: the trail's tpage is not the one the tool resolves (look pending) */
#define FX_HOLY_BLUE_SHIMMER       0x9 /* emitter: blue dot sprites + shimmering particles */
#define FX_HOLY_LIGHT_ORB          0xB /* particles: white-blue glowing orb with a halo */
#define FX_HOLY_BLUE_RINGS         0xF /* rings: blue gouraud rings */
#define FX_HOLY_WHITE_VIOLET_RINGS 0x10 /* rings: white-cyan-violet gouraud rings */
#define FX_HOLY_CYAN_RINGS         0x11 /* rings: white-cyan-blue gouraud rings */
#define FX_HOLY_LIGHT_LINES        0x12 /* emitter: thin blue light lines */
#define FX_HOLY_DARK_BLUE_RINGS    0x16 /* emitter: dark blue gouraud rings */
#define FX_HOLY_LENS_FLARE_LINE    0x17 /* sprite line: lens-flare set (white glow, orange ring, dark red disc, blue dot, small orange ring, olive disc) across the screen as the white blast builds; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_HOLY_SWIRL_TEXTURE      0x18 /* layer table: render-to-texture (btlfx_drawLayerTexture): 2 layers into VRAM (0x1C0,0x1C0) 64x64, a blue water-like swirl; skipping it changes nothing visible; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_HOLY_LIGHT_SPHERE       0x19 /* model: sphere, plain white texture */
#define FX_HOLY_BLUE_SPARKLES_2    0x1A /* emitter: blue sparkle clusters */
#define FX_HOLY_WHITE_GLOW         0x1D /* particles: soft white round glow, the white light of the final blast; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_HOLY_TEXTURED_RING      0x1E /* rings: textured ring, drawn swelling three times; the UV box reads unrelated VRAM; runtime (docs/runtime-findings.md effect-data pass): its UV box reads noise in the run-time pages too (look pending) */
#define FX_HOLY_STARFIELD_TEXTURE  0x1F /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x280,0x150) 64x128, blue sparkling stars on black, sampled by the starry vortex that fills the screen; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_HOLY_VIOLET_RAYS_2      0x21 /* emitter: two fans drawn as narrow violet-white dithered rays */
#define FX_HOLY_SKY_RINGS          0x22 /* rings: white-cyan-blue gouraud rings */
#define FX_HOLY_LILAC_RINGS        0x23 /* rings: white-lilac gouraud rings */
#define FX_HOLY_BURST_RINGS        0x26 /* emitter: particles (texture not in the file) + white-grey-violet-blue / white-cyan-violet rings */
#define FX_HOLY_CLOUD_ARCH         0x27 /* model: arch with a blue cloud texture */
#define FX_HOLY_GLOW_BAND          0x28 /* gouraud model: wide ring band */
#define FX_HOLY_BLUE_RAYS          0x29 /* emitter: two fans drawn as thin vertical blue light rays */
#define FX_HOLY_FLASH_RINGS        0x2A /* emitter: small textured rings + yellow / grey gouraud rings */
#define FX_HOLY_LIGHT_LINES_2      0x2B /* emitter: thin blue light lines */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx057Vec3;

typedef struct {
    s32 state0;
    s32 frame4; /* frame */
    s32 targetOtZ8; /* target 0's model radius / 6 (btlfx_addRadiusSixth), depth bias of its emitters and rings */
    SVECTOR posC; /* position */
    SVECTOR pos14;
    Fx057Vec3 pos1C;
    s32 model28; /* model */
} Fx057Work81; /* 0x2C */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    s32 model8; /* model */
    s32 modelC; /* model */
    SVECTOR pos10; /* position */
    Fx057Vec3 pos18; /* model position */
    SVECTOR pos24;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    Pool6CEntry *emit34;
} Fx057Work80; /* 0x38 */

extern SVECTOR fx_057_targetPos;
extern s32 fx_057_trails[];
extern s32 fx_057_fade; /* fade */
extern Pool6CEntry *fx_057_emitter1;
extern BtlModelBuf fx_057_effModels[];
extern Unk800F78A4 *fx_057_env;
extern SVECTOR fx_057_camRest; /* camera rest position */
extern SVECTOR fx_057_headPos; /* particle head position */
extern Pool6CEntry *fx_057_emitter0;
extern Unk80100C78Entry *fx_057_task;

static void fx_057_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 rot, s32 scale, s32 alpha, s32 blend, s32 frame,
                   s32 n);

#endif
