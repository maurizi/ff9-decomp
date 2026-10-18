#ifndef FX_0DF_H
#define FX_0DF_H

/* Effect module fx_0df (dir 13 file 0x0DF). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* time */
    SVECTOR unk8; /* caster bone position */
} Fx0dfWork84;

typedef struct {
    s32 vx, vy, vz;
} Fx0dfVec3;

typedef struct {
    s16 unk0;
    s16 unk2; /* size */
    SVECTOR unk4; /* position */
    SVECTOR unkC; /* rotation */
    u16 unk14; /* roll */
    u16 unk16; /* roll speed */
} Fx0dfPart;

typedef struct {
    u8 pad0[0x20];
    s32 unk20; /* models */
    s32 unk24;
    s32 unk28;
    s32 unk2C;
} Fx0dfWork;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0dfBolt;

extern Unk800F78A4 *D_801E9E90;
extern SVECTOR D_801EA350[24]; /* recent positions */
extern s32 D_801EA410; /* next slot in D_801EA350 */
extern Unk80100C78Entry *D_801EA414;

void func_801E7EB8(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *arg3, s32 *arg4, s32 *arg5, s32 arg6, s32 arg7);
s32 func_801E80F4(s32 mode, Fx0dfPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0dfWork *work);
void func_801E93F4(Unk80100C78Entry *entry, Fx0dfBolt *bolt, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 arg9, s32 arg10, s32 pad);
s32 fx_0df_op84(s32 mode, Fx0dfWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

/* propagated */
extern s32 D_801EA418;
void func_801E7FE0(s32 r, s32 a1, s32 a, s32 a3, s16 *out);
void func_801E80B0(SVECTOR *a, SVECTOR *b);

typedef struct {
    SVECTOR unk0; /* from */
    SVECTOR unk8; /* to */
    s32 unk10; /* buffer index */
    s32 *unk14; /* per-point (t offset, amplitude, step) */
} Fx0dfArc;

extern Fx0dfBolt D_801EA420[];
s32 func_801E9600(s32 mode, Fx0dfArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame);
extern s32 D_801EA790[][3]; /* random arc profile */

typedef struct {
    SVECTOR unk0; /* caster body position */
    s32 *unk8; /* arc profile table */
    s32 unkC; /* arcs spawned */
    s32 unk10; /* variant */
} Fx0dfWork83; /* 0x14 */
s32 fx_0df_op83(s32 mode, Fx0dfWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

#endif
