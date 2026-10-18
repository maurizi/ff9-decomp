#include "fxmod/fx_1b3.h"

void func_801E773C(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot) {
    Unk800F78A4 *env = D_801EB074;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 c = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + ot), arg4, c, c, c);
}

s32 func_801E77CC(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_op80);

INCLUDE_RODATA("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_headerPtr);

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
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
                q += 4;
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
                p->unk8 = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkA = (ex->btlfx_rand() & 0xFF) - 0x80;
                p->unkC = (ex->btlfx_rand() & 0xFF) - 0x80;
                p++;
                q += 3;
            }
            break;
        }
    }
}

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", func_801EA7E4);
