#ifndef FX_18A_H
#define FX_18A_H

/* Effect module fx_18a (dir 13 file 0x18a). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* angles */
} Fx18aWork82; /* 0x10 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[15];
} Fx18aTrail; /* 0x7C */

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
} Fx18aSpark; /* 0x1C */

typedef struct {
    s32 unk0; /* frame */
    u8 pad4[0xC];
    s16 *unk10; /* pool */
} Fx18aInst;

extern Unk800F78A4 *D_801E8A24;
extern Fx18aTrail D_801E8B04[12];
extern s32 D_801E90D4;

s32 func_801E844C(s32 mode, Fx18aSpark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E85A4(void);
void func_801E85B0(Fx18aInst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to);

#define TRAIL(e) ((Fx18aTrail *)((e)->unk18 - 2))

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14; /* caster body position */
    s32 unk1C; /* charge counter */
    s32 unk20;
    s32 unk24; /* model */
} Fx18aWork80; /* 0x28 */

extern s32 D_801E89D8;
extern BtlModel D_801E8A34[];

#endif
