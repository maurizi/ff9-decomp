#ifndef FX_1B7_H
#define FX_1B7_H

/* Effect module fx_1b7 (dir 13 file 0x1b7). */
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
} Fx1b7Piece; /* 0x1C */

typedef struct {
    s16 unk0; /* model count */
    s16 unk2; /* face count */
    s32 unk4[8]; /* faces per model */
    SVECTOR (*unk24)[3]; /* faces */
    SVECTOR (*unk28)[3]; /* next free face */
    SVECTOR (*unk2C[8])[3]; /* faces of each model */
    Fx1b7Piece unk4C[0x6A4];
} Fx1b7Split;

extern Fx1b7Piece *D_801E87EC;
extern s32 D_801E87F0;

void func_801E7704(Fx1b7Split *s, SVECTOR (*faces)[3]);
s32 func_801E774C(s32 model);
Fx1b7Piece *func_801E7830(void);
s32 func_801E7868(void);

typedef struct {
    u8 pad0[0x50];
    u8 *unk50; /* work buffer */
} Fx1b7SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx1b7SummonData *unk54;
} Fx1b7Summon;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8[3]; /* caster position */
    SVECTOR unk14; /* caster rotation */
    s32 unk1C[3]; /* caster scale */
    SVECTOR unk28; /* caster body position */
    SVECTOR unk30;
    SVECTOR unk38;
    Pool6CEntry *unk40;
    s32 unk44[8]; /* split models */
} Fx1b7Work80; /* 0x64 */

extern s16 D_801E8788[8]; /* model files */
extern Unk80100C78Entry *D_801E87E4;
extern BtlModel D_801E8800[];
extern s32 D_801E8E40[];
extern s32 D_801EA3C0[];

void func_801E7ACC(void);

#define FX1B7_SPAD_STACK(save) __asm__ volatile("sw $29, 0(%0)\n\tmove $29, %1" : : "r"(save), "r"(0x1F8003F8) : "memory")
#define FX1B7_RESET_STACK(save) __asm__ volatile("lw $29, 0(%0)" : : "r"(save) : "memory")

/* copied */
extern Unk800F78A4 *D_801E87E0;
extern Fx1b7Split *D_801E87E8;
/* end copied */

#endif
