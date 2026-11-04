#ifndef FX_12B_H
#define FX_12B_H

/* Effect module fx_12b (dir 13 file 0x12B): enemy Photon (Ark). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_12b effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata.md; "(look pending)" = by kind, appearance not settled). */
#define FX_PHOTON_SMOKE_PUFF    0x8 /* emitter: yellow-to-blue smoke puffs at each shard, + sub sprites (texture not in the file) */
#define FX_PHOTON_LIGHT_STREAK  0x9 /* emitter: orbiting fan, a yellow-to-cyan light streak */
#define FX_PHOTON_ENERGY_ORB    0xA /* emitter: golden-green energy sphere */
#define FX_PHOTON_YELLOW_RING   0xB /* emitter: dark-to-bright yellow gouraud rings */
#define FX_PHOTON_GOLD_RING     0xC /* emitter: dull gold gouraud rings */
#define FX_PHOTON_PATH_KEYS_2   0x21 /* table: position path keys for fxm_posPathInit (second shard path) */
#define FX_PHOTON_PATH_KEYS     0x22 /* table: position path keys for fxm_posPathInit (first shard path) */
#define FX_PHOTON_SPARKS        0x23 /* emitter: white dot sparks */
#define FX_PHOTON_LIGHT_TRAIL   0x24 /* emitter: white glow screen trail */
#define FX_PHOTON_LIGHT_TRAIL_3 0x25 /* emitter: white glow screen trail */
#define FX_PHOTON_LIGHT_TRAIL_2 0x26 /* emitter: white glow screen trail */
#define FX_PHOTON_LIGHT_TRAIL_4 0x27 /* emitter: white glow screen trail */
#define FX_PHOTON_STAR_FLASH_3  0x28 /* emitter: white star glow fading to yellow */
#define FX_PHOTON_STAR_FLASH_4  0x29 /* emitter: white star glow fading to yellow */
#define FX_PHOTON_STAR_FLASH    0x37 /* emitter: white star glow fading to yellow */
#define FX_PHOTON_STAR_FLASH_2  0x38 /* emitter: white star glow fading to yellow */

typedef struct {
    Pool6CEntry *emit0[5];
    SVECTOR pos14; /* target body position */
    SVECTOR pos1C; /* target position */
} Fx12bWork82;

/* particle as the path callbacks see it */
typedef struct {
    VECTOR pos0; /* position */
    u8 pad10[0x1F - 0x10];
    u8 index1F; /* index (1-based) */
} Fx12bParticle;

typedef struct {
    SVECTOR pos0; /* end */
    SVECTOR pos8; /* start */
    s16 width10; /* side sway */
    s16 height12; /* arc height */
    s16 unk14; /* write-only (cleared) */
    u8 pad16[0x18 - 0x16];
    u16 frame18; /* start frame */
    u16 bone1A; /* bone */
    u16 dist1C;
    u8 pad1E[0x20 - 0x1E];
    u16 frame20; /* state */
    u16 unk22; /* unused */
} Fx12bShard; /* 0x24 */

typedef struct {
    s16 x, y, z;
} Fx12bPoint;

typedef struct {
    s16 unk0; /* not read by the module */
    u16 count2; /* key count */
    Fx12bPoint *keys4; /* keys */
} Fx12bPath;

extern Fx12bShard fx_12b_op80Shards0[];
extern Fx12bShard fx_12b_op80Shards1[];
extern Fx12bShard fx_12b_op81Shards0[];
extern Fx12bShard fx_12b_op81Shards1[];
extern Unk800F78A4 *fx_12b_env;
extern s32 fx_12b_op81Trails0[];
extern s32 fx_12b_op81Trails1[];
extern SVECTOR fx_12b_emitPos0;
extern SVECTOR fx_12b_emitPos1;
extern SVECTOR fx_12b_emitPos2;

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target body position */
} Fx12bWork81; /* 0x18 */
extern Fx12bPath fx_12b_op80PosPaths0[];
extern Fx12bPath fx_12b_op80PosPaths1[];
extern Fx12bPath fx_12b_op81PosPath0;
extern Fx12bPath fx_12b_op81PosPath1;
extern SVECTOR fx_12b_bonePoints0[];
extern SVECTOR fx_12b_bonePoints1[];

static s32 fx_12b_op80ParticleMotion0(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion1(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion0(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion1(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
s32 fx_12b_pathParticle(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, Fx12bPath *path, SVECTOR *ofs, s32 frame, s32 n);
s32 fx_12b_arcParticle0(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, s32 frame, s32 n);
s32 fx_12b_arcParticle1(Fx12bParticle *pt, SVECTOR *pos, Fx12bShard *shard, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion2(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op80ParticleMotion3(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion2(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fx_12b_op81ParticleMotion3(Fx12bParticle *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);
static s32 fxm_posPathGet(Fx12bPath *path, s32 i, SVECTOR *out);
static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n);

static void fxm_posPathInit(Fx12bPath *path, u16 *data);
s32 fx_12b_op81(s32 mode, Fx12bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args);
static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value);

/* propagated */
static s32 fxm_pinParticle(Fx12bParticle *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n);

typedef struct {
    Pool6CEntry *emit0[4];
    SVECTOR pos10; /* target body position */
} Fx12bWork80; /* 0x18 */

extern u16 fx_12b_pathFileIds0[6];
extern u16 fx_12b_pathFileIds1[6];
extern u8 fx_12b_op80Trails0[];
extern u8 fx_12b_op80Trails1[];

#endif
