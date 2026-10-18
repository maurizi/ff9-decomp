#ifndef FX_026_H
#define FX_026_H

/* Effect module fx_026 (dir 13 file 0x26). */
#include "btlfx_mod.h"

/* keyframe track read from the effect file: count, then the keys */
typedef struct {
    s16 unk0;
    s16 unk2; /* count */
    s16 unk4[1]; /* keys: 1, 3, 6 or 9 halfwords each */
} Fx026Track;

/* shattered model: triangles (three vertices each) that fall off as particles */
typedef struct {
    SVECTOR unk0; /* pad: already split off */
    SVECTOR unk8;
    SVECTOR unk10;
} Fx026Tri; /* 0x18 */

typedef struct {
    s16 unk0;
    s16 unk2; /* triangle count */
    s32 unk4; /* triangles of the last model */
    Fx026Tri *unk8;
    Fx026Tri *unkC; /* next free triangle */
    Fx026Tri *unk10; /* first triangle of the last model */
} Fx026Shatter; /* 0x14 */

typedef struct {
    s32 unk0; /* position (fixed 20.12) */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14; /* spin */
    s16 unk16;
    s16 unk18; /* frame, -1 = free */
    s16 unk1A; /* triangle */
} Fx026Shard; /* 0x1C */

typedef struct {
    s32 unk0; /* camera path */
    SVECTOR unk4; /* camera target */
    SVECTOR unkC; /* camera position */
} Fx026Work87; /* 0x14 */

/* camera paths: position track, target track, length */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} Fx026CamPath;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx026Vec3;

/* pool item of func_801E7A5C: a model following a keyframe track */
typedef struct {
    s16 unk0; /* done */
    u16 unk2; /* spin */
    s16 unk4; /* track: D_801EA774 index */
    s16 pad6;
    Pool6CEntry *unk8;
    SVECTOR unkC;
} Fx026Item;

typedef struct {
    s32 unk0;
    s32 unk4; /* frame */
    u8 pad8[8];
    SVECTOR unk10;
    u8 pad18[0x10];
    SVECTOR unk28; /* summon bone positions */
    SVECTOR unk30;
    SVECTOR unk38;
    Fx026Vec3 unk40; /* summon position */
    Fx026Vec3 unk4C; /* summon scale */
    SVECTOR unk58; /* summon rotation */
    u8 pad60[4];
    s32 unk64;
    s32 unk68; /* models */
    s32 unk6C;
} Fx026Work80; /* 0x70 */

/* the summon slot behind env->unk4 */
typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx026Summon;

/* op82: the barrier model */
typedef struct {
    s32 unk0;
    s32 unk4; /* frame */
    SVECTOR unk8; /* position */
    s32 unk10; /* model */
} Fx026Work82; /* 0x14 */

/* pool item of func_801E8670 */
typedef struct {
    s16 unk0;
    s16 unk2; /* index */
    s16 unk4; /* type */
    s16 pad6;
    s32 unk8;
} Fx026Item82;

/* line strip passed to btlfx_drawTrail */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    u16 unk6; /* colour, 0 = gap */
} Fx026Pt;

typedef struct {
    u8 unk0; /* count - 1 */
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx026Pt unk4[32];
} Fx026Strip32; /* 0x104 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    Fx026Pt unk4[40];
} Fx026Strip40; /* 0x144 */

extern Fx026Strip32 D_801EC5EC[];
extern Fx026Strip32 D_801ECC0C[];
extern Fx026Strip40 D_801ED22C[];
/* op81: the shattering barrier */
typedef struct {
    s32 unk0;
    s32 unk4; /* frame */
    SVECTOR unk8; /* position */
    SVECTOR unk10;
    s32 unk18[6][3]; /* models of the six pieces */
    s32 pad60[3];
    s32 unk6C[7];
} Fx026Work81; /* 0x88 */

/* pool item of func_801E98BC */
typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* piece */
    s32 pad4[2];
    SVECTOR unkC;
} Fx026Item81;

typedef struct {
    s32 unk0; /* offsets from env->unk30 */
    s32 unk4;
} Fx026ShatterSrc;

extern s32 D_801EA788[6][3];
extern s32 D_801EA828[];
extern SVECTOR D_801EA840[];
extern s32 D_801EA870[];
extern Fx026ShatterSrc D_801EA7EC[];
extern s32 D_801EC5DC;
extern s32 D_801EC5E0;
extern Fx026Shatter D_801EDA0C[];
extern Fx026Tri D_801EF00C[];
extern s32 D_801EA7D0[7];
extern SVECTOR D_801EA780;
extern s32 D_801EA8B0[];
extern s32 D_801EA81C;
extern s32 D_801EA820;
extern s32 D_801EA824;
extern u8 D_801EA93C[];
extern s16 D_801EA774[];
extern BtlModel D_801EACDC[];
extern SVECTOR D_801EC5E4;
extern Fx026CamPath D_801EA8C4[];
extern Unk800F78A4 *D_801EACCC;
extern Fx026Shatter *D_801ED9CC[8];
extern Fx026Shard *D_801ED9EC[8];
extern s32 D_801EDA08; /* shatter line */

void func_801E7704(Fx026Track *track, s32 t, SVECTOR *out);
void func_801E7754(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b);
void func_801E77DC(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c);
s32 func_801E7890(Fx026Track *track, s32 t);
void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy);
void func_801E912C(s32 i, s32 n, Fx026Shatter *buf, Fx026Tri *tris);
s32 func_801E9170(s32 i, s32 model);
Fx026Shard *func_801E928C(s32 i);
void func_801E9564(s32 i);
s32 func_801E92EC(s32 i);
s32 func_801E98BC(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work);
s32 func_801E8670(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work);

#endif
