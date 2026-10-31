#ifndef FX_092_H
#define FX_092_H

/* Effect module fx_092 (dir 13 file 0x92): enemy Hypnotize (Hecteyes). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    SVECTOR pos10; /* unit 0x10 body position */
    SVECTOR rot18; /* unit 0x10 rotation */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28; /* btlfx_getUnitArgBonePos(0x10, 0), moved 0xC8 along rot18 */
    SVECTOR pos30; /* pos20 raised by 0x1C2 */
    SVECTOR pos38; /* pos20 raised by 0xC8 */
    SVECTOR aim40; /* angles from the camera eye to pos28 */
} Fx092Work;

extern Unk800F78A4 *fx_092_env;

#endif
