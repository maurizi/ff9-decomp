#ifndef FX_049_H
#define FX_049_H

/* Effect module fx_049 (dir 13 file 0x49). */
#include "btlfx_mod.h"

/* lightning strip: a header for func_800D1494 and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx049Strip; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx049Branch; /* 0x48 */

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
} Fx049Bolt;

typedef struct {
    SVECTOR unk0; /* target body */
    s32 *unk8; /* jitter table */
    s32 unkC; /* bolts spawned */
} Fx049Work; /* 0x10 */

extern s16 D_801E8D20[];
extern s32 D_801E9738[];
extern Fx049Strip D_801E8D98[];
extern Fx049Branch D_801E93C8[];
extern u16 D_801E8D10[];
extern u16 D_801E8D18[];
extern Unk800F78A4 *D_801E8D88;
extern Unk80100C78Entry *D_801E9728;

void func_801E7704(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E796C(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx049Strip *strip, Fx049Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E7F5C(s32 mode, Fx049Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
void func_801E7CEC(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 arg6, s32 t, s32 n);

#endif
