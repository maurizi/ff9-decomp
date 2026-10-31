#ifndef FX_0A7_H
#define FX_0A7_H

/* Effect module fx_0a7 (dir 13 file 0xA7): enemy Water-gun (Sahagin). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR pos18; /* unit 0x10 body position */
    SVECTOR pos20; /* caster body position */
    SVECTOR pos28; /* caster position */
    SVECTOR pos30; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos38; /* pos30 moved 0x12C along aim48 */
    SVECTOR pos40; /* pos30 moved 0xC8 along aim48 */
    SVECTOR aim48; /* angles from pos30 to pos20 */
    SVECTOR aim50; /* angles from pos20 to pos30 */
} Fx0a7Work;

extern Unk800F78A4 *fx_0a7_env;
extern s32 fx_0a7_trails[];
extern s32 fx_0a7_trails2[];
extern s32 fx_0a7_trails3[];

#endif
