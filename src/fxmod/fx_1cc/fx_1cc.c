#include "fxmod/fx_1cc.h"

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7720);

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7CBC);

void func_801E8284(s16 *from, s16 *to, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9C34)->func_800CFD78(from, to, -(D_801E9BD0[i] * 4), out);
}

s32 func_801E82D0(s32 mode, Fx1ccObj *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5,
                  Fx1ccInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 k;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk8 = 0;
    } else if (o->unk2 == 0) {
        SVECTOR a;
        VECTOR sc;
        SVECTOR p;
        SVECTOR q;
        SVECTOR b;
        SVECTOR c;
        SVECTOR d;
        SVECTOR *tab;
        Fx1ccObj *ch;
        Pool6CEntry *e;
        u8 *f;
        s32 n;
        s32 i;
        s32 r;
        s32 m;
        s32 w;
        s32 x;
        s32 u;

        tab = &D_801E9B68[o->unk4];
        switch (o->unk0) {
        case 0:
            if (o->unk8 == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&o->unk18);
                o->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            sc.vx = 0;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (++o->unk8 > 0) {
                o->unk0 = 1;
                o->unk8 = 0;
            }
            break;
        case 1:
            w = o->unk8;
            sc.vx = sc.vy = ((w << 10) * 0x1000) >> 12;
            sc.vz = (ex->math_sin((w << 10) >> 2) * 0x1000) >> 12;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (++o->unk8 >= 4) {
                o->unk0 = 2;
                o->unk8 = 0;
            }
            break;
        case 2:
            sc.vx = sc.vy = sc.vz = 0x1000;
            BTLFX_COPY_SVEC(&a, tab);
            a.vx -= 0x800;
            if (o->unk34 != NULL) {
                o->unk34->unk5C.vx = a.vx;
                o->unk34->unk5C.vy = a.vy;
                o->unk34->unk5C.vz = a.vz;
            }
            if (frame < 12) {
                u = (frame << 10) / 12;
                a.pad = 1;
                p.vx = o->unkC;
                p.vy = o->unk10;
                p.vz = o->unk14;
                ex->func_800CFD78((s16 *)&p, (s16 *)&a, 0x578, (s16 *)&q);
                f = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x26);
                k = (ex->math_sin(u) * 0x2000) >> 12;
                ex->func_800D23D8(f, &q, &a, k, 0, 1, ex->math_cos(u) * 2 / 3);
            }
            if (inst->unk78 != 0) {
                n = 6;
                o->unk8 = 0;
                if (o->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&c, &o->unk18);
                for (i = 0, m = 0; i < 4; m += 0x29, i++, n++) {
                    ch = o;
                    if (i != 0) {
                        ch = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (ch != NULL) {
                        ch->unk2 = 1;
                        ch->unk0 = n;
                        ch->unk4 = i;
                        ch->unk8 = 0;
                        ch->unk2C = inst->unk20[n];
                        func_801E8284((s16 *)&c, (s16 *)tab, n, (s16 *)&b);
                        BTLFX_COPY_SVEC(&ch->unk20, tab);
                        ch->unkC = b.vx << 4;
                        ch->unk10 = b.vy << 4;
                        ch->unk14 = b.vz << 4;
                        ch->unk28 = ex->btlfx_randCentered(0x84);
                        ch->unk2A = ex->btlfx_randCentered(0x84);
                        x = (ex->btlfx_rand() & 0xF) + 0x80;
                        r = (x - m) << 4;
                        BTLFX_COPY_SVEC(&d, tab);
                        d.vx += 0x80;
                        ex->func_800CFCA8(&d, -r, (s32)&ch->unk18);
                        ch->unk18 += ex->btlfx_randCentered(0x200);
                        ch->unk1A += ex->btlfx_randCentered(0x200);
                        ch->unk1C += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (sc.vx != 0) {
            sc.vx *= 2;
            sc.vy *= 2;
            sc.vz = sc.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(o->unk2C, 2);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->unkC, (s32)&sc, o->unk2C);
            ex->Hi_DrawEffModel(tab, (VECTOR *)&o->unkC, (s32)&sc, o->unk30);
        }
    } else {
        VECTOR pos;
        VECTOR sc;
        s32 c;

        t = ((o->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        o->unkC += o->unk18;
        o->unk10 += o->unk1A;
        o->unk14 += o->unk1C;
        o->unk1A += 0x40;
        o->unk20.vx += o->unk28;
        o->unk20.vy += o->unk2A;
        c = (c * 3) >> 1;
        if (o->unk0 < 6) {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x3000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2000) >> 12;
        } else {
            sc.vx = sc.vy = (ex->math_cos(t) * 0x2000) >> 12;
            sc.vz = (ex->math_cos(t) * 0x2800) >> 12;
        }
        pos.vx = o->unkC >> 4;
        pos.vy = o->unk10 >> 4;
        pos.vz = o->unk14 >> 4;
        ex->Hi_DrawEffModel(&o->unk20, &pos, (s32)&sc, o->unk2C);
        ex->Hi_ModifyEffModelAbr(o->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(o->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++o->unk8 >= 0x16) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_headerPtr);

s32 fx_1cc_op80(s32 mode, Fx1ccWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR r;
    s32 c;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s32 a;
    s32 v;
    s32 rnd;
    s32 acc;
    s32 rad;
    u8 *f;
    SVECTOR *tab;
    Fx1ccObj *ch;
    Fx1ccObj *ch3;
    Pool6CEntry *p;
    Pool6CEntry *p1;

    if (mode == 0) {
        init->unk0 = 0x88;
        init->unk8 = 0x38;
        init->unk14 = (s32)func_801E82D0;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        D_801E9C34 = env;
        ex->btlfx_initEffModels(D_801E9C44, 0x19);
        k = 0x9A;
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x29), k, 0x3DC0, 0x80, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x27), k, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x28), k, 0x3DC0, 0x80, 0);
        work->unk14 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0xBA, 0x3D40, 0, 0x80);
        work->unk18 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0xBA, 0x3D40, 0, 0x80);
        work->unk1C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 0xBA, 0x3D40, 0, 0x80);
        for (i = 0; i < 14; i++) {
            work->unk20[i] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801E9B98[i]), k, 0x3DC0, 0x80, 0);
        }
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk7A = 3;
        work->unk78 = 0;
        work->unk7C = 0;
        work->unk80 = 0;
        work->unk58.vx = env->unkC->unk38;
        work->unk58.vy = env->unkC->unk3A;
        work->unk58.vz = env->unkC->unk3C;
        work->unk60.vx = work->unk58.vx;
        work->unk60.vy = work->unk58.vy;
        work->unk60.vz = work->unk58.vz;
        work->unk60.vy -= 0x555;
        BTLFX_COPY_SVEC(&work->unk70, &work->unk58);
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->unk0;
        pos.vx = work->unk58.vx;
        pos.vy = work->unk58.vy;
        pos.vz = work->unk58.vz;
        sc.vx = sc.vy = 0x3400;
        sc.vz = 0x2200;
        c = 0;
        switch (work->unk0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / 6;
            func_801E7CBC(&work->unk58, &rot, 0x4400, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            a = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            func_801E7CBC(&work->unk58, &rot, 0x4400, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(a) * 700) >> 12), 1);
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            c = t;
            rot.pad = 1;
            f = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x26);
            ex->func_800D23D8(f, &work->unk58, &rot, ((((ex->math_sin(a) * 0x3000) >> 12) + 0x2000) * 3) >> 2, 0, 1,
                              ex->math_cos(a));
            if (frame >= 10) {
                work->unk0 = 2;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk68, &work->unk58);
                work->unk68.vy -= 0xAAA;
                p1 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk58);
                if (p1 != NULL) {
                    p1->unk22 = 0x80;
                }
                p1 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk68);
                if (p1 != NULL) {
                    p1->unk22 = 0x80;
                }
                p1 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk58);
                if (p1 != NULL) {
                    p1->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            c = 0x1000;
            t = frame << 10;
            t = 0x2000 - t;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk8);
            ex->Hi_ModifyEffModelRGB(work->unk8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14);
            if (frame < 6) {
                t = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, t >> 2, (t * 2) / 3, t);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            c = 0x1000;
            if (frame & 1) {
                ch3 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (ch3 != NULL) {
                    ch3->unk2 = 0;
                    ch3->unk4 = work->unk7A;
                    tab = &D_801E9B68[work->unk7A];
                    if (work->unk7A >= 3) {
                        ch3->unk2C = work->unkC;
                        ch3->unk30 = work->unk18;
                    } else {
                        ch3->unk2C = work->unk10;
                        ch3->unk30 = work->unk1C;
                    }
                    ch3->unkC = work->unk58.vx;
                    ch3->unk10 = work->unk58.vy;
                    ch3->unk14 = work->unk58.vz;
                    rad = 0x1DD;
                    ch3->unkC += (ex->math_sin(tab->vy + 0x800) * rad) >> 12;
                    ch3->unk14 += (ex->math_cos(tab->vy + 0x800) * rad) >> 12;
                    ch3->unk18 = ch3->unkC;
                    ch3->unk1A = ch3->unk10;
                    ch3->unk1C = ch3->unk14;
                    work->unk7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14);
            if (work->unk7A >= 6) {
                work->unk0 = 4;
                entry->unk0 = -1;
            }
            break;
        case 4:
            c = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14);
            if (frame >= 8) {
                work->unk0 = 5;
                entry->unk0 = -1;
            }
            break;
        case 5:
            c = 0x1000;
            t = (frame << 12) / 10;
            ex->func_800E806C(0, (t << 7) >> 12);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk14);
            rot.pad = 1;
            if (frame >= 10) {
                work->unk0 = 6;
                work->unk78 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    ch = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (ch != NULL) {
                        ch->unk2 = 1;
                        ch->unk0 = j;
                        ch->unk4 = j;
                        ch->unk8 = 0;
                        ch->unk2C = work->unk20[j];
                        func_801E8284((s16 *)&work->unk58, (s16 *)&rot, j, (s16 *)&r);
                        ((s32 *)&ch->unk20)[0] = ((s32 *)&rot)[0];
                        ((s32 *)&ch->unk20)[1] = ((s32 *)&rot)[1];
                        ch->unkC = r.vx << 4;
                        ch->unk10 = r.vy << 4;
                        ch->unk14 = r.vz << 4;
                        ch->unk28 = ex->btlfx_randCentered(0x5C);
                        ch->unk2A = ex->btlfx_randCentered(0x80);
                        rnd = (ex->btlfx_rand() & 0xF) + 0x5C;
                        ex->func_800CFCA8(&rot, -((rnd - j * 8) << 4), (s32)&ch->unk18);
                        ch->unk18 += ex->btlfx_randCentered(0x3E0);
                        ch->unk1C += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                c = ex->math_cos((frame << 10) / 18);
            }
            if (frame < 8) {
                v = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, v >> 2, (v * 2) / 3, v);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 24) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if ((u32)(work->unk0 - 2) < 4) {
            if (++work->unk7C < 11) {
                acc = work->unk80 + 11;
                work->unk80 = acc - (work->unk7C * 10) / 11;
            } else {
                work->unk80++;
            }
            ex->func_800E6624(work->unk80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (c != 0) {
            r.vx = 0x400;
            r.vy = 0;
            r.vz = work->unk4 * 41;
            r.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &work->unk58, &r, (work->unk4 << 6) + 0x2800,
                              0, 1, c);
            r.vz = work->unk4 * 24 + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &work->unk58, &r, (work->unk4 << 6) + 0x2800,
                              0, 1, c);
            env->unk28 = 0x127;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x25), &work->unk58, NULL, (work->unk4 << 5) + 0x2800,
                              0, 1, c);
            env->unk28 = 0;
        }
        work->unk4++;
    }
    return 0;
}
