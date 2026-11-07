#ifndef FX_179_H
#define FX_179_H

/* Effect module fx_179 (dir 13 file 0x179): Medeo. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_179 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MEDEO_WHITE_RING    0x1 /* emitter: white and lilac gouraud rings */
#define FX_MEDEO_METEOR        0x2 /* model: rock (128 polys, red-brown rock texture) */
#define FX_MEDEO_COLOR_SMOKE   0x4 /* emitter: spiral of yellow, purple, cyan and green smoke puffs */
#define FX_MEDEO_GLOW_TRAIL    0x5 /* emitter: blue and green glow screen trails */
#define FX_MEDEO_SPARKLES      0x7 /* emitter: small green and blue spark sprites */
#define FX_MEDEO_STAR_FLASH    0x8 /* emitter: blue and green star flashes */
#define FX_MEDEO_COLOR_LIGHT_BARS 0x9 /* emitter: three fans of blue / gold / green capsule-shaped light bars, + sprites that render transparent */
#define FX_MEDEO_COLOR_SMOKE_2 0xA /* emitter: spiral of yellow, purple, cyan and green smoke puffs */
#define FX_MEDEO_GREEN_SPECKS  0xB /* emitter: two sprite elements, tiny green specks at run time; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */

/* fx_179 choreography (docs/fxdata.md, "Choreography literals"): op80's states and lengths, the meteor's texture
 * and scale, the four textured ribbons round it (fxm_drawTexCylinder), the shatter pieces, the screen tints. */
#define FX_MEDEO_STATE_ORBIT   0    /* op80 work->state0: METEOR bobs above the targets inside the ribbons, the camera follows */
#define FX_MEDEO_CAMERA_FRAMES 10   /* (over this many frames) */
#define FX_MEDEO_ORBIT_FRAMES  0x10 /* then it shatters; WHITE_RING, GLOW_TRAIL, COLOR_LIGHT_BARS, ... start */
#define FX_MEDEO_STATE_SHATTER 1    /* the pieces fly apart */
#define FX_MEDEO_SHATTER_FRAMES 0x20 /* (the op ends; the models are cleared) */
#define FX_MEDEO_METEOR_TPAGE  0x9A   /* 8-bit page (0x280, 0x100) */
#define FX_MEDEO_METEOR_CLUT   0x3D40 /* CLUT (0, 0xF5) */
#define FX_MEDEO_METEOR_U      0
#define FX_MEDEO_METEOR_V      0
#define FX_MEDEO_METEOR_SCALE  0xAAA  /* METEOR's and the ribbons' scale */
#define FX_MEDEO_RIBBON_TPAGE  0x17   /* the ribbons: 4-bit page (0x1C0, 0x100), u 0, v RIBBON_V, additive; */
#define FX_MEDEO_RIBBON_V      0x40
#define FX_MEDEO_RIBBON_CLUT   0x3D03 /* one CLUT each: (0x30, 0xF4), */
#define FX_MEDEO_RIBBON_2_CLUT 0x3D00 /* (0, 0xF4), */
#define FX_MEDEO_RIBBON_3_CLUT 0x3D04 /* (0x40, 0xF4), */
#define FX_MEDEO_RIBBON_4_CLUT 0x3D06 /* (0x60, 0xF4) */
#define FX_MEDEO_RIBBON_SEGMENTS 16   /* fxm_drawTexCylinder: quads round the ribbon (8-texel tiles; the end ones gouraud-faded) */
#define FX_MEDEO_SHARDS        160    /* fx_179_shatterTris / fx_179_shatterPieces: METEOR's triangles, each flies off */
#define FX_MEDEO_SHARD_SCALE   0xF00
#define FX_MEDEO_SHARD_FRAMES  0x20   /* a piece is gone after this many frames */
#define FX_MEDEO_DARKEN_RGB    100, 100, 100 /* STATE_SHATTER frame 0: subtractive */
#define FX_MEDEO_FLASH_RGB     70, 70, 70    /* frame 1: additive */

typedef struct {
    SVECTOR verts0[3];
} Fx179Tri; /* model triangle */

typedef struct {
    s32 pos0; /* centre, 20.12 */
    s32 pos4;
    s32 pos8;
    s16 velC; /* velocity */
    s16 velE;
    s16 vel10;
    s16 unk12; /* write-only: scale (0x1000) */
    s16 rot14; /* spin */
    s16 rot16;
    s16 frame18; /* frame, -1 = gone */
    s16 delay1A; /* first frame */
} Fx179Shard; /* 0x1C */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* orb position */
    SVECTOR pos10; /* caster position */
    s32 model18; /* model */
    u8 unused1C[4]; /* pads to the size op80 asks for */
} Fx179Work; /* 0x20 */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx179Rgbc; /* primitive colour word */

extern Unk800F78A4 *fx_179_env;
extern BtlModelBuf fx_179_effModels[];
extern u8 fx_179_op80Trails[];
extern Fx179Tri fx_179_shatterTris[FX_MEDEO_SHARDS];
extern Fx179Shard fx_179_shatterPieces[FX_MEDEO_SHARDS];

static void fxm_shatterModel(s32 model, SVECTOR *center);
void fxm_updateShatterPieces(void);
static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);
s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
