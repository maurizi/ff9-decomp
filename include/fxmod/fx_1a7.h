#ifndef FX_1A7_H
#define FX_1A7_H

/* Effect module fx_1a7 (dir 13 file 0x1a7). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8[9];
    SVECTOR unk2C; /* target body position */
    SVECTOR unk34;
} Fx1a7Work80; /* 0x3C */

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    Pool6CEntry *unk8[9];
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    SVECTOR unk34; /* caster body position */
    SVECTOR unk3C; /* target body position */
    SVECTOR unk44; /* target position */
    SVECTOR unk4C; /* angles */
    SVECTOR unk54;
    SVECTOR unk5C;
} Fx1a7Work81; /* 0x64 */

extern u16 D_801E8860[9]; /* files */
extern u16 D_801E8874[9]; /* start frames */
extern u16 D_801E8888[9];
extern u16 D_801E889C[9];
extern s32 D_801E89D0[];
extern s32 D_801E8FD0[9][0xE];
extern s32 D_801E91D0[];
extern s32 D_801E96A0[9][0x2C];
extern s32 D_801E9CD0[];
extern SVECTOR D_801EA5E0;

extern s32 (*D_801E88B0[9])();
extern s32 (*D_801E88D4[9])();
extern Unk800F78A4 *D_801E8940;
extern s16 D_801E8950[9]; /* angles */
extern s16 D_801E8970[9];
extern s16 D_801E8990[9]; /* radii */
extern s16 D_801E89B0[9];
extern SVECTOR D_801EA5C0; /* from */
extern SVECTOR D_801EA5C8;
extern SVECTOR D_801EA5D0; /* to */
extern SVECTOR D_801EA5D8;
s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7748(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E778C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E77D0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7858(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E789C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7924(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7AB4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7AF8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7B3C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7B80(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7BC4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7C08(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7C4C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7C90(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7CD4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);
s32 func_801E7968(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);
s32 func_801E7D18(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames);

#endif
