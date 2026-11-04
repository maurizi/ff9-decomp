#ifndef FX_03D_H
#define FX_03D_H

/* Effect module fx_03d (dir 13 file 0x3D). */
#include "btlfx_mod.h"

/* fx_03d effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LUNA_MAGENTA_RING   0x1 /* emitter: white-magenta gouraud ring + two fans */
#define FX_LUNA_GOLD_RINGS     0x2 /* emitter: white-gold and dark magenta gouraud rings */
#define FX_LUNA_RED_TRAIL      0x3 /* emitter: flat trail with a red strip + green sparkles, sparks (texture not in the file) */
#define FX_LUNA_FAN_EMITTER    0x7 /* emitter: four fan elements (not rendered) (look pending) */
#define FX_LUNA_FAN_EMITTER_2  0x10 /* emitter: one fan element (not rendered) (look pending) */
#define FX_LUNA_SPARKLE_CIRCLE 0x11 /* emitter: circle of gold / white sparkle quads + blue-green specks */
#define FX_LUNA_SO_MODEL_2     0x16 /* 'so' model registered in op82 (look pending) */
#define FX_LUNA_SO_MODEL       0x17 /* 'so' model registered in op82 (look pending) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx03dWork80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    u8 pad10[8];
    SVECTOR pos18;
} Fx03dWork81; /* 0x20 */

typedef struct {
    SVECTOR rot0; /* model rotation */
    VECTOR pos8; /* model position */
    VECTOR scale18; /* model 1 scale */
    VECTOR scale28; /* model 2 scale */
    s32 rgb38; /* model brightness */
    s16 fadeIn3C; /* fade-in countdown */
    s16 fadeOut3E; /* fade-out countdown */
    u16 model40; /* models */
    u16 model42;
} Fx03dWork82; /* 0x44 */

extern Unk800F78A4 *fx_03d_env;
extern BtlModelBuf fx_03d_effModels[];
extern s32 fx_03d_trails[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
