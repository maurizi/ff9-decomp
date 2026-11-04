#ifndef FX_08E_H
#define FX_08E_H

/* Effect module fx_08e (dir 13 file 0x8E): Curaga, one target (party; enemy Curaga of Dummy/Necron). A morphing
 * model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party ability / item
 * tables and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31
 * runtime pass"). */
#include "btlfx_mod.h"

/* fx_08e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURAGA_SPARKLE_BURST  0x1 /* emitter: ring + sparkle sprites, still drawn as coloured squares (texture not resolved) (look pending) */
#define FX_CURAGA_WHITE_RINGS    0x3 /* emitter: white gouraud rings */
#define FX_CURAGA_GOLD_RING      0x5 /* emitter: white-gold gouraud rings */
#define FX_CURAGA_STAR_GLINTS    0x6 /* emitter: green and violet star glints */
#define FX_CURAGA_STAR_TRAIL     0x7 /* emitter: blue starry trail + sparkles */
#define FX_CURAGA_SPARKLE_SPIRAL 0xD /* emitter: spiralling sparkle sprites, still drawn as pale yellow-green squares (look pending) */
#define FX_CURAGA_LIGHT_FOUNTAIN 0xE /* model: tall funnel with a green light-fountain texture */
#define FX_CURAGA_LIGHT_RING     0xF /* model: ring with a green light-fountain texture */
#define FX_CURAGA_SPARKLES       0x10 /* emitter: sparkle sprites, still drawn as white / green squares (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    SVECTOR rot1C; /* model rotation */
    VECTOR pos24; /* model position */
    VECTOR scale34; /* model scale */
    s32 bright44; /* model brightness */
    SVECTOR pos48; /* target body position */
    SVECTOR pos50;
    SVECTOR pos58; /* caster position */
    s32 fadeIn60; /* fade-in countdown */
    s32 fadeOut64; /* fade-out countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx08eWork; /* 0x6C */

extern Unk800F78A4 *fx_08e_env;
extern s16 fx_08e_texAnimSrc[][2];
extern BtlModelBuf fx_08e_effModels[];
extern s32 fx_08e_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
