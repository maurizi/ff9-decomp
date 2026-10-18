#ifndef FX_17D_3_H
#define FX_17D_3_H

/* Effect module fx_17d_3 (dir 13 file 0x17d). */
#include "btlfx_mod.h"

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx17d3Summon;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* counter */
    u8 pad8[8];
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20; /* camera */
    s32 unk28[3]; /* model position */
    SVECTOR unk34; /* model rotation */
    s32 unk3C[3]; /* model scale */
    s32 unk48[3]; /* summon position */
    s32 unk54[3]; /* summon scale */
    SVECTOR unk60;
    s32 unk68; /* models */
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    Pool6CEntry *unk78;
    Pool6CEntry *unk7C;
} Fx17d3Work80; /* 0x80 */

extern Unk800F78A4 *D_801EEC84;
extern BtlModel D_801EEC94[];
extern u8 D_801EEC2C[];
extern u8 D_801EEC30[];
extern u8 D_801EEC34[];
extern u8 D_801EEC38[];
extern u8 D_801EF084[];
extern u8 D_801EFD44[];
extern u8 D_801F02C4[];

#define SUMMON(env) ((Fx17d3Summon *)(env)->unk4)

#endif
