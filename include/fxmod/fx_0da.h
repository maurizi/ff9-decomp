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
} Fx0daWork80;

#endif
