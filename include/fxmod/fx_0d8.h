#ifndef FX_0D8_H
#define FX_0D8_H

/* Effect module fx_0d8 (dir 13 file 0xD8). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d8Work84;

extern Unk80100C78Entry *D_801E9940;
extern u8 D_801E8FB0[][0x84];
extern u8 D_801E95E0[][0x48];

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    u8 unk10; /* index */
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    u8 unk14; /* kind */
    u8 unk15;
    s16 unk16;
    s32 *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx0d8Part; /* pool entry data */

/* fx_003 copies (asm) */
s32 func_801E7F80(s32 mode, Fx0d8Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

void func_801E7CEC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *arg5, s32 arg6,
                   s32 frame, s32 n);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx0d8Strip; /* 0x84 */
extern u16 D_801E8F28[];
void func_801E7704(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E8F30[];
extern Unk800F78A4 *D_801E8FA0;
void func_801E796C(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx0d8Strip *strip, Fx0d8Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* caster position */
    s32 *unk10; /* jitter table (D_801E9950) */
    s32 unk14; /* bolts spawned */
} Fx0d8Work83; /* 0x18 */

extern s16 D_801E8F38[16]; /* bolt kind per frame */
extern s32 D_801E9950[];
s32 fx_0d8_op83(s32 mode, Fx0d8Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

#endif
