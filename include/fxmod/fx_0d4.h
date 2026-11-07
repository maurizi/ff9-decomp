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

/* fx_0d4 choreography (docs/fxdata.md, "Choreography literals"): op80's keyframe and length, op84's sword-flash
 * states and their lengths. */
#define FX_SWDMAG_FIRE_FLAME_BURST_FRAME 2    /* op80: FX_SWDMAG_FIRE_FLAME_BURST starts */
#define FX_SWDMAG_FIRE_FRAMES            0x3C /* op80: the task ends at this frame */
#define FX_SWDMAG_FIRE_FLASH_STATE_IN      0 /* op84: SWORD_GLINT and SWORD_FLASH open up at the sword */
#define FX_SWDMAG_FIRE_FLASH_STATE_FLICKER 1 /* op84: they flicker and grow */
#define FX_SWDMAG_FIRE_FLASH_STATE_FADE    2 /* op84: SWORD_FLASH turns and they fade */
#define FX_SWDMAG_FIRE_FLASH_IN_FRAMES      3 /* FX_SWDMAG_FIRE_FLASH_STATE_IN's length */
#define FX_SWDMAG_FIRE_FLASH_FLICKER_FRAMES 9 /* FX_SWDMAG_FIRE_FLASH_STATE_FLICKER's length */
#define FX_SWDMAG_FIRE_FLASH_FADE_FRAMES    9 /* FX_SWDMAG_FIRE_FLASH_STATE_FADE's length (the op ends) */

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
