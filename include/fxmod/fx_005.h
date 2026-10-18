#ifndef FX_005_H
#define FX_005_H

/* Effect module fx_005 (dir 13 file 0x05). */
#include "btlfx_mod.h"

/* lightning strip: a header for func_800D1494 and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx005Strip; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx005Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10;
    u8 unk11; /* state */
    u8 unk12;
    u8 unk13;
    u8 unk14; /* kind */
    u8 unk15;
    s16 pad16;
    s32 *unk18; /* jitter table */
    Pool6CEntry *unk1C;
} Fx005Bolt;

typedef struct {
    SVECTOR unk0; /* target body */
    SVECTOR unk8; /* target */
    s32 *unk10; /* jitter table */
    s32 unk14; /* bolts spawned */
} Fx005Work; /* 0x18 */

extern s16 D_801E8BD8[];
extern Fx005Strip D_801E8C50[];
extern Fx005Branch D_801E9280[];
extern s32 D_801E95F0[];
extern u16 D_801E8BC8[];
extern u16 D_801E8BD0[];
extern Unk800F78A4 *D_801E8C40;
extern Unk80100C78Entry *D_801E95E0;

void func_801E7704(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E796C(Unk80100C78Entry *entry, Fx005Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx005Strip *strip, Fx005Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E7F60(s32 mode, Fx005Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E7CEC(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 arg6, s32 t, s32 n);

#endif
