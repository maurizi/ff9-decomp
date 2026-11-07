#ifndef FX_0FA_H
#define FX_0FA_H

/* Effect module fx_0fa (dir 13 file 0x0FA): Detect. A model that rises from the caster and turns to the target,
 * with scrolling textures (btlfx_drawScrollMove). Evidence: [data] the party ability / item tables and enemy
 * sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_0fa effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DETECT_METAL_SPHERE 0x1 /* model: 12-sided sphere, bronze riveted-metal texture in the render */
#define FX_DETECT_BLUE_SPIKES 0x2 /* emitter: two fans, blue light spikes */
#define FX_DETECT_GLYPH_TEXT  0x3 /* emitter: blocks of scrolling script glyphs (blue, green, purple, orange) */

/* fx_0fa choreography (docs/fxdata.md, "Choreography literals"): op80's keys, the countdown slots of
 * work->timers0 (by index) and the lengths written into them, the sphere's texture and its flashing VRAM rect. */
#define FX_DETECT_MODEL_SLOTS      2    /* btlfx_initEffModels slots */
#define FX_DETECT_TIMER_SLOTS      10   /* work->timers0 */
#define FX_DETECT_T_RISE           0    /* timers0 slot: the sphere spirals up from the caster */
#define FX_DETECT_T_SINK           1    /* the sphere spirals back down */
#define FX_DETECT_T_SPHERE         2    /* the sphere draws (bobbing) while this runs */
#define FX_DETECT_T_FLASH          4    /* the sphere texture flashes through fx_0fa_scrollSrc0 */
#define FX_DETECT_T_FLICKER        5    /* a random flicker through fx_0fa_scrollSrc1 */
#define FX_DETECT_T_AIM            6    /* flag: BLUE_SPIKES follow the sphere's aim */
#define FX_DETECT_RISE_FRAMES      0x10 /* T_RISE from frame 0 */
#define FX_DETECT_BLUE_SPIKES_FRAME 0x14 /* BLUE_SPIKES start, T_FLASH and T_AIM */
#define FX_DETECT_FLASH_FRAMES     8    /* T_FLASH: two frames per fx_0fa_scrollSrc0 entry */
#define FX_DETECT_GLYPH_TEXT_FRAME 0x20 /* GLYPH_TEXT starts; random flickers after this frame */
#define FX_DETECT_FLICKER_FRAMES   4    /* T_FLICKER: one per fx_0fa_scrollSrc1 entry (1 in 8 frames starts one) */
#define FX_DETECT_SINK_FRAME       0x40 /* T_SINK starts */
#define FX_DETECT_SINK_FRAMES      0x10
#define FX_DETECT_FRAMES           0x50 /* op80's length; T_SPHERE runs for all of it */
#define FX_DETECT_METAL_SPHERE_TPAGE 0xFA   /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 3 */
#define FX_DETECT_METAL_SPHERE_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_DETECT_METAL_SPHERE_U     0x80   /* texture origin (u, v) in the page: VRAM (0x2C0, 0x180) */
#define FX_DETECT_METAL_SPHERE_V     0x80
#define FX_DETECT_SPHERE_VRAM_X    0x2C0 /* btlfx_drawScrollMove destination: the sphere's texture (page 0xFA at u, v 0x80) */
#define FX_DETECT_SPHERE_VRAM_Y    0x180
#define FX_DETECT_SPHERE_VRAM_W    0x40
#define FX_DETECT_SPHERE_VRAM_H    0x40

static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out);

typedef struct {
    s32 timers0[FX_DETECT_TIMER_SLOTS]; /* countdowns and flags, by index (cleared at init) */
    SVECTOR homePos28; /* model home position */
    SVECTOR pos30; /* target position */
    s32 model38; /* model */
    SVECTOR rot3C; /* model rotation */
    SVECTOR pos44; /* model position */
    VECTOR pos4C; /* model position */
    VECTOR scale5C; /* model scale */
    SVECTOR aimRot6C; /* angles towards the model */
    Pool6CEntry *emit74;
} Fx0faWork; /* 0x78 */

typedef struct {
    s32 srcX0;
    s32 srcY4;
} Fx0faFlash;

extern Fx0faFlash fx_0fa_scrollSrc0[];
extern Fx0faFlash fx_0fa_scrollSrc1[];
extern Unk800F78A4 *fx_0fa_env;
extern BtlModelBuf fx_0fa_effModels[];

s32 fx_0fa_op80(s32 mode, Fx0faWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

#endif
