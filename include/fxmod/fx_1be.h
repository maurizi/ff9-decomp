#ifndef FX_1BE_H
#define FX_1BE_H

/* Effect module fx_1be (dir 13 file 0x1be): Atomos, short summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    u8 u;
    u8 v;
} Fx1beUV;

extern SVECTOR fx_1be_gridColors[9];
extern SVECTOR fx_1be_op80ParticleEnds[64];

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1be_env;
/* end copied */

typedef struct {
    u8 unk0[0x20]; /* counters */
    Pool6CEntry *emit20[16];
    s32 model60; /* model */
    VECTOR pos64; /* position */
    VECTOR scale74; /* scale */
    SVECTOR rot84; /* rotation */
    SVECTOR pos8C;
    s32 model94; /* models */
    s32 model98;
    SVECTOR rot9C;
    s32 modelA4;
    s32 modelA8;
    VECTOR scaleAC;
    SVECTOR posBC;
    SVECTOR posC4;
    SVECTOR unkCC;
    SVECTOR posD4;
} Fx1beWork80; /* 0xDC */

typedef struct {
    SVECTOR rot0; /* rotation */
    VECTOR scale8; /* scale */
    SVECTOR pos18; /* home position */
    SVECTOR rot20; /* rotation */
    SVECTOR spin28; /* spin */
    s32 timer30;
    s32 timer34; /* delay */
} Fx1beUnit; /* 0x38 */

extern BtlModelBuf fx_1be_effModels[];
extern s32 D_801E95D0;
extern s32 fx_1be_scrollPos;
extern s32 fx_1be_scrollSpeed;
extern Fx1beUnit fx_1be_units[];
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
void fx_1be_drawGradientGrid(SVECTOR *pos, SVECTOR *rot, s32 step, s32 tpage, s32 clut, s32 abr);
static s32 fx_1be_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames);

#endif
