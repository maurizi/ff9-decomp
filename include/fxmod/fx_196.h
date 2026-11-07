#ifndef FX_196_H
#define FX_196_H

/* Effect module fx_196 (dir 13 file 0x196): Leviathan (short); Tsunami. Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_196 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_LEVIATHAN_WATER_DROPS_2 0x5 /* emitter: water droplet sprites (byte-identical to fx_0b3 entry 0x25) */
#define FX_LEVIATHAN_FOAM          0x6 /* animquads: blue-white foam, drawn along the wave edge (byte-identical to fx_0b3 entry 0x24) */
#define FX_LEVIATHAN_WATER_BUBBLES 0x7 /* emitter: orbiting blue water bubbles + water droplet sprites */
#define FX_LEVIATHAN_FOAM_2        0x8 /* emitter: blue-white foam anim quads, 16 frames */

/* fx_196 choreography (docs/fxdata.md, "Choreography literals"; fx_0b3's names). */
#define FX_LEVIATHAN_STATE_WAVE 0 /* op85: the tidal wave (water sheets, ripples, tints) */
#define FX_LEVIATHAN_STATE_CALM 1 /* op85: the calm water sheet */
#define FX_LEVIATHAN_WAVE_FRAMES 0x51 /* FX_LEVIATHAN_STATE_WAVE's length */
#define FX_LEVIATHAN_DROPS_3_FRAME 0x12 /*   FX_LEVIATHAN_WATER_DROPS_2 start */
#define FX_LEVIATHAN_IMPACT_FRAME 0x2B /*   the ripple grid from here; a half-transparent blue screen tint */
#define FX_LEVIATHAN_IMPACT_TINT_RGB 0x20, 0xAA, 0xFF
#define FX_LEVIATHAN_IMPACT_2_FRAME 0x2D /*   two more blue tints, */
#define FX_LEVIATHAN_IMPACT_3_FRAME 0x32 /*   (here FX_LEVIATHAN_WATER_SPLASH_2 starts too) */
#define FX_LEVIATHAN_IMPACT_2_TINT_RGB 0, 0x64, 0xFF
#define FX_LEVIATHAN_CALM_FRAMES 0x18 /* FX_LEVIATHAN_STATE_CALM's length (the op ends) */
#define FX_LEVIATHAN_OP86_FRAMES 0x2E /* op86: the task ends at this frame */
/* The water texture: 4-bit page (0x1C0, 0x100) (tpage 0x17), the water sheets sampling v 0x80 (WATER_TPAGE / CLUT /
 * U / V); fxm_drawScrollMoveX copies the file's texture from SRC_X beside it into it, scrolled. */
#define FX_LEVIATHAN_WATER_TPAGE 0x17
#define FX_LEVIATHAN_WATER_CLUT 0x3D04 /* CLUT (0x40, 0xF4) */
#define FX_LEVIATHAN_WATER_U 0
#define FX_LEVIATHAN_WATER_V 0x80
#define FX_LEVIATHAN_WATER_VRAM_X 0x1C0
#define FX_LEVIATHAN_WATER_VRAM_Y 0x180
#define FX_LEVIATHAN_WATER_VRAM_W 0x20
#define FX_LEVIATHAN_WATER_VRAM_H 0x80
#define FX_LEVIATHAN_WATER_SRC_X 0x1E0

typedef struct {
    s16 unk0; /* not read by the module */
    s16 count2; /* count */
    s16 keys4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx196Track;

typedef struct {
    u16 fileId0;
    u16 fileId2;
    u16 duration4;
    u16 unk6; /* not read by the module */
} Fx196CamPath;

typedef struct {
    SVECTOR pos0[8]; /* unit positions */
    s32 count40; /* unit count */
} Fx196Work86; /* 0x44 */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame */
    u8 pad8[0x10 - 0x8];
    SVECTOR pos10; /* position */
    SVECTOR camPos18; /* camera */
    u8 pad20[0x48 - 0x20];
    Pool6CEntry *emit48;
} Fx196Work85; /* 0x4C */

extern CVECTOR fx_196_rippleColors[];

extern Fx196CamPath fx_196_camPaths[];
extern Unk800F78A4 *fx_196_env;

static void fxm_keyGetVec(Fx196Track *track, s32 t, SVECTOR *out);
void fx_196_drawWaveMesh(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 col2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 amp, s32 ph, s32 step, s32 bulge, s32 ang, s32 dang);

/* copied */
extern Unk800F78A4 *fx_196_env;
/* end copied */

/* propagated */
s32 fxm_sinHalfNeg(s32 ang);

extern s32 fx_196_nextWavePoint;
extern SVECTOR fx_196_wavePoints[];
static s32 fxm_nextRingPoint(SVECTOR *out);

#endif
