/* Draft of fx_1b3 func_801EA39C. Status: 4 mismatches: in both piece loops the original sets up the vertex giv (q+0x1C
 * / q+0x14) before the piece giv (p+0xC); here it is the other way round. The permuter (900 s) found nothing. */
#include "fxmod/fx_1b3.h"

void func_801EA39C(Fx1b3Split *sp) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    SVECTOR *q;
    Fx1b3Piece *p;
    s32 i;
    s32 j;

    ex->Hi_SplitMdlVertex(sp->unk0, -1, (s32)D_801EB0C0);
    for (i = 0; i < 8; i++) {
        sp->unk8[i] = ex->Hi_GetMdlVertexPtr(sp->unk0, 0, (s32)D_801EB0C0, (s32 *)&sp->unk28[i], i);
    }
    D_801EB0C0 += ex->Hi_GetSplitMdlVertex(sp->unk0, -1);
    for (i = 0; i < 8; i++) {
        sp->unk48[i] = (Fx1b3Piece *)D_801EB0C0;
        D_801EB0C0 += sp->unk8[i] * 0x18;
    }
    for (i = 0; i < 8; i++) {
        p = sp->unk48[i];
        q = sp->unk28[i];
        switch (i) {
        case 0:
        case 2:
        case 4:
        case 6:
            for (j = 0; j < sp->unk8[i]; j++) {
                p->unk6 = -1;
                p->unk10 = q[0].vx;
                p->unk10 += q[1].vx;
                p->unk10 += q[2].vx;
                p->unk10 = (s16)(p->unk10 + q[3].vx) >> 2;
                p->unk12 = q[0].vy;
                p->unk12 += q[1].vy;
                p->unk12 += q[2].vy;
                p->unk12 = (s16)(p->unk12 + q[3].vy) >> 2;
                p->unk14 = q[0].vz;
                p->unk14 += q[1].vz;
                p->unk14 += q[2].vz;
                p->unk14 = (s16)(p->unk14 + q[3].vz) >> 2;
                q += 4;
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            for (j = 0; j < sp->unk8[i]; j++) {
                p->unk6 = -1;
                p->unk10 = q[0].vx;
                p->unk10 += q[1].vx;
                p->unk10 = (s16)(p->unk10 + q[2].vx) / 3;
                p->unk12 = q[0].vy;
                p->unk12 += q[1].vy;
                p->unk12 = (s16)(p->unk12 + q[2].vy) / 3;
                p->unk14 = q[0].vz;
                p->unk14 += q[1].vz;
                p->unk14 = (s16)(p->unk14 + q[2].vz) / 3;
                q += 3;
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
            }
            break;
        }
    }
}
