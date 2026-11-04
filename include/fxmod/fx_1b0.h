#ifndef FX_1B0_H
#define FX_1B0_H

/* Effect module fx_1b0 (dir 13 file 0x1b0): enemy Death (Ozma). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1b0 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_DEATH_ROCKS           0x2 /* emitter: spiralling 3D trail + grey rock boulders */
#define FX_DEATH_LIGHTNING_TRAIL 0x3 /* emitter: 3D trail, blue streak strip and blue lightning */
#define FX_DEATH_FIRE_RINGS      0x4 /* emitter: white-yellow-orange-red gouraud rings */
#define FX_DEATH_DUST_RINGS      0x5 /* emitter: rings textured with tan dust, 24 frames */
#define FX_DEATH_COLOUR_RINGS    0x6 /* emitter: pale-yellow / cyan / orange / magenta gouraud rings */
#define FX_DEATH_BLUE_FLARE      0x9 /* emitter: blue cone-beam fan + blue lens-flare streak + yellow-red and pink gouraud rings */
#define FX_DEATH_ROCK_TRAIL      0xC /* emitter: orbiting screen trail, grey rock strip */
#define FX_DEATH_SPECKS          0xD /* emitter: spiral of faint white speck sprites, 39 frames */
#define FX_DEATH_DUST_RINGS_2    0xE /* emitter: pale-yellow / cyan / magenta gouraud rings + rings textured with tan dust */
#define FX_DEATH_PINK_RINGS      0x11 /* rings: pale-pink to crimson gouraud rings, 24 frames */

typedef struct {
    s32 posX0; /* position, 20.12 */
    s32 posY4;
    s32 posZ8;
    SVECTOR velC; /* velocity (vx, vy, vz), unk12: scale */
    s16 rotX14;
    s16 rotY16;
    s16 frame18; /* age, -1 = free */
    s16 face1A; /* face */
} Fx1b0Piece; /* 0x1C */

typedef struct {
    s16 count0; /* model count */
    s16 count2; /* face count */
    s32 counts4[8]; /* faces per model */
    SVECTOR (*faces24)[3]; /* faces */
    SVECTOR (*nextFace28)[3]; /* next free face */
    SVECTOR (*modelFaces2C[8])[3]; /* faces of each model */
    Fx1b0Piece pieces4C[0x7BC];
} Fx1b0Split;

extern Unk800F78A4 *fx_1b0_env;
extern Fx1b0Split *fx_1b0_split;
extern Fx1b0Piece *fx_1b0_pieces;

void fx_1b0_initSplit(Fx1b0Split *split, SVECTOR (*faces)[3]);
static s32 fxm_splitModelAdd(s32 model);
Fx1b0Piece *fx_1b0_findFreePiece(void);
s32 fx_1b0_launchPieces(SVECTOR *center);
void fx_1b0_updatePieces(void);
static s32 fx_1b0_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

typedef struct {
    SVECTOR angle0; /* angles */
    s16 spin8; /* angle speeds */
    s16 spinA;
} Fx1b0Orbit; /* 0xC */

extern SVECTOR fx_1b0_orbitCenter; /* centre */
extern Fx1b0Orbit fx_1b0_orbits[];

typedef struct {
    u8 pad0[0x50];
    u8 *workBuf50; /* work buffer */
} Fx1b0SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b0SummonData *summon54; /* g_btlfxLoader.summon */
} Fx1b0Summon;

typedef struct {
    s32 state0; /* state */
    s32 frame4; /* frame counter */
    s32 pos8[3]; /* caster position */
    SVECTOR rot14; /* caster rotation */
    s32 scale1C[3]; /* caster scale */
    SVECTOR pos28; /* caster body position */
    SVECTOR unk30; /* write-only (pos28 on the ground) */
    CVECTOR color38; /* model colour */
    s32 model3C[8]; /* split models */
} Fx1b0Work80; /* 0x5C */
s32 fx_1b0_op80(s32 mode, Fx1b0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);

extern u8 fx_1b0_colorGradient[]; /* colour fade table */
extern s16 fx_1b0_modelFileIds[8]; /* model files */
extern s32 fx_1b0_op80Trails0[];
extern s32 fx_1b0_op80Trails1[];
extern s32 fx_1b0_op80Trails2[];
extern Unk80100C78Entry *fx_1b0_task;
extern BtlModelBuf fx_1b0_effModels[];

/* copied */
extern Unk800F78A4 *fx_1b0_env;
/* end copied */

#endif
