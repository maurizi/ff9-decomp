#include "fxmod/fx_018.h"

INCLUDE_ASM("asm/us/fx_018/nonmatchings/fx_018", fx_018_op80);

void func_801E82FC(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 0xE;
    strip->unk0 = 0xE;
    strip->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tbl++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++;
        s = (ex->math_sin(t >> 1) * s) >> 12;
        s += ex->btlfx_lerp(from1, to1, t);
        ex->func_800D044C(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg11;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C88[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C88[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E8564(Unk80100C78Entry *entry, Fx018Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->unk4;
    strip->unk2 = 2;
    strip->unk0 = 2;
    strip->unk1 = 0;
    for (i = 0; i < 3; i++) {
        t = 0x1000 - (i << 11);
        t += *tbl++ / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++ / 2;
        t >>= 1;
        s = (ex->math_sin(t) * s) >> 12;
        s += (ex->math_sin(t) * amp) >> 12;
        ex->func_800D044C(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg10;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C90[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9C90[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

void func_801E87E8(Unk80100C78Entry *entry, Fx018Strip *strip, Fx018Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->unk4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(D_801E9CE8);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    func_801E8564(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x33, 0, arg5, arg6, kind);
}

s32 func_801E88DC(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx018Strip *strip;
    Fx018Strip *br;
    s32 n;
    s32 c;
    s32 s;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
    } else if (it->unk11 == 0) {
        if (it->unk14 < 2) {
            n = 8;
            if (D_801EA9D0 != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9E90[it->unk10];
            br = (Fx018Strip *)&D_801EA650[it->unk10];
            func_801E82FC(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 12, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18, it->unk12, c, ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13,
                          it->unk14);
            func_801E87E8(entry, strip, br, it->unk18 + 3, it->unk13, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        } else {
            n = 6;
            if (D_801EA9D0 != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9E90[it->unk10];
            br = (Fx018Strip *)&D_801EA650[it->unk10];
            func_801E82FC(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E8C84(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9CE8);
    SVECTOR *p = D_801E9E90[ex->btlfx_rand() % 16].unk4;

    p += ex->btlfx_rand() % 15;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

s32 fx_018_op83(s32 mode, Fx018Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    Fx018Bolt *it0;
    s32 r0;
    s32 ang0;
    Fx018Bolt *it1;
    s32 r1;
    s32 ang1;
    Fx018Bolt *it2;
    s32 r2;
    s32 ang2;
    Fx018Bolt *it3;
    Fx018Bolt *it4;
    s32 r4;
    Fx018Bolt *it5;
    Pool6CEntry *p;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 rnd2;
    s32 rnd3;
    s32 rnd5;
    s32 base2;
    s32 base3;
    s32 i;
    s32 j;
    s32 s;

    if (mode == 0) {
        init->unk0 = 0x1C;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E88DC;
        init->unk4 = 0x10;
    } else if (mode == 1) {
        D_801E9CE8 = env;
        kind = init->unk0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        i = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        q = work->unk10 = D_801EA9E0;
        for (; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        work->unk18 = kind;
        D_801E9CEC = entry;
        work->unk14 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk18) {
        case 0:
            D_801EA9D0 = 0;
            if (frame < 29) {
                if (frame & 1) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    it0 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it0 != NULL) {
                        BTLFX_COPY_SVEC(&it0->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r0 = ex->btlfx_randCentered(0x100) + 0x4B0;
                        ang0 = (work->unk14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang0) * r0) >> 12;
                        b.vz += (ex->math_sin(ang0) * r0) >> 12;
                        BTLFX_COPY_SVEC(&it0->unk8, &b);
                        it0->unk10 = work->unk14 % 16;
                        it0->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it0->unk12 = ex->btlfx_rand() % 13;
                        it0->unk13 = ex->btlfx_rand() % 13;
                        it0->unk15 = 0;
                        it0->unk14 = 2;
                    }
                }
            }
            if (frame >= 29) {
                return 1;
            }
            break;
        case 1:
            D_801EA9D0 = 0;
            if (frame == 0) {
                for (j = 0; j < 6; j++) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    it1 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it1 != NULL) {
                        BTLFX_COPY_SVEC(&it1->unk0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r1 = ex->btlfx_randCentered(0x80) + 0x3E8;
                        ang1 = (work->unk14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        b.vx += (ex->math_cos(ang1) * r1) >> 12;
                        b.vz += (ex->math_sin(ang1) * r1) >> 12;
                        BTLFX_COPY_SVEC(&it1->unk8, &b);
                        it1->unk10 = work->unk14 % 16;
                        it1->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it1->unk12 = ex->btlfx_rand() % 13;
                        it1->unk13 = ex->btlfx_rand() % 13;
                        it1->unk15 = -12;
                        it1->unk14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= 6 && frame < 16) {
                work->unk14++;
                BTLFX_COPY_SVEC(&c, &work->unk0);
                c.vy = 0;
                it2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it2 != NULL) {
                    BTLFX_COPY_SVEC(&it2->unk0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    rnd2 = ex->btlfx_randCentered(0x100);
                    base2 = (frame - 6) << 8;
                    rnd2 += 0x5DC;
                    r2 = base2 + rnd2;
                    ang2 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it2->unk8, &d);
                    it2->unk10 = work->unk14 % 16;
                    it2->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it2->unk12 = ex->btlfx_rand() % 13;
                    it2->unk13 = ex->btlfx_rand() % 13;
                    it2->unk15 = 7;
                    it2->unk14 = ex->btlfx_rand() & 1;
                }
                work->unk14++;
                BTLFX_COPY_SVEC(&c, &work->unk0);
                c.vy = 0;
                it3 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it3 != NULL) {
                    BTLFX_COPY_SVEC(&it3->unk0, &c);
                    BTLFX_COPY_SVEC(&d, &c);
                    d.vy -= 0xC00;
                    rnd3 = ex->btlfx_randCentered(0x100);
                    base3 = (frame - 6) << 8;
                    rnd3 += 0x7D0;
                    r2 = base3 + rnd3;
                    ang2 = ((work->unk14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    d.vx += (ex->math_cos(ang2) * r2) >> 12;
                    d.vz += (ex->math_sin(ang2) * r2) >> 12;
                    BTLFX_COPY_SVEC(&it3->unk8, &d);
                    it3->unk10 = work->unk14 % 16;
                    it3->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it3->unk12 = ex->btlfx_rand() % 13;
                    it3->unk13 = ex->btlfx_rand() % 13;
                    it3->unk15 = 0;
                    it3->unk14 = 2;
                }
            }
            if (frame == 8 || frame == 10 || frame == 13) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x10, 0x40, 0x80);
            }
            if (frame == 9) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
            }
            if (frame >= 20) {
                return 1;
            }
            break;
        case 2:
            D_801EA9D0 = 1;
            if (frame == 0) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&work->unk0);
                if (p != NULL) {
                    p->unk24->unk0 = -1;
                    ((EffTailSpawn *)p->unk24)->unk3C = func_801E8C84;
                    p->unk22 = 0x20;
                }
            }
            if (frame < 31) {
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&D_801E9CE0, &work->unk0);
                }
                s = frame * 250 / 31 + 0x333;
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                c.vz = 0;
                BTLFX_COPY_SVEC(&b, &a);
                r4 = s + ex->btlfx_randCentered(0x46);
                c.vy = ex->btlfx_rand();
                c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                ex->func_800CFD78((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                work->unk14++;
                BTLFX_COPY_SVEC(&a, &work->unk0);
                a.vy = 0;
                it4 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it4 != NULL) {
                    BTLFX_COPY_SVEC(&it4->unk0, &b);
                    BTLFX_COPY_SVEC(&it4->unk8, &D_801E9CE0);
                    it4->unk10 = work->unk14 % 16;
                    it4->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                    it4->unk12 = ex->btlfx_rand() % 13;
                    it4->unk13 = ex->btlfx_rand() % 13;
                    it4->unk15 = -(frame * 50 / 31);
                    it4->unk14 = ex->btlfx_rand() & 1;
                }
                BTLFX_COPY_SVEC(&D_801E9CE0, &b);
                if (frame < 19) {
                    work->unk14++;
                    BTLFX_COPY_SVEC(&a, &work->unk0);
                    a.vy = 0;
                    c.vz = 0;
                    it5 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (it5 != NULL) {
                        BTLFX_COPY_SVEC(&it5->unk8, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        rnd5 = ex->btlfx_randCentered(0x46) + 0x400;
                        r4 = s + rnd5;
                        c.vy = ex->btlfx_rand();
                        c.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                        ex->func_800CFD78((s16 *)&b, (s16 *)&c, r4, (s16 *)&b);
                        BTLFX_COPY_SVEC(&it5->unk0, &b);
                        it5->unk10 = work->unk14 % 16;
                        it5->unk18 = work->unk10 + (ex->btlfx_rand() & 0x1F);
                        it5->unk12 = ex->btlfx_rand() % 13;
                        it5->unk13 = ex->btlfx_rand() % 13;
                        it5->unk15 = 0;
                        it5->unk14 = 2;
                    }
                }
            }
            if (frame & 1) {
                if (ex->btlfx_rand() & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x10, 0x20, 0x46);
                }
            }
            if (frame >= 31) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_018/nonmatchings/fx_018", fx_018_headerPtr);
