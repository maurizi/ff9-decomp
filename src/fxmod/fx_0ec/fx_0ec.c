#include "fxmod/fx_0ec.h"

s32 func_801E7704(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 t;
    u8 *d;

    if (mode != 1) {
        t = (frame << 12) / 6;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        d = (u8 *)ex->btlfx_getFileData(entry->unk8, 6);
        ex->btlfx_drawParticles(d, pos, &rot, (ex->math_sin(t >> 2) >> 1) + 0x400, 0, 1, 0x1000 - t, 0, 0);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

void func_801E77FC(SVECTOR *pos) {
    Unk800F78A4 *env = D_801E98A0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p = ex->btlfx_poolSpawn((s16 *)D_801E98A4->unk10);
    MATRIX m;
    VECTOR v;

    if (p != NULL) {
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        ex->math_rotMatrixYXZ(&D_801E98A8, &m);
        ex->ScaleMatrix(&m, &D_801E98C0);
        m.t[0] = D_801E98B0.vx;
        m.t[1] = D_801E98B0.vy;
        m.t[2] = D_801E98B0.vz;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(pos);
        gte_rtv0tr();
        gte_stlvnl(&v);
        v.vy = 0;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
    }
}

void func_801E798C(s32 model, SVECTOR *center) {
    Fx0ecTri *t = D_801EB3D0;
    Fx0ecTri *u = D_801EB3D0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E98A0);
    Fx0ecShard *p;
    SVECTOR d;
    s32 i;
    s32 k;
    s32 r;

    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    p = D_801EBE50;
    for (i = 0; i < 0x70; p++) {
        p->unk18 = 0;
        p->unk0.vx = ((t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3) << 12;
        p->unk0.vy = ((u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3) << 12;
        p->unk0.vz = ((u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3) << 12;
        p->unk1A = 0;
        i++;
        p->unkC.pad = 0x1000;
        p->unk14 = ex->btlfx_randCentered(0x100);
        p->unk16 = ex->btlfx_randCentered(0x200);
        d.vx = (p->unk0.vx >> 12) - center->vx;
        d.vy = (p->unk0.vy >> 12) - center->vy;
        d.vz = (p->unk0.vz >> 12) - center->vz;
        p->unkC.vx = d.vx >> 4;
        t++;
        p->unkC.vy = d.vy >> 4;
        r = ex->btlfx_rand();
        u++;
        /* Dead store: the branch splits sched1's block here and is gone by final, which keeps the p++ increment
         * after the rand() call (sched2 then leaves it there and u++ fills the call's delay slot). */
        if (p == NULL) {
            k = 0;
        }
        k = (r & 0x1F) + 8;
        p->unkC.vz = (d.vz >> 4) + k;
    }
}

void func_801E7B84(void) {
    Fx0ecTri *t = D_801EB3D0;
    Fx0ecShard *s = D_801EBE50;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(D_801E98A0);
    sc.vx = sc.vy = sc.vz = 0xF80;
    for (i = 0; i < 0x70; i++, s++, t++) {
        if (s->unk18 >= 0) {
            if (s->unk18 < s->unk1A) {
                s->unk18++;
            } else {
                c.vx = s->unk0.vx >> 12;
                c.vy = s->unk0.vy >> 12;
                c.vz = s->unk0.vz >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = t->unk0[j].vx - c.vx;
                    loc[j].vy = t->unk0[j].vy - c.vy;
                    loc[j].vz = t->unk0[j].vz - c.vz;
                }
                if (s->unk18 >= 8) {
                    ex->btlfx_stepModuleParticle((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
                    if (s->unkC.vy < 0 && s->unk0.vy < 0) {
                        s->unkC.vy = -(s->unkC.vy >> 1);
                        if (s->unkC.vy < 0x3C) {
                            func_801E77FC(&c);
                            goto kill;
                        }
                    }
                }
                if (s->unk18 == ex->btlfx_rand() % 3 + 4 || s->unk18 >= 8) {
                    rot.vx = s->unk14;
                    rot.vy = s->unk16;
                    rot.vz = 0;
                } else {
                    rot.vx = 0;
                    rot.vy = 0;
                    rot.vz = 0;
                }
                ex->math_rotMatrixYXZ(&rot, &m);
                if (s->unk18 >= 8) {
                    ex->ScaleMatrix(&m, &sc);
                }
                m.t[0] = s->unk0.vx >> 12;
                m.t[1] = s->unk0.vy >> 12;
                m.t[2] = s->unk0.vz >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (j = 0; j < 3; j++) {
                    gte_ldv0(&loc[j]);
                    gte_rtv0tr();
                    gte_stlvnl(&v);
                    t->unk0[j].vx = v.vx;
                    t->unk0[j].vy = v.vy;
                    t->unk0[j].vz = v.vz;
                }
                if (++s->unk18 >= 0x47) {
                kill:
                    s->unk18 = -1;
                    t->unk0[2].vz = 0;
                    *(s32 *)&t->unk0[2] = 0;
                    t->unk0[1].vz = 0;
                    *(s32 *)&t->unk0[1] = 0;
                    t->unk0[0].vz = 0;
                    /* the nested do-while(0)s weight t's refs by loop depth, which puts t ahead of ex in
                     * global-alloc (t in s3, ex in s4) */
                    do {
                        do {
                            *(s32 *)&t->unk0[0] = 0;
                        } while (0);
                    } while (0);
                }
            }
        }
    }
}

s32 func_801E7F00(u8 *buf) {
    Fx0ecShard *p = D_801ECAB0;
    s32 i;

    D_801ECA90.unk0 = 0;
    D_801ECA90.unk2 = 0;
    D_801ECA90.unk10 = buf;
    D_801ECA90.unkC = buf;
    for (i = 0; i < 300; i++) {
        p->unk18 = -1;
        p++;
    }
}

s32 func_801E7F40(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E98A0);
    Fx0ecSplit *sp = &D_801ECA90;
    Fx0ecVertex *v = (Fx0ecVertex *)sp->unk10;
    s32 n;
    s32 i;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / sizeof(Fx0ecVertex);
    ex->Hi_SplitMdlVertex(model, -1, (s32)v);
    {
        s32 k = D_801ECA90.unk0;
        sp->unk14[k] = (u8 *)v;
        sp->unk4[k] = n;
    }
    sp->unk10 = (u8 *)(v + n);
    sp->unk2 += n;
    D_801ECA90.unk0++;
    for (i = 0; i < n; i++) {
        v->unk6 = 0;
        v++;
    }
    return sp->unk2;
}

Fx0ecShard *func_801E802C(void) {
    Fx0ecShard *p = D_801ECAB0;
    s32 i;

    for (i = 0; i < 300; i++) {
        if (p->unk18 < 0) {
            return p;
        }
        p++;
    }
    return NULL;
}

s32 func_801E8064(s32 y, SVECTOR *center) {
    Fx0ecTri *t;
    Fx0ecTri *u;
    BtlfxExports *ex;
    Fx0ecShard *p;
    SVECTOR d;
    s32 i;
    s32 k;
    s32 n;
    Fx0ecSplit *sp;

    i = k = 0;
    sp = &D_801ECA90;
    t = (Fx0ecTri *)sp->unkC;
    u = t;
    ex = BTLFX_EXPORTS(D_801E98A0);
    for (n = k; i < sp->unk2; i++, u++, t++, k++) {
        if (y < (u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3 && u->unk0[0].pad == 0) {
            p = func_801E802C();
            if (p != NULL) {
                u->unk0[0].pad = 1;
                p->unk0.vx = ((t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3) << 12;
                p->unk0.vy = ((u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3) << 12;
                p->unk0.vz = ((u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = k;
                p->unkC.pad = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                d.vx = (p->unk0.vx >> 12) - center->vx;
                d.vy = (p->unk0.vy >> 12) - center->vy;
                d.vz = (p->unk0.vz >> 12) - center->vz;
                n++;
                p->unkC.vx = (d.vx >> 3) + ex->btlfx_randCentered(0x10);
                p->unkC.vy = ex->btlfx_randCentered(0x10);
                p->unkC.vz = ex->btlfx_randCentered(0x20) + 0x1E;
            }
        }
    }
    return n;
}

void func_801E82D8(void) {
    Fx0ecSplit *sp = &D_801ECA90;
    Fx0ecShard *s = D_801ECAB0;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    Fx0ecTri *t;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(D_801E98A0);
    sc.vx = sc.vy = sc.vz = 0xEC0;
    for (i = 0; i < 300; i++, s++) {
        if (s->unk18 >= 0) {
            t = (Fx0ecTri *)sp->unkC + s->unk1A;
            c.vx = s->unk0.vx >> 12;
            c.vy = s->unk0.vy >> 12;
            c.vz = s->unk0.vz >> 12;
            for (j = 0; j < 3; j++) {
                loc[j].vx = t->unk0[j].vx - c.vx;
                loc[j].vy = t->unk0[j].vy - c.vy;
                loc[j].vz = t->unk0[j].vz - c.vz;
            }
            ex->btlfx_stepModuleParticle((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
            if (s->unkC.vy < 0 && s->unk0.vy < 0) {
                func_801E77FC(&c);
                goto kill;
            }
            rot.vx = s->unk14;
            rot.vy = s->unk16;
            rot.vz = 0;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sc);
            m.t[0] = s->unk0.vx >> 12;
            m.t[1] = s->unk0.vy >> 12;
            m.t[2] = s->unk0.vz >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            for (j = 0; j < 3; j++) {
                gte_ldv0(&loc[j]);
                gte_rtv0tr();
                gte_stlvnl(&v);
                t->unk0[j].vx = v.vx;
                t->unk0[j].vy = v.vy;
                t->unk0[j].vz = v.vz;
            }
            if (++s->unk18 >= 0x20) {
            kill:
                s->unk18 = -1;
                t->unk0[2].vz = 0;
                *(s32 *)&t->unk0[2] = 0;
                t->unk0[1].vz = 0;
                *(s32 *)&t->unk0[1] = 0;
                t->unk0[0].vz = 0;
                *(s32 *)&t->unk0[0] = 0;
            }
        }
    }
}

s32 fx_0ec_op80(s32 mode, Fx0ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR sv;
    VECTOR pos;
    VECTOR sc;
    MATRIX m;
    VECTOR out;
    SVECTOR c[2]; /* [1] unused: the original's frame has 8 more bytes of locals */
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 h;
    s32 r;
    s32 i;
    s32 j0; /* model index per case: block-local, so local-alloc gives it s0 */
    s32 j1;
    s32 j2;
    s32 x;
    s32 x2;
    s32 tp0;
    s32 tp1;
    s32 k;

    if (mode == 0) {
        init->unk0 = 0x64;
        init->unk8 = 8;
        init->unk14 = (s32)func_801E7704;
        init->unk4 = 0x80;
    } else if (mode == 1) {
        D_801E98A0 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        r = ex->btlfx_getUnitRadius(0) + 0xF0;
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk10);
        work->unk10.vy &= 0xFFF;
        if (work->unk10.vy - 0x400U > 0x800) {
            work->unk10.vy = 0;
        } else {
            work->unk10.vy = 0x800;
        }
        ex->func_800CFD78((s16 *)&work->unk8, (s16 *)&work->unk10, r, (s16 *)&work->unk18);
        if (ex->btlfx_isEnemyUnit(0) != 0 && work->unk18.vz < -0x200) {
            work->unk18.vz = -0x200;
        }
        ex->func_800CFCA8(&work->unk10, -0xD2, (s32)&rot);
        sv.vx = work->unk18.vx + rot.vx;
        sv.vy = work->unk18.vy + rot.vy - 0x140;
        sv.vz = work->unk18.vz + rot.vz;
        env->unk2C->unk0[1].vx = sv.vx;
        env->unk2C->unk0[1].vy = sv.vy;
        env->unk2C->unk0[1].vz = sv.vz;
        ex->func_800CFCA8(&work->unk10, -ex->btlfx_getUnitRadius(0), (s32)&rot);
        sv.vx = work->unk8.vx + rot.vx;
        sv.vy = work->unk8.vy + rot.vy - 0x140;
        sv.vz = work->unk8.vz + rot.vz;
        env->unk2C->unk0[2].vx = sv.vx;
        env->unk2C->unk0[2].vy = sv.vy;
        env->unk2C->unk0[2].vz = sv.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_initEffModels(D_801EEB80, 9);
        work->unk38 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0x97, 0x3D40,
                                                      0x80, 0x80);
        work->unk3C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x97, 0x3D40,
                                                      0x80, 0x80);
        work->unk40[0] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0xB7,
                                                         0x3DC0, 0, 0x80);
        work->unk4C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x97, 0x3DC0,
                                                      0, 0x80);
        work->unk50 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0x39, 0x3D01,
                                                      0, 0);
        work->unk54 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0x119,
                                                      0xFFFF, 0, 0x90);
        work->unk58 = (s16)ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        x = env->unk18->disp.x;
        if (x < 0x100) {
            x2 = x + 0x140;
        } else {
            x2 = x - 0x140;
        }
        tp0 = getTPage(2, 0, x, env->unk18->disp.y);
        tp1 = getTPage(2, 0, x2, env->unk18->disp.y);
        work->unk40[1] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), tp0 | 0x20,
                                                         0xFFFF, 0x40, 0);
        work->unk40[2] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), tp1 | 0x20,
                                                         0xFFFF, 0x40, 0);
        D_801E98A4 = (Fx0ecWork *)entry; /* the shard callbacks spawn into entry->unk10 */
        k = (ex->btlfx_getUnitRadius(0) << 12) / 390;
        if (k < 0x1400) {
            k = 0x1400;
        }
        if (k > 0x1755) {
            k = 0x1755;
        }
        work->unk5C = k;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk18);
        if (p != NULL) {
            p->unk22 = -0x40;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk20);
        if (p != NULL) {
            p->unk22 = 0x80;
        }
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        sc.vx = sc.vy = sc.vz = work->unk5C;
        rot.vx = work->unk10.vx;
        rot.vy = work->unk10.vy;
        rot.vz = work->unk10.vz;
        t = (frame << 12) / 22;
        rot.vy += (0x1000 - t) >> 1;
        pos.vx = work->unk18.vx;
        pos.vy = work->unk18.vy;
        pos.vz = work->unk18.vz;
        h = (ex->math_cos(t >> 2) * 0x514) >> 12;
        pos.vy += h;
        work->unk20.vx = work->unk18.vx;
        work->unk20.vy = work->unk18.vy;
        work->unk20.vz = work->unk18.vz;
        work->unk20.vy -= (t * 0x140) >> 12;
        p = (Pool6CEntry *)ex->btlfx_getFileData(entry->unk8, 9);
        sv.vx = pos.vx;
        sv.vy = pos.vy;
        sv.vz = pos.vz;
        sv.vy -= (((SVECTOR *)p)->pad * sc.vy) >> 12;
        env->unk2C->unk0[0].vx = sv.vx;
        env->unk2C->unk0[0].vy = sv.vy;
        env->unk2C->unk0[0].vz = sv.vz;
        ex->Hi_SetEffModelSlice(work->unk38, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->unk38);
        ex->Hi_SetEffModelSlice(work->unk3C, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->unk3C);
        j0 = (work->unk4 & 1) + 1;
        ex->Hi_SetEffModelSlice(work->unk40[j0], h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->unk40[j0]);
        ex->Hi_ModifyEffModelRGB(work->unk40[j0], ((t / 2 + 0x800) >> 5) - 0x80, ((t / 2 + 0x800) >> 5) - 0x80,
                                 ((t / 2 + 0x800) >> 5) - 0x80);
        ex->Hi_SetEffModelSlice(work->unk54, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->unk54);
        ex->Hi_ModifyEffModelRGB(work->unk54, -0x60, -0x60, -0x60);
        if (frame >= 22) {
            work->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    case 1:
        sc.vx = sc.vy = sc.vz = work->unk5C;
        rot.vx = work->unk10.vx;
        rot.vy = work->unk10.vy;
        rot.vz = work->unk10.vz;
        pos.vx = work->unk18.vx;
        pos.vy = work->unk18.vy;
        pos.vz = work->unk18.vz;
        t = (frame << 12) / 20;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk3C);
        ex->Hi_SetEffModelOffset(work->unk58, 0x28);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk58);
        ex->Hi_SetEffModelOffset(work->unk54, 0x14);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk54);
        ex->Hi_ModifyEffModelRGB(work->unk54, ((t * 3 / 4 + 0x400) >> 5) - 0x80, ((t * 3 / 4 + 0x400) >> 5) - 0x80,
                                 ((t * 3 / 4 + 0x400) >> 5) - 0x80);
        j1 = (work->unk4 & 1) + 1;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk40[j1]);
        ex->Hi_ModifyEffModelRGB(work->unk40[j1], ((0x1000 - t / 2) >> 5) - 0x80, ((0x1000 - t / 2) >> 5) - 0x80,
                                 ((0x1000 - t / 2) >> 5) - 0x80);
        if (frame >= 20) {
            work->unk0 = 2;
            entry->unk0 = -1;
        }
        break;
    case 2:
        sc.vx = sc.vy = sc.vz = work->unk5C;
        rot.vx = work->unk10.vx;
        rot.vy = work->unk10.vy;
        rot.vz = work->unk10.vz;
        pos.vx = work->unk18.vx;
        pos.vy = work->unk18.vy;
        pos.vz = work->unk18.vz;
        t = (frame << 12) / 30;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk3C);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk40[0]);
        if (frame < 8) {
            ex->Hi_ModifyEffModelAbr(work->unk40[0], 1);
            ex->Hi_ModifyEffModelRGB(work->unk40[0], ((frame << 9) >> 5) - 0x80, ((frame << 9) >> 5) - 0x80,
                                     ((frame << 9) >> 5) - 0x80);
        }
        ex->Hi_SetEffModelOffset(work->unk58, 0x31);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk58);
        ex->Hi_SetEffModelOffset(work->unk54, 0x27);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk54);
        {
            s32 q = t >> 2;

            ex->Hi_ModifyEffModelRGB(work->unk54, ((0x1000 - ex->math_sin(q)) >> 5) - 0x80,
                                     ((0x1000 - ex->math_sin(q)) >> 5) - 0x80, ((0x1000 - ex->math_sin(q)) >> 5) - 0x80);
        }
        if (frame < 16) {
            j2 = (work->unk4 & 1) + 1;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk40[j2]);
            ex->Hi_ModifyEffModelRGB(work->unk40[j2], ((0x1000 - (frame << 8)) >> 6) - 0x80,
                                     ((0x1000 - (frame << 8)) >> 6) - 0x80, ((0x1000 - (frame << 8)) >> 6) - 0x80);
        }
        if (frame == 20) {
            for (i = 0; i < 2; i++) {
                /* two sets keep cse2 from folding data + 4 into the first read (the original reads all three
                 * through v1); the register is pinned because local-alloc otherwise ties v to the call's v0 */
                register SVECTOR *v REG_ASM("$3");

                v = (SVECTOR *)ex->btlfx_getFileData(entry->unk8, D_801E9850[i]);
                v = (SVECTOR *)((u8 *)v + 4);
                sv.vx = v->vx;
                sv.vy = v->vy;
                sv.vz = v->vz;
                gte_SetRotMatrix(env->unk14);
                gte_SetTransMatrix(env->unk14);
                ex->math_rotMatrixYXZ(&rot, &m);
                ex->ScaleMatrix(&m, &sc);
                m.t[0] = pos.vx;
                m.t[1] = pos.vy;
                m.t[2] = pos.vz;
                gte_SetRotMatrix(&m);
                gte_SetTransMatrix(&m);
                gte_ldv0(&sv);
                gte_rtv0tr();
                gte_stlvnl(&out);
                setVector(&work->unk28[i], out.vx, out.vy, out.vz);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk28[i]);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
        }
        if (frame >= 30) {
            work->unk0 = 3;
            entry->unk0 = -1;
            ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 100, 100, 0);
            func_801E7F00(D_801E98D0);
            func_801E7F40(work->unk38);
            func_801E7F40(work->unk3C);
            c[0].vx = 0;
            c[0].vy = 0;
            c[0].vz = 0;
            func_801E798C(work->unk4C, c);
        }
        break;
    case 3:
        if (frame == 8) {
            ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0xC8, 0, 0);
        }
        if (frame >= 8) {
            if (frame == 8) {
                work->unk20.vx = work->unk18.vx;
                work->unk20.vy = work->unk18.vy;
                work->unk20.vz = work->unk18.vz;
                work->unk20.vy -= (work->unk5C << 10) >> 12;
                work->unk60 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk18);
                if (p == NULL) {
                    goto skip; /* the original's NULL branch skips the unk60 check (unk60 is NULL then) */
                }
                p->unk2C = (s32)&work->unk20;
            }
            if (work->unk60 != NULL && work->unk60->unk30 == -1) {
                work->unk60 = NULL;
            }
        skip:
            sv.vx = work->unk10.vx;
            sv.vy = work->unk10.vy;
            sv.vz = work->unk10.vz;
            sv.vy += 0x800;
            if (work->unk60 != NULL) {
                work->unk60->unk5C.vx = sv.vx;
                work->unk60->unk5C.vy = sv.vy;
                work->unk60->unk5C.vz = sv.vz;
            }
        }
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
        sc.vx = sc.vy = sc.vz = work->unk5C;
        rot.vx = work->unk10.vx;
        rot.vy = work->unk10.vy;
        rot.vz = work->unk10.vz;
        pos.vx = work->unk18.vx;
        pos.vy = work->unk18.vy;
        pos.vz = work->unk18.vz;
        D_801E98C0.vx = sc.vx;
        D_801E98C0.vy = sc.vy;
        D_801E98C0.vz = sc.vz;
        D_801E98A8.vx = rot.vx;
        D_801E98A8.vy = rot.vy;
        D_801E98A8.vz = rot.vz;
        D_801E98A8.vy += 0x800;
        D_801E98B0.vx = pos.vx;
        D_801E98B0.vy = pos.vy;
        D_801E98B0.vz = pos.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk38);
        ex->Hi_ModifyEffModelRGB(work->unk38, 0, 0, 0);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk3C);
        ex->Hi_ModifyEffModelRGB(work->unk3C, 0, 0, 0);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk4C);
        {
            s32 a;

            if (frame < 8) {
                a = (frame << 8) + 0x800;
            } else {
                a = 0x1000;
            }
            ex->Hi_ModifyEffModelRGB(work->unk4C, (a >> 5) - 0x80, (a >> 5) - 0x80, (a >> 5) - 0x80);
        }
        if (frame < 47) {
            func_801E8064(1000 - (((0x1000 - ex->math_cos((frame << 10) / 47)) * 1000) >> 12), &sv);
        }
        func_801E82D8();
        func_801E7B84();
        if (frame >= 71) {
            work->unk0 = 3;
            entry->unk0 = -1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x11), work->unk4);
    work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ec/nonmatchings/fx_0ec", fx_0ec_headerPtr);
