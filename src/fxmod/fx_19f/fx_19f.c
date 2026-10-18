#include "fxmod/fx_19f.h"

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = D_801EAA48;
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

void func_801E7894(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        i++;
        h += 0x20 + ((ex->math_sin(a) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg5);
        p->pad = arg9;
        arg5 += arg6 + ex->btlfx_randCentered(0x100);
        p++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0, NULL, arg7, 0x1000, arg8, 1, (u8 *)trail, 0);
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_op81);

s32 fx_19f_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx19fKeys *k;
    s32 frame;
    s32 f;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EAA48 = env;
    } else {
        frame = entry->unk0;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2B);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2C);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

void func_801E9968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801EAA48;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    trail->unk0 = trail->unk2 = cnt - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < cnt; i++, pt++) {
        t = 0x1000 - (i << 12) / (cnt - 1);
        t += *noise++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++;
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        h += ex->btlfx_lerp(h0, h1, t);
        ex->func_800D044C(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(a) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->unk28 = 0;
}

void func_801E9C08(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *path, Fx19fTrail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E9968(entry, file, ot, trail, n, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801E9D20(s32 mode, Fx19fBolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 s;
    s32 kind;
    Fx19fTrail *tr;
    Fx19fTrail *tr2;

    if (mode != 1) {
        kind = b->unk11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            tr = (Fx19fTrail *)&D_801EB3B8[b->unk10];
            tr2 = (Fx19fTrail *)&D_801EC538[b->unk10];
            if (b->unk11 == 0) {
                b->unk8.vx = D_801EA9AC.vx;
                b->unk8.vy = D_801EA9AC.vy;
                b->unk8.vz = D_801EA9AC.vz;
                func_801E9968(entry, 0x1E, 0, tr, 0x8C, b->unk18, &b->unk0, &b->unk8, 0, 0x180, frame * 0x97, 0x140, frame,
                              c, ((0x1000 - t) * b->unk15) >> 12, ((t * 3 * 128) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    k = 2;
                } else if (frame < 4) {
                    k = 3;
                } else {
                    k = 0;
                }
                s = ((0x1000 - t) * b->unk15) >> 12;
                if (frame == 0) {
                    s = (s * 3) >> 1;
                }
                func_801E9968(entry, 2, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk0, &b->unk8, 0, 0x180, frame * 0xC0,
                              0x168, frame, c, s, 0);
                func_801E9C08(entry, 3, 0, tr, tr2, 0x24, b->unk18, b->unk12, frame, c, s >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E9FC4(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR *p = D_801EB3BC[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

s32 fx_19f_op83(s32 mode, Fx19fWork83 *work, Unk800F78A4 *env, Fx19fInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR p;
    SVECTOR q;
    SVECTOR c;
    SVECTOR d;
    s32 frame;
    s32 i;
    s32 n;
    s32 r;
    s32 a;
    s32 col;
    s32 dist;
    Fx19fBolt *b;
    Fx19fBolt *b2;
    s32 *np;
    s32 k;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E9D20;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        i = init->unk0;
        D_801EAA48 = env;
        work->unk0 = D_801EC9B8[0];
        np = D_801EC9B8[0];
        for (n = 0; n < 64; n++) {
            *np++ = ex->btlfx_randCentered(0xD2);
            *np++ = ex->btlfx_randCentered(0x1C3);
            *np++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = i;
        work->unk4 = 0;
    } else {
        frame = inst->unk0;
        switch (work->unk8) {
        case 0:
            FX19F_W(base, 0) = FX19F_W(D_801EA9AC, 0);
            FX19F_W(base, 1) = FX19F_W(D_801EA9AC, 1);
            if (frame >= 0x14) {
                return 1;
            }
            work->unk4++;
            FX19F_W(p, 0) = FX19F_W(base, 0);
            p.vy = 0;
            FX19F_W(p, 1) = FX19F_W(base, 1);
            FX19F_W(q, 0) = FX19F_W(base, 0);
            FX19F_W(q, 1) = FX19F_W(base, 1);
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                r = ex->btlfx_randCentered(0x200) + 0x7D0;
                k = ex->btlfx_randCentered(0x200) - 0x200;
                a = ((work->unk4 / 2) << 10) + k;
                p.vx += (ex->math_cos(a) * r) >> 12;
                p.vz += (ex->math_sin(a) * r) >> 12;
                ((s32 *)&b->unk0)[0] = FX19F_W(p, 0);
                ((s32 *)&b->unk0)[1] = FX19F_W(p, 1);
                ((s32 *)&b->unk8)[0] = FX19F_W(q, 0);
                ((s32 *)&b->unk8)[1] = FX19F_W(q, 1);
                b->unk11 = 0;
                b->unk10 = work->unk4 % 32;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0x3E;
                b->unk14 = 0;
            }
            work->unk4++;
            FX19F_W(p, 0) = FX19F_W(base, 0);
            FX19F_W(q, 0) = FX19F_W(base, 0);
            p.vy = 0;
            FX19F_W(p, 1) = FX19F_W(base, 1);
            FX19F_W(q, 1) = FX19F_W(base, 1);
            if (frame >= 0xB) {
                b = ex->btlfx_poolSpawn(inst->unk10);
                if (b != NULL) {
                    r = ex->btlfx_randCentered(0x100) + 0x3E8;
                    a = ((work->unk4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                    p.vx += (ex->math_cos(a) * r) >> 12;
                    p.vz += (ex->math_sin(a) * r) >> 12;
                    ((s32 *)&b->unk0)[0] = FX19F_W(p, 0);
                    ((s32 *)&b->unk0)[1] = FX19F_W(p, 1);
                    ((s32 *)&b->unk8)[0] = FX19F_W(q, 0);
                    ((s32 *)&b->unk8)[1] = FX19F_W(q, 1);
                    b->unk11 = 0;
                    b->unk10 = work->unk4 % 32;
                    b->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                    b->unk12 = ex->btlfx_rand() % 15;
                    b->unk13 = ex->btlfx_rand() % 15;
                    b->unk15 = 0x32;
                    b->unk14 = 0;
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            FX19F_W(p, 0) = FX19F_W(D_801EA9AC, 0);
            FX19F_W(p, 1) = FX19F_W(D_801EA9AC, 1);
            if (frame < 0x18) {
                col = D_801EA9B4 * 100 / 2048 + 0x3C;
                if (col >= 0x100) {
                    col = 0xFF;
                }
                for (i = 0; i < 2; i++) {
                    work->unk4++;
                    FX19F_W(c, 0) = FX19F_W(p, 0);
                    FX19F_W(d, 0) = FX19F_W(p, 0);
                    FX19F_W(c, 1) = FX19F_W(p, 1);
                    FX19F_W(d, 1) = FX19F_W(p, 1);
                    b2 = ex->btlfx_poolSpawn(inst->unk10);
                    if (b2 != NULL) {
                        dist = D_801EA9B4;
                        ex->btlfx_randRotation(&q);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&q, 0x180, (s16 *)&c);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&q, dist + 0x180, (s16 *)&d);
                        ((s32 *)&b2->unk0)[0] = FX19F_W(c, 0);
                        ((s32 *)&b2->unk0)[1] = FX19F_W(c, 1);
                        ((s32 *)&b2->unk8)[0] = FX19F_W(d, 0);
                        ((s32 *)&b2->unk8)[1] = FX19F_W(d, 1);
                        b2->unk11 = 1;
                        b2->unk10 = work->unk4 % 32;
                        b2->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                        b2->unk12 = ex->btlfx_rand() % 15;
                        b2->unk13 = ex->btlfx_rand() % 15;
                        b2->unk15 = col;
                        b2->unk14 = 0;
                    }
                }
                if (frame >= 0xD) {
                    for (i = 0; i < 2; i++) {
                        work->unk4++;
                        FX19F_W(c, 0) = FX19F_W(p, 0);
                        FX19F_W(d, 0) = FX19F_W(p, 0);
                        FX19F_W(c, 1) = FX19F_W(p, 1);
                        FX19F_W(d, 1) = FX19F_W(p, 1);
                        b2 = ex->btlfx_poolSpawn(inst->unk10);
                        if (b2 != NULL) {
                            dist = D_801EA9B4;
                            ex->btlfx_randRotation(&q);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&q, 0x180, (s16 *)&c);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&q, dist + 0x180, (s16 *)&d);
                            ((s32 *)&b2->unk0)[0] = FX19F_W(c, 0);
                            ((s32 *)&b2->unk0)[1] = FX19F_W(c, 1);
                            ((s32 *)&b2->unk8)[0] = FX19F_W(d, 0);
                            ((s32 *)&b2->unk8)[1] = FX19F_W(d, 1);
                            b2->unk11 = 1;
                            b2->unk10 = work->unk4 % 32;
                            b2->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                            b2->unk12 = ex->btlfx_rand() % 15;
                            b2->unk13 = ex->btlfx_rand() % 15;
                            b2->unk15 = col - 0x32;
                            b2->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_headerPtr);
