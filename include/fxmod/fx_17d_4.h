#ifndef FX_17D_4_H
#define FX_17D_4_H

/* Effect module fx_17d_4 (dir 13 file 0x17d): Ark, full summon (load group 4). Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_17d_4_env;
extern s16 fx_17d_4_particleFileIds[16];
extern SVECTOR fx_17d_4_particleOffsets[3];
extern s16 fx_17d_4_pointFileIds[3];

typedef struct {
    s16 x, y, z;
} Fx17d4Pt;

/* propagated */

typedef struct {
    u8 lastSlot0; /* ring: highest slot index (lastSlot0 + 1 slots) */
    u8 head1; /* slot of the newest point (btlfx_pushTrailPoint) */
    u8 count2; /* points pushed, capped at lastSlot0 */
    u8 pad3;
    SVECTOR points4[2];
} Fx17d4Line;

extern Fx17d4Line fx_17d_4_lineTrail;
static void fxm_drawTrailLine(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern s16 fx_17d_4_strandInfo[18 * 4]; /* per strand: file, start, stride, width */
extern Fx17d4Line *fx_17d_4_strandTrails[18];
void fx_17d_4_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d4Vec3;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    SVECTOR pos8; /* home position */
    SVECTOR pos10; /* particle position */
    u8 pad18[8];
    SVECTOR pos20; /* camera target */
    Fx17d4Vec3 pos28; /* model position */
    SVECTOR rot34; /* model rotation */
    Fx17d4Vec3 scale3C; /* model scale */
    Fx17d4Vec3 pos48; /* summon position */
    Fx17d4Vec3 scale54; /* summon scale */
    SVECTOR rot60; /* summon rotation */
    s32 model68; /* models */
    s32 model6C;
    s32 model70;
    s32 model74;
    s32 model78;
    u8 pad7C[8];
} Fx17d4Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x19C];
        BtlUnit9CC *motion19C;
        BtlUnit9CC *motion1A0;
    } *summon54; /* g_btlfxLoader.summon */
    u8 pad58[8];
    s32 model60; /* summon model */
} Fx17d4Summon;

#define SUMMON(env) ((Fx17d4Summon *)(env)->loader)

extern SVECTOR fx_17d_4_centerPos;
extern s32 fx_17d_4_op80Trails[];
extern s8 fx_17d_4_unused; /* no reader in any fx_17d group, other module or ovl_0b */
extern BtlModel fx_17d_4_effModels[];
void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);
void fxm_drawOffsetParticles3(Unk80100C78Entry *entry, SVECTOR *from, s32 ot);

#endif
