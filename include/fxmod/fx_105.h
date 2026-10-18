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
void func_801EA6D4(s16 *data, s32 c, s32 c2, s32 n);
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

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx105Summon;

typedef struct {
    s32 unk0; /* summon visible */
    s32 unk4; /* countdowns */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 pad34[3];
    Pool6CEntry *unk40;
    Pool6CEntry *unk44;
    Pool6CEntry *unk48;
    Pool6CEntry *unk4C;
    Pool6CEntry *unk50;
    Pool6CEntry *unk54;
    SVECTOR unk58;
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    SVECTOR unk78;
    SVECTOR unk80;
    SVECTOR unk88;
    SVECTOR unk90;
    SVECTOR unk98;
    SVECTOR unkA0;
    Pool6CEntry *unkA8;
    Pool6CEntry *unkAC;
    Pool6CEntry *unkB0;
    SVECTOR unkB4;
    SVECTOR unkBC;
    SVECTOR unkC4;
    SVECTOR unkCC;
    Pool6CEntry *unkD4;
    Pool6CEntry *unkD8;
    Pool6CEntry *unkDC;
    Pool6CEntry *unkE0;
    SVECTOR unkE4;
    SVECTOR unkEC;
    SVECTOR unkF4;
    SVECTOR unkFC;
    SVECTOR unk104;
    SVECTOR unk10C;
    SVECTOR unk114;
    CVECTOR unk11C;
    CVECTOR unk120;
    Pool6CEntry *unk124;
    SVECTOR unk128;
    SVECTOR unk130;
    s32 unk138[8]; /* models */
    s32 unk158[3];
    s32 pad164;
    SVECTOR unk168;
    s32 unk170[3];
    s32 pad17C;
    s32 unk180;
    s32 unk184;
    s32 unk188;
    s32 unk18C;
    SVECTOR unk190;
    s32 unk198[3];
    s32 pad1A4;
    s32 unk1A8[3];
    s32 pad1B4;
    s32 unk1B8[8];
} Fx105Work80; /* 0x1D8 */

typedef struct {
    s32 unk0; /* x */
    s32 unk4; /* y */
    s32 unk8; /* speed */
    s32 padC;
    s32 unk10;
} Fx105Drop; /* 0x14 */

typedef struct {
    s32 unk0; /* delay */
    s32 unk4; /* frames */
} Fx105Flash; /* 8 */

extern Fx105Flash D_801EAA90[8];
extern Fx105Seg D_801EAC98[];
extern SVECTOR D_801EADA8;
extern VECTOR D_801EADB8;
extern VECTOR D_801EADC8;
extern BtlModel D_801EADD8[];
extern Fx105Drop D_801EB998[32];
void func_801E7BF8(void);

#define SUMMON105(env) ((Fx105Summon *)(env)->unk4)

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

#endif
