#ifndef FX_1B4_H
#define FX_1B4_H

/* Effect module fx_1b4 (dir 13 file 0x1b4). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2; /* key count */
    SVECTOR *unk4; /* keys (6 bytes each) */
} Fx1b4Path;

typedef struct {
    Pool6CEntry *unk0[8];
    Pool6CEntry *unk20[8];
    Pool6CEntry *unk40;
    SVECTOR unk44; /* target position */
    SVECTOR unk4C[8];
    SVECTOR unk8C;
} Fx1b4Work80; /* 0x94 */

typedef struct {
    Pool6CEntry *unk0[4];
    SVECTOR unk10; /* target position */
} Fx1b4Work81; /* 0x18 */

typedef struct {
    Pool6CEntry *unk0[7];
    SVECTOR unk1C; /* target position */
} Fx1b4Work82; /* 0x24 */

typedef struct {
    Pool6CEntry *unk0;
    SVECTOR unk4; /* target position */
} Fx1b4Work83; /* 0xC */

extern u16 D_801E85BC[8];
extern u16 D_801E85CC[8];
extern u16 D_801E85DC[8];
extern s32 (*D_801E85EC[8])();
extern Unk800F78A4 *D_801E8654;
extern Fx1b4Path D_801E8664[8];
extern Fx1b4Path D_801E86A4[8];
extern s32 D_801E86E4[8][0x3C];
extern s32 D_801E8E64[8][0xE0];
extern s32 D_801EAA64[];
extern s32 D_801ECE64[];
extern s32 D_801EE0F4[];
extern SVECTOR D_801EF174; /* origin */
extern SVECTOR D_801EF17C;

s32 func_801E78A4(Pool30Entry *e, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames);
void func_801E8540(Fx1b4Path *path, u8 *data);
s32 func_801E8558(Fx1b4Path *path, s32 i, SVECTOR *out);

#endif
