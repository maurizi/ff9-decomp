#ifndef FX_0A9_H
#define FX_0A9_H

/* Effect module fx_0a9 (dir 13 file 0xA9). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[11]; /* bolt points */
} Fx0a9Bolt; /* 0x5C */

typedef struct {
    s32 unk0[3]; /* position, 20.12 */
    s16 unkC[3]; /* velocity */
    s16 unk12;
    u16 unk14; /* bolt */
    u8 unk16; /* point index from the end */
    u8 unk17;
    SVECTOR *unk18; /* bolt point */
} Fx0a9Part; /* pool entry data */

typedef struct {
    s32 unk0; /* state */
    SVECTOR unk4; /* caster body position */
} Fx0a9Work80;

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* btlfx_getUnitArgBonePos(0x10, 0) */
} Fx0a9Work83;

extern Unk800F78A4 *D_801E7F58;
extern Fx0a9Bolt D_801E7F68[16];
extern s32 D_801E8528; /* next bolt */

s32 func_801E77EC(s32 mode, Fx0a9Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0a9Work83 *work);
void func_801E79C8(void);
void func_801E79D4(Unk80100C78Entry *entry, SVECTOR *from, SVECTOR *to);

#endif
