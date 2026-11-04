#ifndef FX_0D5_H
#define FX_0D5_H

/* Effect module fx_0d5 (dir 13 file 0xD5): Sword Magic Fira. Evidence: [data] the party ability / item tables and
 * enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0d5 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_FIRA_FLAME_BURST   0x1 /* emitter: flame explosion with ember sparks, at the target body (byte-identical to fx_002 entry 0x4) */
#define FX_SWDMAG_FIRA_SMOKE         0x2 /* emitter: grey smoke puffs (byte-identical to fx_002 entry 0x5) */
#define FX_SWDMAG_FIRA_FLAME_BURST_2 0x4 /* emitter: second flame explosion (embers, red-tinted burst) (byte-identical to fx_002 entry 0x7) */
#define FX_SWDMAG_FIRA_SMOKE_2       0x5 /* emitter: grey smoke puffs after the second burst (byte-identical to fx_002 entry 0x8) */
#define FX_SWDMAG_FIRA_FLAME_SWIRL   0x6 /* emitter: flame-strip trails orbiting the target (byte-identical to fx_002 entry 0x9) */
#define FX_SWDMAG_FIRA_GROUND_RING   0xD /* rings: red gouraud ring drawn flat on the ground (byte-identical to fx_002 entry 0xD) */
#define FX_SWDMAG_FIRA_SWORD_GLINT   0xE /* rings: white gouraud ring flash at the sword (btlfx_getWeaponPos), drawn flat */
#define FX_SWDMAG_FIRA_SWORD_GLINT_2 0xF /* rings: white gouraud ring flash at the sword, rotated */
#define FX_SWDMAG_FIRA_HEAT_RING     0x12 /* emitter: white-red gouraud rings at the target's feet (byte-identical to fx_002 entry 0xe) */

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4; /* caster position */
    SVECTOR posC; /* caster body position */
} Fx0d5Work80;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d5Work84;

extern Unk800F78A4 *fx_0d5_env;
extern s32 fx_0d5_trails[];

#endif
