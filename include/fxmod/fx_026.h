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
s32 func_801E92EC(s32 i);

#endif
