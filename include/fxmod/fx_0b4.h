#ifndef FX_0B4_H
#define FX_0B4_H

/* Effect module fx_0b4 (dir 13 file 0xB4). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0b4Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0b4Key *unk4;
} Fx0b4Path;

typedef struct {
    u16 unk0; /* start frame */
    u16 unk2; /* frames since the start */
} Fx0b4Timer;

typedef struct {
    Pool6CEntry *emit0[6];
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    Pool6CEntry *emit20;
    Pool6CEntry *emit24;
    Pool6CEntry *emit28;
    SVECTOR pos2C; /* caster body position */
    SVECTOR pos34; /* caster position */
} Fx0b4Work;

extern Fx0b4Timer D_801E80EC[6];
extern u16 D_801E8104[6]; /* particle files */
extern u16 D_801E8110[6]; /* path files */
extern SVECTOR D_801E811C;
extern s32 (*fx_0b4_op80ParticleMotions[6])(); /* motion callbacks */
extern Unk800F78A4 *fx_0b4_env;
extern SVECTOR D_801E818C; /* path origin */
extern SVECTOR D_801E8194[6];
extern Fx0b4Path D_801E81C4[6];
extern s32 D_801E81F4[6][0x360];
extern s32 D_801ED2F4[];

s32 fx_0b4_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 fx_0b4_op80ParticleMotion5(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n);
static s32 fxm_rotPosPathGet(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos);
typedef struct {
    s16 x, y, z;
} Fx0b4Point;
static void fxm_rotPosPathInit(Fx0b4Path *path, u16 *data);

#endif
