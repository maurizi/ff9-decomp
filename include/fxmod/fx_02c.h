#ifndef FX_02C_H
#define FX_02C_H

/* Effect module fx_02c (dir 13 file 0x2C). */
#include "btlfx_mod.h"

/* fx_02c effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BAD_BREATH_GAS_CLOUD 0x7 /* emitter: yellow-grey and purple gas clouds + green / pink glows */
#define FX_BAD_BREATH_GAS_BURST 0x8 /* emitter: yellow-grey and purple gas clouds, glows, green gas burst */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10;
    SVECTOR aim18; /* angles from pos8 to pos10 */
} Fx02cWork;

extern Unk800F78A4 *fx_02c_env;

#endif
