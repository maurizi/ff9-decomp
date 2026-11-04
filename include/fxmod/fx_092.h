#ifndef FX_092_H
#define FX_092_H

/* Effect module fx_092 (dir 13 file 0x92): enemy Hypnotize (Hecteyes). Emitters only. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_092 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_HYPNOTIZE_MAGENTA_RINGS 0x1 /* emitter: violet-cream and magenta gouraud rings */
#define FX_HYPNOTIZE_HALO_RINGS    0x2 /* emitter: glowing circle particles, violet to orange */
#define FX_HYPNOTIZE_VIOLET_RING   0x3 /* emitter: violet-tan gouraud rings */
#define FX_HYPNOTIZE_SPARKLE_DOTS  0x4 /* emitter: faint white sparkle dots (btlfx_emitDots) */

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
