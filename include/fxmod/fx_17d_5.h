#ifndef FX_17D_5_H
#define FX_17D_5_H

/* Effect module fx_17d_5 (dir 13 file 0x17d, group 5). */
#include "btlfx_mod.h"

typedef struct {
    s32 vx, vy, vz;
} Fx17d5Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* sparks spawned */
    SVECTOR unkC; /* spark origin */
    SVECTOR unk14; /* spark target */
    SVECTOR unk1C; /* spark angles */
    SVECTOR unk24;
    u8 pad2C[0x3C - 0x2C];
    SVECTOR unk3C; /* camera */
    Fx17d5Vec3 unk44; /* glow position */
    SVECTOR unk50; /* glow rotation */
    Fx17d5Vec3 unk58; /* glow scale */
    Fx17d5Vec3 unk64; /* summon position */
    Fx17d5Vec3 unk70; /* summon scale */
    SVECTOR unk7C; /* summon rotation */
    s32 unk84; /* models */
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
} Fx17d5Work80; /* 0x98 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* speed */
    s16 unk4; /* turn rate */
    s16 unk6; /* index */
    SVECTOR unk8; /* position */
    s16 unk10; /* angles; unk10 also the kind at spawn */
    s16 unk12;
    s16 unk14; /* homing done */
    s16 unk16;
    u8 *unk18; /* trail */
    s16 unk1C; /* target */
    s16 unk1E;
    s16 unk20;
    s16 unk22; /* trail width */
} Fx17d5Spark; /* 0x24 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x1A4];
        BtlUnit9CC *unk1A4;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx17d5Summon;

extern SVECTOR D_801EE4DC; /* aim point */
extern Unk800F78A4 *D_801EE52C;
extern BtlModel D_801EE53C[];
extern u8 D_801EE860[][0xCC]; /* spark trails */

s32 func_801EC704(s32 mode, Fx17d5Spark *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent);

#endif
