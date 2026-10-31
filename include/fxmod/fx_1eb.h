#ifndef FX_1EB_H
#define FX_1EB_H

/* Effect module fx_1eb (dir 13 file 0x1eb). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1ebPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1ebPoint *unk4;
} Fx1ebPath;

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1ebKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1ebKey *unk4;
} Fx1ebKeyPath;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR pos8; /* caster body position */
    SVECTOR rot10; /* caster rotation */
} Fx1ebWork80; /* 0x18 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    Pool6CEntry *emitC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    SVECTOR rot18; /* model rotation */
    VECTOR pos20; /* model position */
    VECTOR scale30; /* model scale */
    s32 unk40; /* brightness */
    SVECTOR pos44; /* target position */
    s32 unk4C; /* fade countdown */
    u16 unk50;
    u16 model52[3]; /* models */
    u16 model58; /* morph target */
} Fx1ebWork82; /* 0x60 */

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR pos4;
    s32 unkC; /* unit */
} Fx1ebWork83;

extern u16 D_801E8210[3]; /* model files */
extern u16 D_801E8218[3]; /* path files */
extern Unk800F78A4 *fx_1eb_env;
extern BtlModel fx_1eb_effModels[];
extern Fx1ebKeyPath D_801E8728[3];
extern Fx1ebPath D_801E8740;
extern s32 D_801E8748[];

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_rotPosPathInit(Fx1ebKeyPath *path, u16 *data);
static void fxm_valuePathInit(Fx1ebPath *path, u16 *data);

/* copied */
extern Unk800F78A4 *fx_1eb_env;
/* end copied */

/* propagated */
static s32 fxm_rotPosPathGet(Fx1ebKeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos);
static s32 fxm_valuePathGet(Fx1ebPath *keys, s32 i, s32 *out);

#endif
