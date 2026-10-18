#ifndef FX_1BD_H
#define FX_1BD_H

/* Effect module fx_1bd (dir 13 file 0x1bd). */
#include "btlfx_mod.h"

typedef struct {
    s16 x, y, z;
} Fx1bdPoint;

typedef struct {
    s16 unk0;
    u16 unk2; /* count */
    Fx1bdPoint *unk4;
} Fx1bdPath;

typedef struct {
    s16 x, y, z;
} Fx1bdKey6;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey6 *unk4;
} Fx1bdTrack6;

typedef struct {
    s16 unk0, unk2, unk4; /* position */
    s16 unk6, unk8, unkA; /* rotation */
} Fx1bdKey12;

typedef struct {
    u16 unk0; /* count */
    Fx1bdKey12 *unk4;
} Fx1bdTrack12;

/* copied */
extern Unk800F78A4 *D_801E9E54;
/* end copied */

#endif
