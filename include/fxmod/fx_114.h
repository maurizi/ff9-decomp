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
    s32 color30; /* model brightness */
    SVECTOR pos34; /* summon bone position */
    SVECTOR pos3C;
    s16 timer44; /* fade-in countdown */
    u16 model46; /* model */
} Fx114Work81; /* 0x48 */

/* View of Unk800F78A44 up to the word fx_114 reads (ovl_0b.h has it as padding). */
typedef struct {
    u8 pad0[0x60];
    s32 summonModel60; /* summon model */
} Fx114Summon;

typedef struct {
    Pool6CEntry *emit0;
    SVECTOR rot4; /* model rotation */
    VECTOR posC; /* model position */
    VECTOR scale1C; /* model scale */
    SVECTOR unk2C; /* write-only (0, 0, 0) */
    VECTOR unk34; /* write-only (0, 0, 0x320) */
    VECTOR unk44; /* write-only (0x1000 each) */
    u8 pad54[0x58 - 0x54];
    SVECTOR pos58;
    s16 timer60; /* fade-in countdown */
    s16 timer62; /* fade-out countdown */
    u16 frame64; /* frame */
    u16 model66; /* model */
    u16 model68; /* model */
} Fx114Work82; /* 0x6C */


typedef struct {
    s16 pos0, pos2, pos4; /* position */
    s16 rot6, rot8, rotA; /* rotation */
} Fx114Key12;

typedef struct {
    u16 count0; /* count */
    Fx114Key12 *keys4;
} Fx114Track12;
static void fxm_posTrackInit(Fx114Track12 *track, u16 *data);

typedef struct {
    s16 x, y, z;
} Fx114Key6;

typedef struct {
    u16 count0; /* count */
    Fx114Key6 *keys4;
} Fx114Track6;
static s32 fxm_posTrackGet(Fx114Track6 *track, s32 t, SVECTOR *out);

typedef struct {
    s16 x, y, z;
} Fx114Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx114Point *keys4;
} Fx114Path;
static s32 fxm_posPathGet(Fx114Path *path, s32 t, SVECTOR *out);
static void fxm_rotPosTrackInit(Fx114Track12 *track, u16 *data);
static s32 fxm_rotPosTrackGet(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos);
static void fxm_vecPathInit(Fx114Path *path, u16 *data);
static s32 fxm_vecPathGet(Fx114Path *path, s32 t, VECTOR *out);
static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);

typedef struct {
    Fx114Path path0;
    s16 start8;
    s16 countA;
    s16 fadeC; /* brightness */
    s16 scaleE;
    s16 fadeFrom10;
    s16 fadeTo12;
} Fx114Wave;

extern Unk800F78A4 *fx_114_env;
extern Fx114Wave fx_114_pathTrail;
extern Fx114Track6 fx_114_vecTrack3; /* scale keys */
extern u32 fx_114_bgOt[];
extern u8 fx_114_op82Trails[13][0x50];

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_posPathInit(Fx114Path *path, u16 *data);
static void fxm_vecTrackInit(Fx114Track6 *track, u16 *data);
static s32 fxm_vecTrackGet(Fx114Track6 *track, s32 t, VECTOR *out);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

/* propagated */
extern SVECTOR fx_114_pinPoints[]; /* particle positions */
static s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

static void fxm_buildPathTrail(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip);
static s32 fxm_offsetTableEntry(s32 base, s32 i);

