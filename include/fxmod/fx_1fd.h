#ifndef FX_1FD_H
#define FX_1FD_H

/* Effect module fx_1fd (dir 13 file 0x1fd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1fdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1fdPoint *unk4;
} Fx1fdPath;

typedef struct {
    Pool6CEntry *unk0[5];
    SVECTOR unk14[5];
} Fx1fdWork80; /* 0x3C */

extern u16 D_801E9DF8[5]; /* files */
extern BtlModel D_801E9E5C[];
extern Fx1fdPath D_801EBB7C[5];
extern s32 D_801EBBAC[5][0x78];
extern SVECTOR D_801ED70C[5];

typedef struct {
    Pool6CEntry *unk0[8];
    Pool6CEntry *unk20[8];
    SVECTOR unk40[8]; /* unit positions */
    SVECTOR unk80[8];
    s16 unkC0[8]; /* landed */
} Fx1fdWork84; /* 0xD0 */

typedef struct {
    SVECTOR unk0; /* unit position */
    SVECTOR unk8; /* unit rotation */
    s16 unk10; /* distance from the centre */
    s16 unk12; /* angle around it */
} Fx1fdOrbit; /* 0x14 */
extern s32 D_801EC50C[8][0x90];

void func_801E97C4(Fx1fdPath *path, u16 *data);
s32 func_801E97DC(Fx1fdPath *path, s32 t, SVECTOR *out);

/* copied */
extern Unk800F78A4 *D_801E9E4C;
/* end copied */

/* propagated */

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx1fdKey; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx1fdKey *unk4;
} Fx1fdPath_0b4;
s32 func_801E9858(Fx1fdPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos);
u16 *func_801E9710(u8 *base, u16 i);
s32 func_801E9984(s32 a0, s32 t, s32 n, SVECTOR *b);

typedef struct {
    s16 *unk0;
    s16 unk4; /* start frame */
    s16 unk6; /* frames left, -1 = free */
    s16 unk8; /* channel */
    u16 unkA; /* destination slots, 4 bits each */
    u16 unkC;
    u16 unkE;
} Fx1fdTrack; /* 0x10 */
extern Fx1fdTrack D_801ED73C[];
extern s32 D_801EDB3C[];
void func_801E9A50(void);
void func_801E9A9C(s16 start, u16 dst, s16 ch, s16 *data);
void func_801E9AF8(s32 ch);

typedef struct {
    DVECTOR unk0; /* screen xy of one edge end */
    DVECTOR unk4; /* screen xy of the other */
    s32 unk8; /* ot z */
} Fx1fdEdge;
extern Fx1fdEdge D_801EB80C[];
void func_801E7AF8(void *track, s32 arg1, s32 arg2, SVECTOR *scale, s32 w, s32 rotY);
extern Fx1fdOrbit D_801EBADC[];
s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);
void func_801E7704(void *trackA, void *trackB, s32 arg2, s32 arg3, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist);
void func_801E7E18(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1);

#endif
