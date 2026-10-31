#ifndef FX_123_H
#define FX_123_H

/* Effect module fx_123 (dir 13 file 0x123): enemy Raining Swords (Maliris). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR pos14; /* centre of the units */
} Fx123Work82;

typedef struct {
    s16 timer0; /* fade-in countdown */
    s16 timer2; /* fade-out countdown */
} Fx123Work83;

typedef struct {
    Pool6CEntry *emit0[6];
    Pool6CEntry *emit18[6];
    Pool6CEntry *emit30[6];
    Pool6CEntry *emit48;
    SVECTOR pos4C; /* caster position */
    SVECTOR pos54;
    SVECTOR pos5C[6]; /* bone positions */
} Fx123Work80; /* 0x8C */

typedef struct {
    u16 bone0; /* bone */
    u16 dist2; /* offset */
} Fx123Bone;

extern Fx123Bone fx_123_op80Bones[6];
extern Unk800F78A4 *fx_123_env;
extern SVECTOR fx_123_emitPos0[6];
extern SVECTOR fx_123_emitPos1[6];

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx123Point;

typedef struct {
    s16 unk0;
    u16 count2; /* count */
    Fx123Point *keys4;
} Fx123Path;
static void fxm_rotPosPathInit(Fx123Path *path, u16 *data);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

/* propagated */
extern SVECTOR fx_123_pinPos0[]; /* particle positions */
static s32 fxm_pinParticle0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
extern SVECTOR fx_123_pinPos1[]; /* particle positions */
static s32 fxm_pinParticle5(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle6(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle7(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle9(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle10(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fxm_pinParticle11(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx123Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 count2; /* keys */
    Fx123Key *keys4;
} Fx123Path_0b4;
static s32 fxm_rotPosPathGet(Fx123Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

/* PsyQ libgte.h macros: candidates for psyq.h */
#define copyVector(d, s) (d)->vx = (s)->vx, (d)->vy = (s)->vy, (d)->vz = (s)->vz
#define addVector(d, s) (d)->vx += (s)->vx, (d)->vy += (s)->vy, (d)->vz += (s)->vz

typedef struct {
    Pool6CEntry *emit0[6];
    u8 pad18[0x30 - 0x18];
    Pool6CEntry *emit30[6];
    Pool6CEntry *emit48[6];
    Pool6CEntry *emit60;
    SVECTOR pos64;
    SVECTOR rot6C[6];
    VECTOR pos9C[6];
    VECTOR scaleFC;
    s32 fade10C;
    SVECTOR pos110[6];
    SVECTOR pos140;
    SVECTOR pos148[6];
    s16 unk178;
    u16 model17A[6];
    u16 model186;
} Fx123Work81; /* 0x188 */

typedef struct {
    SVECTOR pos0;
    SVECTOR rot8;
    SVECTOR pos10;
    u16 active18;
    u16 frame1A;
    s16 unk1C;
    s16 timer1E;
    s16 angle20;
    s16 unk22;
} Fx123Shot; /* 0x24 */

extern Fx123Shot fx_123_op81Shots[6];
extern u16 fx_123_pathFileIds[6];
extern u16 fx_123_emitFileIds[6];
extern s32 fx_123_pinParticles[6];
extern BtlModelBuf fx_123_effModels[];
extern Fx123Path_0b4 fx_123_rotPosPath[6];
extern u8 fx_123_op81Trails[6][0xE0];
extern SVECTOR fx_123_emitPos2[6];
extern SVECTOR fx_123_emitPos3[6];

#endif
