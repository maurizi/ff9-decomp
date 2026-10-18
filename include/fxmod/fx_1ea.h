#ifndef FX_1EA_H
#define FX_1EA_H

/* Effect module fx_1ea (dir 13 file 0x1ea). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1eaPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1eaPoint *unk4;
} Fx1eaPath;

typedef struct {
    Pool6CEntry *unk0;
    Pool6CEntry *unk4;
    SVECTOR unk8; /* caster body position */
    SVECTOR unk10; /* caster rotation */
} Fx1eaWork80; /* 0x18 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[1];
} Fx1eaTrail;

typedef struct {
    s16 unk0; /* fade-in countdown */
    s16 unk2; /* fade-out countdown */
} Fx1eaWork82;

extern SVECTOR D_801E9B58; /* origin */


void func_801E8B9C(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);
s32 func_801E8C4C(Fx1eaPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *D_801E8DD8;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1eaKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1eaKey *unk4;
} Fx1eaPath_0b4;
s32 func_801E8CC8(Fx1eaPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1eaVec3;

typedef struct {
    Pool6CEntry *unk0[11];
    SVECTOR unk2C; /* model rotation */
    Fx1eaVec3 unk34; /* model position */
    s32 pad40;
    Fx1eaVec3 unk44; /* model scale */
    s32 pad50;
    s32 unk54; /* model colour */
    u8 pad58[0x84 - 0x58];
    SVECTOR unk84; /* caster position */
    SVECTOR unk8C; /* caster rotation */
    s32 unk94; /* morph frame */
    s32 unk98; /* fade-in countdown */
    s32 unk9C;
    s32 unkA0;
    u16 unkA4[10]; /* models */
} Fx1eaWork81; /* 0xB8 */

typedef struct {
    Fx1eaPath unk0;
    s16 unk8; /* base */
    s16 unkA; /* count */
    s16 unkC; /* alpha */
    s16 unkE;
    s16 unk10; /* pad */
    u8 pad12[0x18 - 0x12];
} Fx1eaRibbon; /* 0x18 */

extern u16 D_801E8D5C[];
extern u16 D_801E8D68[];
extern u16 D_801E8D74[];
extern u16 D_801E8D80[];
extern u16 D_801E8D84[];
extern BtlModel D_801E8DE8[];
extern Fx1eaRibbon D_801E95B8;
extern Fx1eaRibbon D_801E95D8;
extern Fx1eaPath_0b4 D_801E95F0;
extern Fx1eaPath_0b4 D_801E95F8[];
extern u8 D_801E9628[][0x100];
extern u8 D_801E9828[][0x88];

void func_801E7804(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad);
void func_801E79D0(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad, s32 ang);
s32 func_801E8B44(s32 from, s32 to, s32 t, s32 n);
void func_801E8C34(Fx1eaPath *path, u16 *data);
void func_801E8CB0(Fx1eaPath *path, u16 *data);

#endif
