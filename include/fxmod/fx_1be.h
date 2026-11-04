#ifndef FX_1BE_H
#define FX_1BE_H

/* Effect module fx_1be (dir 13 file 0x1be): Atomos, short summon. Evidence: btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

/* fx_1be effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_ATOMOS_PURPLE_RINGS 0x1 /* emitter: white / magenta / purple and pale yellow / grey gouraud rings (byte-identical to fx_0b8 entry 0x21) */
#define FX_ATOMOS_FAN_EMITTER  0x2 /* emitter: fan elements (not rendered; byte-identical to fx_0b8 entry 0x7) (look pending) */
#define FX_ATOMOS_VIOLET_RING  0x4 /* emitter: white-violet and white-blue gouraud rings */
#define FX_ATOMOS_ROCK_DEBRIS  0x5 /* emitter: orbiting grey-blue rock debris particles, four elements */
#define FX_ATOMOS_BLUE_SPECKS  0x6 /* emitter: tiny blue speck sprites */
#define FX_ATOMOS_FIRE_RINGS   0x7 /* emitter: violet-to-navy and white-crimson gouraud rings + rings textured with a pink / gold / blue fire circle */
#define FX_ATOMOS_NAVY_RINGS   0x8 /* emitter: lilac / navy gouraud rings + small violet sprite */
#define FX_ATOMOS_VORTEX_TORUS 0x9 /* 'so' model: torus, dark violet cloud texture */
#define FX_ATOMOS_ORANGE_RING  0xA /* 'so' model: flat ring, orange cloud texture */
#define FX_ATOMOS_SPHERE       0xC /* 'so' model: segmented sphere (texture page 0x117 renders plain white) (look pending) */
#define FX_ATOMOS_VORTEX_DOME  0xD /* 'so' model: dome, violet and cyan cloud texture */
#define FX_ATOMOS_RING_FUNNEL  0xE /* 'so' model: gouraud ring funnel, white glow at the hole */
#define FX_ATOMOS_BACKDROP     0xF /* table: backdrop layers (btlfx_drawBackdrop) */
#define FX_ATOMOS_GREY_RINGS   0x10 /* emitter: white-to-grey gouraud rings, 8 frames */

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
    u8 counters0[0x20]; /* counters and flags, by index (cleared at init) */
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
    SVECTOR unkCC; /* write-only (0, -0x2000, 0x418) */
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
extern s32 fx_1be_scrollInitWord;
extern s32 fx_1be_scrollPos;
extern s32 fx_1be_scrollSpeed;
extern Fx1beUnit fx_1be_units[];
static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to);
void fx_1be_drawGradientGrid(SVECTOR *pos, SVECTOR *rot, s32 bright, s32 tpage, s32 clut, s32 abr);
static s32 fx_1be_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
