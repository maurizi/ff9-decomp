#ifndef FX_12B_H
#define FX_12B_H

/* Effect module fx_12b (dir 13 file 0x12B): enemy Photon (Ark). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
} Fx12bWork82;

/* particle as the path callbacks see it */
typedef struct {
    VECTOR pos0; /* position */
    u8 pad10[0x1F - 0x10];
    u8 index1F; /* index (1-based) */
} Fx12bParticle;

typedef struct {
    SVECTOR pos0; /* end */
    SVECTOR pos8; /* start */
    s16 width10; /* side sway */
    s16 height12; /* arc height */
    s16 unk14;
    u8 pad16[0x18 - 0x16];
    u16 frame18; /* start frame */
    u16 bone1A; /* bone */
    u16 dist1C;
    u8 pad1E[0x20 - 0x1E];
    u16 frame20; /* state */
    u16 unk22;
} Fx12bShard; /* 0x24 */

typedef struct {
    s16 x, y, z;
} Fx12bPoint;

typedef struct {
    s16 unk0;
    u16 count2; /* key count */
    Fx12bPoint *keys4; /* keys */
} Fx12bPath;

extern Fx12bShard fx_12b_op80Shards0[];
extern Fx12bShard fx_12b_op80Shards1[];
extern Fx12bShard fx_12b_op81Shards0[];
extern Fx12bShard fx_12b_op81Shards1[];
extern Unk800F78A4 *fx_12b_env;
extern s32 fx_12b_op81Trails0[];
extern s32 fx_12b_op81Trails1[];
extern SVECTOR fx_12b_emitPos0;
extern SVECTOR fx_12b_emitPos1;
extern SVECTOR fx_12b_emitPos2;

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target body position */
} Fx12bWork81; /* 0x18 */
extern Fx12bPath fx_12b_op80PosPaths0[];
extern Fx12bPath fx_12b_op80PosPaths1[];
extern Fx12bPath fx_12b_op81PosPath0;
extern Fx12bPath fx_12b_op81PosPath1;
extern SVECTOR fx_12b_bonePoints0[];
extern SVECTOR fx_12b_bonePoints1[];

static s32 fx_12b_op80ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_12b_pathParticle(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n);
s32 fx_12b_arcParticle0(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n);
s32 fx_12b_arcParticle1(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion2(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion3(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion2(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion3(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_posPathGet(Fx12bPath *path, s32 i, SVECTOR *out);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

static void fxm_posPathInit(Fx12bPath *path, u16 *data);
s32 fx_12b_op81(s32 mode, Fx12bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v);

/* propagated */
static s32 fxm_pinParticle(Fx12bParticle *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target body position */
} Fx12bWork80; /* 0x18 */

extern u16 fx_12b_pathFileIds0[6];
extern u16 fx_12b_pathFileIds1[6];
extern u8 fx_12b_op80Trails0[];
extern u8 fx_12b_op80Trails1[];

#endif
