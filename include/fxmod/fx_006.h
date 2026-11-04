#ifndef FX_006_H
#define FX_006_H

/* Effect module fx_006 (dir 13 file 0x06). */
#include "btlfx_mod.h"

/* fx_006 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_BLIZZARD_ICE_FLARES     0x1 /* emitter: cyan ice glint / light-streak frames */
#define FX_BLIZZARD_ICE_REFLECTION 0x2 /* model: ice shell drawn with the scrolling reflection map */
#define FX_BLIZZARD_ICE_BLOCK      0x3 /* model: ice block (cracked-ice texture), morph start */
#define FX_BLIZZARD_ICE_CRACKING   0x4 /* model: the block breaking into pieces, morph middle */
#define FX_BLIZZARD_ICE_SHARDS     0x5 /* model: scattered ice shards, morph end */
#define FX_BLIZZARD_ICE_FLARES_2   0x6 /* emitter: cyan ice glints */
#define FX_BLIZZARD_FROST_RING     0xD /* emitter: lilac-blue gouraud rings */
#define FX_BLIZZARD_FREEZE_BURST   0xE /* emitter: white-pink rings + spark sprites as the ice forms */
#define FX_BLIZZARD_SHATTER_WAVE   0x10 /* emitter: blue ripple wave when the ice shatters */
#define FX_BLIZZARD_FROST_SWIRL    0x12 /* emitter: orbiting pale trail, cyan ice flecks and white dots */

typedef struct {
    s32 state0; /* state */
    s32 model4; /* models */
    s32 model8;
    s32 modelC;
    s32 model10;
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
    s32 frame24; /* frame */
} Fx006Work; /* 0x28 */

extern Unk800F78A4 *fx_006_env;
extern BtlModelBuf fx_006_effModels[];
extern s32 fx_006_trails[];

#endif
