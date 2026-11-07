#ifndef FX_04F_H
#define FX_04F_H

/* Effect module fx_04f (dir 13 file 0x4F: Reflect, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_04f effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_REFLECT_HALO_RINGS 0x4 /* emitter: white-pink gouraud rings, golden glow strip, leaf-shaped halo, white-green / white-pink rings */
#define FX_REFLECT_STAR       0x6 /* model: four-pointed star, sketch texture */
#define FX_REFLECT_SPARKLE_TEXTURE 0xA /* layer table: render-to-texture (btlfx_drawLayerTexture): 6 layers into VRAM (0x1C0,0x1C0) 64x64, red light with gold sparkles on the four-point Reflect star; runtime (docs/runtime-findings.md effect-data pass) */
#define FX_REFLECT_GLOW_BAND  0xB /* gouraud model: wide ring band */
#define FX_REFLECT_PANELS     0xC /* model: three curved panels, black / white texture */
#define FX_REFLECT_STREAK_TEXTURE 0xD /* layer table: render-to-texture (btlfx_drawLayerTexture): 3 layers into VRAM (0x1C0,0x120) 128x32, a red comet-like streak; skipping it changes nothing visible; runtime (docs/runtime-findings.md effect-data pass, medium) */

/* fx_04f choreography (docs/fxdata.md, "Choreography literals"): op80's states and their lengths, the glow-band pool, the models'
 * texture. */
#define FX_REFLECT_STATE_OPEN    0    /* op80: the star and panels open (scale ramps), */
#define FX_REFLECT_OPEN_FRAMES   8    /* for this many frames; then FX_REFLECT_HALO_RINGS start */
#define FX_REFLECT_STATE_GLOW    1    /* the star whitens and grows, glow bands spawn, the halo rings move out, */
#define FX_REFLECT_GLOW_FRAMES   20   /* for this many frames */
#define FX_REFLECT_WHITEN_FRAMES 10   /* FX_REFLECT_STATE_GLOW: the star's green / blue rise to full over this many */
#define FX_REFLECT_BAND_INTERVAL 5    /* FX_REFLECT_STATE_GLOW: a glow-band pool item spawns every this many frames */
#define FX_REFLECT_STATE_FADE    2    /* everything fades out over FADE_FRAMES; the op ends a frame later */
#define FX_REFLECT_FADE_FRAMES   12
#define FX_REFLECT_BAND_COUNT    3    /* glow-band pool items (op80 poolCount) */
#define FX_REFLECT_BAND_FRAMES   9    /* glow-band item: FX_REFLECT_GLOW_BAND flies out, brightening then dimming, for this many frames */
#define FX_REFLECT_MODEL_COUNT   4    /* btlfx_initEffModels slots */
/* FX_REFLECT_STAR and FX_REFLECT_PANELS: 15-bit page (0x1C0, 0x100), no clut; the star at v 0x80 (VRAM y 0x180),
 * where FX_REFLECT_SPARKLE_TEXTURE renders (0x1C0, 0x1C0); the panels at v 0, where FX_REFLECT_STREAK_TEXTURE
 * renders (0x1C0, 0x120). */
#define FX_REFLECT_TPAGE         0x117
#define FX_REFLECT_CLUT          0xFFFF
#define FX_REFLECT_STAR_V        0x80

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 unk8; /* unused (runtime watch, round 33) */
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* in front of the target */
    SVECTOR pos1C;
    SVECTOR pos24;
    s32 pos2C[3]; /* model position (a VECTOR without its pad word) */
    SVECTOR rot38; /* model rotation (target facing) */
    s32 radius40; /* target radius */
    Pool6CEntry *emit44;
    s32 frame48; /* frame */
    s32 scaleX4C; /* model scale x */
    s32 scaleZ50; /* model scale z */
} Fx04fWork; /* 0x54 */

extern Unk800F78A4 *fx_04f_env;
extern BtlModelBuf fx_04f_effModels[];

static s32 fx_04f_op80PoolTask(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work);

#endif
