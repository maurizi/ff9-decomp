#ifndef FX_0D7_H
#define FX_0D7_H

/* Effect module fx_0d7 (dir 13 file 0xD7). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d7Work84;

extern u8 D_801E88D0[][0x84];
extern u8 D_801E8F00[][0x48];

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
    u8 unk10; /* index */
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    s16 unk14; /* kind */
    s16 unk16;
    s32 *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx0d7Part; /* pool entry data */

/* fx_003 copies (asm) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx0d7Strip; /* 0x84 */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* bolt index */
    u8 unk11; /* state */
    u8 unk12; /* branch points */
    u8 unk13;
    s16 unk14; /* kind */
    s32 *unk18; /* jitter table (D_801E9260) */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx0d7Bolt; /* 0x24 */
typedef struct {
    SVECTOR unk0; /* target body position */
    s32 *unk8; /* D_801E9260 */
    s32 unkC; /* bolts spawned */
} Fx0d7Work; /* 0x10 */
extern u16 D_801E8868[];
void func_801E7704(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E8870[];
void func_801E796C(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx0d7Strip *strip, Fx0d7Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E88C0;
extern s32 D_801E9260[];
s32 func_801E7CEC(s32 mode, Fx0d7Part *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
