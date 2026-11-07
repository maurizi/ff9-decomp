#ifndef FX_0DE_H
#define FX_0DE_H

/* Effect module fx_0de (dir 13 file 0xDE): Sword Magic Water. A morphing model at the caster, and the targets
 * thrown up and dropped (fx_0de_op81). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0de effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_WATER_SPLASH_DROPS 0x1 /* emitter: water blobs, blue glow and glints */
#define FX_SWDMAG_WATER_GLOBE_1 0x2 /* model: water funnel rising, morph stage 1 */
#define FX_SWDMAG_WATER_GLOBE_2 0x3 /* model: water vase shape, morph stage 2 */
#define FX_SWDMAG_WATER_GLOBE_3 0x4 /* model: water bulb closing, morph stage 3 */
#define FX_SWDMAG_WATER_GLOBE_0 0x5 /* model: flat water pool (water texture), morph stage 0 */
#define FX_SWDMAG_WATER_SPLASH_FUNNEL 0x6 /* model: swirling funnel, mist texture, morph end of the spray */
#define FX_SWDMAG_WATER_SPLASH_COLUMN 0x7 /* model: open cylinder, mist texture, morph start of the spray */
#define FX_SWDMAG_WATER_BASE_DISC 0x8 /* model: flat disc, water texture */
#define FX_SWDMAG_WATER_BLUE_RING     0x9 /* emitter: white-blue gouraud rings */
#define FX_SWDMAG_WATER_GLOBE 0xD /* model: closed water sphere, morph end */
#define FX_SWDMAG_WATER_SWORD_GLINT   0xE /* rings: white 6-segment rings flashed at the sword (op84, first draw) */
#define FX_SWDMAG_WATER_SWORD_FLASH   0xF /* rings: white 8-segment rings flashed rotating at the sword (op84) */

/* fx_0de choreography (docs/fxdata.md, "Choreography literals"): op80's globe / spray phases, the model textures
 * and the animated water texture rect; op81's lift of the targets; op84's sword flash. */
#define FX_SWDMAG_WATER_GLOBE_FRAMES  0x18 /* op80: the globe morphs GLOBE_0 -> GLOBE_1 -> 2 -> 3 -> GLOBE over this many frames */
#define FX_SWDMAG_WATER_SPRAY_FRAMES  26   /* then the globe fades and SPLASH_COLUMN morphs to SPLASH_FUNNEL over this many */
#define FX_SWDMAG_WATER_SPLASH_DROPS_FRAME 0x10 /* op80: FX_SWDMAG_WATER_SPLASH_DROPS starts */
#define FX_SWDMAG_WATER_FRAMES        0x32 /* op80's length (the op ends) */
#define FX_SWDMAG_WATER_MODEL_COUNT   8    /* btlfx_initEffModels slots: model14[] (GLOBE_0..3, GLOBE, SPLASH_COLUMN, SPLASH_FUNNEL, BASE_DISC) */
#define FX_SWDMAG_WATER_GLOBE_0_TPAGE 0x39 /* Hi_RegisterTexEffModel texture: 4-bit page (0x240, 0x100), abr 1, water texture (the rect below) */
#define FX_SWDMAG_WATER_GLOBE_0_CLUT  0x3D00 /* CLUT at (0x0, 0xF4) */
#define FX_SWDMAG_WATER_GLOBE_0_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_WATER_GLOBE_0_V     0x80
#define FX_SWDMAG_WATER_SPLASH_COLUMN_TPAGE 0x3B /* Hi_RegisterTexEffModel texture: 4-bit page (0x2C0, 0x100), abr 1, mist texture */
#define FX_SWDMAG_WATER_SPLASH_COLUMN_CLUT  0x3D01 /* CLUT at (0x10, 0xF4) */
#define FX_SWDMAG_WATER_SPLASH_COLUMN_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_WATER_SPLASH_COLUMN_V     0x80
/* The globe models sample (WATER_TPAGE 4-bit page (0x240, 0x100), v 0x80) the rect below; each frame
 * btlfx_drawScrollMove copies the next of the SCROLL_FRAMES water frames from fx_0de_scrollSrc into it. */
#define FX_SWDMAG_WATER_WATER_VRAM_X  0x240
#define FX_SWDMAG_WATER_WATER_VRAM_Y  0x180
#define FX_SWDMAG_WATER_WATER_VRAM_W  0x10
#define FX_SWDMAG_WATER_WATER_VRAM_H  0x40
#define FX_SWDMAG_WATER_SCROLL_FRAMES 23   /* fx_0de_scrollSrc entries 1..23 (frame % 23 + 1) */
#define FX_SWDMAG_WATER_LIFT_FRAMES   0x20 /* op81: the targets rise and fall back over this many frames, */
#define FX_SWDMAG_WATER_DROP_FRAMES   4    /* then drop to their home positions over this many (the op ends) */
#define FX_SWDMAG_WATER_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_WATER_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_WATER_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_WATER_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_WATER_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_WATER_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

typedef struct {
    SVECTOR homePos0[8]; /* unit home positions */
    s32 unitCount40; /* unit count */
    s32 jumpHeight44; /* jump height */
    s32 height48[8]; /* unit heights */
} Fx0deWork81; /* 0x68 */

extern Unk800F78A4 *fx_0de_env;
extern s32 fx_0de_jumpScale; /* jump scale */

s32 fx_0de_op81(s32 mode, Fx0deWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0deWork84;
s32 fx_0de_op84(s32 mode, Fx0deWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

typedef struct {
    s32 hold0; /* dead early-out flag, cleared at init and never set (see Fx046Work.hold0) */
    SVECTOR pos4; /* caster body position */
    SVECTOR posC; /* caster position */
    s32 model14[8]; /* models */
    s32 scale34; /* scale from the caster's size */
} Fx0deWork80; /* 0x38 */
typedef struct {
    u16 srcX0;
    u16 srcY2;
} Fx0deFlash;
extern Fx0deFlash fx_0de_scrollSrc[];
extern BtlModelBuf fx_0de_effModels[];

#endif
