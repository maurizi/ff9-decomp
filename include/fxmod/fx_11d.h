#ifndef FX_11D_H
#define FX_11D_H

/* Effect module fx_11d (dir 13 file 0x11D): enemy Absorb (Garnet/Prison Cage). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_11d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ABSORB_VIOLET_BLOBS 0x1 /* emitter: violet blob clouds, 19 animated frames */
#define FX_ABSORB_TEAL_RING    0x3 /* emitter: teal-white gouraud ring */
#define FX_ABSORB_PINK_ORBS    0x9 /* emitter: 3D trail, the pink glowing orbs flying between the units (byte-identical to entry 0xA); runtime (docs/runtime-findings.md effect-data pass 2, medium) */
#define FX_ABSORB_PINK_ORBS_2  0xA /* emitter: 3D trail, the second emitter of the pink orb pair (byte-identical to entry 0x9; not ablated); runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_ABSORB_RED_GLOW     0xB /* emitter: red glow orb with a white / cyan core, 11 frames (byte-identical to entry 0xC) */
#define FX_ABSORB_RED_GLOW_2   0xC /* emitter: red glow orb with a white / cyan core, 11 frames (byte-identical to entry 0xB) */
#define FX_ABSORB_LENS_FLARE   0xD /* emitter: rainbow-rimmed lens flare sprites spiralling + a 3D trail (renders black) */

/* fx_11d choreography (docs/fxdata.md, "Choreography literals"): the orb waves along the caster's bone keys, the
 * trail buffers and op80's keys. */
#define FX_ABSORB_PATH_KEYS          4    /* fx_11d_boneKeys0 / 1: one bone-pair key per particle frame (the last held) */
#define FX_ABSORB_ORB_INTERVAL       12   /* a RED_GLOW pair and a PINK_ORBS pair start every this many frames */
#define FX_ABSORB_ORBS_FRAMES        0x18 /* while frame < this */
#define FX_ABSORB_ORB_TRAIL_SLOTS    4    /* PINK_ORBS / PINK_ORBS_2 trail buffers (fx_11d_op80Trails0 / 1): slots */
#define FX_ABSORB_ORB_TRAIL_STRIDE   0x70 /* bytes per slot */
#define FX_ABSORB_LENS_FLARE_FRAME   0x1E /* LENS_FLARE starts above the caster */
#define FX_ABSORB_FLARE_TRAIL_SLOTS  8    /* its trail buffer (fx_11d_op80Trails2): slots */
#define FX_ABSORB_FLARE_TRAIL_STRIDE 0x204 /* bytes per slot */
#define FX_ABSORB_FRAMES             0x50 /* op80's length (the op ends) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR pos1C; /* caster position */
    SVECTOR pos24;
    SVECTOR pos2C;
} Fx11dWork; /* 0x34 */

typedef struct {
    u16 bone0; /* bone from */
    u16 bone2; /* bone to */
    u16 blend4; /* blend */
} Fx11dPathKey;

/* effect file header as the particle path callback sees it */
typedef struct {
    u8 pad0[8];
    s32 (*motion8)(VECTOR *, s32, SVECTOR *, s32, s32, s32);
} Fx11dEffHdr;

extern Fx11dPathKey fx_11d_boneKeys0[FX_ABSORB_PATH_KEYS];
extern Fx11dPathKey fx_11d_boneKeys1[FX_ABSORB_PATH_KEYS];
extern Unk800F78A4 *fx_11d_env;
extern s32 fx_11d_op80Trails0[];
extern s32 fx_11d_op80Trails1[];
extern s32 fx_11d_op80Trails2[];
extern SVECTOR fx_11d_emitPos0;
extern SVECTOR fx_11d_emitPos1;

static s32 fx_11d_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_11d_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_boneLerpParticle(VECTOR *out, SVECTOR *pos, Fx11dPathKey *key, s32 frame, s32 n);

#endif
