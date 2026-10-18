/* Draft of fx_1be func_801E8E10. Status: about 25 strict mismatches: the original copies e to s2 before the divide and
 * keeps the quotient in a0. */
#include "fxmod/fx_1be.h"

s32 func_801E8E10(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;
    SVECTOR from;

    from.vx = 0;
    from.vy = -0x800;
    from.vz = 0x418;
    func_801E7704(1, 0x1000, ((0x1000000 / frames) * frame) >> 12, &from, &D_801E8FE8[(e->unk1F - 1) & 0x3F], &v);
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
