#include "fxmod/fx_179.h"

void func_801E7704(s32 model, SVECTOR *center) {
    Fx179Tri *t = D_801E8A1C;
    Fx179Tri *u = D_801E8A1C;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A0C);
    Fx179Shard *s;
    SVECTOR d;
    s32 i;

    ex->Hi_SplitMdlVertex(model, -1, (s32)D_801E8A1C);
    s = D_801E991C;
    for (i = 0; i < 160; i++, t++, u++, s++) {
        s->unk18 = 0;
        s->unk0 = (t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3 << 12;
        s->unk4 = (u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3 << 12;
        s->unk8 = (u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3 << 12;
        s->unk1A = 0;
        s->unk12 = 0x1000;
        s->unk14 = ex->btlfx_randCentered(0x100);
        s->unk16 = ex->btlfx_randCentered(0x200);
        d.vx = (s->unk0 >> 12) - center->vx;
        d.vy = (s->unk4 >> 12) - center->vy;
        d.vz = (s->unk8 >> 12) - center->vz;
        s->unkC = d.vx >> 4;
        s->unkE = (d.vy >> 4) + 0x80;
        s->unk10 = d.vz >> 4;
    }
}

void func_801E78E8(void) {
    Fx179Tri *t = D_801E8A1C;
    Fx179Shard *s = D_801E991C;
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
    ex = BTLFX_EXPORTS(D_801E8A0C);
    sc.vx = sc.vy = sc.vz = 0xF00;
    for (i = 0; i < 160; i++, s++, t++) {
        if (s->unk18 >= 0) {
            if (s->unk18 >= s->unk1A) {
                c.vx = s->unk0 >> 12;
                c.vy = s->unk4 >> 12;
                c.vz = s->unk8 >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = t->unk0[j].vx - c.vx;
                    loc[j].vy = t->unk0[j].vy - c.vy;
                    loc[j].vz = t->unk0[j].vz - c.vz;
                }
                ex->btlfx_stepModuleParticle((Pool6CEntry *)s, -8, (SVECTOR *)bias, &out);
                rot.vx = s->unk14;
                rot.vy = s->unk16;
                rot.vz = 0;
                ex->func_800316D8(&rot, &m);
                ex->ScaleMatrix(&m, &sc);
                m.t[0] = s->unk0 >> 12;
                m.t[1] = s->unk4 >> 12;
                m.t[2] = s->unk8 >> 12;
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
                    s->unk18 = -1;
                }
            } else {
                s->unk18++;
            }
        }
    }
}

