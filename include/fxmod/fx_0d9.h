#ifndef FX_0D9_H
#define FX_0D9_H

/* Effect module fx_0d9 (dir 13 file 0xD9). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0d9Work84;

/* lightning strip: a header for btlfx_drawTrail and the points (pad = width); as Fx018Strip */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0d9Strip; /* 0x7C */
extern u16 D_801E9E40[];
void func_801E8188(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9E48[];
void func_801E83F0(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E8674(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9EA0;
extern SVECTOR D_801EA04C[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_0D9_STRIPS ((Fx0d9Strip *)((u8 *)D_801EA04C - 4))
s32 func_801E8B10(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);

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

/* propagated */

typedef struct {
    SVECTOR unk0; /* target body */
    SVECTOR unk8; /* target */
    s32 *unk10; /* jitter table */
    s32 unk14; /* bolts spawned */
    s32 unk18; /* kind */
} Fx0d9Work83; /* 0x1C */
extern SVECTOR D_801E9E98; /* previous bolt end */
extern Unk80100C78Entry *D_801E9EA4;
extern s32 D_801EAB98[];
s32 fx_0d9_op83(s32 mode, Fx0d9Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* caster position */
    s32 unk10; /* state */
    s32 unk14; /* model */
    s32 unk18; /* model */
    Pool6CEntry *unk1C;
    s32 pad20;
} Fx0d9Work80; /* 0x24 */

extern BtlModel D_801E9EA8[];
extern s32 D_801EA038;
#endif
