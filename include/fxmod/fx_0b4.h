#ifndef FX_0B4_H
#define FX_0B4_H

/* Effect module fx_0b4 (dir 13 file 0xB4). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0b4Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0b4Key *unk4;
} Fx0b4Path;

typedef struct {
    u16 unk0; /* start frame */
    u16 unk2; /* frames since the start */
} Fx0b4Timer;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    Pool6CEntry *unk28;
    SVECTOR unk2C; /* caster body position */
    SVECTOR unk34; /* caster position */
} Fx0b4Work;

extern Fx0b4Timer D_801E80EC[6];
extern u16 D_801E8104[6]; /* particle files */
extern u16 D_801E8110[6]; /* path files */
extern SVECTOR D_801E811C;
extern s32 (*D_801E8124[6])(); /* motion callbacks */
extern Unk800F78A4 *D_801E8184;
extern SVECTOR D_801E818C; /* path origin */
extern SVECTOR D_801E8194[6];
extern Fx0b4Path D_801E81C4[6];
extern s32 D_801E81F4[];
extern s32 D_801ED2F4[];

s32 func_801E77BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E783C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E787C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E78BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E78FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E793C(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n);
void func_801E8040(Fx0b4Path *path, s32 data);
s32 func_801E8058(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos);

#endif
