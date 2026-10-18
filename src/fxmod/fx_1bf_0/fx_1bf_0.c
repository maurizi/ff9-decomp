#include "fxmod/fx_1bf_0.h"

void func_801E7704(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8830);
    SVECTOR *v;

    D_801E881C.unk0 = D_801E881C.unk2 = 1;
    D_801E881C.unk1 = 0;
    v = D_801E881C.unk4;
    v[0].vx = a->vx;
    v[0].vy = a->vy;
    v[0].vz = a->vz;
    v[0].pad = arg6;
    v[1].vx = b->vx;
    v[1].vy = b->vy;
    v[1].vz = b->vz;
    v[1].pad = arg6;
    ex->btlfx_drawTrail(file, 0, NULL, arg3, 0x1000, arg4, arg5, (u8 *)&D_801E881C, 0);
}

void func_801E77C0(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag) {
    u8 *data;
    s32 k;
    s32 width;
    BtlfxExports *ex;
    SVECTOR last;
    SVECTOR p0;
    SVECTOR p1;
    s16 *pts;
    Fx1bf0Line *tr;
    SVECTOR *dst;
    s32 j;
    s32 i;
    s32 cnt;
    s32 t;

    ex = BTLFX_EXPORTS(D_801E8830);
    k = 0;
    data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D);
    w = (w * 3) >> 1;
    do {
        for (j = 0; j < 18; j++) {
            cnt = *(u16 *)(data + ((j + k * 18) << 2));
            pts = (s16 *)(data + cnt);
            cnt = pts[1];
            tr = D_801E8754[j];
            tr = (Fx1bf0Line *)((u8 *)tr + D_801E86C4[j * 4 + 2] * k);
            width = w * (D_801E86C4[j * 4 + 3] + 1);
            tr->unk0 = tr->unk2 = cnt - 1;
            tr->unk1 = 0;
            pts += 2;
            dst = tr->unk4;
            for (i = 0; i < cnt; i++, dst++, pts += 3) {
                t = D_801E86C4[j * 4 + 1];
                if (i == 0) {
                    last.vx = from->vx + pts[0] * 2;
                    last.vy = from->vy + pts[1] * 2;
                    last.vz = from->vz + pts[2] * 2;
                    last.pad = 0;
                }
                t += i;
                if (n >= t) {
                    dst->vx = from->vx + pts[0] * 2;
                    dst->vy = from->vy + pts[1] * 2;
                    dst->vz = from->vz + pts[2] * 2;
                    dst->pad = width;
                    BTLFX_COPY_SVEC(&last, dst);
                    if (t == n && flag) {
                        BTLFX_COPY_SVEC(&p0, dst);
                        BTLFX_COPY_SVEC(&p1, dst);
                        p1.vy -= 0x1000;
                        func_801E7704((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &p1, &p0, 0, 0x800, 1, 0x32);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E86C4[j * 4]), 0, (s32 *)from, 0, 0x1000, a,
                                1, (u8 *)tr, fade);
        }
        k++;
    } while (k < 3);
}

void func_801E7B70(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    s32 k = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8830);
    Fx1bf0Pt *p;
    s32 i;
    s32 a;

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    for (; k < 3; k++) {
        p = (Fx1bf0Pt *)ex->btlfx_getFileData(entry->unk8, D_801E87B4[k]);
        p = (Fx1bf0Pt *)((u8 *)p + 4);
        for (i = 0; i < 20; i++, p++) {
            if (i < n) {
                pos.vx = from->vx + p->x * 2;
                pos.vy = from->vy + p->y * 2;
                pos.vz = from->vz + p->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E879C[i & 7]), &pos, &rot, 0x3000,
                                        0, 1, ot, 0, 0);
            }
        }
    }
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 27; i++) {
            if (i <= n * 27 / 20) {
                a = ((i + k * 27) << 12) / 81;
                pos.vx = from->vx + ((ex->math_cos(a) * 1600) >> 12);
                pos.vy = from->vy;
                pos.vz = from->vz + ((ex->math_sin(a) * 1600) >> 12);
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E879C[i & 7]), &pos, &rot, 0x1800,
                                        0, 1, ot, 0, 0);
            }
        }
    }
}

void func_801E7E98(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *d = D_801E87BC;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8830);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + d->vx * 2;
        pos.vy = from->vy + d->vy * 2;
        pos.vz = from->vz + d->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E879C[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        d++;
        i++;
    } while (i < 3);
}

INCLUDE_ASM("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_op80);

INCLUDE_RODATA("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_headerPtr);
