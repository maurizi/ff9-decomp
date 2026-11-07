#ifndef FX_04E_H
#define FX_04E_H

/* Effect module fx_04e (dir 13 file 0x4E: Sleep, from runtime traces of btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_04e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SLEEP_LIGHT_SPIRAL   0x6 /* emitter: helix of white screen trails, white-cyan rings, green glint (same call site and look as fx_01a entry 0x2) */
#define FX_SLEEP_MOON           0x7 /* emitter: quads sampling the moon texture, the pale moon / mist disc at the target (same call site and look as fx_01a entry 0x3); runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_SLEEP_LIGHT_SPIRAL_2 0x8 /* emitter: helix of two white screen trails with green / violet glows (same call site and look as fx_01a entry 0x4) */
#define FX_SLEEP_MOON_TEXTURE   0x9 /* layer table: render-to-texture (btlfx_drawLayerTexture): 4 layers into VRAM (0x200,0x180) 128x128, a cloud-veiled moon that FX_SLEEP_MOON samples (same call site and look as fx_01a entry 0x1); runtime (docs/runtime-findings.md effect-data pass) */

/* fx_04e choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes and length (the names of fx_01a, the other Sleep
 * module). */
#define FX_SLEEP_SPIRAL_2_FRAME 0x13 /* op80: FX_SLEEP_LIGHT_SPIRAL_2 starts, FX_SLEEP_MOON_TEXTURE is rendered from here (its frame 0) */
#define FX_SLEEP_MOON_FRAME     0x27 /* op80: FX_SLEEP_MOON starts */
#define FX_SLEEP_FRAMES         0x50 /* op80: the task ends at this frame */
#define FX_SLEEP_LIGHT_SPIRAL_TRAIL_SLOTS  0xE /* FX_SLEEP_LIGHT_SPIRAL's trail buffer fx_04e_trails: slots of */
#define FX_SLEEP_LIGHT_SPIRAL_TRAIL_STRIDE 0x50 /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_SLEEP_LIGHT_SPIRAL_2_TRAIL_SLOTS  0x1E /* FX_SLEEP_LIGHT_SPIRAL_2's trail buffer fx_04e_trails2: slots of */
#define FX_SLEEP_LIGHT_SPIRAL_2_TRAIL_STRIDE 0x50 /* this many bytes (Pool6CEntry trailSlots / trailStride) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* position: env->targets->centerX..Z */
} Fx04eWork;

extern Unk800F78A4 *fx_04e_env;
extern s32 fx_04e_trails[];
extern s32 fx_04e_trails2[];

#endif
