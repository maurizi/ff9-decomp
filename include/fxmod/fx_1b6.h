#ifndef FX_1B6_H
#define FX_1B6_H

/* Effect module fx_1b6 (dir 13 file 0x1b6): enemy Death (Silver Dragon). Evidence: the static sweep of the ability, item
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
} Fx1b6Piece; /* 0x1C */

typedef struct {
    s16 count0; /* model count */
    s16 count2; /* face count */
    s32 counts4[7]; /* faces per model */
    SVECTOR (*faces20)[3]; /* faces */
    SVECTOR (*nextFace24)[3]; /* next free face */
    SVECTOR (*modelFaces28[7])[3]; /* faces of each model */
    Fx1b6Piece pieces44[0x708];
} Fx1b6Split;

extern Unk800F78A4 *fx_1b6_env;
extern Fx1b6Split *fx_1b6_split;
extern Fx1b6Piece *fx_1b6_pieces;
extern s32 fx_1b6_sweepLine;

void fx_1b6_initSplit(Fx1b6Split *s, SVECTOR (*faces)[3]);
static s32 fxm_splitModelAdd(s32 model);
Fx1b6Piece *fx_1b6_findFreePiece(void);
void fx_1b6_launchPieces(void);
void fx_1b6_updatePieces(void);

typedef struct {
    u8 pad0[0x50];
    u8 *workBuf50; /* work buffer */
} Fx1b6SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b6SummonData *summon54; /* g_btlfxLoader.summon */
} Fx1b6Summon;

typedef struct {
    s32 state0; /* state */
    s32 unk4; /* write-only: frame counter */
    s32 pos8[3]; /* caster position */
    SVECTOR rot14; /* caster rotation */
    s32 scale1C[3]; /* caster scale */
    s32 model28[7]; /* split models */
    SVECTOR pos44; /* caster body position */
    SVECTOR pos4C;
    SVECTOR pos54;
    Pool6CEntry *emit5C;
    Pool6CEntry *emit60;
} Fx1b6Work80; /* 0x64 */

extern s16 fx_1b6_modelFileIds[7]; /* model files */
extern Unk80100C78Entry *fx_1b6_task;
extern s32 fx_1b6_op80Trails[];
extern BtlModelBuf fx_1b6_effModels[];

#define FX1B6_SPAD_STACK(save) __asm__ volatile("sw $29, 0(%0)\n\tmove $29, %1" : : "r"(save), "r"(0x1F8003F8) : "memory")
#define FX1B6_RESET_STACK(save) __asm__ volatile("lw $29, 0(%0)" : : "r"(save) : "memory")

/* copied */
extern Unk800F78A4 *fx_1b6_env;
/* end copied */

#endif
