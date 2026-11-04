#ifndef FX_03E_H
#define FX_03E_H

/* Effect module fx_03e (dir 13 file 0x3E). */
#include "btlfx_mod.h"

/* fx_03e effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZARD_ICE_CRACKING   0x1 /* model: the block breaking into pieces, morph middle (same call site as fx_006 entry 0x4) */
#define FX_BLIZZARD_ICE_REFLECTION 0x2 /* model: ice shell drawn with the scrolling reflection map (byte-identical to fx_006 entry 0x2) */
#define FX_BLIZZARD_ICE_BLOCK      0x3 /* model: ice block (cracked-ice texture), morph start (byte-identical to fx_006 entry 0x3) */
#define FX_BLIZZARD_FROST_SWIRL    0x7 /* emitter: orbiting pale trail, cyan ice flecks and white dots (same call site as fx_006 entry 0x12) */
#define FX_BLIZZARD_SHATTER_WAVE   0x8 /* emitter: blue ripple wave with ice sparkles */
#define FX_BLIZZARD_FROST_RING     0xA /* emitter: lilac-blue gouraud rings */
#define FX_BLIZZARD_ICE_FLARES_2   0x15 /* emitter: cyan ice glints */
#define FX_BLIZZARD_ICE_FLARES     0x16 /* emitter: cyan ice glint / light-streak frames */
#define FX_BLIZZARD_FREEZE_BURST   0x18 /* emitter: white-pink rings + spark sprites as the ice forms (same call site as fx_006 entry 0xe) */
#define FX_BLIZZARD_ICE_SHARDS     0x19 /* model: scattered ice shards, morph end (same call site as fx_006 entry 0x5) */

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
    s32 frame24; /* frame */
} Fx03eWork; /* 0x28 */

extern Unk800F78A4 *fx_03e_env;
extern BtlModelBuf fx_03e_effModels[];
extern s32 fx_03e_trails[];

#endif
