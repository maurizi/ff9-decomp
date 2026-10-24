#ifndef FX_0E3_0_H
#define FX_0E3_0_H

/* Effect module fx_0e3_0 (dir 13 file 0x0E3). */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[49];
} Fx0e30Bolt;

typedef struct {
    SVECTOR unk0; /* position */
    s32 unk8; /* phase */
    s16 unkC; /* size */
    s16 unkE; /* speed */
} Fx0e30Puff;

typedef struct {
    SVECTOR unk0; /* pad: point valid */
    SVECTOR unk8;
} Fx0e30Seg; /* 0x10: one step of the ribbon history */

extern Unk800F78A4 *D_801EA868;
extern s32 D_801EB370;
extern s32 D_801EB374;
extern s32 D_801EB378;

void func_801E7730(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e30Seg *buf, s32 abr, s32 phase);
void func_801E7B40(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
void func_801E7D40(Unk80100C78Entry *entry, s32 arg1, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 arg8, s32 arg9);
s32 func_801E7F48(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry);

typedef struct {
    s32 vx, vy, vz;
} Fx0e30Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* summon position */
    Fx0e30Vec3 unk10; /* summon model position */
    Fx0e30Vec3 unk1C; /* summon model scale */
    SVECTOR unk28; /* summon model rotation */
    SVECTOR unk30; /* camera */
    SVECTOR unk38; /* bone position */
    SVECTOR unk40; /* bone position */
    s32 unk48;
    Pool6CEntry *unk4C;
    s32 unk50; /* bolts need a reset */
    s32 unk54; /* models */
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    SVECTOR unk68;
    Fx0e30Vec3 unk70;
    Fx0e30Vec3 unk7C;
    SVECTOR unk88;
    Fx0e30Vec3 unk90;
    Fx0e30Vec3 unk9C;
} Fx0e30Work80; /* 0xA8 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
        BtlUnit9CC *unk184;
        BtlUnit9CC *unk188;
        BtlUnit9CC *unk18C;
        BtlUnit9CC *unk190;
        BtlUnit9CC *unk194;
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e30Summon;

typedef struct {
    s16 unk0;
    s16 unk2;
} Fx0e30BonePair;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[13];
} Fx0e30Arc; /* 0x6C */

extern SVECTOR D_801EA7E8;
extern s16 D_801EA7F0[];
extern Fx0e30BonePair D_801EA800[8];
extern Fx0e30Seg D_801EA878[4][15];
extern u8 D_801EAC38[4][0x12C];
extern Fx0e30Arc D_801EB0E8[];
extern Fx0e30Bolt D_801EB388[];
extern BtlModel D_801EB6A8[];

#define SUMMON(env) ((Fx0e30Summon *)(env)->loader)

#endif
