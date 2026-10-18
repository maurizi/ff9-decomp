#ifndef FX_17D_4_H
#define FX_17D_4_H

/* Effect module fx_17d_4 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E9890;
extern s16 D_801E97FC[16];
extern SVECTOR D_801E981C[3];
extern s16 D_801E9814[3];

typedef struct {
    s16 x, y, z;
} Fx17d4Pt;

/* propagated */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[2];
} Fx17d4Line;

extern Fx17d4Line D_801E987C;
void func_801E7720(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern s16 D_801E9724[18 * 4]; /* per strand: file, start, stride, width */
extern Fx17d4Line *D_801E97B4[18];
void func_801E77DC(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);

#endif
