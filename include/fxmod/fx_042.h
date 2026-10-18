#ifndef FX_042_H
#define FX_042_H

/* Effect module fx_042 (dir 13 file 0x42). */
#include "btlfx_mod.h"

/* lightning strip: a header for func_800D1494 and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx042Strip; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx042Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* bolt index */
    u8 unk11; /* state */
    u8 unk12; /* branch points */
    u8 unk13;
    s16 unk14; /* kind */
    s32 *unk18; /* jitter table */
    Pool6CEntry *unk1C;
} Fx042Bolt;

typedef struct {
    SVECTOR unk0; /* origin */
    s32 *unk8; /* jitter table */
    s32 unkC; /* bolts spawned */
} Fx042Work; /* 0x10 */

extern s32 D_801E8F0C[];
extern u16 D_801E8514[];
extern u16 D_801E851C[];
extern Unk800F78A4 *D_801E856C;
extern Fx042Strip D_801E857C[];
extern Fx042Branch D_801E8BAC[];

void func_801E7704(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E796C(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx042Strip *strip, Fx042Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E7CEC(s32 mode, Fx042Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
