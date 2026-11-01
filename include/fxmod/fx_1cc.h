#ifndef FX_1CC_H
#define FX_1CC_H

/* Effect module fx_1cc (dir 13 file 0x1cc): Black Waltz 1's "Sealion appears". Fire / ice drawn as a grid of texel
 * quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that play this
 * file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_1cc_env;
extern s16 fx_1cc_pieceDists[];

typedef struct Fx1ccObj {
    s16 state0; /* state; flying pieces: piece index (< 6 picks the scale) */
    s16 kind2; /* 0: main object, 1: piece */
    s32 index4; /* direction index */
    s32 frame8; /* frame */
    s32 posXC; /* position, 28.4 (low halves used by the main object) */
    s32 posY10;
    s32 posZ14;
    s16 velX18; /* velocity (main object: SVECTOR position, the emitter origin) */
    s16 velY1A;
    s16 velZ1C;
    s16 unk1E; /* unused (pad of the main object's position) */
    SVECTOR rot20; /* rotation */
    s16 spin28; /* rotation speed */
    s16 spin2A;
    s32 model2C; /* model */
    s32 model30; /* model */
    Pool6CEntry *emit34; /* particle */
} Fx1ccObj;

typedef struct {
    u8 pad0[0x20];
    s32 model20[14]; /* models */
    u8 pad58[0x78 - 0x58];
    s16 flag78; /* burst */
} Fx1ccInst;

extern SVECTOR fx_1cc_dirRots[];
void fx_1cc_piecePos(s16 *from, s16 *to, s32 i, s16 *out);

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 model8; /* models */
    s32 modelC;
    s32 model10;
    s32 model14;
    s32 model18;
    s32 model1C;
    s32 model20[14]; /* piece models */
    SVECTOR pos58; /* position */
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR unk70; /* unused: only the -= 0x80 in mode 1 reads it */
    s16 flag78;
    s16 index7A; /* spawned directions */
    s32 frame7C;
    s32 scroll80;
} Fx1ccWork80; /* 0x84 */

extern BtlModelBuf fx_1cc_effModels[];
extern s32 fx_1cc_pieceFileIds[14];
static s32 fx_1cc_op80PoolTask(s32 mode, Fx1ccObj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx1ccInst *inst);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

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
static void fxm_drawBulgeGrid14(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr);

#endif
