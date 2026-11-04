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
