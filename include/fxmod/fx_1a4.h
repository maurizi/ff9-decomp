#ifndef FX_1A4_H
#define FX_1A4_H

/* Effect module fx_1a4 (dir 13 file 0x1a4): enemy Death0 (Deathguise). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1a4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH0_ROCK_DEBRIS    0x1 /* emitter: brown rock chips + orange spark dots */
#define FX_DEATH0_PINK_RINGS     0x2 /* emitter: spiral of white-pink-magenta gouraud rings + tan dust cloud */
#define FX_DEATH0_DUST_BURST     0x3 /* emitter: tan dust clouds + grey smoke */
#define FX_DEATH0_FIRE_DEBRIS    0x4 /* emitter: brown rock chips + spark dots + red fire flames */
#define FX_DEATH0_COLOUR_STREAKS 0x5 /* emitter: fans of thin blue / orange / green streaks with violet and teal smoke + pale-blue rings */
#define FX_DEATH0_FLAME_COLUMN   0x7 /* 'so' model: tall cylinder, flame texture */
#define FX_DEATH0_DUST_SPIRAL    0xC /* emitter: spiral of tan dust clouds + grey smoke */
#define FX_DEATH0_DUST_SPIRAL_2  0xD /* emitter: spiral of tan dust clouds + grey smoke */
#define FX_DEATH0_FLAME_COIL     0xE /* 'so' model: helix coil, flame texture */
#define FX_DEATH0_FLAME_COIL_2   0xF /* 'so' model: helix coil, flame texture */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24; /* caster position */
    SVECTOR pos2C;
    s32 fade34; /* brightness */
    s32 flag38; /* flag */
    s16 timer3C; /* countdowns */
    s16 timer3E;
    s16 timer40;
} Fx1a4Work80; /* 0x44 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotations */
    SVECTOR rot10;
    SVECTOR rot18;
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 color40; /* brightness */
    SVECTOR pos44; /* caster body position */
    s16 frame4C; /* counter */
    s16 speed4E;
    s16 speed50;
    s16 timer52; /* fade countdown */
    u16 model54; /* models */
    u16 model56;
    u16 model58;
} Fx1a4Work81; /* 0x5C */

extern Unk800F78A4 *fx_1a4_env;
extern BtlModelBuf fx_1a4_effModels[];
extern SVECTOR fx_1a4_emitPos0;
extern SVECTOR fx_1a4_emitPos1;

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* copied */
extern Unk800F78A4 *fx_1a4_env;
/* end copied */

#endif
