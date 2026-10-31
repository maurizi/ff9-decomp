#ifndef FX_1BD_H
#define FX_1BD_H

/* Effect module fx_1bd (dir 13 file 0x1bd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1bdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1bdPoint *unk4;
} Fx1bdPath;

typedef struct {
    s16 x, y, z;
} Fx1bdKey6;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey6 *unk4;
} Fx1bdTrack6;

typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx1bdKey12;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey12 *unk4;
} Fx1bdTrack12;

extern BtlModel fx_1bd_effModels[];


typedef struct {
    Fx1bdPath unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC; /* alpha */
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Fx1bdLine;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* rotation */
    VECTOR posC; /* position */
    VECTOR scale1C; /* scale */
    SVECTOR unk2C;
    VECTOR unk34;
    VECTOR unk44;
    u8 pad54[4];
    SVECTOR pos58;
    s16 unk60; /* fade-in countdown */
    s16 unk62; /* fade-out countdown */
    u16 unk64; /* frame */
    u16 model66; /* model */
    u16 model68; /* model */
} Fx1bdWork82; /* 0x6C */

extern Fx1bdLine D_801EB764;
extern u8 D_801EBC54[13][0x50]; /* trails */
extern u32 D_801EB854[];

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);
static void fxm_posPathInit(Fx1bdPath *path, u16 *data);
static s32 fxm_posPathGet(Fx1bdPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *fx_1bd_env;
/* end copied */

/* propagated */

static void fxm_buildPathTrail(Fx1bdPath *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static u16 *fxm_offsetTableEntry(u16 *base, u16 i);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1bdTrack; /* 0x10 */
extern Fx1bdTrack D_801EC064[];
extern s32 D_801EC464[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);

typedef struct {
    SVECTOR unk0; /* rotation */
    VECTOR unk8; /* position */
    VECTOR unk18; /* scale */
} Fx1bdModel; /* 0x28 */

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC;
    SVECTOR unk14;
    s32 unk1C;
    Fx1bdModel unk20;
    Fx1bdModel unk48;
    Fx1bdModel unk70;
    s32 unk98;
    Fx1bdModel unk9C;
    s32 unkC4;
    s32 unkC8;
    Fx1bdModel unkCC;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR unk104;
    s16 unk10C; /* frame */
    s16 unk10E; /* fade countdowns */
    s16 unk110;
    s16 unk112;
    s16 unk114;
    s16 unk116;
    u16 unk118[24]; /* models */
    u16 unk148;
    u16 model14A;
    u16 model14C;
} Fx1bdWork83; /* 0x150 */

typedef struct {
    u8 unk0; /* start frame */
    u8 unk1; /* flags */
    u8 unk2; /* done */
    u8 pad3;
    s16 unk4; /* speed */
    s16 unk6; /* lift */
    SVECTOR unk8; /* direction */
    SVECTOR unk10; /* rotation */
    VECTOR pos18; /* position */
} Fx1bdRock; /* 0x28 */

extern Fx1bdRock D_801E9A14[23];
extern u8 D_801E9DAC[];
extern s16 D_801E9DC4[];
extern s16 D_801E9DD0[];
extern s16 D_801E9DDC[];
extern s16 D_801E9DE4[];
extern s16 D_801E9DEC[];
extern s16 D_801E9DF4[];
extern s16 D_801E9DFC[];
extern s16 D_801E9E04[];
extern Fx1bdTrack6 D_801EB77C;
extern Fx1bdTrack12 D_801EB784[23];
extern Fx1bdTrack6 D_801EB83C;
extern Fx1bdTrack6 D_801EB844;
extern Fx1bdTrack6 D_801EB84C;

static void fxm_posTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_posTrackGet(Fx1bdTrack6 *track, s32 t, SVECTOR *out);
static void fxm_rotPosTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx1bdTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecTrackInit(Fx1bdTrack12 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx1bdTrack6 *track, s32 t, VECTOR *out);


/* PsyQ gte_stszotz: SZ3 / 4 */
#define gte_stszotz(r0) __asm__ volatile( \
    "mfc2\t$12, $19;" \
    "nop;" \
    "sra\t$12, $12, 2;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")
/* VZ0 = VZ1 = VZ2 = 0 */
#define gte_ldVZ012_0() __asm__ volatile( \
    "mtc2\t$0, $1;" \
    "mtc2\t$0, $3;" \
    "mtc2\t$0, $5")
/* V0..V2 x and y only (VZ0..VZ2 are loaded once with gte_ldVZ012_0) */
#define gte_ldv3xy(r0, r1, r2) __asm__ volatile( \
    "lwc2\t$0, 0(%0);" \
    "lwc2\t$2, 0(%1);" \
    "lwc2\t$4, 0(%2)" \
    : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0xy(r0) __asm__ volatile( \
    "lwc2\t$0, 0(%0)" \
    : : "r"(r0))
/* gte_rtps_b with a memory clobber: fx_114's copy of the fx_007 colour grid reloads c3 after it */
#define gte_rtps_bm() __asm__ volatile(".word\t0x4A180001" : : : "memory")
static void fxm_drawTexelQuadsB(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip);

#endif
