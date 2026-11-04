#ifndef FX_048_H
#define FX_048_H

/* Effect module fx_048 (dir 13 file 0x48): Firaga, all targets (and one large target). Fire / ice drawn as a grid
 * of texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

/* fx_048 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FIRAGA_COLUMN_SEG_3  0x2 /* 'so' model: fire-column segment (byte-identical to fx_007 entry 0x19) (look pending) */
#define FX_FIRAGA_FLAME_BURST_2 0x4 /* emitter: flame explosion at the pillar's start (same call site as fx_007 entry 0xe) */
#define FX_FIRAGA_FLAME_BURST   0x5 /* emitter: flame explosion (byte-identical to fx_007 entry 0xa) */
#define FX_FIRAGA_COLUMN_SEG_1  0x9 /* 'so' model: fire-column segment (byte-identical to fx_007 entry 0x17) (look pending) */
#define FX_FIRAGA_FLAME_PILLAR  0xA /* model: open cylinder with a flame texture (the pillar) (byte-identical to fx_007 entry 0x8) */
#define FX_FIRAGA_COLUMN_SEG_2  0xB /* 'so' model: fire-column segment (byte-identical to fx_007 entry 0x18) (look pending) */
#define FX_FIRAGA_COLUMN_SEG_0  0xC /* 'so' model: fire-column segment, drawn coiled 18 times (byte-identical to fx_007 entry 0x16) (look pending) */
#define FX_FIRAGA_FLAME_DISC    0xE /* model: flat octagon, flame texture; registered, never drawn (byte-identical to fx_007 entry 0x9) */
#define FX_FIRAGA_GLOW          0x10 /* rings: white-red gouraud glow disc at the target (same call site as fx_007 entry 0xb) */
#define FX_FIRAGA_ORBIT_TRAIL   0x11 /* emitter: orbiting trail + sprites at the start (texture not in the file) (byte-identical to fx_007 entry 0xc) (look pending) */
#define FX_FIRAGA_COLUMN_SPARK  0x12 /* rings: small red-white glow drawn at random fire-column segments (byte-identical to fx_007 entry 0xd) */
#define FX_FIRAGA_PILLAR_RING   0x13 /* rings: textured flame ring rising with the pillar (byte-identical to fx_007 entry 0xf) */
#define FX_FIRAGA_EMBERS        0x14 /* emitter: dark red ember sprites (byte-identical to fx_007 entry 0x12) */
#define FX_FIRAGA_GOLD_SPARKS_2 0x15 /* emitter: golden spark sprites + spark sub-particles (byte-identical to fx_007 entry 0x11) */
#define FX_FIRAGA_GOLD_SPARKS   0x16 /* emitter: golden spark sprites (byte-identical to fx_007 entry 0x10) */
#define FX_FIRAGA_SMOKE_BURST   0x17 /* emitter: dark teal-tinted burst (smoke after the blast) (byte-identical to fx_007 entry 0x13) */
#define FX_FIRAGA_CONE_GLOW     0x18 /* emitter: cone, one dark particle frame (same call site as fx_007 entry 0x15) (look pending) */
#define FX_FIRAGA_CONE_SPARKS   0x19 /* emitter: cone of spark sprites (texture not in the file) (byte-identical to fx_007 entry 0x14) (look pending) */

typedef struct {
    SVECTOR pos0; /* above the target centre */
    SVECTOR pos8; /* target centre on the ground */
    s32 state10; /* state */
    s32 model14[4]; /* models */
    s32 model24; /* model */
    s32 model28; /* model */
    Pool6CEntry *emit2C;
    Pool6CEntry *emit30;
    s32 frame34; /* frame counter */
} Fx048Work; /* 0x38 */

extern Unk800F78A4 *fx_048_env;
extern BtlModelBuf fx_048_effModels[];
extern s32 fx_048_trails[];
extern u16 fx_048_texels[];

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

/* propagated */

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
static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
