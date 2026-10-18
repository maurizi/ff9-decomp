#ifndef FX_0DB_H
#define FX_0DB_H

/* Effect module fx_0db (dir 13 file 0xDB). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0dbWork84;

extern Unk800F78A4 *D_801E894C;
extern SVECTOR D_801E88B4[10];
extern BtlModel D_801E895C[];

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    SVECTOR unk14; /* caster body position */
    SVECTOR unk1C; /* caster position */
    s16 unk24;
    s16 unk26; /* particles spawned */
    s32 unk28; /* frame counter */
    s32 unk2C;
    s32 unk30;
} Fx0dbWork80;

typedef struct {
    s32 unk0; /* index into D_801E88B4 */
    s32 unk4[2];
    s32 unkC[3];
    s32 unk18[9];
    s32 unk3C;
    s32 unk40;
} Fx0dbPart; /* pool entry data */

s32 func_801E7704();

#endif
