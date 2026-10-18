#ifndef FX_1A2_H
#define FX_1A2_H

/* Effect module fx_1a2 (dir 13 file 0x1a2). */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0;
    SVECTOR unk8;
} Fx1a2Seg; /* 0x10 */

typedef struct {
    SVECTOR unk0[3];
} Fx1a2Tri; /* 0x18 */

typedef struct {
    s32 unk0; /* position, 20.12 */
    s32 unk4;
    s32 unk8;
    s16 unkC; /* velocity */
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fx1a2Piece; /* 0x1C */

extern Fx1a2Tri D_801E9854[130];
extern Fx1a2Piece D_801EA484[130];
typedef struct {
    s16 unk0; /* bounce count */
    s16 unk2; /* sink speed */
    s16 unk4; /* bounce height */
    s16 unk6; /* wait frames */
    s32 unk8; /* state */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s32 unk1C; /* scale */
} Fx1a2Rock;

typedef struct {
    u8 pad0[0x48];
    s32 unk48; /* model */
} Fx1a2Inst;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* unit position */
    SVECTOR unk10; /* body position */
    SVECTOR unk18; /* particle position */
    s16 unk20; /* bouncing rock position */
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s32 unk28; /* bouncing rock velocity */
    s32 unk2C; /* scale */
    s32 unk30[3]; /* models */
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} Fx1a2Work80; /* 0x54 */

extern BtlModel D_801EB2C4[];
extern CVECTOR D_801E97F0;
extern CVECTOR D_801E97F4;
extern CVECTOR D_801E97F8;
extern Fx1a2Seg D_801EC1B4[];
extern Fx1a2Seg D_801EC3B4[];
extern Fx1a2Seg D_801EC5B4[];
extern s32 D_801EBA94[];
extern s32 D_801EBF54[];

/* copied */
extern Unk800F78A4 *D_801E9844;
/* end copied */

#endif
