#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9). */
#include "btlfx_mod.h"

/* lightning strip: a header for func_800D1494 and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0d9Strip; /* 0x7C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx0d9Branch; /* 0x38 */

/* one bolt, a pool item; as Fx018Bolt */
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
} Fx0d9Bolt;

extern Fx0d9Strip D_801EA048[];
extern Fx0d9Branch D_801EA808[];
extern s32 D_801EAB88; /* faster bolts */

void func_801E8188(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E8674(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E8768(s32 mode, Fx0d9Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
