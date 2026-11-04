#ifndef FX_1B4_H
#define FX_1B4_H

/* Effect module fx_1b4 (dir 13 file 0x1b4): enemy Flare Star (Kuja/Trance Kuja, ...). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b4 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_FLARE_STAR_PINK_SPARKS    0xA /* emitter: pink / orange glyph-like spark sprites + pink and lilac gouraud rings */
#define FX_FLARE_STAR_FAN_EMITTER    0xB /* emitter: two fan elements, nearly invisible in the render (look pending) */
#define FX_FLARE_STAR_FLAME_TRAIL    0xC /* emitter: white fan bars + screen trail with a red flame strip */
#define FX_FLARE_STAR_YELLOW_RINGS   0xD /* emitter: pale-yellow / tan gouraud rings */
#define FX_FLARE_STAR_YELLOW_RINGS_2 0xE /* emitter: pale-yellow / pink gouraud rings */
#define FX_FLARE_STAR_PINK_RINGS     0xF /* emitter: thin pink streak fan + white-pink gouraud rings */
#define FX_FLARE_STAR_PINK_SPARKS_2  0x10 /* emitter: pink glyph-like spark sprites, 33 frames */
#define FX_FLARE_STAR_RED_RINGS      0x11 /* emitter: golden sparkle ring + lilac and crimson gouraud rings */
#define FX_FLARE_STAR_SMOKE_TRAIL    0x12 /* emitter: spiralling 3D trail, grey smoke strip */
#define FX_FLARE_STAR_RED_ORB        0x13 /* emitter: red glowing orb particles + violet-pink rings + red streak trail */
#define FX_FLARE_STAR_BACKDROP       0x1C /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_FLARE_STAR_PINK_RINGS_2   0x29 /* emitter: white-pink-magenta and yellow-olive gouraud rings */
#define FX_FLARE_STAR_ORANGE_GLOW    0x2A /* emitter: fading orange glow + red flare streaks */
#define FX_FLARE_STAR_RED_SHARDS     0x2B /* emitter: red shard sprites, 31 frames */

typedef struct {
    s16 x, y, z;
} Fx1b4Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1b4Point *keys4;
} Fx1b4Path;

typedef struct {
    Pool6CEntry *emit0[8];
    Pool6CEntry *emit20[8];
    Pool6CEntry *emit40;
    SVECTOR pos44; /* target position */
    SVECTOR pos4C[8];
    SVECTOR pos8C;
} Fx1b4Work80; /* 0x94 */

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target position */
} Fx1b4Work81; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0[7];
    SVECTOR pos1C; /* target position */
} Fx1b4Work82; /* 0x24 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4; /* target position */
} Fx1b4Work83; /* 0xC */

extern u16 fx_1b4_emitFileIds0[8];
extern u16 fx_1b4_emitFileIds1[8];
extern u16 fx_1b4_pathFileIds[8];
extern s32 (*fx_1b4_op80ParticleMotions[8])();
extern Unk800F78A4 *fx_1b4_env;
extern Fx1b4Path fx_1b4_posPath[8];
extern Fx1b4Path fx_1b4_op80ParticlePaths[8];
extern s32 fx_1b4_op80Trails0[8][0x3C];
extern s32 fx_1b4_op80Trails1[8][0xE0];
extern s32 fx_1b4_op81Trails[];
extern s32 fx_1b4_op82Trails0[];
extern s32 fx_1b4_op82Trails1[];
extern SVECTOR fx_1b4_pathOrigin; /* origin */
extern SVECTOR fx_1b4_emitPos;

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames);
static void fxm_posPathInit(Fx1b4Path *path, u16 *data);
static s32 fxm_posPathGet(Fx1b4Path *path, s32 t, SVECTOR *out);

/* propagated */
s32 fx_1b4_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion6(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_1b4_op80ParticleMotion7(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
