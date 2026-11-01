#ifndef FX_1B7_H
#define FX_1B7_H

/* Effect module fx_1b7 (dir 13 file 0x1b7): enemy Death (Nova Dragon). Evidence: the static sweep of the ability, item
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
} Fx1b7Piece; /* 0x1C */

typedef struct {
    s16 count0; /* model count */
    s16 count2; /* face count */
    s32 counts4[8]; /* faces per model */
    SVECTOR (*faces24)[3]; /* faces */
    SVECTOR (*nextFace28)[3]; /* next free face */
    SVECTOR (*modelFaces2C[8])[3]; /* faces of each model */
    Fx1b7Piece pieces4C[0x6A4];
} Fx1b7Split;

extern Fx1b7Piece *fx_1b7_pieces;
extern s32 fx_1b7_sweepLine;

void fx_1b7_initSplit(Fx1b7Split *split, SVECTOR (*faces)[3]);
static s32 fxm_splitModelAdd(s32 model);
Fx1b7Piece *fx_1b7_findFreePiece(void);
s32 fx_1b7_launchPieces(void);

typedef struct {
    u8 pad0[0x50];
    u8 *workBuf50; /* work buffer */
} Fx1b7SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b7SummonData *summon54; /* g_btlfxLoader.summon */
} Fx1b7Summon;

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    s32 pos8[3]; /* caster position */
    SVECTOR rot14; /* caster rotation */
    s32 scale1C[3]; /* caster scale */
    SVECTOR pos28; /* caster body position */
    SVECTOR pos30;
    SVECTOR pos38;
    Pool6CEntry *emit40;
    s32 model44[8]; /* split models */
} Fx1b7Work80; /* 0x64 */

extern s16 fx_1b7_modelFileIds[8]; /* model files */
extern Unk80100C78Entry *fx_1b7_task;
extern BtlModelBuf fx_1b7_effModels[];
extern s32 fx_1b7_op80Trails0[];
extern s32 fx_1b7_op80Trails1[];

void fx_1b7_updatePieces(void);

#define FX1B7_SPAD_STACK(save) __asm__ volatile("sw $29, 0(%0)\n\tmove $29, %1" : : "r"(save), "r"(0x1F8003F8) : "memory")
#define FX1B7_RESET_STACK(save) __asm__ volatile("lw $29, 0(%0)" : : "r"(save) : "memory")

/* copied */
extern Unk800F78A4 *fx_1b7_env;
extern Fx1b7Split *fx_1b7_split;
/* end copied */

#endif
