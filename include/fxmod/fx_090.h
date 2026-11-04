#ifndef FX_090_H
#define FX_090_H

/* Effect module fx_090 (dir 13 file 0x90): Curaga, all targets (party; enemy Curaga of Hilgigars and others). A
 * morphing model (Hi_DrawMorphEffModel) and texture frames copied by MoveImage. Evidence: [data] the party
 * ability / item tables and enemy sequences that play this file id (tools/effect_labels.py;
 * docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_090 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_CURAGA_STAR_GLINTS    0x2 /* emitter: white star glints + faint coloured arcs */
#define FX_CURAGA_SPARKLE_BURST  0x3 /* emitter: ring + sparkle sprites (drawn as squares: texture not resolved) (look pending) */
#define FX_CURAGA_STAR_TRAIL     0x5 /* emitter: blue starry trail + sparkles */
#define FX_CURAGA_LIGHT_FOUNTAIN 0xB /* model: tall funnel with a cyan light-fountain texture */
#define FX_CURAGA_WHITE_RINGS    0xC /* emitter: white gouraud rings */
#define FX_CURAGA_LIGHT_RING     0xD /* model: ring with a cyan light-fountain texture */
#define FX_CURAGA_TEAL_RING      0xE /* emitter: white-teal gouraud rings */
#define FX_CURAGA_SPARKLES       0xF /* emitter: sparkle sprites (drawn as squares) (look pending) */
#define FX_CURAGA_SPARKLE_SPIRAL 0x10 /* emitter: spiralling sparkle sprites (drawn as squares) (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    s32 unk4; /* unused */
    s32 unk8; /* unused */
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    SVECTOR rot24; /* model rotation */
    VECTOR pos2C; /* model position */
    VECTOR scale3C; /* model scale */
    s32 bright4C; /* model brightness */
    SVECTOR pos50; /* position: env->targets->centerX..Z */
    SVECTOR pos58; /* pos50 raised by 0x320 */
    SVECTOR pos60; /* pos50 raised by 0x4B0 */
    s32 fadeIn68; /* fade-in countdown */
    s32 fadeOut6C; /* fade-out countdown */
    u16 model70; /* models */
    u16 model72;
} Fx090Work; /* 0x74 */

extern s16 fx_090_texAnimSrc[][2];
extern Unk800F78A4 *fx_090_env;
extern BtlModelBuf fx_090_effModels[];
extern s32 fx_090_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
