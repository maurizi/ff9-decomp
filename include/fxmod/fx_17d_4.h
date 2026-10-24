#ifndef FX_17D_4_H
#define FX_17D_4_H

/* Effect module fx_17d_4 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E9890;
extern s16 D_801E97FC[16];
extern SVECTOR D_801E981C[3];
extern s16 D_801E9814[3];

typedef struct {
    s16 x, y, z;
} Fx17d4Pt;

/* propagated */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[2];
} Fx17d4Line;

extern Fx17d4Line D_801E987C;
void func_801E7720(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern s16 D_801E9724[18 * 4]; /* per strand: file, start, stride, width */
extern Fx17d4Line *D_801E97B4[18];
void func_801E77DC(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d4Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR unk10; /* particle position */
    u8 pad18[8];
    SVECTOR unk20; /* camera target */
    Fx17d4Vec3 unk28; /* model position */
    SVECTOR unk34; /* model rotation */
    Fx17d4Vec3 unk3C; /* model scale */
    Fx17d4Vec3 unk48; /* summon position */
    Fx17d4Vec3 unk54; /* summon scale */
    SVECTOR unk60; /* summon rotation */
    s32 unk68; /* models */
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 pad7C[8];
} Fx17d4Work80; /* 0x84 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x19C];
        BtlUnit9CC *unk19C;
        BtlUnit9CC *unk1A0;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx17d4Summon;

#define SUMMON(env) ((Fx17d4Summon *)(env)->unk4)

extern SVECTOR D_801E971C;
extern s32 D_801E989C[];
extern s8 D_801EB25C;
extern BtlModel D_801EB65C[];
void func_801E7B8C(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot);
void func_801E7EB4(Unk80100C78Entry *entry, SVECTOR *from, s32 ot);

#endif
