#ifndef FX_123_H
#define FX_123_H

/* Effect module fx_123 (dir 13 file 0x123): enemy Raining Swords (Maliris). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_123 effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s4.md; "(look pending)" = by kind, appearance not settled). */
#define FX_RAINING_SWORDS_SPECKS        0x4 /* emitter: cyan dot specks */
#define FX_RAINING_SWORDS_SWORD_RING    0x5 /* model: six swords in a star ring (weapon texture) */
#define FX_RAINING_SWORDS_SWORD         0x7 /* model: single sword (weapon texture) */
#define FX_RAINING_SWORDS_CYAN_FLAMES   0x8 /* emitter: cyan spirit flames + speck sprites */
#define FX_RAINING_SWORDS_CYAN_FLAMES_2 0x9 /* emitter: cyan spirit flame burst + speck sprites */
#define FX_RAINING_SWORDS_FLAME_SPIRAL  0xA /* emitter: cyan spirit flames spiralling + rings */
#define FX_RAINING_SWORDS_CYAN_FLAMES_3 0xB /* emitter: cyan spirit flames + speck sprites */
#define FX_RAINING_SWORDS_CYAN_SPECKS   0xC /* emitter: rings and sprites, tiny cyan specks at run time, + cyan dot specks; runtime (docs/runtime-findings.md effect-data pass 2, low) (look pending: low) */
#define FX_RAINING_SWORDS_CYAN_RINGS    0xD /* emitter: cyan to white gouraud rings, 16 frames */
#define FX_RAINING_SWORDS_TEAL_RINGS    0xE /* emitter: teal gouraud rings + a fan element */
#define FX_RAINING_SWORDS_CYAN_RINGS_2  0xF /* emitter: cyan and teal gouraud rings, 8 frames */
#define FX_RAINING_SWORDS_BLUE_RINGS    0x20 /* emitter: navy to blue gouraud rings, 24 frames */

/* fx_123 choreography (docs/fxdata.md, "Choreography literals"): op80's flames on the caster's six bones, op81's
 * sword ring and the six swords' paths and shots, op82's rings, op83's white flash. */
#define FX_RAINING_SWORDS_COUNT            6    /* swords (and per-sword emitters, paths, bones, trail buffers) */
#define FX_RAINING_SWORDS_FLAMES_FOLLOW_FRAMES 0x12 /* op80: CYAN_FLAMES_2 (frame 0) follow the caster's bones for this many frames */
#define FX_RAINING_SWORDS_FLAMES_2_FRAME   20   /* a second set of CYAN_FLAMES_2 starts and follows the bones */
#define FX_RAINING_SWORDS_FLAMES_3_FRAME   30   /* CYAN_FLAMES_3 start and follow the bones */
#define FX_RAINING_SWORDS_FLAMES_STOP_FRAME 38  /* the second set stops */
#define FX_RAINING_SWORDS_FLAMES_3_END_FRAME 0x37 /* CYAN_FLAMES_3 follow the bones until this frame */
#define FX_RAINING_SWORDS_SPECKS_FRAME     43   /* SPECKS start in front of the caster */
#define FX_RAINING_SWORDS_SUMMON_FRAMES    0x50 /* op80's length */
#define FX_RAINING_SWORDS_MODEL_SLOTS      (FX_RAINING_SWORDS_COUNT + 1) /* op81: btlfx_initEffModels slots, the swords and SWORD_RING */
#define FX_RAINING_SWORDS_SWORD_TPAGE      0x97   /* Hi_RegisterTexEffModel texture of SWORD and SWORD_RING: 8-bit page (0x1C0, 0x100), abr 0 */
#define FX_RAINING_SWORDS_SWORD_CLUT       0x3D40 /* CLUT at (0x0, 0xF5) */
#define FX_RAINING_SWORDS_SWORD_U          0x80   /* texture origin (u, v) in the page */
#define FX_RAINING_SWORDS_SWORD_V          0
#define FX_RAINING_SWORDS_RING_HEIGHT      0x1013 /* SWORD_RING (and CYAN_RINGS_2, the camera point) this high above the caster */
#define FX_RAINING_SWORDS_RING_BACK        0x5F5  /* and this far towards the party */
#define FX_RAINING_SWORDS_RING_FRAME       3    /* SWORD_RING draws from this frame */
#define FX_RAINING_SWORDS_RING_END_FRAME   11   /* until this one; the swords' paths hold meanwhile; CYAN_FLAMES start on the swords */
#define FX_RAINING_SWORDS_RING_PATH_FRAME  3    /* the path frame the swords hold while the ring shows */
#define FX_RAINING_SWORDS_SHOT_FRAME       19   /* each sword's shot path (from above the target outwards) is set */
#define FX_RAINING_SWORDS_SWORDS_FRAMES    0x32 /* the swords draw while frame < this */
#define FX_RAINING_SWORDS_PATH_FRAMES      11   /* a sword follows its file path for this many (shot) frames */
#define FX_RAINING_SWORDS_SHOT_DELAY       13   /* then, frame1A + this many frames in, it flies its shot */
#define FX_RAINING_SWORDS_SHOT_FLY_FRAMES  4    /* over this many frames (the << 10 lerp); its emitter starts */
#define FX_RAINING_SWORDS_SLICE_FRAME      16   /* from frame1A + this it draws sliced at the ground */
#define FX_RAINING_SWORDS_SHOT_FADE_FRAME  21   /* at frame1A + this it fades out */
#define FX_RAINING_SWORDS_SHOT_FADE_FRAMES 4    /* over this many frames */
#define FX_RAINING_SWORDS_SHOT_TRAIL_SLOTS  2   /* the shot emitters' trail buffers (fx_123_op81Trails): slots */
#define FX_RAINING_SWORDS_SHOT_TRAIL_STRIDE 0x70 /* bytes per slot */
#define FX_RAINING_SWORDS_FLAMES_END_FRAME 29   /* CYAN_FLAMES follow the swords until this frame */
#define FX_RAINING_SWORDS_RAIN_FRAMES      0x50 /* op81's length */
#define FX_RAINING_SWORDS_RINGS_FRAME      0xC  /* op82: CYAN_RINGS and CYAN_SPECKS start (TEAL_RINGS, SPECKS, BLUE_RINGS on frame 0) */
#define FX_RAINING_SWORDS_HIT_FRAMES       0x1E /* op82's length */
#define FX_RAINING_SWORDS_FLASH_IN_FRAMES  0x10 /* op83: the screen brightens to white from frame 0 over this many frames */
#define FX_RAINING_SWORDS_FLASH_OUT_FRAME  20   /* and back from this frame */
#define FX_RAINING_SWORDS_FLASH_OUT_FRAMES 0x10 /* over this many */
#define FX_RAINING_SWORDS_FLASH_FRAMES     0x20 /* op83's length */

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR pos14; /* centre of the units */
} Fx123Work82; /* 0x1C */

