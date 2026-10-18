#ifndef FX_105_H
#define FX_105_H

/* Effect module fx_105 (dir 13 file 0x105). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801EAB18;
typedef struct {
    s16 vx, vy, vz, pad, unk8, unkA;
} Fx105Spark; /* 0xC */

extern Fx105Spark D_801EC168[];

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb);
void func_801E9E1C(void);
void func_801E9F28(s32 c);
void func_801EA6D4(s16 *data, u8 c, u8 c2, s32 n);
void func_801EA298(s16 *data, s32 c, s32 h, s32 n);

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
    s32 z;
} Fx105RibbonPt; /* 0xC */

extern Fx105RibbonPt D_801EC378[];

typedef struct {
    DVECTOR xy0;
    DVECTOR xy1;
} Fx105BandPt; /* 8 */

extern Fx105BandPt D_801EC2E8[];

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx105Seg; /* 0x10 */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx105Seg *buf, s32 abr);

/* propagated */

typedef struct {
    s32 unk0; /* start frame */
    s32 unk4; /* frames left, -1 = free */
    u32 unk8; /* destination slots, 4 bits each (1-based) */
    u32 unkC; /* source slots, 4 bits each (1-based, 0 = none) */
    s16 *unk10; /* data */
} Fx105Track; /* 0x14 */

extern s32 D_801EAB1C; /* frame */
extern Fx105Track D_801EAB28[];
void func_801E7B74(void);
void func_801E7BA8(s32 start, u32 dst, u32 src, s16 *data);
extern SVECTOR D_801EAC68[];
void func_801E7BF8(void);

#endif
