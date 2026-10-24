#ifndef FX_0DB_H
#define FX_0DB_H

/* Effect module fx_0db (dir 13 file 0xDB). */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* btlfx_getWeaponPos(0x10, 1) */
} Fx0dbWork84;

extern Unk800F78A4 *D_801E894C;
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
    s32 unkC[3];
    s32 unk18[9];
    s32 unk3C;
    s32 unk40;
} Fx0dbPart; /* pool entry data */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0dbPos;
typedef struct {
    s32 unk0; /* index into D_801E88B4 */
    s32 unk4; /* state */
    s32 unk8; /* frame */
    Fx0dbPos unkC[3];
    SVECTOR unk30; /* rotation once split */
    s16 unk38; /* rotation speed */
    s16 unk3A;
    s32 unk3C; /* size */
    Pool6CEntry *unk40;
} Fx0dbItem; /* 0x44 */
typedef struct {
    s32 unk0;
    s32 unk4; /* models */
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    SVECTOR unk18;
    SVECTOR unk20;
    s16 unk28; /* split now */
    s16 unk2A;
    s32 unk2C;
    s32 unk30;
} Fx0dbWork;
extern SVECTOR D_801E88B4[];
s32 func_801E7704(s32 mode, Fx0dbItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx0dbWork80 *work);

#endif