typedef struct {
    s16 *keys0;
    s16 frame4; /* start frame */
    s16 timer6; /* frames left, -1 = free */
    s16 chan8; /* channel */
    u16 dstSlotsA; /* destination slots, 4 bits each */
    u16 indexC;
    u16 countE;
} Fx114Track; /* 0x10 */
extern Fx114Track fx_114_camKeys[];
#define FX_114_KEY_FREE (-1) /* timer6: slot not in use (fxm_*Reset sets it, fxm_*Add takes such a slot) */
extern s32 fx_114_camChanFrames[];
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
    SVECTOR unkC; /* write-only: rotation */
    SVECTOR pos14; /* position (only copied into unk104) */
    s32 unk1C; /* write-only (0x1800) */
    SVECTOR rot20; /* body rotation */
    Fx114Vec3 pos28; /* body position */
    s32 pad34;
    Fx114Vec3 scale38; /* body scale */
    s32 pad44;
    SVECTOR rot48; /* rock rotation */
    Fx114Vec3 pos50; /* rock position */
    s32 pad5C;
    Fx114Vec3 scale60; /* rock scale */
    s32 pad6C;
    SVECTOR rot70;
    Fx114Vec3 pos78;
    s32 pad84;
    Fx114Vec3 scale88;
    s32 unk94; /* unused */
    s32 color98;
    SVECTOR rot9C;
    Fx114Vec3 posA4;
    s32 padB0;
    Fx114Vec3 scaleB4;
    s32 padC0;
    s32 colorC4;
    s32 colorC8;
    SVECTOR rotCC;
    Fx114Vec3 posD4;
    s32 padE0;
    Fx114Vec3 scaleE4;
    s32 padF0;
    SVECTOR posF4;
    SVECTOR posFC;
    SVECTOR unk104; /* write-only: pos14 raised by 0xF0 */
    s16 frame10C; /* frame */
    s16 timer10E;
    s16 timer110;
    s16 timer112;
    s16 timer114;
    s16 timer116;
    u16 model118[24]; /* models */
    u16 model148;
    u16 model14A;
    u16 model14C;
} Fx114Work83; /* 0x150 */

typedef struct {
    u8 frame0; /* start frame */
    u8 flags1; /* spin flags */
    u8 done2; /* landed */
    u8 pad3;
    s16 speed4; /* speed */
    s16 vel6; /* lift */
    SVECTOR angle8; /* direction */
    SVECTOR rot10; /* rotation */
    Fx114Vec3 pos18; /* position */
    s32 pad24;
} Fx114Rock; /* 0x28 */

extern Fx114Rock fx_114_op83Pieces[];
extern u8 fx_114_op83TexSelect[];
extern u8 fx_114_texInfo1[];
extern s16 fx_114_texPos1[];
extern u8 fx_114_texInfo2[];
extern u8 fx_114_texInfo3[];
extern s16 fx_114_texPos2[];
extern s16 fx_114_texPos3[];
extern u8 fx_114_texInfo4[];
extern s16 fx_114_texPos4[];
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
        BtlMotion *motion180[4]; /* summon motions */
    } *summon54;
    u8 pad58[8];
    s32 summonModel60; /* summon model */
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
    Fx114Vec3 pos28; /* summon position */
    s32 pad34;
    Fx114Vec3 scale38; /* summon scale */
    s32 pad44;
    s32 color48;
    SVECTOR rot4C;
    SVECTOR pos54;
    s32 scale5C;
    SVECTOR rot60;
    Fx114Vec3 pos68;
    s32 pad74;
    Fx114Vec3 scale78;
    s32 pad84;
    SVECTOR rot88;
    Fx114Vec3 pos90;
    s32 pad9C;
    Fx114Vec3 scaleA0;
    s32 padAC;
    s32 colorB0;
    SVECTOR rotB4;
    Fx114Vec3 posBC;
    s32 padC8;
    Fx114Vec3 scaleCC;
    s32 padD8;
    s32 colorDC;
    SVECTOR rotE0;
    Fx114Vec3 posE8;
    s32 padF4;
    Fx114Vec3 scaleF8;
    s32 pad104;
    s32 color108;
    SVECTOR rot10C;
    Fx114Vec3 pos114;
    s32 pad120;
    Fx114Vec3 scale124;
    s32 pad130;
    SVECTOR pos134;
    SVECTOR pos13C;
    SVECTOR unk144; /* write-only (0, 0, -0x4B0) */
    SVECTOR pos14C;
    u8 pad154[8];
    u8 color15C;
    u8 color15D;
    u8 color15E;
    u8 pad15F;
    s32 index160;
    s32 loopMotion164; /* Hi_DrawSummonModel flags bit 0: 1 loops Ifrit's motion (before frame 0x30), 0 holds its last frame */
    s32 frame168;
    s16 frame16C;
    s16 timer16E;
    s16 timer170;
    s16 timer172;
    s16 timer174;
    s16 timer176;
    u16 model178;
    u16 model17A;
    u16 model17C;
    u16 model17E;
    u16 model180;
    u8 pad182[0x198 - 0x182];
} Fx114Work80; /* 0x198 */

extern BtlModelBuf fx_114_effModels[];
extern u8 fx_114_texInfo0[];
extern u8 fx_114_texPos0[];
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
extern u16 fx_114_texelColors[];
extern u8 fx_114_pinBones[];

#endif
