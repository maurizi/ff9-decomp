#ifndef FX_07D_H
#define FX_07D_H

/* Effect module fx_07d (dir 13 file 0x7D). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    s32 unk10; /* buffer index */
    s32 *unk14; /* per-point (t offset, amplitude, step) */
} Fx07dArc;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx07dStrip; /* 0x7C */

extern Unk800F78A4 *D_801E9FB0;
extern SVECTOR D_801EA470[24];
extern s32 D_801EA530;
extern Unk80100C78Entry *D_801EA534;
extern s32 D_801EA538;
extern Fx07dStrip D_801EA540[];

void func_801E7ECC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame);
void func_801E7FF4(s32 d, s32 a1, s32 a, s32 a3, s16 *out);
void func_801E80C4(s16 *a, s16 *b);
void func_801E9834(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad);
s32 func_801E9A40(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

#endif
