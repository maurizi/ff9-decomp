#ifndef FX_018_H
#define FX_018_H

/* Effect module fx_018 (dir 13 file 0x18). */
#include "btlfx_mod.h"

/* lightning strip: a header for func_800D1494 and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx018Strip; /* 0x7C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx018Branch; /* 0x38 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* bolt index */
    u8 unk11; /* state */
    u8 unk12; /* branch points */
    u8 unk13;
    u8 unk14; /* kind */
    s8 unk15; /* extra width */
    u8 pad16[2];
    s32 *unk18; /* jitter table */
} Fx018Bolt;

typedef struct {
    SVECTOR unk0; /* target body */
    SVECTOR unk8; /* target */
    s32 unk10; /* state */
    s32 unk14; /* models */
    s32 unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx018Work80; /* 0x24 */

extern BtlModel D_801E9CF0[];
extern s32 D_801E9E80; /* ring radius */
extern u16 D_801E9C88[];
extern u16 D_801E9C90[];
extern Unk800F78A4 *D_801E9CE8;
extern Fx018Strip D_801E9E90[];
extern Fx018Branch D_801EA650[];
extern s32 D_801EA9D0; /* faster bolts */

void func_801E82FC(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E8564(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E87E8(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E88DC(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
s32 func_801E8C84(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

#endif
