#include "fxmod/fx_0ba.h"

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = D_801EC5F0;
    POLY_G4 *prim;
    BtlfxExports *ex;
    s32 i;

    w >>= 1;
    prim = (POLY_G4 *)env->unk24;
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++, prim++) {
        setlen(prim, 8);
        prim->code = 0x3A;
        if (!(i & 1)) {
            prim->r0 = prim->r2 = 0;
            prim->g0 = prim->g2 = 0;
            prim->b0 = prim->b2 = b;
            prim->r1 = prim->r3 = r;
            prim->g1 = prim->g3 = g;
            prim->b1 = prim->b3 = b;
            prim->x0 = prim->x2 = 0xA0 - w;
            prim->x1 = prim->x3 = 0xA0;
        } else {
            prim->r1 = prim->r3 = 0;
            prim->g1 = prim->g3 = 0;
            prim->b1 = prim->b3 = b;
            prim->r0 = prim->r2 = r;
            prim->g0 = prim->g2 = g;
            prim->b0 = prim->b2 = b;
            prim->x0 = prim->x2 = 0xA0;
            prim->x1 = prim->x3 = w + 0xA0;
        }
        prim->y0 = prim->y1 = (i >> 1) * 0x78;
        prim->y2 = prim->y3 = prim->y0 + 0x78;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
    }
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op80);

void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR pos;
    SVECTOR ang;
    SVECTOR *v;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->unk4;
    buf->unk0 = buf->unk2 = 24;
    buf->unk1 = 0;
    for (i = 0; i < 25; i++, v++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += 0x20 + ((ex->math_sin(t >> 1) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + ex->btlfx_randCentered(0x100);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0, NULL, a7, 0x1000, a8, 1, (u8 *)buf, 0);
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op81);

s32 fx_0ba_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 *keys;
    SVECTOR v;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EC5F0 = env;
    } else {
        frame = entry->unk0;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2E);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2F);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave) {
    Unk800F78A4 *env = D_801EC5F0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 n;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    n = (size - 4) / 8;
    v = buf->unk4;
    buf->unk0 = buf->unk2 = n - 1;
    buf->unk1 = 0;
    for (i = 0; i < n; i++, v++) {
        t = 0x1000 - (i << 12) / (n - 1);
        t += *tab++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        if (wave != 0) {
            v->vy -= (ex->math_sin(t >> 1) * wave) >> 12;
        }
        v->pad = pad;
        s += step + *tab++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, a12, 0x1000, a13, 1, (u8 *)buf, a2);
    env->unk28 = 0;
}

void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad) {
    SVECTOR *v;
    BtlfxExports *ex;
    SVECTOR to;

    v = path->unk4;
    v += i;
    ex = BTLFX_EXPORTS(D_801EC5F0);
    ex->btlfx_lerpVec((s16 *)(v + 1), (s16 *)v, 0x4000, (s16 *)&to);
    func_801EA850(entry, file, a2, buf, size, tab, v, &to, 0, 0x20, ex->btlfx_rand(), 0x33, a12, a13, pad, 0);
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801EAC08);

s32 func_801EB1CC(SVECTOR *v) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR *p = D_801ED424[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;
    v->vx = p->vx;
    v->vy = p->vy;
    v->vz = p->vz;
    v->pad = 1;
    return 0;
}

