#ifndef FX_045_H
#define FX_045_H

/* Effect module fx_045 (dir 13 file 0x045). */
#include "btlfx_mod.h"

/* fx_045 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERIAL_SLASH_STAR_FLASH    0x1 /* emitter: white star flash with a blue halo (same call site as fx_024 entry 0xb) */
#define FX_AERIAL_SLASH_BLUE_GUST     0x2 /* emitter: fan drawn as a blue crescent gust + small blue spark quads (byte-identical to fx_01d entry 0x5) */
#define FX_AERIAL_SLASH_CRESCENT      0x3 /* emitter: fan drawn as a pale green-white crescent slash + faint green spark quads (same call site as fx_01d entry 0x2) */
#define FX_AERIAL_SLASH_CRESCENT_2    0x4 /* emitter: fan drawn as a pale green-white crescent slash + faint green spark quads (same call site as fx_01d entry 0x1) */
#define FX_AERIAL_SLASH_WIND_RINGS    0x5 /* emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xb) */
#define FX_AERIAL_SLASH_DUST_RING     0x6 /* emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xc) */
#define FX_AERIAL_SLASH_DUST_CLOUD    0x7 /* emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) */

/* fx_045 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, flash and length, the gust's target point;
 * op81's length (the names of fx_01d, the other Aerial Slash module, with this module's values). */
#define FX_AERIAL_SLASH_GUST_FRAME  10   /* op80: the wind rings, dust ring, star flash and blue gust start */
#define FX_AERIAL_SLASH_FLASH_FRAME 11   /* op80: additive white flash (one frame) */
#define FX_AERIAL_SLASH_FLASH_LEVEL 0x80 /* the flash's screen tint (r = g = b) */
#define FX_AERIAL_SLASH_GRID_FRAME  0xD  /* op80: the screen grid (btlfx_drawScreenGrid) is drawn, growing, from here */
#define FX_AERIAL_SLASH_FRAMES      0x28 /* op80: the task ends at this frame */
#define FX_AERIAL_SLASH_GUST_REACH  0x3E8 /* pos38, where the crescents fly and the gust starts: this far in front of the caster, */
#define FX_AERIAL_SLASH_GUST_HEIGHT 0xC8  /* this high */
#define FX_AERIAL_SLASH_BLADE_REACH 0x320 /* the two crescents start this far along the caster's bones 28 and 54 */
#define FX_AERIAL_SLASH_DUST_FRAMES 0x28 /* op81: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* caster position */
    SVECTOR rot20; /* caster rotation */
    SVECTOR pos28; /* bone 28 point */
    SVECTOR pos30; /* bone 54 point */
    SVECTOR pos38; /* in front of the caster */
    SVECTOR aim40; /* angles from pos28 to pos38 */
    SVECTOR aim48; /* angles from pos30 to pos38 */
    s32 tint50; /* screen tint */
} Fx045Work80; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 targetC; /* target index */
} Fx045Work81;

extern Unk800F78A4 *fx_045_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
