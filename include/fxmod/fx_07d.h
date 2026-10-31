#ifndef FX_07D_H
#define FX_07D_H

/* Effect module fx_07d (dir 13 file 0x7D). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    s32 unk10; /* buffer index */
    s32 *unk14; /* per-point (t offset, amplitude, step) */
} Fx07dArc;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx07dStrip; /* 0x7C */

extern Unk800F78A4 *fx_07d_env;
extern SVECTOR D_801EA470[24];
extern s32 D_801EA530;
extern Unk80100C78Entry *fx_07d_task;
extern s32 D_801EA538;
extern Fx07dStrip D_801EA540[];
extern s32 D_801EA8B0[64][3]; /* random arc profile */

typedef struct {
    s16 unk0;
    s16 unk2; /* scale */
    SVECTOR unk4; /* position */
    SVECTOR rotC; /* glow rotation */
    s16 unk14; /* spin angle */
    s16 unk16; /* spin speed */
} Fx07dOrb;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* ordering-table offset */
    SVECTOR posC; /* caster body position */
    s32 unk14; /* caster body position, words */
    s32 unk18;
    s32 unk1C;
    s32 model20; /* glow model */
    s32 model24; /* model */
    s32 model28; /* model */
    s32 model2C; /* model */
    s32 pad30;
    s32 unk34; /* backdrop scroll */
    s32 unk38; /* yaw offset */
    Pool6CEntry *emit3C;
} Fx07dWork80; /* 0x40 */

extern BtlModel fx_07d_effModels[];

typedef struct {
    SVECTOR pos0; /* caster body position */
    s32 *unk8; /* arc profile table */
    s32 unkC; /* arcs spawned */
    s32 unk10; /* variant */
} Fx07dWork83; /* 0x14 */

static void fx_07d_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame);
static void fxm_twistVertex(s32 d, s32 a1, s32 a, s32 a3, s16 *out);
static void fxm_anglesFromEye(s16 *a, s16 *b);
void func_801E9834(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad);
static s32 fx_07d_op83PoolTask(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
static s32 fx_07d_op80PoolTask(s32 mode, Fx07dOrb *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx07dWork80 *work);
static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *));

#endif