s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    s32 frame;
    Fx0baBolt *p;
    Fx0baPt *pt;
    s32 *tab;
    s32 i;
    s32 r;
    s32 ang;
    s32 amp;
    s32 dist;
    Fx0baBolt *q;
    s32 len;
    s32 j;
    s32 k;
    s32 n;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0baWork83);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0baBolt);
        ((EffInit *)arg4)->unk14 = (s32)func_801EAC08;
        ((EffInit *)arg4)->unk4 = 0x20;
    } else if (mode == 1) {
        n = *arg4;
        D_801EC5F0 = env;
        work->unk0 = D_801EEA20;
        tab = work->unk0;
        for (i = 0; i < 64; i++) {
            *tab++ = ex->btlfx_randCentered(0xD2);
            *tab++ = ex->btlfx_randCentered(0x1C3);
            *tab++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk8) {
        case 0:
            BTLFX_COPY_SVEC(&base, &D_801EC554);
            if (frame < 0x14) {
                work->unk4++;
                BTLFX_COPY_SVEC(&a, &base);
                a.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (p != NULL) {
                    r = ex->btlfx_randCentered(0x200) + 0x7D0;
                    ang = ((work->unk4 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x200);
                    a.vx += (ex->math_cos(ang) * r) >> 12;
                    a.vz += (ex->math_sin(ang) * r) >> 12;
                    BTLFX_COPY_SVEC(&p->unk0, &a);
                    BTLFX_COPY_SVEC(&p->unk8, &b);
                    p->unk11 = 0;
                    p->unk10 = work->unk4 % 32;
                    p->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    p->unk12 = ex->btlfx_rand() % 15;
                    p->unk13 = ex->btlfx_rand() % 15;
                    p->unk15 = 0x3E;
                    p->unk14 = 0;
                }
                work->unk4++;
                BTLFX_COPY_SVEC(&a, &base);
                a.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                if (frame >= 0xB) {
                    p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (p != NULL) {
                        r = ex->btlfx_randCentered(0x100) + 0x3E8;
                        ang = ((work->unk4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                        a.vx += (ex->math_cos(ang) * r) >> 12;
                        a.vz += (ex->math_sin(ang) * r) >> 12;
                        BTLFX_COPY_SVEC(&p->unk0, &a);
                        BTLFX_COPY_SVEC(&p->unk8, &b);
                        p->unk11 = 0;
                        p->unk10 = work->unk4 % 32;
                        p->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                        p->unk12 = ex->btlfx_rand() % 15;
                        p->unk13 = ex->btlfx_rand() % 15;
                        p->unk15 = 0x32;
                        p->unk14 = 0;
                    }
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            BTLFX_COPY_SVEC(&a, &D_801EC554);
            if (frame < 0x1D) {
                amp = (D_801EC55C * 100) / 2048 + 0x3C;
                if (amp >= 0x100) {
                    amp = 0xFF;
                }
                for (j = 0; j < 2; j++) {
                    work->unk4++;
                    BTLFX_COPY_SVEC(&c, &a);
                    BTLFX_COPY_SVEC(&d, &a);
                    q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (q != NULL) {
                        len = D_801EC55C;
                        ex->btlfx_randRotation(&b);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&b, 0x180, (s16 *)&c);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&b, len + 0x180, (s16 *)&d);
                        BTLFX_COPY_SVEC(&q->unk0, &c);
                        BTLFX_COPY_SVEC(&q->unk8, &d);
                        q->unk11 = 1;
                        q->unk10 = work->unk4 % 32;
                        q->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                        q->unk12 = ex->btlfx_rand() % 15;
                        q->unk13 = ex->btlfx_rand() % 15;
                        q->unk15 = amp;
                        q->unk14 = 0;
                    }
                }
                if (frame >= 0xF) {
                    for (j = 0; j < 2; j++) {
                        work->unk4++;
                        BTLFX_COPY_SVEC(&c, &a);
                        BTLFX_COPY_SVEC(&d, &a);
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (q != NULL) {
                            len = D_801EC55C;
                            ex->btlfx_randRotation(&b);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&b, 0x180, (s16 *)&c);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&b, len + 0x180, (s16 *)&d);
                            BTLFX_COPY_SVEC(&q->unk0, &c);
                            BTLFX_COPY_SVEC(&q->unk8, &d);
                            q->unk11 = 1;
                            q->unk10 = work->unk4 % 32;
                            q->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                            q->unk12 = ex->btlfx_rand() % 15;
                            q->unk13 = ex->btlfx_rand() % 15;
                            q->unk15 = amp - 0x32;
                            q->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x1D) {
                return 1;
            }
            break;
        case 2:
            if ((D_801EC550 != 0 || !(frame & 1)) && D_801EC54C == 0) {
                do {
                    k = ex->btlfx_rand() % 10;
                    pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23) + 4) + k;
                    b.vx = -pt->vx + D_801ECAB0.vx;
                    b.vy = pt->vy + D_801ECAB0.vy;
                    b.vz = -pt->vz + D_801ECAB0.vz;
                    if ((D_801ECAB8.vx != 0 || D_801ECAB8.vy != 0 || D_801ECAB8.vz != 0) && (frame & 1)) {
                        BTLFX_COPY_SVEC(&c, &D_801ECAB8);
                        b.vz >>= 1;
                        b.vx >>= 1;
                        b.vy += 0x400;
                    } else {
                        k = ex->btlfx_rand() % 10;
                        pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24) + 4) + k;
                        c.vx = -pt->vx + D_801ECAB0.vx;
                        c.vy = pt->vy + D_801ECAB0.vy;
                        c.vz = -pt->vz + D_801ECAB0.vz;
                    }
                    dist = (b.vx - c.vx) * (b.vx - c.vx) + (b.vy - c.vy) * (b.vy - c.vy) +
                           (b.vz - c.vz) * (b.vz - c.vz);
                } while ((u32)(dist - 0x1000000) > 0x4000FFF);
                work->unk4++;
                p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (p != NULL) {
                    BTLFX_COPY_SVEC(&p->unk0, &b);
                    BTLFX_COPY_SVEC(&p->unk8, &c);
                    p->unk11 = 2;
                    p->unk10 = work->unk4 % 32;
                    p->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    p->unk12 = ex->btlfx_rand() % 15;
                    p->unk13 = ex->btlfx_rand() % 15;
                    p->unk15 = ex->btlfx_rand() % 70 + 0x37;
                    p->unk14 = 0;
                }
            }
            if (D_801EC54C != 0) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    s32 c;
    u8 *q;
    s32 (*fn)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *);

    if (mode == 1) {
        p->unk12 = ex->btlfx_randCentered(0x20) + 0x1080;
    } else {
        c = 0x1000 - (frame << 12) / 5;
        q = p->unk18;
        if (frame != 0) {
            c >>= 1;
        }
        ((s16 *)q)[3] = (0x16 - frame * 4) >> 1;
        fn = ex->func_800E0014;
        bias[0] = 0;
        bias[1] = 0;
        fn((Pool6CEntry *)p, 0, (SVECTOR *)bias, (SVECTOR *)q);
        if (p->unk16 == 0) {
            p->unk18[-0x10] = p->unk18[-0xE] = 0x10;
            p->unk18[-0xF] = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x32), 0, NULL, frame, 0x1000, c, 1,
                              p->unk18 - 4, 0);
        }
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

