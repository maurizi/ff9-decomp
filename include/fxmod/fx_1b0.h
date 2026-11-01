#ifndef FX_1B0_H
#define FX_1B0_H

/* Effect module fx_1b0 (dir 13 file 0x1b0): enemy Death (Ozma). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

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
