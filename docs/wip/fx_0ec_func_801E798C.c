/* Draft of func_801E798C (module fx_0ec). Status: 76/126 aligned instructions with ccprobe; the t++ placement in the loop differs.
 * Declarations it needs that are not yet in include/fxmod/fx_0ec.h are repeated below. */
#include "fxmod/fx_0ec.h"

void func_801E798C(s32 model, SVECTOR *center) {
    Fx0ecTri *t = D_801EB3D0;
    Fx0ecTri *u = D_801EB3D0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E98A0);
    Fx0ecShard *p;
    SVECTOR d;
    s32 i;
    s32 k;

    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    p = D_801EBE50;
    for (i = 0; i < 0x70; i++, t++, u++, p++) {
        p->unk18 = 0;
        p->unk0.vx = ((t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3) << 12;
        p->unk0.vy = ((u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3) << 12;
        p->unk0.vz = ((u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3) << 12;
        p->unk1A = 0;
        p->unkC.pad = 0x1000;
        p->unk14 = ex->btlfx_randCentered(0x100);
        p->unk16 = ex->btlfx_randCentered(0x200);
        d.vx = (p->unk0.vx >> 12) - center->vx;
        d.vy = (p->unk0.vy >> 12) - center->vy;
        d.vz = (p->unk0.vz >> 12) - center->vz;
        p->unkC.vx = d.vx >> 4;
        p->unkC.vy = d.vy >> 4;
        k = (ex->btlfx_rand() & 0x1F) + 8;
        p->unkC.vz = (d.vz >> 4) + k;
    }
}
