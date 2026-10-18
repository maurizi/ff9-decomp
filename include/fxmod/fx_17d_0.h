#ifndef FX_17D_0_H
#define FX_17D_0_H

/* Effect module fx_17d_0 (dir 13 file 0x17D). */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4; /* line index */
    SVECTOR unk8;
    s16 unk10;
} Fx17d0Part;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4; /* key file */
    s16 unk6; /* morph key file */
} Fx17d0Piece;

extern Fx17d0Piece D_801E9370[21];
extern Unk800F78A4 *D_801E9470;
extern u8 D_801EBC90[][0x24];

void func_801E7704(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b);
void func_801E7894(s32 n, s32 r, SVECTOR *pts);
void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr);
void func_801E7D18(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s32 models, s16 *morph, s32 scale);
s32 fx_17d_0_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
s32 func_801E7FE0(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);

/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d0Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* summon position */
    SVECTOR unk10; /* pieces origin */
    Fx17d0Vec3 unk18; /* model position */
    u8 pad24[0x30 - 0x24];
    SVECTOR unk30;
    SVECTOR unk38; /* model rotation */
    SVECTOR unk40; /* spawn position */
    SVECTOR unk48; /* camera */
    u8 pad50[0x58 - 0x50];
    s32 unk58; /* spark count */
    s16 unk5C[21]; /* piece models */
    s16 unk86[21]; /* piece morph models */
    s16 unkB0[5]; /* body models */
    s16 unkBA; /* model */
} Fx17d0Work80; /* 0xBC */

typedef struct {
    u8 pad0[0x18];
    u16 unk18[6];
    u16 unk24[6];
    u16 unk30[6];
} Fx17d0SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx17d0SummonData *unk54;
} Fx17d0Summon;

extern SVECTOR D_801E9368;
extern s16 D_801E9418[];
extern s16 D_801E9424[];
extern SVECTOR D_801E9480[];
extern BtlModel D_801EA380[];
extern VECTOR D_801EBC80;

#endif