void func_801E7B68(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A0C);
    Fx179Rgbc c0;
    Fx179Rgbc c1;
    Fx179Rgbc c2;
    MATRIX m;
    SVECTOR vv[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 k;
    s32 j;
    s32 row;
    s32 h;
    s32 y0;
    s32 y1;
    s32 s0;
    s32 s1;
    s32 ra;
    s32 rb;
    s32 a;
    s32 ft;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 jn;
    s32 kn;

    gte_SetRotMatrix(D_801E8A0C->viewMatrix);
    gte_SetTransMatrix(D_801E8A0C->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)D_801E8A0C->prim;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    D_801E8A0C->prim = (u8 *)(ft4 + 14);
    c0.r = c0.g = c0.b = c;
    c1.r = c1.g = c1.b = c;
    c0.code = abr != 0xFF ? 0x2E : 0x2C;
    c2.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    k = 0;
    h = 0;
    w0 >>= 1;
    w1 >>= 1;
    for (row = 0; row < 1; row++, h += dy) {
        for (j = 0; j < 16; j++, k++, ang += dang << 8) {
            jn = (j + 1) << 8;
            kn = (k + 1) << 8;
            y0 = h + (((j << 8) * dy) >> 12);
            y1 = h + ((jn * dy) >> 12);
            s0 = ex->math_sin((k << 8) >> 2);
            s1 = ex->math_sin(kn >> 2);
            ra = ex->btlfx_lerp(r0, r1, s0);
            rb = ex->btlfx_lerp(r0, r1, s1);
            vv[0].vx = vv[1].vx = (ex->math_cos(ang) * ra) >> 12;
            vv[0].vy = vv[1].vy = (ex->math_sin(ang) * ra) >> 12;
            vv[0].vz = y0 - ex->btlfx_lerp(w0, w1, s0);
            vv[1].vz = y0 + ex->btlfx_lerp(w0, w1, s1);
            a = ang + (dang << 8);
            vv[2].vx = vv[3].vx = (ex->math_cos(a) * rb) >> 12;
            vv[2].vy = vv[3].vy = (ex->math_sin(a) * rb) >> 12;
            vv[2].vz = y1 - ex->btlfx_lerp(w0, w1, s0);
            vv[3].vz = y1 + ex->btlfx_lerp(w0, w1, s1);
            gte_ldv3c(vv);
            gte_rtpt();
            t = k & 0xF;
            u0 = u + t * 8;
            u1 = u0 + 8;
            if (t == 15) {
                u1 = u0 + 7;
            }
            v0 = v;
            v1 = v0 + 0x1F;
            gte_nclip();
            if (k == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
            } else if (k >= 15) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
                if (ft) {
                    ft4->u0 = ft4->u1 = u0;
                    ft4->u2 = ft4->u3 = u1;
                    ft4->v0 = ft4->v2 = v0;
                    ft4->v1 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u1 = u0;
                    gt4->u2 = gt4->u3 = u1;
                    gt4->v0 = gt4->v2 = v0;
                    gt4->v1 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&z);
                gte_ldv0(&vv[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, z, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
        }
    }
}

s32 fx_179_op80(s32 mode, Fx179Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    SVECTOR out;
    union {
        VECTOR v;
        SVECTOR s;
    } u;
    s32 frame;
    s32 a;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        D_801E8A0C = env;
        ex->btlfx_initEffModels(D_801EAA9C, 1);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x9A, 0x3D40, 0, 0);
    } else {
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        a = (frame << 8) >> 2;
        pos.vz = work->unk10.vz + ex->math_cos(a) * 3 / 2;
        pos.vy = -(ex->math_cos(a) * 3 / 2 + 0x180) + work->unk10.vy;
        rot.vx = 0x200;
        rot.vz = -frame << 6;
        pos.vx = 0;
        rot.vy = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        work->unk8.vx = pos.vx;
        work->unk8.vy = pos.vy;
        work->unk8.vz = pos.vz;
        if (frame < 0xB) {
            from.vx = pos.vx;
            from.vy = pos.vy;
            from.vz = pos.vz;
            BTLFX_COPY_SVEC(&to, &work->unk10);
            to.vy -= 0x200;
            ex->btlfx_lerpVec((s16 *)&from, (s16 *)&to, (frame << 12) / 10, (s16 *)&out);
            env->camera->unk0[3].vx = out.vx;
            env->camera->unk0[3].vy = out.vy;
            env->camera->unk0[3].vz = out.vz;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18);
        u.v.vx = u.v.vy = u.v.vz = 0xAAA;
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x398, 0x80, 0x17, 0x3D03, 0, 0x40,
                      -(work->unk4 * 0x180), 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x400, 0x80, 0x17, 0x3D00, 0, 0x40,
                      -(work->unk4 * 0x180) + 0x400, 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D04, 0, 0x40,
                      -(work->unk4 * 0x180) + 0x800, 0x2000, 1, 1);
        func_801E7B68(&work->unk8, &rot, &u.v, 0x3E8, 0x2BC, 0x300, 0x300, 0x80, 0x17, 0x3D06, 0, 0x40,
                      -(work->unk4 * 0x180) + 0xC00, 0x2000, 1, 1);
        if (frame >= 0x10) {
            work->unk0 = 1;
            entry->unk0 = -1;
            u.s.vx = 0;
            u.s.vy = 0;
            u.s.vz = 0;
            func_801E7704(work->unk18, &u.s);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EAB6C;
                p->unk12 = 0xA;
                p->unkC = 0x5C;
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 100, 100, 100);
        }
        if (frame == 1) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 70, 70, 70);
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        scale.vx = 0xAAA;
        scale.vy = 0xAAA;
        scale.vz = 0xAAA;
        pos.vx = work->unk10.vx;
        pos.vy = work->unk10.vy;
        pos.vz = work->unk10.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18);
        func_801E78E8();
        if (frame >= 0x20) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_179/nonmatchings/fx_179", fx_179_headerPtr);
