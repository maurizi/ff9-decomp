#ifndef FX_0EE_H
#define FX_0EE_H

/* Effect module fx_0ee (dir 13 file 0x0EE): enemy Mirror. Emitters in front of the caster. Evidence: [data] the
 * party ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0ee effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_MIRROR_WHITE_RINGS   0x1 /* emitter: white rings + green glow */
#define FX_MIRROR_WHITE_RINGS_2 0x2 /* emitter: white rings + green glow */
#define FX_MIRROR_GREEN_SWIRL   0x3 /* emitter: white rings + green swirl particle */
#define FX_MIRROR_LIGHT_SPIKES  0x5 /* emitter: two fans, white-violet light spikes */

typedef struct {
    SVECTOR pos0;
    SVECTOR rot8;
} Fx0eeWork;

extern Unk800F78A4 *fx_0ee_env;

#endif
