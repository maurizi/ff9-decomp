#ifndef FX_02B_H
#define FX_02B_H

/* Effect module fx_02b (dir 13 file 0x02B). */
#include "btlfx_mod.h"

/* fx_02b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERIAL_SLASH_CRESCENT      0x1 /* emitter: fan drawn as a pale green-white crescent slash + faint green spark quads (same call site as fx_01d entry 0x2) */
#define FX_AERIAL_SLASH_CRESCENT_2    0x2 /* emitter: fan drawn as a pale green-white crescent slash + faint green spark quads (same call site as fx_01d entry 0x1) */
#define FX_AERIAL_SLASH_DUST_CLOUD    0x3 /* emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) */
#define FX_AERIAL_SLASH_BLUE_GUST     0x5 /* emitter: fan drawn as a blue crescent gust + small blue spark quads (byte-identical to fx_01d entry 0x5) */
#define FX_AERIAL_SLASH_WIND_RINGS    0xB /* emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (byte-identical to fx_01d entry 0xB) */
#define FX_AERIAL_SLASH_DUST_RING     0xC /* emitter: ring with a grey dust texture (byte-identical to fx_01d entry 0xC) */

/* fx_02b choreography (docs/fxdata.md, "Choreography literals"): op80's keyframes, flash and length, the gust's target point;
 * op81's length (the names of fx_01d, the other Aerial Slash module, with this module's values). */
#define FX_AERIAL_SLASH_FLASH_FRAME 10   /* op80: additive white flash (this frame and the next); the wind rings, dust ring and blue gust start */
#define FX_AERIAL_SLASH_FLASH_LEVEL 0x80 /* the flash's screen tint (r = g = b) */
#define FX_AERIAL_SLASH_GRID_FRAME  0xD  /* op80: the screen grid (btlfx_drawScreenGrid) is drawn, growing, from here */
#define FX_AERIAL_SLASH_FRAMES      0x28 /* op80: the task ends at this frame */
#define FX_AERIAL_SLASH_GUST_REACH  0x7D0 /* pos34, where the crescents fly and the gust starts: this far in front of the caster, */
#define FX_AERIAL_SLASH_GUST_HEIGHT 0xC8  /* this high */
#define FX_AERIAL_SLASH_BLADE_REACH 0x0   /* the two crescents start this far along the caster's bones 31 and 45 */
#define FX_AERIAL_SLASH_DUST_FRAMES 0x28 /* op81: the task ends at this frame */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14; /* caster position */
    SVECTOR rot1C; /* caster rotation */
    SVECTOR pos24; /* bone 31 point */
    SVECTOR pos2C; /* bone 45 point */
    SVECTOR pos34; /* in front of the caster */
    SVECTOR aim3C; /* angles from pos24 to pos34 */
    SVECTOR aim44; /* angles from pos2C to pos34 */
    s32 tint4C; /* screen tint */
} Fx02bWork80; /* 0x50 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 targetC; /* target index */
} Fx02bWork81;

extern Unk800F78A4 *fx_02b_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
