/* Draft of fx_1ac func_801E81C4. Status: 8 mismatches. The original loads src->vx inside each switch branch (lhu
 * %lo(D)(v0)) before joining; here it is loaded after the join. */
#include "fxmod/fx_1ac.h"

s32 func_801E81C4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR *src;
    SVECTOR v;
    s16 x, y, z;

    switch (e->unk1F - 1) {
    case 0:
        src = &D_801E85A8;
        break;
    default:
        src = &D_801E85B0;
        break;
    }
    x = src->vx;
    y = src->vy;
    z = src->vz;
    v.vy = y;
    v.vx = x;
    v.vz = z;
    D_801E85B0.vx = e->unk0.vx >> 12;
    D_801E85B0.vy = e->unk0.vy >> 12;
    D_801E85B0.vz = e->unk0.vz >> 12;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->unk0.vx = v.vx << 12;
    e->unk0.vy = v.vy << 12;
    e->unk0.vz = v.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}
