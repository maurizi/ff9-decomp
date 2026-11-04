#ifndef FX_1EC_H
#define FX_1EC_H

/* Effect module fx_1ec (dir 13 file 0x1ec): enemy Ultima (Kuja/Trance Kuja). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ec effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s7.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ULTIMA_RED_FLARE_2    0x1 /* emitter: red lens streak and white-red gouraud rings (byte-identical to fx_180 entry 0x18) */
#define FX_ULTIMA_NOISE_RING     0x2 /* emitter: ring with a texture not resolved (noise) + pink gouraud ring (byte-identical to fx_180 entry 0x2) (look pending) */
#define FX_ULTIMA_ROCK_DEBRIS    0x3 /* emitter: grey rock chunks (byte-identical to fx_180 entry 0x3) */
#define FX_ULTIMA_GREY_TRAIL     0x4 /* emitter: wobbling 3D trail with a grey streak texture (byte-identical to fx_180 entry 0x4) */
#define FX_ULTIMA_PINK_SPIKES    0x6 /* emitter: pink spike particles + spark sprites (byte-identical to fx_180 entry 0x14) */
#define FX_ULTIMA_RED_LENS_FLARE 0x7 /* emitter: red lens streak, white-pink rings, red sparks (byte-identical to fx_180 entry 0x6) */
#define FX_ULTIMA_COLOR_RING     0x8 /* emitter: yellow, lilac, violet and magenta gouraud rings (byte-identical to fx_180 entry 0x7) */
#define FX_ULTIMA_ROCK_DEBRIS_2  0x9 /* emitter: spiral of grey rock chunks (byte-identical to fx_180 entry 0x8) */
#define FX_ULTIMA_PATH_SET_2     0xB /* table: 30 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x81 (byte-identical to fx_180 entry 0x1A) */
#define FX_ULTIMA_SPIKE_CROWN 0xD /* 'so' model: ring crown of red-pink spikes, drawn with FX_ULTIMA_GLOW_DOME (byte-identical to fx_180 entry 0x17) */
#define FX_ULTIMA_GLOW_DOME      0xE /* model: gouraud octagonal dome (20 polys) (byte-identical to fx_180 entry 0x1) */
#define FX_ULTIMA_GREY_TRAIL_2   0xF /* emitter: wobbling screen trail with a grey streak texture (byte-identical to fx_180 entry 0xB) */
#define FX_ULTIMA_BLAST_RINGS    0x10 /* emitter: white-pink, lilac and yellow-red gouraud rings, pink glows (byte-identical to fx_180 entry 0xC) */
#define FX_ULTIMA_PINK_SPARKS    0x11 /* emitter: sparse pink spark sprites (byte-identical to fx_180 entry 0xE) */
#define FX_ULTIMA_NOISE_RING_2   0x12 /* emitter: ring with a texture not resolved (noise) + pink-lilac gouraud rings (byte-identical to fx_180 entry 0xF) (look pending) */
#define FX_ULTIMA_RED_FLARE      0x13 /* emitter: red lens streak and white-red gouraud rings (byte-identical to fx_180 entry 0x10) */
#define FX_ULTIMA_CRIMSON_STREAK 0x14 /* emitter: orbiting fan, a white-to-crimson light streak (byte-identical to fx_180 entry 0x11, same render) */
#define FX_ULTIMA_PINK_TRAIL     0x15 /* emitter: pink screen trail (byte-identical to fx_180 entry 0x12) */
#define FX_ULTIMA_PINK_TRAIL_2   0x16 /* emitter: orbiting pink screen trail (byte-identical to fx_180 entry 0x13) */
#define FX_ULTIMA_PINK_ORB_BURST 0x17 /* emitter: pink orb and blade particles, sparks, white-yellow-red gouraud rings (byte-identical to fx_180 entry 0x16) */
#define FX_ULTIMA_RED_BURST      0x18 /* emitter: red flame-burst particles */
#define FX_ULTIMA_PATH_SET       0x1E /* table: 18 position paths (fxm_offsetTableEntry, fxm_posPathInit), op 0x80 (byte-identical to fx_180 entry 0x22) */
#define FX_ULTIMA_ROCK_SWIRL     0x20 /* emitter: spiralling 3D trail of rock pieces */
#define FX_ULTIMA_PINK_RING      0x21 /* emitter: pink gouraud rings + fan (byte-identical to fx_180 entry 0x9) */
#define FX_ULTIMA_ROCK_TRAIL     0x22 /* emitter: screen trail of rock pieces (byte-identical to fx_180 entry 0xA) */
#define FX_ULTIMA_ROCK_SWIRL_2   0x23 /* emitter: spiralling 3D trail of rock pieces + sparks */
#define FX_ULTIMA_PINK_SPIKES_2  0x24 /* emitter: pink spike particles + spark sprites (byte-identical to fx_180 entry 0x15) */
#define FX_ULTIMA_WHITE_BEAM     0x25 /* emitter: spiralling white beam screen trail, pink sparks (byte-identical to fx_180 entry 0xD) */
#define FX_ULTIMA_BACKDROP       0x26 /* table: background layers for btlfx_drawBackdrop (byte-identical to fx_180 entry 0x19) */
#define FX_ULTIMA_NOISE_RING_3   0x27 /* emitter: ring with a texture not resolved (noise) + lilac gouraud ring (look pending) */

