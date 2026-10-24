#include "fxmod/fx_1a2.h"

void func_801E7718(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx1a2Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E9844;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx1a2Seg *s;
    u32 i;
    s32 p0;
    s32 p1;

    if (p < 0) {
        p = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (ca == NULL) {
        c0.r = c0.g = c0.b = 0;
    } else {
        gte_DpqColorFar_nst(ca, p);
        gte_strgb(&c0);
    }
    if (cb == NULL) {
        c1.r = c1.g = c1.b = 0;
    } else {
        gte_DpqColorFar_nst(cb, p);
        gte_strgb(&c1);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    c0.cd = abr != 0xFF ? 0x3A : 0x38;
    c1.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        p0 = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        p1 = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&c0, p0);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&c1, p0);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&c0, p1);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&c1, p1);
            gte_strgb(&prim->r3);
        }
    }
}

void func_801E7B88(s32 model, SVECTOR *center) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9844);
    SVECTOR v;
    Fx1a2Tri *t;
    Fx1a2Piece *p;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)D_801E9854);
    t = D_801E9854;
    p = D_801EA484;
    for (i = 0; i < 130; i++, t++, p++) {
        p->unk18 = 0;
        p->unk0 = ((t->unk0[0].vx + t->unk0[1].vx + t->unk0[2].vx) / 3) << 12;
        p->unk4 = ((t->unk0[0].vy + t->unk0[1].vy + t->unk0[2].vy) / 3) << 12;
        p->unk8 = ((t->unk0[0].vz + t->unk0[1].vz + t->unk0[2].vz) / 3) << 12;
        p->unk1A = 0;
        p->unk12 = 0x1000;
        p->unk14 = ex->btlfx_randCentered(0x100);
        p->unk16 = ex->btlfx_randCentered(0x200);
        v.vx = (p->unk0 >> 12) - center->vx;
        v.vy = (p->unk4 >> 12) - center->vy;
        v.vz = (p->unk8 >> 12) - center->vz;
        p->unkC = v.vx >> 2;
        p->unkE = v.vy >> 3;
        p->unk10 = v.vz >> 2;
    }
}

