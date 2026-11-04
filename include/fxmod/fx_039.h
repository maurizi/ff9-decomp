#ifndef FX_039_H
#define FX_039_H

/* Effect module fx_039 (dir 13 file 0x39). */
#include "btlfx_mod.h"

/* fx_039 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_OSMOSE_SPARKLES     0x7 /* emitter: blue glint + small blue / white sparkle dots */
#define FX_OSMOSE_DRAIN_STREAKS 0x8 /* emitter: two fans drawn as white-violet comet streaks */
#define FX_OSMOSE_DRAIN_ORB    0x9 /* emitter: white-violet-cyan gouraud rings + screen trail, flies between caster and target */
#define FX_OSMOSE_ABSORB_FLASH 0xA /* emitter: white-lilac / white-violet-cyan rings + purple light streaks */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* target position */
    SVECTOR pos10; /* target body position */
} Fx039Work80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* target position */
    SVECTOR pos18; /* projectile position */
    s32 proj20; /* projectile index (0..2) */
} Fx039Work81; /* 0x24 */
#define FX_039_PROJ_ARC_NEG_X 1 /* proj20: the shot arcs toward -x */
#define FX_039_PROJ_ARC_POS_X 2 /* proj20: the shot arcs toward +x (0 flies straight) */

extern Unk800F78A4 *fx_039_env;
extern s32 fx_039_trails[][0x318];
extern SVECTOR fx_039_emitPos[];

#endif
