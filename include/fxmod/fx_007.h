#ifndef FX_007_H
#define FX_007_H

/* Effect module fx_007 (dir 13 file 0x07): Firaga, one target (party and enemy). Fire / ice drawn as a grid of
 * texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play
 * this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_007 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRAGA_FLAME_PILLAR  0x8 /* model: open cylinder with a flame texture (the pillar) */
#define FX_FIRAGA_FLAME_DISC    0x9 /* model: flat octagon, flame texture; registered, never drawn */
#define FX_FIRAGA_FLAME_BURST   0xA /* emitter: flame explosion */
#define FX_FIRAGA_GLOW          0xB /* rings: white-red gouraud glow disc at the target */
#define FX_FIRAGA_EMBER_TRAIL   0xC /* emitter: orbiting red glow trail + red ember dots */
#define FX_FIRAGA_COLUMN_SPARK  0xD /* rings: small red-white glow drawn at random fire-column segments */
#define FX_FIRAGA_FLAME_BURST_2 0xE /* emitter: flame explosion at the pillar's start */
#define FX_FIRAGA_PILLAR_RING   0xF /* rings: textured flame ring rising with the pillar */
#define FX_FIRAGA_GOLD_SPARKS   0x10 /* emitter: golden spark sprites */
#define FX_FIRAGA_GOLD_SPARKS_2 0x11 /* emitter: golden spark sprites + spark sub-particles */
#define FX_FIRAGA_EMBERS        0x12 /* emitter: dark red ember sprites */
#define FX_FIRAGA_SMOKE_BURST   0x13 /* emitter: dark teal-tinted burst (smoke after the blast) */
#define FX_FIRAGA_EMBER_CONE    0x14 /* emitter: cone of red and grey ember dots */
#define FX_FIRAGA_RED_HAZE      0x15 /* emitter: soft dark-red additive blob, the red heat haze filling the fire column; runtime (docs/runtime-findings.md effect-data pass, medium) */
#define FX_FIRAGA_FLAME_RIBBON_0 0x16 /* 'so' model: flat flame-textured ribbon piece, drawn coiled 18 times as the fire column */
#define FX_FIRAGA_FLAME_RIBBON_1 0x17 /* 'so' model: flat flame-textured ribbon piece */
#define FX_FIRAGA_FLAME_RIBBON_2 0x18 /* 'so' model: flat flame-textured ribbon piece */
#define FX_FIRAGA_FLAME_RIBBON_3 0x19 /* 'so' model: flat flame-textured ribbon piece */

/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")

/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))

extern Unk800F78A4 *fx_007_env;

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

typedef struct {
    SVECTOR pos0; /* target body position */
    SVECTOR pos8; /* target position */
    s32 state10; /* state */
    s32 model14[4]; /* models */
    s32 model24;
    s32 model28;
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 frame34; /* frame */
} Fx007Work80; /* 0x38 */

extern BtlModelBuf fx_007_effModels[];
extern u8 fx_007_trails[];
extern u16 fx_007_texels[];

#endif
