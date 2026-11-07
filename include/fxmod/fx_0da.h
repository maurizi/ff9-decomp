#ifndef FX_0DA_H
#define FX_0DA_H

/* Effect module fx_0da (dir 13 file 0xDA): Sword Magic Blizzard. Evidence: [data] the party ability / item tables
 * and enemy sequences that play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime
 * pass"). */
#include "btlfx_mod.h"

/* fx_0da effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s3.md; "(look pending)" = by kind, appearance not settled). */
#define FX_SWDMAG_BLIZZARD_ICE_FLARES     0x1 /* emitter: cyan ice glint / light-streak frames (byte-identical to fx_006 entry 0x1) */
#define FX_SWDMAG_BLIZZARD_ICE_REFLECTION 0x2 /* model: ice shell drawn with the scrolling reflection map (byte-identical to fx_006 entry 0x2) */
#define FX_SWDMAG_BLIZZARD_ICE_BLOCK      0x3 /* model: ice block (cracked-ice texture), morph start (byte-identical to fx_006 entry 0x3) */
#define FX_SWDMAG_BLIZZARD_ICE_CRACKING   0x4 /* model: the block breaking into pieces, morph middle (byte-identical to fx_006 entry 0x4) */
#define FX_SWDMAG_BLIZZARD_ICE_SHARDS     0x5 /* model: scattered ice shards, morph end (byte-identical to fx_006 entry 0x5) */
#define FX_SWDMAG_BLIZZARD_ICE_FLARES_2   0x6 /* emitter: cyan ice glints (byte-identical to fx_006 entry 0x6) */
#define FX_SWDMAG_BLIZZARD_FROST_RING     0xD /* emitter: lilac-blue gouraud rings (byte-identical to fx_006 entry 0xD) */
#define FX_SWDMAG_BLIZZARD_FREEZE_BURST   0xE /* emitter: white-pink rings + spark sprites as the ice forms (byte-identical to fx_006 entry 0xE) */
#define FX_SWDMAG_BLIZZARD_SHATTER_WAVE   0x10 /* emitter: blue ripple wave when the ice shatters (byte-identical to fx_006 entry 0x10) */
#define FX_SWDMAG_BLIZZARD_SWORD_FLASH    0x13 /* rings: white 8-segment rings flashed rotating at the sword (op84) */
#define FX_SWDMAG_BLIZZARD_SWORD_GLINT    0x14 /* rings: white 6-segment rings flashed at the sword (op84, first draw) */

/* fx_0da choreography (docs/fxdata.md, "Choreography literals"; as fx_006 Blizzard): op80's states, their lengths
 * and keyframes, the VRAM rect of the reflection map, the model textures; op84's sword flash. */
