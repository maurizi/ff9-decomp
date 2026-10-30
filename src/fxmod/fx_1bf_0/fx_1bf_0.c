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
    data = (u8 *)ex->btlfx_getFileData(entry->file, 0x1D);
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
                        func_801E7704((u8 *)ex->btlfx_getFileData(entry->file, 5), &p1, &p0, 0, 0x800, 1, 0x32);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, D_801E86C4[j * 4]), 0, (s32 *)from, 0, 0x1000, a,
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
        p = (Fx1bf0Pt *)ex->btlfx_getFileData(entry->file, D_801E87B4[k]);
        p = (Fx1bf0Pt *)((u8 *)p + 4);
        for (i = 0; i < 20; i++, p++) {
            if (i < n) {
                pos.vx = from->vx + p->x * 2;
                pos.vy = from->vy + p->y * 2;
                pos.vz = from->vz + p->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, D_801E879C[i & 7]), &pos, &rot, 0x3000,
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
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, D_801E879C[i & 7]), &pos, &rot, 0x1800,
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
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, D_801E879C[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        d++;
        i++;
    } while (i < 3);
}

s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR q;
    SVECTOR p;
    SVECTOR rot;
    SVECTOR q2;
    SVECTOR p2;
    s32 frame;
    s32 t;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 n;
    s32 flag;
    s32 fade;
    s32 m;
    s32 w;
    s32 alpha;
    s32 s2;
    s32 s0;
    s32 bright;
    s16 y6;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 5;
        work->unk4 = 0;
        D_801E8830 = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(D_801EA5FC, 5);
        work->unk8.vx = D_801E86BC.vx;
        work->unk8.vy = D_801E86BC.vy;
        work->unk8.vz = D_801E86BC.vz;
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        env->camera->points[3].vx = work->unk8.vx;
        env->camera->points[3].vy = work->unk8.vy;
        env->camera->points[3].vz = work->unk8.vz;
        D_801EA1FC[0] = 0;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 5:
            env->otZ = 0x10;
            if (frame >= 0x26) {
                t = ((frame - 0x26) << 12) / 24;
                a = ex->math_cos(t >> 1) >> 1;
                b = ex->math_cos(t >> 2) >> 1;
                c = (ex->math_cos(a) * b) >> 12;
                e = (ex->math_sin(a) * b) >> 12;
                p.vx = work->unk8.vx + c;
                p.vy = work->unk8.vy;
                p.vz = work->unk8.vz + e;
                q.vx = p.vx;
                q.vy = work->unk8.vy - 0x1000;
                q.vz = p.vz;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), &p, NULL, 0xAAA, 0, 1, t);
                func_801E7704((u8 *)ex->btlfx_getFileData(entry->file, 5), &p, &q, frame, t, 1, 0x20);
            }
            env->otZ = 0;
            if (frame >= 0x3E) {
                work->unk0 = 6;
                entry->frame = -1;
            }
            break;
        case 6:
            p2.vx = work->unk8.vx;
            p2.vy = work->unk8.vy;
            p2.vz = work->unk8.vz;
            q2.vx = p2.vx;
            y6 = work->unk8.vy;
            q2.vz = p2.vz;
            q2.vy = y6 - 0x1000;
            env->otZ = 0x10;
            t = (frame << 12) / 51;
            bright = 0x1000 - t;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), &p2, NULL, 0xAAA, 0, 1, bright);
            func_801E7704((u8 *)ex->btlfx_getFileData(entry->file, 5), &p2, &q2, frame, bright, 1, (t >> 7) + 0x20);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            if (frame < 0x10) {
                s32 x = (frame << 12) >> 6;
                s32 y = ex->math_sin(x) * 5;
                s32 z = ex->math_cos(x);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &p2, &rot, y, 0, 1, z);
            }
            if (frame >= 0x1E) {
                s0 = (frame - 0x1E) << 9;
                if (s0 > 0x1000) {
                    s0 = 0x1000;
                }
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &p2, &rot, 0x5000, 0, 1, s0);
            }
            flag = 0;
            if (frame < 0x26) {
                if (frame < 0x1E) {
                    n = frame;
                    w = 0x20;
                    alpha = 0x1000;
                    fade = 0;
                    flag = 1;
                    m = 0;
                    s2 = fade;
                    s0 = fade;
                } else {
                    s32 x = frame - 0x1E;
                    d = x << 9;
                    n = 0x1D;
                    w = 0x27;
                    alpha = 0x1000;
                    fade = 0;
                    m = x * 3;
                    s2 = alpha;
                    if (d >= 0x800) {
                        s0 = (d - 0x800) * 2;
                    } else {
                        s0 = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, ((0x1000 - d) * 100) >> 12, 0, 0);
                }
            } else {
                d = ((frame - 0x26) << 12) / 13;
                fade = 1;
                n = 0x1D;
                m = 0x15;
                alpha = 0x1000 - d * 2 / 3;
                w = (d >> 3) + 0x40;
                s0 = alpha;
                s2 = s0;
            }
            func_801E77C0(entry, &D_801E86BC, n, w, alpha, fade, flag);
            if (s2 != 0) {
                func_801E7B70(entry, &D_801E86BC, m, s2);
                func_801E7E98(entry, &D_801E86BC, s0);
            }
            if (frame >= 0x33) {
                rot.vx = D_801E86BC.vx;
                rot.vy = D_801E86BC.vy;
                rot.vz = D_801E86BC.vz;
                rot.vy -= 0x300;
                env->camera->points[3].vx = rot.vx;
                env->camera->points[3].vy = rot.vy;
                env->camera->points[3].vz = rot.vz;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_headerPtr);
