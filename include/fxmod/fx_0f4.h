#ifndef FX_0F4_H
#define FX_0F4_H

/* Effect module fx_0f4 (dir 13 file 0x0F4): Shift Break. Two growing textured models with layer textures
 * (btlfx_drawLayerTexture). Evidence: [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0f4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SHIFT_BREAK_WHITE_RINGS  0x2 /* emitter: white gouraud rings in a cone + a textured ring (texture not resolved) */
#define FX_SHIFT_BREAK_RAINBOW_HALO 0x3 /* emitter: orbiting particles, dark disc with a rainbow rim */
#define FX_SHIFT_BREAK_VIOLET_HALO  0x4 /* emitter: orbiting white swirl marks + blue-violet glow ring */
#define FX_SHIFT_BREAK_BACKDROP     0x8 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_SHIFT_BREAK_DOME         0xA /* model: stepped dome, white texture */
#define FX_SHIFT_BREAK_DOME_2       0xB /* model: stepped dome, white texture (byte-identical to entry 0xA) */
#define FX_SHIFT_BREAK_CONE_RINGS   0xC /* emitter: textured rings in a cone; the render reads garbled multicolour texels (texture not resolved) (look pending) */
#define FX_SHIFT_BREAK_LIGHTNING    0xD /* emitter: white-cyan gouraud rings + blue lightning streak particles */
#define FX_SHIFT_BREAK_BACKDROP_2   0xE /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    SVECTOR rot14; /* model rotation */
    VECTOR pos1C; /* model position */
    VECTOR scale2C; /* model scale */
    s32 rgb3C; /* model brightness */
    SVECTOR rot40; /* model 2 rotation */
    VECTOR scale48; /* model 2 scale */
    SVECTOR pos58; /* centre */
    SVECTOR startPos60;
    s16 fadeInTimer68; /* fade-in countdown */
    s16 fadeOutTimer6A; /* fade-out countdown */
    u16 model6C; /* model */
    u16 model6E; /* model 2 */
} Fx0f4Work; /* 0x70 */

extern Unk800F78A4 *fx_0f4_env;
extern BtlModelBuf fx_0f4_effModels[];

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

#endif
