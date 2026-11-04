#ifndef FX_0AE_H
#define FX_0AE_H

/* Effect module fx_0ae (dir 13 file 0xAE): enemy Venom Breath (Chimera and others). Emitters only. Evidence:
 * [data] the party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ae effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VENOM_BREATH_VENOM_CLOUD  0x1 /* emitter: violet venom blobs + droplets */
#define FX_VENOM_BREATH_VENOM_SPLASH 0x2 /* emitter: violet venom blobs and droplet splash */

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* unit 0x10 bone position */
    SVECTOR pos10; /* caster body position */
    SVECTOR pos18; /* caster position */
    SVECTOR aim20; /* angles from pos8 to pos10 */
} Fx0aeWork;

extern Unk800F78A4 *fx_0ae_env;
extern s32 fx_0ae_trails[];

#endif