void func_801EC124(void) {
    D_801EEFDC = 0;
}

void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to) {
    Fx0baTrail *trail = &D_801EED20[D_801EEFDC];
    SVECTOR *p = &trail->unk4[16];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR v;
    Fx0baPart *part;
    s32 i;
    s32 t;

    trail->unk0 = trail->unk2 = 16;
    trail->unk1 = 0;
    for (i = 0; i < 17; i++, p--) {
        t = 0x1000 - (i << 8);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += 0x20 + ex->btlfx_randCentered(0x10);
        v.vy += 0x46 + ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)center, (s16 *)&v, r + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 6),
                          (s16 *)p);
        part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (part != NULL) {
            part->unk14 = D_801EEFDC;
            part->unk16 = 0x10 - i;
            part->unk18 = (u8 *)p;
            part->unk0 = p->vx << 12;
            part->unk4 = p->vy << 12;
            part->unk8 = p->vz << 12;
            part->unkC = 0;
            part->unkE = 0;
            part->unk10 = 0;
            part->unkC = ((p->vx - center->vx) >> 5) + ex->btlfx_randCentered(0xE);
            part->unkE = ((p->vy - center->vy) >> 5) + ex->btlfx_randCentered(0x18);
            part->unk10 = ((p->vz - center->vz) >> 5) + ex->btlfx_randCentered(0xE);
        }
    }
    D_801EEFDC = (D_801EEFDC + 1) % 5;
}

s32 fx_0ba_op85(s32 mode, Fx0baWork85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    SVECTOR c;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0baWork85);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0baPart);
        ((EffInit *)arg4)->unk14 = (s32)func_801EBFB8;
        ((EffInit *)arg4)->unk4 = 0x55;
    } else if (mode == 1) {
        D_801EC5F0 = env;
        func_801EC124();
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801EC54C == 0) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0xFF);
            work->unk8.vy += 0x800 + (ex->btlfx_rand() & 0x1FF);
            BTLFX_COPY_SVEC(&c, &D_801ECAB0);
            c.vy -= 0x140;
            func_801EC130(entry, &c, 0x100, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801EC54C != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_headerPtr);
