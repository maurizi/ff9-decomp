#ifndef FX_1E9_H
#define FX_1E9_H

/* Effect module fx_1e9 (dir 13 file 0x1e9). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14; /* caster body position */
    s32 unk1C; /* loop counter */
    s32 unk20;
    s32 unk24; /* model */
} Fx1e9Work80; /* 0x28 */

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* angles */
} Fx1e9Work82; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[9];
} Fx1e9Trail; /* 0x4C */

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u16 unk14; /* trail */
    s16 unk16; /* trail point index */
    SVECTOR *unk18; /* trail point */
} Fx1e9Spark; /* 0x1C */

typedef struct {
    u8 pad0[0x10];
    s16 *unk10; /* pool */
} Fx1e9Inst;

extern s32 D_801E8518;
extern Unk800F78A4 *D_801E8564;
extern BtlModel D_801E8574[];
extern Fx1e9Trail D_801E8644[8];
extern s32 D_801E88A4;

s32 func_801E7FCC(s32 mode, Fx1e9Spark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E810C(void);
void func_801E8118(Fx1e9Inst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to);

#endif
