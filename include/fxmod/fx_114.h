#ifndef FX_114_H
#define FX_114_H

/* Effect module fx_114 (dir 13 file 0x114): Ifrit, full summon. Fire drawn as a grid of texel quads
 * (fxm_drawTexelQuadsB). Evidence: btlfx_start and btlfx_loadFile get the same id for summons
 * (docs/runtime-findings.md). */
#include "btlfx_mod.h"

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    SVECTOR rot8; /* model rotation */
    VECTOR pos10; /* model position */
    VECTOR scale20; /* model scale */
    s32 unk30; /* model brightness */
    SVECTOR pos34; /* summon bone position */
    SVECTOR pos3C;
    s16 unk44; /* fade-in countdown */
    u16 model46; /* model */
} Fx114Work81; /* 0x48 */

/* View of Unk800F78A44 up to the word fx_114 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x60];
    s32 unk60; /* summon model */
} Fx114Summon;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    SVECTOR unk2C;
    VECTOR unk34;
    VECTOR unk44;
    u8 pad54[0x58 - 0x54];
    SVECTOR pos58;
    s16 unk60; /* fade-in countdown */
    s16 unk62; /* fade-out countdown */
    u16 unk64; /* frame */
    u16 model66; /* model */
    u16 model68; /* model */
} Fx114Work82; /* 0x6C */


typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx114Key12;

typedef struct {
    u16 unk0; /* count */
    Fx114Key12 *unk4;
} Fx114Track12;
static void fxm_posTrackInit(Fx114Track12 *track, u16 *data);

typedef struct {
    s16 x, y, z;
} Fx114Key6;

typedef struct {
    u16 unk0; /* count */
    Fx114Key6 *unk4;
} Fx114Track6;
static s32 fxm_posTrackGet(Fx114Track6 *track, s32 t, SVECTOR *out);

typedef struct {
    s16 x, y, z;
} Fx114Point;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx114Point *unk4;
} Fx114Path;
static s32 fxm_posPathGet(Fx114Path *path, s32 t, SVECTOR *out);
static void fxm_rotPosTrackInit(Fx114Track12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecPathInit(Fx114Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx114Path *path, s32 t, VECTOR *out);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx114Path unk0;
    s16 unk8;
    s16 unkA;
    s16 unkC; /* brightness */
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Fx114Wave;

extern Unk800F78A4 *fx_114_env;
extern Fx114Wave D_801ED078;
extern Fx114Track6 fx_114_vecTrack3; /* scale keys */
extern u32 D_801ED1C8[];
extern u8 D_801EE658[13][0x50];

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
static void fxm_posPathInit(Fx114Path *path, u16 *data);
static void fxm_vecTrackInit(Fx114Track6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx114Track6 *track, s32 t, VECTOR *out);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot);

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */
extern SVECTOR D_801EF268[]; /* particle positions */
static s32 fxm_pinParticle0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

static void fxm_buildPathTrail(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static s32 fxm_offsetTableEntry(s32 base, s32 i);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx114Track; /* 0x10 */
extern Fx114Track D_801EF2B8[];
extern s32 D_801EF6B8[];
static void fxm_camChanReset(void);
static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data);
static void fxm_camChanUpdate(s32 ch);


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

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx114Vec3;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    SVECTOR unkC; /* rotation */
    SVECTOR unk14; /* position */
    s32 unk1C;
    SVECTOR rot20; /* body rotation */
    Fx114Vec3 unk28; /* body position */
    s32 pad34;
    Fx114Vec3 unk38; /* body scale */
    s32 pad44;
    SVECTOR rot48; /* rock rotation */
    Fx114Vec3 unk50; /* rock position */
    s32 pad5C;
    Fx114Vec3 unk60; /* rock scale */
    s32 pad6C;
    SVECTOR rot70;
    Fx114Vec3 unk78;
    s32 pad84;
    Fx114Vec3 unk88;
    s32 unk94;
    s32 unk98;
    SVECTOR rot9C;
    Fx114Vec3 unkA4;
    s32 padB0;
    Fx114Vec3 unkB4;
    s32 padC0;
    s32 unkC4;
    s32 unkC8;
    SVECTOR rotCC;
    Fx114Vec3 unkD4;
    s32 padE0;
    Fx114Vec3 unkE4;
    s32 padF0;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR unk104;
    s16 unk10C; /* frame */
    s16 unk10E;
    s16 unk110;
    s16 unk112;
    s16 unk114;
    s16 unk116;
    u16 unk118[24]; /* models */
    u16 unk148;
    u16 model14A;
    u16 model14C;
} Fx114Work83; /* 0x150 */

