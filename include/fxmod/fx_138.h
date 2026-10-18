#ifndef FX_138_H
#define FX_138_H

/* Effect module fx_138 (dir 13 file 0x138). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx138Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* key count */
    Fx138Point *unk4; /* keys */
} Fx138Path;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    SVECTOR unk20; /* caster body position */
    SVECTOR unk28;
    u16 unk30; /* frame, up to 5 */
    u16 unk32; /* scale */
} Fx138Work81; /* 0x34 */

typedef struct {
    u8 pad0[0x24];
    VECTOR unk24; /* model position */
    VECTOR unk34; /* model scale */
    u8 pad44[0x48 - 0x44];
    SVECTOR unk48; /* model rotation */
    VECTOR unk50; /* model position */
    VECTOR unk60; /* model scale */
    SVECTOR unk70; /* caster position */
    SVECTOR unk78; /* caster rotation */
    u8 pad80[0x88 - 0x80];
    s16 unk88;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    s16 unk90; /* model shown */
    s16 unk92; /* model countdown */
    s16 unk94; /* frame */
    u16 unk96[3]; /* ring models */
    u16 unk9C; /* model */
} Fx138Work80; /* 0xA0 */

typedef struct {
    SVECTOR unk0; /* rotation */
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u16 unkE;
    u8 pad10[0x18 - 0x10];
} Fx138Ring; /* 0x18 */

extern Fx138Ring D_801E8408[3];
extern u16 D_801E8450[6][4];
extern u16 D_801E8480[3];
extern u16 D_801E8494[6];
extern BtlModel D_801E8510[];
extern u16 D_801E8488[6]; /* trail files */
extern s32 D_801E84A0[6];
extern Unk800F78A4 *D_801E8500;
extern u8 D_801E8860[];
extern SVECTOR D_801E8E70[6];
extern Fx138Path D_801E8830[6];
extern SVECTOR D_801E8E60; /* path origin */
extern SVECTOR D_801E8E68; /* path rotation */

s32 func_801E7704(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7728(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E774C(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7770(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E7794(SVECTOR *out, s32 arg1, s32 i);
s32 func_801E77B8(SVECTOR *out, s32 arg1, s32 i);
void func_801E77DC(SVECTOR *out, Fx138Path *path, s32 i);
s32 func_801E81F0(Fx138Path *path, s32 i, SVECTOR *out);
s32 fx_138_op80(s32 mode, Fx138Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801E813C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
s32 func_801E8180(s32 from, s32 to, s32 t, s32 n);

void func_801E81D8(Fx138Path *path, u16 *data);
void func_801E8254(Unk800F78A4 *env, s32 i, s32 v);
void func_801E8268(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

#endif