void func_801E7D68(void) {
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR v[3];
    SVECTOR bias;
    VECTOR o;
    VECTOR scale;
    MATRIX m;
    BtlfxExports *ex;
    Fx1a2Tri *t;
    Fx1a2Piece *p;
    s32 i;
    s32 k;

    t = D_801E9854;
    p = D_801EA484;
    ((s32 *)&bias)[0] = 0;
    ((s32 *)&bias)[1] = 0;
    ex = BTLFX_EXPORTS(D_801E9844);
    scale.vx = scale.vy = scale.vz = 0xF80;
    for (i = 0; i < 130; i++, p++, t++) {
        if (p->unk18 >= 0) {
            if (p->unk18 < p->unk1A) {
                p->unk18++;
            } else {
                c.vx = p->unk0 >> 12;
                c.vy = p->unk4 >> 12;
                c.vz = p->unk8 >> 12;
                for (k = 0; k < 3; k++) {
                    v[k].vx = t->unk0[k].vx - c.vx;
                    v[k].vy = t->unk0[k].vy - c.vy;
                    v[k].vz = t->unk0[k].vz - c.vz;
                }
                if (p->unk18 >= 3) {
                    ex->btlfx_stepModuleParticle((Pool6CEntry *)p, -3, &bias, &out);
                    if (p->unkE < 0 && p->unk4 < 0) {
                        p->unkE = -(p->unkE >> 1);
                    }
                }
                rot.vx = p->unk14;
                rot.vy = p->unk16;
                rot.vz = 0;
                ex->func_800316D8(&rot, &m);
                ex->ScaleMatrix(&m, &scale);
                m.t[0] = p->unk0 >> 12;
                m.t[1] = p->unk4 >> 12;
                m.t[2] = p->unk8 >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (k = 0; k < 3; k++) {
                    gte_ldv0(&v[k]);
                    gte_rtv0tr();
                    gte_stlvnl(&o);
                    t->unk0[k].vx = o.vx;
                    t->unk0[k].vy = o.vy;
                    t->unk0[k].vz = o.vz;
                }
                if (++p->unk18 >= 0x40) {
                    p->unk18 = -1;
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
}

s32 func_801E804C(s32 mode, Fx1a2Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1a2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    VECTOR pos;
    SVECTOR rot;
    s32 t;
    s32 d;
    s32 y;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk4 = (ex->btlfx_rand() & 0x7F) + 100;
    } else {
    d = 0;
    switch (o->unk8) {
    case 0:
        t = frame << 9;
        ex->func_800CFD78((s16 *)&o->unkC, (s16 *)&o->unk14, (o->unk1C << 5) >> 12, (s16 *)&o->unkC);
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (o->unk2 * (0x1000 - t)) >> 12;
        y = t >> 1; /* a fresh single-set temporary (sched1 birthing boost) */
        pos.vy -= (ex->math_sin(y) * o->unk4) >> 12;
        d = -(ex->math_sin(y) >> 2);
        if (frame >= 8) {
            o->unk8 = 1;
            ((s16 *)o)[-1] = -1;
            o->unk2 = 0;
            o->unk6 = (ex->btlfx_rand() & 3) + 1;
        }
        break;
    case 1:
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (ex->btlfx_rand() & 0xF) + o->unk2;
        if (frame >= o->unk6) {
            o->unk8 = 0;
            ((s16 *)o)[-1] = -1;
            if (o->unk1C < 0x1800) {
                o->unk14.vy += ex->btlfx_randCentered(0x400);
            }
            o->unk4 = ex->btlfx_rand() % 160 + 0x118;
            if (++o->unk0 >= 10) {
                return 1;
            }
        }
        break;
    }
    rot.vx = o->unk14.vx;
    rot.vy = o->unk14.vy;
    rot.vz = o->unk14.vz;
    rot.vx += 0x400;
    rot.vx += d;
    rot.vy += 0x800;
    scale.vx = scale.vy = scale.vz = o->unk1C;
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, inst->unk48);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a2/nonmatchings/fx_1a2", fx_1a2_headerPtr);

s32 fx_1a2_op80(s32 mode, Fx1a2Work80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR a;
    VECTOR scale;
    VECTOR pos;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    SVECTOR e;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m;
    SVECTOR sv;
    Fx1a2Rock *o;
    Pool6CEntry *p;
    SVECTOR *tbl;
    s32 frame;
    s32 t;
    s32 s;
    s32 k;
    s32 i;
    s32 cc;
    SVECTOR *q;

    if (mode == 0) {
        init->unk0 = 0x54;
        init->unk8 = 0x20;
        init->unk14 = (s32)func_801E804C;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        w->unk0 = 0;
        w->unk4 = 0;
        D_801E9844 = env;
        ex->btlfx_initEffModels(D_801EB2C4, 10);
        w->unk3C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0xB8, 0x3DC0, 0x80, 0);
        w->unk40 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xB8, 0x3DC0, 0x80, 0);
        w->unk30[0] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0xBA, 0x3E40, 0, 0xE0);
        w->unk30[1] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0xBA, 0x3E40, 0, 0xE0);
        w->unk30[2] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0xBA, 0x3E40, 0, 0xE0);
        w->unk44 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xB8, 0x3DC0, 0x80, 0);
        w->unk4C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0x98, 0x3E00, 0, 0x80);
        w->unk50 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x98, 0x3E00, 0, 0x80);
        rot.vx = rot.vz = 0x97;
        rot.vy = rot.pad = 0x3D40;
        a.vy = a.vx = a.vz = 0;
        a.pad = 0x80;
        w->unk48 = (s16)((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->unk8, 6), &rot, &a);
        ex->btlfx_getUnitPos(0, (s16 *)&w->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&w->unk10);
        ex->func_800E8980(0);
        w->unk2C = 0x8F8;
    } else {
    frame = entry->unk0;
    switch (w->unk0) {
    case 0:
        t = (frame << 12) / 93;
        ex->func_800DC818(0x10, 0x10, (u16 *)&a);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m1);
        ex->func_800CFE88(&m1, 0x10E, (s32)&b);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m2);
        ex->func_800CFE88(&m2, 0x147, (s32)&c);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m3);
        ex->func_800CFE88(&m3, 0x155, (s32)&d);
        if (frame == 0) {
            func_801E7718(&a, &b, 0x407, &D_801E97F0, &D_801E97F4, 0, D_801EC1B4, 1);
            func_801E7718(&b, &c, 0x407, &D_801E97F4, &D_801E97F8, 0, D_801EC3B4, 1);
            func_801E7718(&c, &d, 0x407, &D_801E97F8, &D_801E97F0, 0, D_801EC5B4, 1);
        }
        func_801E7718(&a, &b, 0x1F, &D_801E97F0, &D_801E97F4, 0x1000, D_801EC1B4, 1);
        func_801E7718(&b, &c, 0x1F, &D_801E97F4, &D_801E97F8, 0x1000, D_801EC3B4, 1);
        func_801E7718(&c, &d, 0x1F, &D_801E97F8, &D_801E97F0, 0x1000, D_801EC5B4, 1);
        env->unk28 = 0x20;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &c, NULL, 0x400, 0, 1, ex->math_sin(t >> 2) >> 1);
        e.pad = 0;
        e.vy = 0;
        e.vx = 0;
        e.vz = frame << 4;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &c, &e, 0x800, 0, 1, ex->math_sin(t >> 2));
        env->unk28 = 0;
        ((s32 *)&w->unk18)[0] = ((s32 *)&c)[0];
        ((s32 *)&w->unk18)[1] = ((s32 *)&c)[1];
        if (frame == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&w->unk18);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
        }
        if (frame >= 0x5D) {
            w->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    case 1:
        if (frame < 0xB) {
            tbl = (SVECTOR *)ex->btlfx_getFileData(entry->unk8, 0xE);
            tbl = (SVECTOR *)((u8 *)tbl + 4);
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            if (frame == 4) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&w->unk8);
                if (p != NULL) {
                    p->unk22 = -0x40;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&w->unk8);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame < 7) {
                s = 0x1000;
            } else {
                s = 0x1000 - ((frame - 7) << 10);
            }
            scale.vx = scale.vz = (w->unk2C * 13) >> 4;
            scale.vy = w->unk2C * 3;
            for (i = 0; i < 6; i++) {
                if (frame - i >= 0) {
                    rot.vy = ((s16 *)tbl)[(frame - i) * 3] + 0x800;
                    if (frame < 3) {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk30[frame]);
                    } else {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk30[2]);
                        ex->Hi_ModifyEffModelRGB(w->unk30[2], ((s - i * s / 5) >> 5) - 0x80, ((s - i * s / 5) >> 5) - 0x80,
                                                 ((s - i * s / 5) >> 5) - 0x80);
                    }
                }
            }
            gte_SetRotMatrix(env->viewMatrix);
            gte_SetTransMatrix(env->viewMatrix);
            rot.vy = ((s16 *)tbl)[frame * 3] + 0x800;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = pos.vx;
            m.t[1] = pos.vy;
            m.t[2] = pos.vz;
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            sv.vx = 0x3E8;
            sv.vy = 0;
            sv.vz = 0;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&pos);
            sv.vx = pos.vx;
            sv.vy = pos.vy;
            sv.vz = pos.vz;
            ((s32 *)&w->unk18)[0] = ((s32 *)&sv)[0];
            ((s32 *)&w->unk18)[1] = ((s32 *)&sv)[1];
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&w->unk18);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            ex->func_800E6624(0, 0x280, 0x1E0, 0x40, 0x20, 0x2C0, ((frame & 7) << 5) + 0x100);
            if (frame < 0xB) {
                break;
            }
        }
        w->unk0 = 2;
        entry->unk0 = -1;
        break;
    case 2:
        if (frame >= 0xC) {
            w->unk0 = 3;
            entry->unk0 = -1;
        }
        break;
    case 3:
        t = (frame << 12) / 29;
        if (frame == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EBA94;
                p->unk12 = 0x10;
                p->unkC = 0x4C;
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&w->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EBF54;
                p->unk12 = 8;
                p->unkC = 0x4C;
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)(q = &w->unk8));
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)q);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&w->unk8);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            /* Dead load (flow deletes it): q's last use comes after the 0x17 spawn's, so cse makes q the class
             * head for the 7/0xF spawns and keeps the single-set copy (sched1 places it after the call). */
            k = q->vx;

        }
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        rot.vx = 0;
        rot.vy = frame << 5;
        rot.vz = 0;
        scale.vx = scale.vz = (w->unk2C * 14) >> 4;
        scale.vy = w->unk2C + ex->math_cos(t >> 2) * 2;
        ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, w->unk40, w->unk3C, t);
        ex->Hi_ModifyEffModelRGB(w->unk40, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        scale.vx = scale.vz = (w->unk2C * 14) >> 4;
        if (frame < 4) {
            scale.vy = (w->unk2C * frame / 4) * 2;
        } else {
            scale.vy = ((w->unk2C * ex->math_cos(t >> 2)) >> 12) * 2;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk44);
        ex->Hi_ModifyEffModelRGB(w->unk44, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        ex->func_800E6624(frame * 8, 0x240, 0x100, 0x30, 0x80, 0x240, 0x180);
        if (frame >= 10) {
            if (frame == 10) {
                ((SVECTOR *)&m)->vx = 0;
                ((SVECTOR *)&m)->vy = 0;
                ((SVECTOR *)&m)->vz = 0;
                func_801E7B88(w->unk4C, (SVECTOR *)&m);
                w->unk22 = w->unk8.vy;
                w->unk28 = -0x28;
                w->unk20 = w->unk8.vx;
                w->unk24 = w->unk8.vz;
                w->unk22 -= 0x1AE;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x3C, 0x46);
                ex->func_800E806C(0, 0);
                ex->func_800E806C(0x11, -2);
                ex->btlfx_setUnitPos(0x11, (s16 *)&w->unk8);
            }
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            ex->btlfx_getUnitRot(0, (s16 *)&rot);
            scale.vx = scale.vy = scale.vz = 0x3800;
            rot.vy += 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk4C);
            func_801E7D68();
            rot.vx = (w->unk28 + 0x28) * 32;
            pos.vx = w->unk20;
            pos.vy = w->unk22;
            pos.vz = w->unk24;
            w->unk22 += w->unk28;
            w->unk20 += 7;
            w->unk24 += 0xC;
            w->unk28 += 4;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk50);
        }
        if ((u32)(frame - 10) < 4) {
            for (i = 0; i < 6; i++) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = w->unk8.vx;
                    o->unkC.vy = w->unk8.vy;
                    o->unkC.vz = w->unk8.vz;
                    o->unkC.vx += ex->btlfx_randCentered(0x100);
                    o->unkC.vy += ex->btlfx_randCentered(0x180) - 0x100;
                    o->unkC.vz += ex->btlfx_randCentered(0x100);
                    o->unk2 = w->unk8.vy - w->unk10.vy;
                    o->unkC.vy = w->unk8.vy;
                    o->unk14.vz = 0;
                    o->unk14.vx = 0;
                    o->unk14.vy = ex->btlfx_randCentered(0x400);
                    o->unk1C = ((ex->btlfx_rand() & 3) << 8) + 0xAAA;
                    o->unk8 = 0;
                }
            }
            if (frame == 10) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = w->unk8.vx;
                    o->unkC.vy = w->unk8.vy;
                    o->unkC.vz = w->unk8.vz - 0x20;
                    o->unk2 = 0x1A4;
                    o->unk1C = 0x1800;
                    o->unk8 = 1;
                    o->unk14.vz = 0;
                    o->unk14.vx = 0;
                    o->unk14.vy = 0;
                    o->unk6 = 0x40;
                }
            }
        }
        if (frame >= 0x1D) {
            w->unk0 = 4;
            entry->unk0 = -1;
        }
        break;
    case 4:
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        ex->btlfx_getUnitRot(0, (s16 *)&rot);
        scale.vx = scale.vy = scale.vz = 0x3800;
        rot.vy += 0x800;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk4C);
        func_801E7D68();
        rot.vx = (w->unk28 + 0x28) * 32;
        pos.vx = w->unk20;
        pos.vy = w->unk22;
        pos.vz = w->unk24;
        w->unk22 += w->unk28;
        w->unk20 += 7;
        w->unk24 += 0xC;
        w->unk28 += 4;
        if (w->unk28 > 0 && w->unk22 > 0) {
            w->unk28 = -w->unk28 / 2;
        }
        if (frame < 0x30) {
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk50);
            if ((u32)(frame - 0x20) < 0x10) {
                k = 0x1000 - ((frame - 0x20) << 8);
                ex->Hi_ModifyEffModelAbr(w->unk50, 1);
                ex->Hi_ModifyEffModelRGB(w->unk50, (k >> 5) - 0x80, (k >> 5) - 0x80, (k >> 5) - 0x80);
            }
        }
        if (frame == 0x4B) {
            ex->func_800E83EC();
        }
        if (frame >= 0x64) {
            ex->btlfx_poolClear((Unk800DA1B8 *)entry->unk10);
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    w->unk4++;
    }
    return 0;
}

s32 fx_1a2_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 a;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E9844 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        return 0;
    } else {
        frame = entry->unk0;
        work->vz += 0x60;
        ex->btlfx_setUnitPos(0x10, (s16 *)work);
        if (frame < 0x24) {
            a = 0x80;
        } else {
            a = 0x80 - (frame - 0x24) * 0x10;
        }
        ex->func_800E806C(0x10, a);
        if (frame >= 0x2C) {
            return 1;
        }
    }
    return 0;
}
