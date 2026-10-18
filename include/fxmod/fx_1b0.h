#ifndef FX_1B0_H
#define FX_1B0_H

/* Effect module fx_1b0 (dir 13 file 0x1b0). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    SVECTOR unkC; /* velocity (vx, vy, vz), unk12: scale */
    s16 unk14;
    s16 unk16;
    s16 unk18; /* age, -1 = free */
    s16 unk1A; /* face */
} Fx1b0Piece; /* 0x1C */

typedef struct {
    s16 unk0; /* model count */
    s16 unk2; /* face count */
    s32 unk4[8]; /* faces per model */
    SVECTOR (*unk24)[3]; /* faces */
    SVECTOR (*unk28)[3]; /* next free face */
    SVECTOR (*unk2C[8])[3]; /* faces of each model */
    Fx1b0Piece unk4C[0x7BC];
} Fx1b0Split;

extern Unk800F78A4 *D_801EBF34;
extern Fx1b0Split *D_801EBF3C;
extern Fx1b0Piece *D_801EBF40;

void func_801E7704(Fx1b0Split *s, SVECTOR (*faces)[3]);
s32 func_801E774C(s32 model);
Fx1b0Piece *func_801E7830(void);
s32 func_801E7868(SVECTOR *center);
void func_801E7B04(void);
s32 func_801E7E48(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

typedef struct {
    SVECTOR unk0; /* angles */
    s16 unk8; /* angle speeds */
    s16 unkA;
} Fx1b0Orbit; /* 0xC */

extern SVECTOR D_801EC714; /* centre */
extern Fx1b0Orbit D_801EC724[];

typedef struct {
    u8 pad0[0x50];
    u8 *unk50; /* work buffer */
} Fx1b0SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b0SummonData *unk54;
} Fx1b0Summon;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8[3]; /* caster position */
    SVECTOR unk14; /* caster rotation */
    s32 unk1C[3]; /* caster scale */
    SVECTOR unk28; /* caster body position */
    SVECTOR unk30;
    CVECTOR unk38; /* model colour */
    s32 unk3C[8]; /* split models */
} Fx1b0Work80; /* 0x5C */

extern u8 D_801E8CAC[]; /* colour fade table */
extern s16 D_801E8CEC[8]; /* model files */
extern s32 D_801E8D44[];
extern s32 D_801EA2C4[];
extern s32 D_801EAFE4[];
extern Unk80100C78Entry *D_801EBF38;
extern BtlModel D_801EBF44[];

/* copied */
extern Unk800F78A4 *D_801EBF34;
/* end copied */

#endif
