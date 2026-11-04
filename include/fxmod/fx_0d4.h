#ifndef FX_0D4_H
#define FX_0D4_H

/* Effect module fx_0d4 (dir 13 file 0xD4): Sword Magic Fire. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0d4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_FIRE_FIREBALL      0x1 /* emitter: fireball sprite with ember sparks, at the target body (byte-identical to fx_000 entry 0x5) */
#define FX_SWDMAG_FIRE_FLAME_BURST   0x2 /* emitter: flame explosion (burst, smoke, embers) rising around the target (byte-identical to fx_000 entry 0x6) */
#define FX_SWDMAG_FIRE_HEAT_RING     0x9 /* emitter: white-red gouraud rings at the target's feet (byte-identical to fx_000 entry 0x7) */
#define FX_SWDMAG_FIRE_SWORD_GLINT   0xB /* rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat */
#define FX_SWDMAG_FIRE_SWORD_FLASH 0xC /* rings: white 8-segment rings flashed rotating at the sword (byte-identical to the SWORD_FLASH of every Sword Magic module) */

typedef struct {
    s32 state0; /* state */
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
} Fx0d4Work80;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d4Work84;

extern Unk800F78A4 *fx_0d4_env;

#endif
