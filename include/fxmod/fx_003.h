#ifndef FX_003_H
#define FX_003_H

/* Effect module fx_003 (dir 13 file 0x03). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width) */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[16];
} Fx003Strip; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[8];
    u8 pad44[4];
} Fx003Branch; /* 0x48 */

/* one bolt, a pool item */
typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    u8 unk10; /* bolt index */
    u8 unk11; /* state */
    u8 unk12; /* branch points */
    u8 unk13;
    s16 unk14; /* kind */
    s32 *unk18; /* jitter table (D_801E8F20) */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx003Bolt; /* 0x24 */

typedef struct {
    SVECTOR unk0; /* target body position */
    s32 *unk8; /* D_801E8F20 */
    s32 unkC; /* bolts spawned */
} Fx003Work; /* 0x10 */

extern u16 D_801E8528[];
extern u16 D_801E8530[];
extern Unk800F78A4 *D_801E8580;
extern Fx003Strip D_801E8590[];
extern Fx003Branch D_801E8BC0[];
extern s32 D_801E8F20[];

void func_801E7704(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E796C(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E7BF8(Unk80100C78Entry *entry, Fx003Strip *strip, Fx003Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E7CEC(s32 mode, Fx003Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
