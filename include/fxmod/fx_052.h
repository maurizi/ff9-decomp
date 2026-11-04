#ifndef FX_052_H
#define FX_052_H

/* Effect module fx_052 (dir 13 file 0x52). */
#include "btlfx_mod.h"

/* fx_052 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LIMIT_GLOVE_SPHERE 0x7 /* 'so' model: sphere, texture still outside the file and the common pages (magenta), registered in op80 (look pending) */
#define FX_LIMIT_GLOVE_GOLD_SPARKLES 0x8 /* emitter: rings of golden sparkles */
#define FX_LIMIT_GLOVE_GOLD_GLOW     0x9 /* emitter: golden glow, red streak trail, pink dots */
#define FX_LIMIT_GLOVE_GOLD_GLOW_2   0xA /* emitter: golden glow, red streak trail, pink dots */
#define FX_LIMIT_GLOVE_GRID_SPHERE   0xB /* 'so' model: sphere textured with glowing blue grid lines, registered in op80 */
#define FX_LIMIT_GLOVE_BACKDROP      0xC /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture; not drawn by the tool) (look pending) */
#define FX_LIMIT_GLOVE_LIGHT_BEAM    0xD /* emitter: blue glow + pink-white light beam */
#define FX_LIMIT_GLOVE_GLOW_DOME     0xE /* gouraud model: shallow dome */
#define FX_LIMIT_GLOVE_BLUE_RINGS    0xF /* emitter: white-cyan / white-blue / grey gouraud rings */
#define FX_LIMIT_GLOVE_PINK_SPECKS   0x10 /* emitter: faint pink specks */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR rotC; /* model rotation */
    SVECTOR rot14; /* second model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR pos40; /* target body position */
    SVECTOR pos48; /* target position */
    s16 fadeIn50; /* fade-in countdown */
    u16 model52; /* models */
    u16 model54;
    u16 model56;
} Fx052Work80; /* 0x58 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR posC; /* target body position */
} Fx052Work81; /* 0x14 */

typedef struct {
    s16 fadeIn0; /* fade-in countdown */
    s16 fadeOut2; /* fade-out countdown */
} Fx052Work82;

extern Unk800F78A4 *fx_052_env;
extern BtlModelBuf fx_052_effModels[];
extern s32 fx_052_trails[];
extern s32 fx_052_trails2[];

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);

#endif