typedef struct {
    s16 timer0; /* fade-in countdown */
    s16 timer2; /* fade-out countdown */
} Fx123Work83; /* 0x4 */

typedef struct {
    Pool6CEntry *emit0[FX_RAINING_SWORDS_COUNT];
    Pool6CEntry *emit18[FX_RAINING_SWORDS_COUNT];
    Pool6CEntry *emit30[FX_RAINING_SWORDS_COUNT];
    Pool6CEntry *emit48;
    SVECTOR pos4C; /* caster position */
    SVECTOR pos54;
    SVECTOR pos5C[FX_RAINING_SWORDS_COUNT]; /* bone positions */
} Fx123Work80; /* 0x8C */

typedef struct {
    u16 bone0; /* bone */
    u16 dist2; /* offset */
} Fx123Bone;

extern Fx123Bone fx_123_op80Bones[FX_RAINING_SWORDS_COUNT];
extern Unk800F78A4 *fx_123_env;
extern SVECTOR fx_123_emitPos0[FX_RAINING_SWORDS_COUNT];
extern SVECTOR fx_123_emitPos1[FX_RAINING_SWORDS_COUNT];

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

typedef struct {
    s16 x, y, z;
} Fx123Point;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* count */
    Fx123Point *keys4;
} Fx123Path;
static void fxm_rotPosPathInit(Fx123Path *path, u16 *data);
static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot);

/* propagated */
extern SVECTOR fx_123_pinPos0[]; /* particle positions */
static s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
extern SVECTOR fx_123_pinPos1[]; /* particle positions */
static s32 fxm_pinParticle5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle6(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle7(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle8(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle9(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle10(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fxm_pinParticle11(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

typedef struct {
    s16 pos0[3]; /* position */
    s16 rot6[3]; /* rotation */
} Fx123Key; /* 0xC */

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* keys */
    Fx123Key *keys4;
} Fx123Path_0b4;
static s32 fxm_rotPosPathGet(Fx123Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos);

typedef struct {
    Pool6CEntry *emit0[FX_RAINING_SWORDS_COUNT];
    u8 pad18[0x30 - 0x18];
    Pool6CEntry *emit30[FX_RAINING_SWORDS_COUNT];
    Pool6CEntry *emit48[FX_RAINING_SWORDS_COUNT];
    Pool6CEntry *emit60;
    SVECTOR pos64;
    SVECTOR rot6C[FX_RAINING_SWORDS_COUNT];
    VECTOR pos9C[FX_RAINING_SWORDS_COUNT];
    VECTOR scaleFC;
    s32 fade10C;
    SVECTOR pos110[FX_RAINING_SWORDS_COUNT];
    SVECTOR pos140;
    SVECTOR pos148[FX_RAINING_SWORDS_COUNT];
    s16 unk178; /* write-only (-1, then 4) */
    u16 model17A[FX_RAINING_SWORDS_COUNT];
    u16 model186;
} Fx123Work81; /* 0x188 */

typedef struct {
    SVECTOR pos0;
    SVECTOR rot8;
    SVECTOR pos10;
    u16 active18;
    u16 frame1A;
    s16 unk1C; /* unused */
    s16 timer1E;
    s16 angle20;
    s16 unk22; /* unused */
} Fx123Shot; /* 0x24 */

extern Fx123Shot fx_123_op81Shots[FX_RAINING_SWORDS_COUNT];
extern u16 fx_123_pathFileIds[FX_RAINING_SWORDS_COUNT];
extern u16 fx_123_emitFileIds[FX_RAINING_SWORDS_COUNT];
extern s32 fx_123_pinParticles[FX_RAINING_SWORDS_COUNT];
extern BtlModelBuf fx_123_effModels[];
extern Fx123Path_0b4 fx_123_rotPosPath[FX_RAINING_SWORDS_COUNT];
extern u8 fx_123_op81Trails[FX_RAINING_SWORDS_COUNT][FX_RAINING_SWORDS_SHOT_TRAIL_SLOTS * FX_RAINING_SWORDS_SHOT_TRAIL_STRIDE];
extern SVECTOR fx_123_emitPos2[FX_RAINING_SWORDS_COUNT];
extern SVECTOR fx_123_emitPos3[FX_RAINING_SWORDS_COUNT];

#endif