#define FX_SWDMAG_BLIZZARD_STATE_GROW    1 /* op80 (starts here): FX_SWDMAG_BLIZZARD_ICE_BLOCK grows to full size */
#define FX_SWDMAG_BLIZZARD_STATE_GLEAM   2 /* the block turns, FX_SWDMAG_BLIZZARD_ICE_REFLECTION gleams over it */
#define FX_SWDMAG_BLIZZARD_STATE_SHATTER 3 /* the block morphs to ICE_CRACKING, then ICE_SHARDS */
#define FX_SWDMAG_BLIZZARD_GROW_FRAMES       4    /* STATE_GROW's length */
#define FX_SWDMAG_BLIZZARD_GLEAM_FRAMES      0x10 /* STATE_GLEAM's length; at its end the freeze burst, frost ring and flares start */
#define FX_SWDMAG_BLIZZARD_SHATTER_FRAMES    0xC  /* STATE_SHATTER's length (the op ends) */
#define FX_SWDMAG_BLIZZARD_SHATTER_WAVE_FRAME 1   /* STATE_SHATTER: FX_SWDMAG_BLIZZARD_SHATTER_WAVE starts */
#define FX_SWDMAG_BLIZZARD_CRACK_FRAMES      2    /* STATE_SHATTER: ICE_BLOCK -> ICE_CRACKING morph frames, then */
#define FX_SWDMAG_BLIZZARD_SHARDS_FRAMES     14   /* ICE_CRACKING -> ICE_SHARDS over this many (cut off by SHATTER_FRAMES) */
#define FX_SWDMAG_BLIZZARD_ICE_SCALE         0xC00 /* FX_SWDMAG_BLIZZARD_ICE_BLOCK's full scale (0x1000 = 1) */
#define FX_SWDMAG_BLIZZARD_MODEL_COUNT       4    /* btlfx_initEffModels slots: ICE_BLOCK, ICE_CRACKING, ICE_SHARDS, ICE_REFLECTION */
#define FX_SWDMAG_BLIZZARD_FREEZE_TINT_RGB   0, 0x20, 0x46 /* additive screen tint (blue) as FX_SWDMAG_BLIZZARD_FREEZE_BURST starts */
#define FX_SWDMAG_BLIZZARD_ICE_BLOCK_TPAGE 0xBA /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 1 */
#define FX_SWDMAG_BLIZZARD_ICE_BLOCK_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARD_ICE_BLOCK_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARD_ICE_BLOCK_V     0x0
#define FX_SWDMAG_BLIZZARD_ICE_CRACKING_TPAGE 0xBA /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 1 */
#define FX_SWDMAG_BLIZZARD_ICE_CRACKING_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARD_ICE_CRACKING_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARD_ICE_CRACKING_V     0x0
#define FX_SWDMAG_BLIZZARD_ICE_SHARDS_TPAGE 0xBA /* Hi_RegisterTexEffModel texture: 8-bit page (0x280, 0x100), abr 1 */
#define FX_SWDMAG_BLIZZARD_ICE_SHARDS_CLUT  0x3DC0 /* CLUT at (0x0, 0xF7) */
#define FX_SWDMAG_BLIZZARD_ICE_SHARDS_U     0x0 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARD_ICE_SHARDS_V     0x0
#define FX_SWDMAG_BLIZZARD_ICE_REFLECTION_TPAGE 0xB8 /* Hi_RegisterTexEffModel texture: 8-bit page (0x200, 0x100), abr 1, the reflection rect below */
#define FX_SWDMAG_BLIZZARD_ICE_REFLECTION_CLUT  0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_SWDMAG_BLIZZARD_ICE_REFLECTION_U     0x80 /* texture origin (u, v) in the page */
#define FX_SWDMAG_BLIZZARD_ICE_REFLECTION_V     0x80
/* FX_SWDMAG_BLIZZARD_ICE_REFLECTION samples (ICE_REFLECTION_TPAGE / _U / _V) the rect the file fills with a "dummy"
 * placeholder; each GLEAM frame btlfx_drawScrollMove copies the reflection texture from (SRC_X, SRC_Y) into it,
 * scrolled vertically. */
#define FX_SWDMAG_BLIZZARD_REFLECTION_VRAM_X 0x240
#define FX_SWDMAG_BLIZZARD_REFLECTION_VRAM_Y 0x180
#define FX_SWDMAG_BLIZZARD_REFLECTION_VRAM_W 0x40
#define FX_SWDMAG_BLIZZARD_REFLECTION_VRAM_H 0x80
#define FX_SWDMAG_BLIZZARD_REFLECTION_SRC_X  0x240
#define FX_SWDMAG_BLIZZARD_REFLECTION_SRC_Y  0x100
#define FX_SWDMAG_BLIZZARD_SWORD_STATE_APPEAR  0 /* op84: SWORD_GLINT / SWORD_FLASH grow in at the weapon */
#define FX_SWDMAG_BLIZZARD_SWORD_STATE_FLICKER 1 /* op84: they flicker and turn */
#define FX_SWDMAG_BLIZZARD_SWORD_STATE_FADE    2 /* op84: they spread and fade (the op ends) */
#define FX_SWDMAG_BLIZZARD_SWORD_APPEAR_FRAMES  3 /* SWORD_STATE_APPEAR's length */
#define FX_SWDMAG_BLIZZARD_SWORD_FLICKER_FRAMES 9 /* SWORD_STATE_FLICKER's length */
#define FX_SWDMAG_BLIZZARD_SWORD_FADE_FRAMES    9 /* SWORD_STATE_FADE's length */

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0daWork84;

extern Unk800F78A4 *fx_0da_env;
extern BtlModelBuf fx_0da_effModels[];

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* caster body position */
    SVECTOR pos1C; /* caster position */
    s32 frame24; /* frame counter */
} Fx0daWork80; /* 0x28 */

#endif
