#ifndef FX_085_H
#define FX_085_H

/* Effect module fx_085 (dir 13 file 0x85). */
#include "btlfx_mod.h"

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx085Strip; /* 0x7C */
extern u16 D_801E9D94[];
void func_801E82A0(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9D9C[];
void func_801E8508(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E878C(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9DF4;
extern SVECTOR D_801E9FA0[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)D_801E9FA0 - 4))
s32 func_801E8C28(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[3];
    u8 pad1C[0x1C];
} Fx085Branch; /* 0x38 */

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
} Fx085Bolt;

extern Fx085Strip D_801E9F9C[];
extern Fx085Branch D_801EA75C[];
extern s32 D_801EAADC; /* faster bolts */

void func_801E82A0(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
void func_801E878C(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
s32 func_801E8880(s32 mode, Fx085Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

typedef struct {
    SVECTOR unk0; /* target body */
    SVECTOR unk8; /* target */
    s32 *unk10; /* jitter table */
    s32 unk14; /* bolts spawned */
    s32 unk18; /* kind */
} Fx085Work83; /* 0x1C */

extern SVECTOR D_801E9DEC; /* previous bolt end */
extern Unk80100C78Entry *D_801E9DF8;
extern s32 D_801EAAEC[];

typedef struct {
    SVECTOR unk0; /* target position */
    SVECTOR unk8; /* ground position */
    s32 unk10; /* state */
    s32 unk14; /* model */
    s32 unk18; /* model */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx085Work80; /* 0x24 */

extern BtlModel D_801E9DFC[];
extern s32 D_801E9F8C;
#endif
