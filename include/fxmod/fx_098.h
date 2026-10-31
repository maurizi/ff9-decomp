#ifndef FX_098_H
#define FX_098_H

/* Effect module fx_098 (dir 13 file 0x98): Blizzaga, all targets (and one large target). Fire / ice drawn as a grid
 * of texel quads (fxm_drawTexelQuads). Evidence: [data] the party ability / item tables and enemy sequences that
 * play this file id (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

extern s16 fx_098_pieceDist[];
extern Unk800F78A4 *fx_098_env;

void fx_098_piecePos(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx098Vec3;

/* one shard, a pool item */
typedef struct {
    s16 state0; /* state */
    s16 flying2; /* flying */
    s32 index4; /* index */
    s32 frame8; /* frame */
    Fx098Vec3 posC; /* position << 4 */
    SVECTOR pos18; /* velocity */
    SVECTOR rot20; /* rotation */
    s16 spinX28; /* rotation speed */
    s16 spinY2A;
    s32 model2C; /* models */
    s32 model30;
    Pool6CEntry *emit34;
} Fx098Item; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 models20[22]; /* models */
    s16 shatter78;
} Fx098Work;

extern SVECTOR fx_098_shardRot[];
static s32 fx_098_op80PoolTask(s32 mode, Fx098Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx098Work *work);

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
    SVECTOR unk70;
    s16 shatter78;
    s16 shardCount7A; /* spawned directions */
    s32 scrollStep7C;
    s32 scroll80;
    Pool6CEntry *emit84; /* particle */
} Fx098Work80; /* 0x88 */

extern BtlModel fx_098_effModels[];
extern s32 fx_098_pieceBlocks[14];
s32 fx_098_op80(s32 mode, Fx098Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

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
