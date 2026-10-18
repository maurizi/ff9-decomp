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

void func_801EA7E4(Fx1b3Split *sp) {
    SVECTOR d[4];
    VECTOR out;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
    Fx1b3Piece *p;
    SVECTOR *v;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 dz;

    for (i = 0; i < 8; i++) {
        v = sp->unk28[i];
        p = sp->unk48[i];
        for (j = 0; j < sp->unk8[i]; j++) {
            if (p->unk6 > 0) {
                ex->func_800316D8(&p->unk8, &m);
                m.t[0] = p->unk10 + p->unk0;
                m.t[1] = p->unk12 + p->unk2;
                m.t[2] = p->unk14 + p->unk4;
                scale.vx = scale.vy = scale.vz = p->unk16;
                p->unk16 -= p->unkE;
                ex->ScaleMatrix(&m, &scale);
                gte_SetRotMatrix(&m);
                gte_SetTransMatrix(&m);
                d[0].vx = v[0].vx - p->unk10;
                d[0].vy = v[0].vy - p->unk12;
                d[0].vz = v[0].vz - p->unk14;
                gte_ldv0(&d[0]);
                gte_rtv0tr();
                d[1].vx = v[1].vx - p->unk10;
                d[1].vy = v[1].vy - p->unk12;
                d[1].vz = v[1].vz - p->unk14;
                gte_stlvnl(&out);
                gte_ldv0(&d[1]);
                gte_rtv0tr();
                v[0].vx = out.vx;
                v[0].vy = out.vy;
                v[0].vz = out.vz;
                d[2].vx = v[2].vx - p->unk10;
                d[2].vy = v[2].vy - p->unk12;
                d[2].vz = v[2].vz - p->unk14;
                gte_stlvnl(&out);
                gte_ldv0(&d[2]);
                gte_rtv0tr();
                v[1].vx = out.vx;
                v[1].vy = out.vy;
                v[1].vz = out.vz;
                p->unk6--;
                gte_stlvnl(&out);
                v[2].vx = out.vx;
                v[2].vy = out.vy;
                v[2].vz = out.vz;
                if (!(i & 1)) {
                    d[3].vx = v[3].vx - p->unk10;
                    d[3].vy = v[3].vy - p->unk12;
                    d[3].vz = v[3].vz - p->unk14;
                    gte_ldv0(&d[3]);
                    gte_rtv0tr();
                    gte_stlvnl(&out);
                    v[3].vx = out.vx;
                    v[3].vy = out.vy;
                    v[3].vz = out.vz;
                }
                p->unk10 += p->unk0;
                p->unk12 += p->unk2;
                p->unk14 += p->unk4;
            } else {
                dx = D_801EB0D4.unk4 - p->unk10;
                dz = D_801EB0D4.unk8 - p->unk14;
                dy = D_801EB0D4.unk6 - p->unk12;
                switch (D_801EB0D4.unk0) {
                case 0:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x40;
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                        p->unk0 = (ex->btlfx_rand() & 0x7F) + 0x20;
                        p->unk2 = (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk4 = 0;
                    }
                    break;
                case 1:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x40;
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                        p->unk0 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        p->unk2 = (ex->btlfx_rand() & 0x1F) - 0x10;
                        p->unk4 = -0x20 - (ex->btlfx_rand() & 0x7F);
                    }
                    break;
                case 2:
                    if (abs(dy) < D_801EB0D4.unkC) {
                        p->unk6 = 0x80;
                        p->unk0 = -(dx >> 3);
                        p->unk2 = -(dy >> 3);
                        p->unk4 = -(dz >> 3);
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 3:
                    if (abs(dx) < D_801EB0D4.unkC && abs(dz) < D_801EB0D4.unkC) {
                        p->unk6 = 0x80;
                        p->unk0 = -(dx >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk2 = -(ex->btlfx_rand() & 0x7F) + 0x40;
                        p->unk4 = -(dz >> 2) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 4:
                    if (abs(dx) < D_801EB0D4.unkC) {
                        p->unk6 = 0x22;
                        p->unk0 = -(dx >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk2 = -(ex->btlfx_rand() & 0x3F);
                        p->unk4 = -(dz >> 4) + (ex->btlfx_rand() & 0x3F) - 0x20;
                        p->unk16 = 0x1000;
                        p->unkE = 0x10;
                    }
                    break;
                case 5:
                    if (D_801EB0D4.unkC < ex->btlfx_sqrt(dy * dy + dx * dx, 0xF)) {
                        p->unk6 = 0x40;
                        p->unk0 = dx >> 7;
                        p->unk2 = dy >> 7;
                        p->unk4 = -(dz >> 7);
                        p->unk16 = 0x1000;
                        p->unkE = 0x20;
                    }
                    break;
                }
            }
            if (!(i & 1)) {
                v += 4;
            } else {
                v += 3;
            }
            p++;
        }
    }
}
