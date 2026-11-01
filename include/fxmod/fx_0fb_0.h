#ifndef FX_0FB_0_H
#define FX_0FB_0_H

/* Effect module fx_0fb_0 (dir 13 file 0x0FB): Madeen, full summon, and Terra Homing (load group 0). Ribbon trails
 * (fxm_drawRibbonTrail). Evidence: btlfx_start and btlfx_loadFile get the same id for summons
 * (docs/runtime-findings.md). [data] the party ability / item tables and enemy sequences that play this file id
 * (tools/effect_labels.py; docs/runtime-findings.md, "Round 31 runtime pass"). */
#include "btlfx_mod.h"

typedef struct {
    s32 state0; /* state */
    SVECTOR homePos4[8]; /* unit home positions */
    SVECTOR rot44[8]; /* unit rotations */
    SVECTOR spin84[8]; /* spin speeds */
    SVECTOR curRotC4[8]; /* current rotations */
    s32 unitCount104; /* unit count */
    s32 lift108; /* lift height */
} Fx0fb0Work81; /* 0x10C */

typedef struct {
    s32 state0; /* state */
    s16 kind4; /* kind */
    s16 bob6; /* bob height, then phase */
    s32 frame8; /* frame */
    SVECTOR posC; /* position */
    SVECTOR rot14; /* rotation */
    u16 spinX1C; /* spin speed */
    u16 spinY1E;
    s32 model20; /* model */
} Fx0fb0Rock;

extern s32 fx_0fb_0_rocksReleased;
extern s32 fx_0fb_0_unitsRestored;
extern Unk800F78A4 *fx_0fb_0_env;

static s32 fx_0fb_0_op80PoolTask(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env);
s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

typedef struct {
    SVECTOR a0;
    SVECTOR b8;
} Fx0fb0Seg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0fb0Seg *buf, s32 abr);

typedef struct {
    s32 vx, vy, vz;
} Fx0fbVec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 rockCount8; /* rocks spawned */
    SVECTOR basePosC; /* base position */
    SVECTOR pos14;
    SVECTOR pos1C;
    SVECTOR pos24;
    Fx0fbVec3 summonPos2C; /* model position */
    Fx0fbVec3 summonScale38; /* model scale */
    SVECTOR rot44; /* model rotation */
    SVECTOR pos4C;
    SVECTOR pos54;
    SVECTOR pos5C; /* summon position */
    SVECTOR camPos64;
    s32 model6C[27]; /* rock models */
    Fx0fbVec3 posD8;
    Fx0fbVec3 scaleE4;
    SVECTOR rotF0;
    Fx0fbVec3 posF8;
    Fx0fbVec3 scale104;
    SVECTOR rot110;
    s32 model118; /* models */
    s32 model11C;
    s32 model120;
    s32 model124;
} Fx0fb0Work80; /* 0x128 */

typedef struct {
    u8 data[0x84];
} Fx0fb0Trail16; /* 4 + 16 points */

typedef struct {
    Fx0fb0Seg seg[12];
} Fx0fb0SegBuf; /* 0xC0 */

typedef struct {
    s16 x;
    s16 z;
} Fx0fb0Ofs;

typedef struct {
    u8 pad0[0x38];
    BtlModelBuf *effModels38;
    u8 pad3C[0x18];
    struct {
        u8 pad0[0x180];
        BtlMotion *motion180[8]; /* summon motions */
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx0fb0Summon;

#define FX0FB_W(v, i) (((s32 *)&(v))[i])
#define SUMMON0FB(env) ((Fx0fb0Summon *)(env)->loader)

void fx_0fb_0_drawWaveSheet(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr);
void fx_0fb_0_drawWaveCurtain(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2);
extern SVECTOR fx_0fb_0_origin;
extern s16 fx_0fb_0_ribbonBones[4][2];
extern s16 fx_0fb_0_trailBones[10];
extern u16 fx_0fb_0_rockBlocks[27];
extern s16 fx_0fb_0_rockOrder[25];
extern s16 fx_0fb_0_rockOffsets[];
extern CVECTOR fx_0fb_0_rippleRgb[];
extern CVECTOR fx_0fb_0_ribbonRgb[];
extern SVECTOR fx_0fb_0_bonePts[10];
extern SVECTOR fx_0fb_0_bonePts1[];
extern Fx0fb0SegBuf fx_0fb_0_ribbons[4];
extern Fx0fb0Trail16 fx_0fb_0_strips[10];

#endif
