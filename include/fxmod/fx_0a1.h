#ifndef FX_0A1_H
#define FX_0A1_H

/* Effect module fx_0a1 (dir 13 file 0xA1): enemy Blaster (Torama and others). A btlfx_drawTrail beam from the
 * caster bone to the target, then emitters. Evidence: [data] the party ability / item tables and enemy sequences
 * that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0a1 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLASTER_TEX_RING       0x1 /* emitter: textured green-grey ring band */
#define FX_BLASTER_PINK_HALO      0x2 /* emitter: green ring band + pink halo rings */
#define FX_BLASTER_PINK_HALO_2    0x3 /* emitter: pink halo rings */
#define FX_BLASTER_LIGHT_CONE     0x4 /* emitter: fan drawn as a narrowing light cone fading pink to yellow */
#define FX_BLASTER_PINK_STARBURST 0x5 /* emitter: pink starburst + violet petals */
#define FX_BLASTER_VIOLET_PETALS  0x6 /* emitter: violet petal sprites */
#define FX_BLASTER_WHITE_HAZE     0x7 /* emitter: flat orbiting trail, the white-violet haze band over the target's mouth; runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_BLASTER_GREY_RING      0x8 /* emitter: lilac-grey gouraud rings */
#define FX_BLASTER_VIOLET_RING    0xF /* emitter: white-violet gouraud rings */
#define FX_BLASTER_TRAIL_2        0x10 /* trail: small strip, mostly black with magenta texels; runtime (docs/runtime-findings.md effect-data pass 2): drawn from the start (12 calls), its strip reads pink-violet stripes at run time; ablating it changes < 250 px (look pending) */

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only (cleared at init) */
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* btlfx_getUnitArgBonePos(0x10, 0) */
    SVECTOR pos20; /* btlfx_getUnitArgBonePos(0x10, 0), every frame */
    SVECTOR aim28; /* angles from pos8 to pos18 */
} Fx0a1Work;

extern Unk800F78A4 *fx_0a1_env;
extern u8 fx_0a1_trail[];
extern s32 fx_0a1_trails[];
extern s32 fx_0a1_trails2[];

#endif
