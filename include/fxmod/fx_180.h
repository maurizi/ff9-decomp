#ifndef FX_180_H
#define FX_180_H

/* Effect module fx_180 (dir 13 file 0x180): enemy Ultima (Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_180 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ULTIMA_GLOW_DOME      0x1 /* model: gouraud octagonal dome (20 polys) */
#define FX_ULTIMA_NOISE_RING     0x2 /* emitter: ring whose texture still reads as noise, + pink gouraud ring (look pending) */
#define FX_ULTIMA_ROCK_DEBRIS    0x3 /* emitter: grey rock chunks */
#define FX_ULTIMA_GREY_TRAIL     0x4 /* emitter: wobbling 3D trail with a grey streak texture */
#define FX_ULTIMA_RED_LENS_FLARE 0x6 /* emitter: red lens streak, white-pink rings, red sparks */
#define FX_ULTIMA_COLOR_RING     0x7 /* emitter: yellow, lilac, violet and magenta gouraud rings */
#define FX_ULTIMA_ROCK_DEBRIS_2  0x8 /* emitter: spiral of grey rock chunks */
#define FX_ULTIMA_PINK_RING      0x9 /* emitter: pink gouraud rings + fan */
#define FX_ULTIMA_ROCK_TRAIL     0xA /* emitter: screen trail of rock pieces */
#define FX_ULTIMA_GREY_TRAIL_2   0xB /* emitter: wobbling screen trail with a grey streak texture */
#define FX_ULTIMA_BLAST_RINGS    0xC /* emitter: white-pink, lilac and yellow-red gouraud rings, pink glows */
#define FX_ULTIMA_WHITE_BEAM     0xD /* emitter: spiralling white beam screen trail, pink sparks */
#define FX_ULTIMA_PINK_SPARKS    0xE /* emitter: sparse pink spark sprites */
#define FX_ULTIMA_NOISE_RING_2   0xF /* emitter: ring whose texture still reads as noise, + pink-lilac gouraud rings (look pending) */
#define FX_ULTIMA_RED_FLARE      0x10 /* emitter: red lens streak and white-red gouraud rings */
#define FX_ULTIMA_CRIMSON_STREAK 0x11 /* emitter: orbiting fan, a white-to-crimson light streak */
#define FX_ULTIMA_PINK_TRAIL     0x12 /* emitter: pink screen trail */
#define FX_ULTIMA_PINK_TRAIL_2   0x13 /* emitter: orbiting pink screen trail */
#define FX_ULTIMA_PINK_SPIKES    0x14 /* emitter: pink spike particles + spark sprites */
#define FX_ULTIMA_PINK_SPIKES_2  0x15 /* emitter: pink spike particles + spark sprites */
#define FX_ULTIMA_PINK_ORB_BURST 0x16 /* emitter: pink orb and blade particles, sparks, white-yellow-red gouraud rings */
#define FX_ULTIMA_SPIKE_CROWN    0x17 /* 'so' model: ring crown of red-pink spikes, drawn with FX_ULTIMA_GLOW_DOME */
#define FX_ULTIMA_RED_FLARE_2    0x18 /* emitter: red lens streak and white-red gouraud rings */
#define FX_ULTIMA_BACKDROP       0x19 /* layer table: layers rendered into an off-screen VRAM texture the effect samples (btlfx_drawLayerTexture) */
#define FX_ULTIMA_PATH_SET_2     0x1A /* table: 30 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x81 */
#define FX_ULTIMA_ROCK_SWIRL     0x1D /* emitter: spiralling 3D trail of rock pieces, pink glows */
#define FX_ULTIMA_ROCK_SWIRL_2   0x1E /* emitter: spiralling 3D trail of rock pieces, pink glows */
#define FX_ULTIMA_NOISE_RING_3   0x1F /* emitter: ring whose texture still reads as noise, + lilac gouraud ring (look pending) */
#define FX_ULTIMA_ROCK_TRAIL_2   0x20 /* emitter: spiralling screen trail of rock pieces */
#define FX_ULTIMA_PURPLE_HAZE    0x21 /* emitter: pink-purple smoke puffs */
#define FX_ULTIMA_PATH_SET       0x22 /* table: 18 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x80 */

typedef struct {
    s16 x, y, z;
} Fx180Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx180Point *keys4;
} Fx180Path;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR pos24; /* caster body position */
    SVECTOR pos2C; /* caster position */
    SVECTOR pos34; /* target position */
    SVECTOR pos3C; /* bone position */
    SVECTOR pos44;
    SVECTOR unk4C; /* write-only (copy of pos24) */
    SVECTOR camPos54; /* camera targets */
    SVECTOR camPos5C;
    SVECTOR camPos64;
    SVECTOR camPos6C;
} Fx180Work80; /* 0x74 */

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR pos2C; /* target position */
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR pos44;
    SVECTOR pos4C;
} Fx180Work81; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 fade48; /* brightness */
    SVECTOR pos4C; /* target position */
    SVECTOR pos54;
    SVECTOR pos5C;
    s32 timer64; /* fade countdown */
    u16 model68; /* models */
    u16 model6A;
} Fx180Work82; /* 0x6C */

typedef struct {
    Pool6CEntry *emit0;
    u8 pad4[8 - 4];
    SVECTOR pos8; /* caster body position */
    SVECTOR pos10; /* caster position */
    SVECTOR pos18; /* destination */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28;
    SVECTOR pos30;
    s16 timer38; /* fade countdown */
    s16 done3A;
} Fx180Work83; /* 0x3C */

extern Unk800F78A4 *fx_180_env;
extern BtlModelBuf fx_180_effModels[];
extern Fx180Path fx_180_posPath0[0x12];
extern Fx180Path fx_180_posPath1[0x1E];
extern s32 fx_180_op80Trails0[];
extern s32 fx_180_op80Trails1[];
extern s32 fx_180_op81Trails0[];
extern s32 fx_180_op81Trails1[];
extern s32 fx_180_op81Trails2[];
extern s32 fx_180_op81Trails3[];
extern s32 fx_180_op82Trails0[];
extern s32 fx_180_op82Trails1[];
extern s32 fx_180_op83Trails[];
extern SVECTOR fx_180_op80PathOrigin;
extern SVECTOR fx_180_op81PathOrigin;
extern SVECTOR fx_180_emitPos0;
extern SVECTOR fx_180_emitPos1;

static s32 fx_180_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fx_180_op81ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);
static void fxm_posPathInit(Fx180Path *path, u16 *data);
static s32 fxm_posPathGet(Fx180Path *path, s32 t, SVECTOR *out);
static void fxm_startUnitMotion(s32 unit, s32 motion);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_180_env;
/* end copied */

/* propagated */
static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start);
static u16 *fxm_offsetTableEntry(u8 *base, u16 idx);

#endif
