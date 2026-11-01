#ifndef FX_17D_6_H
#define FX_17D_6_H

/* Effect module fx_17d_6 (dir 13 file 0x17d): Ark, full summon (load group 6). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[1];
} Fx17d6Trail;

extern Unk800F78A4 *fx_17d_6_env;

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[17];
} Fx17d6Trail17; /* 0x8C */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[5];
} Fx17d6Trail5; /* 0x2C */

typedef struct {
    SVECTOR to0; /* to */
    SVECTOR from8; /* from */
    u8 slot10; /* trail index */
    u8 kind11;
    u8 index12;
    u8 unk13; /* write-only (rand % 15) */
    u8 angle14; /* angle */
    u8 width15;
    u8 pad16[2];
    s32 *noise18; /* noise */
} Fx17d6Bolt;

extern Fx17d6Trail17 fx_17d_6_boltTrails[];
extern Fx17d6Trail5 fx_17d_6_branchTrails[];

typedef struct {
    s32 frame0; /* frame */
    u8 pad4[0xC];
    s16 *pool10; /* pool */
} Fx17d6Inst;

typedef struct {
    s32 *noise0; /* noise */
    s32 count4; /* spawn count */
    s32 kind8; /* always 0: the file 0x17D script runs group 6's op 0x83 only as `83 00 00` */
} Fx17d6Work83;

extern SVECTOR fx_17d_6_boltTo; /* to */
extern SVECTOR fx_17d_6_boltFrom; /* from */
extern s32 fx_17d_6_boltState; /* state */
extern s32 fx_17d_6_noise[64][3]; /* noise */

static s32 fx_17d_6_op83PoolTask(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d6Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* bolt target */
    SVECTOR pos10; /* particle position */
    SVECTOR pos18; /* camera position */
    SVECTOR pos20; /* camera target */
    SVECTOR pos28; /* bone positions */
    SVECTOR pos30;
    Fx17d6Vec3 pos38; /* model position */
    SVECTOR rot44; /* model rotation */
    Fx17d6Vec3 scale4C; /* model scale */
    Fx17d6Vec3 pos58; /* summon position */
    Fx17d6Vec3 scale64; /* summon scale */
    SVECTOR rot70; /* summon rotation */
    Pool6CEntry *emit78;
    s32 model7C; /* models */
    s32 model80;
} Fx17d6Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A8];
        BtlMotion *motion1A8;
        BtlMotion *motion1AC;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d6Summon;

extern SVECTOR fx_17d_6_basePos;
extern u8 fx_17d_6_bones[];
extern BtlModelBuf fx_17d_6_effModels[];
extern s32 fx_17d_6_op80Trails[];
extern SVECTOR fx_17d_6_boltTo;
extern SVECTOR fx_17d_6_boltFrom;

#define SUMMON(env) ((Fx17d6Summon *)(env)->loader)

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
