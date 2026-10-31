#ifndef FX_105_H
#define FX_105_H

/* Effect module fx_105 (dir 13 file 0x105): Odin, full summon. Ribbon trails (fxm_drawRibbonTrail). Evidence:
 * btlfx_start and btlfx_loadFile get the same id for summons (docs/runtime-findings.md). */
#include "btlfx_mod.h"

extern Unk800F78A4 *fx_105_env;
typedef struct {
    s16 vx, vy, vz, pad, unk8, unkA;
} Fx105Spark; /* 0xC */

extern Fx105Spark fx_105_quads[];

void fx_105_drawScreenBand(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb);
void fx_105_initQuads(void);
void fx_105_drawQuads(s32 c);
void fx_105_drawWorldRibbon(s16 *data, s32 c, s32 c2, s32 n);
void fx_105_drawCamRibbon(s16 *data, s32 c, s32 h, s32 n);

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
    s32 z;
} Fx105RibbonPt; /* 0xC */

extern Fx105RibbonPt fx_105_worldRibbonPts[];

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
} Fx105BandPt; /* 8 */

extern Fx105BandPt fx_105_camRibbonPts[];

typedef struct {
    SVECTOR pos0;
    SVECTOR pos8;
} Fx105Seg; /* 0x10 */
static void fxm_drawRibbonTrail(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx105Seg *buf, s32 abr);

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[8]; /* summon motions */
    } *summon54;
    u8 pad58[8];
    s32 summonModel60; /* summon model */
} Fx105Summon;

typedef struct {
    s32 visible0; /* summon visible */
    s32 timer4; /* countdowns */
    s32 timer8;
    s32 timerC;
    s32 flag10;
    s32 timer14;
    s32 timer18;
    s32 timer1C;
    s32 timer20;
    s32 timer24;
    s32 timer28;
    s32 timer2C;
    s32 timer30;
    s32 pad34[3];
    Pool6CEntry *emit40;
    Pool6CEntry *emit44;
    Pool6CEntry *emit48;
    Pool6CEntry *emit4C;
    Pool6CEntry *emit50;
    Pool6CEntry *emit54;
    SVECTOR pos58;
    SVECTOR pos60;
    SVECTOR pos68;
    SVECTOR pos70;
    SVECTOR pos78;
    SVECTOR pos80;
    SVECTOR pos88;
    SVECTOR pos90;
    SVECTOR pos98;
    SVECTOR posA0;
    Pool6CEntry *emitA8;
    Pool6CEntry *emitAC;
    Pool6CEntry *emitB0;
    SVECTOR posB4;
    SVECTOR posBC;
    SVECTOR posC4;
    SVECTOR rotCC;
    Pool6CEntry *emitD4;
    Pool6CEntry *emitD8;
    Pool6CEntry *emitDC;
    Pool6CEntry *emitE0;
    SVECTOR posE4;
    SVECTOR posEC;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR pos104;
    SVECTOR pos10C;
    SVECTOR pos114;
    CVECTOR color11C;
    CVECTOR color120;
    Pool6CEntry *emit124;
    SVECTOR pos128;
    SVECTOR pos130;
    s32 model138[8]; /* models */
    s32 pos158[3];
    s32 pad164;
    SVECTOR rot168;
    s32 scale170[3];
    s32 pad17C;
    s32 model180;
    s32 model184;
    s32 model188;
    s32 model18C;
    SVECTOR rot190;
    s32 pos198[3];
    s32 pad1A4;
    s32 scale1A8[3];
    s32 pad1B4;
    s32 hold1B8[8];
} Fx105Work80; /* 0x1D8 */

typedef struct {
    s32 posX0; /* x */
    s32 posY4; /* y */
    s32 speed8; /* speed */
    s32 padC;
    s32 flicker10;
} Fx105Drop; /* 0x14 */

typedef struct {
    s32 timer0; /* delay */
    s32 timer4; /* frames */
} Fx105Flash; /* 8 */

extern Fx105Flash fx_105_modelTimers[8];
extern Fx105Seg fx_105_ribbonSegs[];
extern SVECTOR fx_105_modelRot;
extern VECTOR fx_105_modelPos;
extern VECTOR fx_105_modelScale;
extern BtlModel fx_105_effModels[];
extern Fx105Drop fx_105_screenSprites[32];
static void fxm_camKeyUpdate(void);

#define SUMMON105(env) ((Fx105Summon *)(env)->loader)

/* propagated */

typedef struct {
    s32 frame0; /* start frame */
    s32 timer4; /* frames left, -1 = free */
    u32 dstSlots8; /* destination slots, 4 bits each (1-based) */
    u32 srcSlotsC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *keys10; /* data */
} Fx105Track; /* 0x14 */

extern s32 fx_105_camFrame; /* frame */
extern Fx105Track fx_105_camKeys[];
static void fxm_camKeyReset(void);
static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR fx_105_camKeyPoints[];
static void fxm_camKeyUpdate(void);

#endif
