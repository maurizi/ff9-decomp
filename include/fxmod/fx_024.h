#ifndef FX_024_H
#define FX_024_H

/* Effect module fx_024 (dir 13 file 0x024). */
#include "btlfx_mod.h"

/* fx_024 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_AERIAL_SLASH_DUST_CLOUD    0x7 /* emitter: lilac dust clouds + green glint (byte-identical to fx_01d entry 0x3) */
#define FX_AERIAL_SLASH_STAR_FLASH    0xB /* emitter: white star flash with a blue halo */
#define FX_AERIAL_SLASH_FAN_EMITTER_3 0xC /* emitter: fan + small blue spark quads (fan not rendered) (same call site as fx_01d entry 0x5) (look pending) */
#define FX_AERIAL_SLASH_FAN_EMITTER   0xD /* emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x2) (look pending) */
#define FX_AERIAL_SLASH_FAN_EMITTER_2 0xE /* emitter: fan + faint green spark quads (fan not rendered) (same call site as fx_01d entry 0x1) (look pending) */
#define FX_AERIAL_SLASH_WIND_RINGS    0xF /* emitter: ring with a violet sparkle-curtain texture + pink-white gouraud rings (same look as fx_01d entry 0xb) */
#define FX_AERIAL_SLASH_DUST_RING     0x10 /* emitter: ring with a grey dust texture (same look as fx_01d entry 0xc) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* caster position */
    SVECTOR rot20; /* caster rotation */
    SVECTOR pos28; /* bone 13 point */
    SVECTOR pos30; /* bone 29 point */
    SVECTOR pos38; /* in front of the caster */
    SVECTOR aim40; /* angles from pos28 to pos38 */
    SVECTOR aim48; /* angles from pos30 to pos38 */
    s32 tint50; /* screen tint */
} Fx024Work80; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
    s32 targetC; /* target index */
} Fx024Work81;

extern Unk800F78A4 *fx_024_env;

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

#endif
