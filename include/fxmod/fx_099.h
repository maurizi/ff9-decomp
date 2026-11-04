#ifndef FX_099_H
#define FX_099_H

/* Effect module fx_099 (dir 13 file 0x99): Break. Evidence: the file btlfx_loadFile loads for the spell (docs/runtime-findings.md, btlfx_loadFile). */
#include "btlfx_mod.h"

/* fx_099 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BREAK_STONE_CRUST 0x1 /* 'so' model: jagged brown stone pieces (cracked rock texture) */
#define FX_BREAK_CYAN_RINGS 0x2 /* emitter: white-cyan / lilac gouraud rings + fan */
#define FX_BREAK_DUST_CLOUD 0x3 /* emitter: sparkles, brown dust cloud, pale puffs */
#define FX_BREAK_ROCK_RING  0x4 /* emitter: ring of white rock chunks */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* caster position on the ground */
    SVECTOR pos10; /* caster body position */
    s32 scale18; /* model scale */
    s32 model1C; /* model */
    SVECTOR rot20; /* model rotation */
    s32 scale28[3]; /* model scale (VECTOR without its pad word) */
    s32 modelPos34[3]; /* model position (VECTOR without its pad word) */
} Fx099Work;

extern Unk800F78A4 *fx_099_env;
extern BtlModelBuf fx_099_effModels[];

#endif
