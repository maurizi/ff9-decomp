#ifndef FX_101_H
#define FX_101_H

/* Effect module fx_101 (dir 13 file 0x101). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[12];
} Fx101Bolt; /* 0x64 */

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* bolt */
    s16 unk16; /* point index from the tip */
    SVECTOR *unk18; /* bolt point */
} Fx101Spark; /* 0x1C */

typedef struct {
    s32 unk0; /* work size */
    s32 unk4; /* count */
    s32 unk8; /* entry size */
    u8 padC[0x14 - 0xC];
    void *unk14; /* entry callback */
} Fx101PoolCfg;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    Pool6CEntry *unk8;
    SVECTOR unkC; /* caster position */
    SVECTOR unk14; /* caster body position */
    s32 unk1C; /* loop counter */
    s32 unk20; /* camera shake on */
    s32 unk24; /* model */
} Fx101Work80;

/* View of Unk800F78A40 up to the byte fx_101 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x10];
    u8 unk10;
} Fx101BattleState;

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* angles */
} Fx101Work82;

extern s32 D_801E85B0; /* bolts stop */
extern BtlModel D_801E860C[];
extern Unk800F78A4 *D_801E85FC;
extern Fx101Bolt D_801E86DC[8];
extern s32 D_801E89FC; /* next bolt */

s32 fx_101_op80(s32 mode, Fx101Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 func_801E8048(s32 mode, Fx101Spark *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E8188(void);
void func_801E8194(Unk80100C78Entry *entry, SVECTOR *origin, s32 ang, SVECTOR *from, SVECTOR *to);
s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg);

#endif
