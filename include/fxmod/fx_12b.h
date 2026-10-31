#ifndef FX_12B_H
#define FX_12B_H

/* Effect module fx_12b (dir 13 file 0x12B). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR unk14; /* target body position */
    SVECTOR unk1C; /* target position */
} Fx12bWork82;

/* particle as the path callbacks see it */
typedef struct {
    VECTOR unk0; /* position */
    u8 pad10[0x1F - 0x10];
    u8 unk1F; /* index (1-based) */
} Fx12bParticle;

typedef struct {
    SVECTOR unk0; /* end */
    SVECTOR unk8; /* start */
    s16 unk10; /* side sway */
    s16 unk12; /* arc height */
    s16 unk14;
    u8 pad16[0x18 - 0x16];
    u16 unk18; /* start frame */
    u16 unk1A; /* bone */
    u16 unk1C;
    u8 pad1E[0x20 - 0x1E];
    u16 unk20; /* state */
    u16 unk22;
} Fx12bShard; /* 0x24 */

typedef struct {
    s16 x, y, z;
} Fx12bPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* key count */
    Fx12bPoint *unk4; /* keys */
} Fx12bPath;

extern Fx12bShard D_801E97BC[];
extern Fx12bShard D_801E9894[];
extern Fx12bShard D_801E996C[];
extern Fx12bShard D_801E9A20[];
extern Unk800F78A4 *fx_12b_env;
extern s32 D_801EC9D4[];
extern s32 D_801EE0E4[];
extern SVECTOR D_801EF854;
extern SVECTOR D_801EF85C;
extern SVECTOR D_801EF864;

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR unk10; /* target body position */
} Fx12bWork81; /* 0x18 */
extern Fx12bPath D_801E9B44[];
extern Fx12bPath D_801E9B74[];
extern Fx12bPath D_801E9BA4;
extern Fx12bPath D_801E9BAC;
extern SVECTOR D_801EF7F4[];
extern SVECTOR D_801EF824[];

static s32 fx_12b_op80ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion0(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion1(Fx12bParticle *p, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E7994(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n);
s32 func_801E7B10(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n);
s32 func_801E7CA8(Fx12bParticle *p, SVECTOR *pos, Fx12bShard *s, s32 frame, s32 n);
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
    SVECTOR unk10; /* target body position */
} Fx12bWork80; /* 0x18 */

extern u16 D_801E9AD4[6];
extern u16 D_801E9AE0[6];
extern u8 D_801E9BB4[];
extern u8 D_801EB2C4[];

#endif
