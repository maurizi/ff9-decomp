#ifndef FX_001_H
#define FX_001_H

/* Effect module fx_001 (dir 13 file 0x01). */
#include "btlfx_mod.h"

/* fx_001 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DAGGERS_MAGIC_SPARKLES       0x1 /* emitter: tiny rings + white/green sparkle sprites (at pos54) */
#define FX_DAGGERS_MAGIC_WHITE_RINGS    0x3 /* emitter: white gouraud rings */
#define FX_DAGGERS_MAGIC_GOLD_RINGS     0x5 /* emitter: white-gold gouraud rings */
#define FX_DAGGERS_MAGIC_STAR_FLARES    0x6 /* emitter: yellow-green and violet star flares, faint glow */
#define FX_DAGGERS_MAGIC_BLUE_SWIRL     0x7 /* emitter: blue swirl-textured trail + green/yellow/white sparkle sprites */
#define FX_DAGGERS_MAGIC_SPARKLE_SHOWER 0xD /* emitter: green/yellow/white sparkle sprites (at pos4C) */
#define FX_DAGGERS_MAGIC_LIGHT_SPIRES   0xE /* model: spiky spire shapes, green light-arc texture */
#define FX_DAGGERS_MAGIC_LIGHT_RING     0xF /* model: ring, green light-arc texture */
#define FX_DAGGERS_MAGIC_SPARKLE_BURST  0x10 /* emitter: white/yellow/green sparkle sprites */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 rgb48; /* model brightness */
    SVECTOR pos4C; /* target body position */
    SVECTOR pos54;
    SVECTOR pos5C; /* caster position */
    s32 fadeIn64; /* fade-in countdown */
    s32 fadeOut68; /* fade-out countdown */
    u16 model6C; /* models */
    u16 model6E;
} Fx001Work; /* 0x70 */

extern Unk800F78A4 *fx_001_env;
extern s16 fx_001_texAnimXY[][2];
extern BtlModelBuf fx_001_effModels[];
extern s32 fx_001_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
