#ifndef FX_186_H
#define FX_186_H

/* Effect module fx_186 (dir 13 file 0x186): Earth Shake; enemy Earth Shake (Adamantoise, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_186 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_EARTH_SHAKE_ROCK_ARCH     0xB /* 'so' model: arch with a rock texture, registered at the start */
#define FX_EARTH_SHAKE_GREY_RING     0xF /* rings: white-to-grey gouraud rings at each target */
#define FX_EARTH_SHAKE_ROCK_DEBRIS   0x1A /* emitter: small rock debris frames */
#define FX_EARTH_SHAKE_RUBBLE_STRIP  0x1B /* 'so' model: flat strip of rubble with a grey smoke texture, registered at the start */
#define FX_EARTH_SHAKE_ROCK_DEBRIS_2 0x1D /* emitter: small rock debris frames, brighter */
#define FX_EARTH_SHAKE_LILAC_RING    0x1E /* rings: white-lilac gouraud rings */
#define FX_EARTH_SHAKE_CYAN_RING     0x1F /* rings: white-cyan gouraud rings */

typedef struct {
    s32 state0; /* state */
    s16 pad4;
    s16 bobVelY6; /* bob height (state 0), then y velocity (state 1) */
    s16 vel8; /* velocity */
    s16 velA;
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    s16 spin1C; /* spin */
    s16 spin1E;
    s32 model20; /* model */
} Fx186Rock;

typedef struct {
    s32 state0; /* state */
    SVECTOR pos4[8]; /* home positions */
    SVECTOR rot44[8]; /* rotations */
    SVECTOR spin84[8]; /* spin */
    SVECTOR rotC4[8]; /* current rotations */
    s32 count104; /* unit count */
    s32 height108; /* bounce height */
} Fx186Work81; /* 0x10C */

extern Unk800F78A4 *fx_186_env;

typedef struct {
    s32 unk0; /* unused (the typedef has no users) */
    s32 unk4; /* unused (the typedef has no users) */
} Fx186Word2; /* an SVECTOR copied as two words */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* position */
    SVECTOR pos10;
    Pool6CEntry *emit18;
    s32 model1C[10]; /* rock models */
    s32 model44; /* model */
    s32 model48; /* model */
} Fx186Work80; /* 0x4C */

extern s16 fx_186_vramSample[]; /* StoreImage buffer */
extern u16 fx_186_modelFileIds0[10]; /* rock model files */
extern u16 fx_186_modelFileIds1[10]; /* rock model files (alternate) */
extern u32 fx_186_bgOt[];
extern BtlModelBuf fx_186_effModels[];
static s32 fx_186_op80PoolTask(s32 mode, Fx186Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame);

#endif