typedef struct {
    s16 x, y, z;
} Fx1ecPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx1ecPoint *keys4;
} Fx1ecPath;

typedef struct {
    Pool6CEntry *emit0[9];
    SVECTOR pos24; /* caster body position */
    SVECTOR pos2C; /* target position */
    SVECTOR pos34;
    SVECTOR unk3C; /* unused (runtime watch, round 33) */
    SVECTOR unk44; /* unused (runtime watch, round 33) */
    SVECTOR camPos4C; /* camera targets */
    SVECTOR camPos54;
    SVECTOR camPos5C;
} Fx1ecWork80; /* 0x64 */

typedef struct {
    Pool6CEntry *emit0[11];
    SVECTOR pos2C; /* target position */
    SVECTOR pos34;
    SVECTOR pos3C;
    SVECTOR pos44;
    SVECTOR pos4C;
} Fx1ecWork81; /* 0x54 */

typedef struct {
    Pool6CEntry *emit0[8];
    SVECTOR rot20; /* model rotation */
    VECTOR pos28; /* model position */
    VECTOR scale38; /* model scale */
    s32 rgb48; /* brightness */
    SVECTOR pos4C; /* target position */
    SVECTOR pos54;
    SVECTOR pos5C;
    s32 timer64; /* fade countdown */
    s16 timer68; /* screen fade countdown */
    s16 flag6A; /* screen flash */
    u16 model6C; /* models */
    u16 model6E;
} Fx1ecWork82; /* 0x70 */

extern BtlModelBuf fx_1ec_effModels[];
extern s32 fx_1ec_op80Trails0[];
extern s32 fx_1ec_op80Trails1[];
extern s32 fx_1ec_op81Trails0[];
extern s32 fx_1ec_op81Trails1[];
extern s32 fx_1ec_op81Trails2[];
extern s32 fx_1ec_op81Trails3[];
extern s32 fx_1ec_op82Trails0[];
extern s32 fx_1ec_op82Trails1[];
extern SVECTOR fx_1ec_emitPos0;
extern SVECTOR fx_1ec_emitPos1;

static s32 fx_1ec_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_1ec_op81ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);
static void fxm_posPathInit(Fx1ecPath *path, u16 *data);
static s32 fxm_isMotionDone(s32 unit);
static void fxm_startHeavyHitMotionIfActive(s32 unit);

/* copied */
extern Unk800F78A4 *fx_1ec_env;
extern Fx1ecPath fx_1ec_posPath0[0x12];
static s32 fxm_posPathGet(Fx1ecPath *path, s32 t, SVECTOR *out);
extern SVECTOR fx_1ec_op80Origin;
extern Fx1ecPath fx_1ec_posPath1[0x1E];
extern SVECTOR fx_1ec_op81Origin;
static void fxm_startUnitMotion(s32 unit, s32 motion);
/* end copied */

/* propagated */
static u16 *fxm_offsetTableEntry(u8 *base, u16 idx);

#endif