typedef struct {
    u8 unk0; /* start frame */
    u8 unk1; /* spin flags */
    u8 unk2; /* landed */
    u8 pad3;
    s16 unk4; /* speed */
    s16 unk6; /* lift */
    SVECTOR unk8; /* direction */
    SVECTOR unk10; /* rotation */
    Fx114Vec3 unk18; /* position */
    s32 pad24;
} Fx114Rock; /* 0x28 */

extern Fx114Rock D_801EB2F4[];
extern u8 D_801EB68C[];
extern u8 D_801EB6D4[];
extern s16 D_801EB6E0[];
extern u8 D_801EB6EC[];
extern u8 D_801EB6F4[];
extern s16 D_801EB6FC[];
extern s16 D_801EB704[];
extern u8 D_801EB70C[];
extern s16 D_801EB714[];
extern Fx114Track12 fx_114_rotPosTrack[];
extern Fx114Track12 fx_114_posTrack2;
extern Fx114Track6 fx_114_vecTrack4;
extern Fx114Track6 fx_114_vecTrack5;

/* Hi_RegisterTexListModel called with three arguments (the original leaves a3 unset) */
#define REG_TEX_LIST(ex) ((u16 (*)(void *, void *, void *))(ex)->Hi_RegisterTexListModel)

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *motion180[4]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx114Summon80;

typedef struct {
    Pool6CEntry *emit0;
    Pool6CEntry *emit4;
    Pool6CEntry *emit8;
    s32 padC;
    Pool6CEntry *emit10;
    Pool6CEntry *emit14;
    Pool6CEntry *emit18;
    Pool6CEntry *emit1C;
    SVECTOR rot20; /* summon rotation */
    Fx114Vec3 unk28; /* summon position */
    s32 pad34;
    Fx114Vec3 unk38; /* summon scale */
    s32 pad44;
    s32 unk48;
    SVECTOR unk4C;
    SVECTOR unk54;
    s32 unk5C;
    SVECTOR rot60;
    Fx114Vec3 unk68;
    s32 pad74;
    Fx114Vec3 unk78;
    s32 pad84;
    SVECTOR rot88;
    Fx114Vec3 unk90;
    s32 pad9C;
    Fx114Vec3 unkA0;
    s32 padAC;
    s32 unkB0;
    SVECTOR rotB4;
    Fx114Vec3 unkBC;
    s32 padC8;
    Fx114Vec3 unkCC;
    s32 padD8;
    s32 unkDC;
    SVECTOR rotE0;
    Fx114Vec3 unkE8;
    s32 padF4;
    Fx114Vec3 unkF8;
    s32 pad104;
    s32 unk108;
    SVECTOR rot10C;
    Fx114Vec3 unk114;
    s32 pad120;
    Fx114Vec3 unk124;
    s32 pad130;
    SVECTOR unk134;
    SVECTOR pos13C;
    SVECTOR unk144;
    SVECTOR pos14C;
    u8 pad154[8];
    u8 unk15C;
    u8 unk15D;
    u8 unk15E;
    u8 pad15F;
    s32 unk160;
    s32 unk164;
    s32 unk168;
    s16 unk16C;
    s16 unk16E;
    s16 unk170;
    s16 unk172;
    s16 unk174;
    s16 unk176;
    u16 model178;
    u16 model17A;
    u16 unk17C;
    u16 model17E;
    u16 model180;
    u8 pad182[0x198 - 0x182];
} Fx114Work80; /* 0x198 */

extern BtlModel fx_114_effModels[];
extern u8 D_801EB6A4[];
extern u8 D_801EB6BC[];
extern Fx114Path fx_114_posPath0;
extern Fx114Path fx_114_posPath1;
extern Fx114Path fx_114_posPath2;
extern Fx114Path fx_114_posPath3;
extern Fx114Track6 fx_114_posTrack0;
extern Fx114Track6 fx_114_posTrack1;
extern Fx114Path fx_114_posPath4;
extern Fx114Path fx_114_vecPath;
extern Fx114Track6 fx_114_vecTrack0;
extern Fx114Track6 fx_114_vecTrack1;
extern Fx114Track6 fx_114_vecTrack2;
extern SVECTOR fx_114_emitPos0;
extern SVECTOR fx_114_emitPos1[4];
extern u8 fx_114_op80Trails1[];
extern u8 fx_114_op80Trails2[];
extern u8 fx_114_op80Trails3[];
extern u8 fx_114_op80Trails4[];
extern u8 fx_114_op80Trails0[];
extern u16 D_801EEA68[];
extern u8 D_801EB71C[];

#endif
