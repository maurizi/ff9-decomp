#include "fxmod/fx_0d3.h"

void func_801E7720(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801ED534;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR v[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 z;
    s32 flag;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *p;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 hh;
    s32 x0;
    s32 col;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        hh = h + 1;
        prim = (POLY_G4 *)env->unk24;
        env->unk24 = (u8 *)(prim + w * hh);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->func_800316D8(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&z);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        x0 = -(w * size) >> 1;
        h = hh;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = x0;
            p = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, p++) {
                v[0].vx = v[2].vx = px;
                v[0].vy = v[1].vy = py;
                v[1].vx = v[3].vx = px + size;
                v[2].vy = v[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        col = p[0];
                        c1.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        col = p[w - 1];
                        c3.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&v[0], &v[1], &v[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&flag);
                gte_ldv0xy(&v[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_b();
                *(s32 *)&c2 = *(s32 *)&c3;
                *(s32 *)&c0 = *(s32 *)&c1;
                gte_stsxy(&prim->x3);
                if (flag != 0) {
                    ex->btlfx_addPrim(0x8000000, z, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, z, tpage);
    }
    w--;
    h--;
    x0 = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = x0;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E7CBC);

void func_801E8470(Fx0d3Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx0d3Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E84C0(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

void func_801E8548(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

s32 func_801E85FC(Fx0d3Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E8628(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = D_801ED534;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c;
    s32 otz;
    POLY_GT4 *prim;
    Fx0d3Seg *s;
    s32 i;
    s32 t;
    s32 k;
    s32 j;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(u32)(n - 1)];
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
    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    c.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    /* the twin of fx_0e1_0 func_801E7CB4: i = j in the increment, and the last uv store (u1) is the giv base */
    for (i = 0; i < n; i = j, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        c.r = c.g = c.b = p;
        gte_rtpt_b();
        t = ((0x1000 - t) * p) >> 12;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&c;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&c;
        j = i + 1;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            k = phase & 1;
            prim->v2 = prim->v3 = v + ((h << k) >> 1);
            prim->u0 = prim->u2 = u;
            prim->v0 = prim->v1 = v + ((k * h) >> 1);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v) {
    frame += 7;
    v[0] = (v[0] * frame) >> 3;
    v[1] = (v[1] * frame) >> 3;
}

s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 k;
    s32 c;
    s32 m;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d3Work80);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED534 = env;
        ex->btlfx_initEffModels(D_801ED544, 7);
        D_801EB050 = work->unk60 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        m = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x26), 1);
        work->unk28.vy = 0x800;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        work->unk64 = m;
        work->unk28.vx = 0;
        work->unk28.vz = 0;
        work->unk10.vx = D_801EB048.vx;
        work->unk10.vy = D_801EB048.vy;
        work->unk10.vz = D_801EB048.vz;
        work->unk10.vy -= 0x800;
        work->unk8.vx = work->unk10.vx;
        work->unk8.vy = work->unk10.vy;
        work->unk8.vz = work->unk10.vz;
        env->unk2C->unk0[3].vx = work->unk10.vx;
        env->unk2C->unk0[3].vy = work->unk10.vy;
        env->unk2C->unk0[3].vz = work->unk10.vz;
        D_801EB050 = m;
        work->unk30 = NULL;
        work->unk34 = NULL;
        work->unk68 = 0;
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 75;
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x28) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EB114;
                    p->unk12 = 0xC0;
                    p->unkC = 0x1C;
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x32) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
                work->unk30 = p;
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x1E) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                work->unk34 = p;
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0x3C) {
                ex->Hi_SetSummonMotion(((Fx0d3Summon *)env->unk4)->unk54->unk180, ((Fx0d3Summon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->unk4)->unk60, 0);
            } else if (frame >= 0x3C) {
                k = frame - 0x3C;
                if (k < 8) {
                    ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->unk4)->unk60, 0);
                }
                work->unk10.vy += 0xC00;
                ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                                       ((Fx0d3Summon *)env->unk4)->unk60, 0);
                work->unk10.vy -= 0xC00;
                if (k < 8) {
                    ex->Hi_ModifySummonModelRGB(((Fx0d3Summon *)env->unk4)->unk60, ((k * 0xA0) >> 3) - 0xA0,
                                                ((k * 0xA0) >> 3) - 0xA0, ((k * 0xA0) >> 3) - 0xA0);
                    ex->Hi_ModifySummonModelAbr(((Fx0d3Summon *)env->unk4)->unk60, 1);
                }
            }
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            rot.vy = 0x800;
            rot.vx = 0;
            rot.vz = frame * 8;
            scale.vz = scale.vx = scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk60);
            c = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk60, c, c, c);
            rot.vz = -frame * 4;
            rot.vx = 0;
            rot.vy = 0x800;
            scale.vz = scale.vx = scale.vy = 0x738;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, c, c, c);
            work->unk68 += 2;
            work->unk68 += t >> 9;
            ex->func_800E6624(work->unk68, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->func_800E6624(work->unk68 * 2 / 3, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
            func_801E7CBC(2, 0xA0, 0x78, 0x10, 4, 0x29, ((ex->math_sin(t >> 2) << 5) >> 12) + 0x40, 0, 0,
                          func_801E8A10);
            if (frame >= 0x4B) {
                ex->func_800E3C58();
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

s32 func_801E9050(SVECTOR *v) {
    s32 i = D_801EB098;
    Unk800F78A4 *env = D_801ED534;
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    v->pad = 1;
    ex->Hi_GetSummonBonePos(((s32 *)env->unk4)[0x60 / 4], D_801EB084[i], (u16 *)v);
    i++;
    if (i >= 5) {
        i = 0;
    }
    D_801EB098 = i;
    return 0;
}

void func_801E90D4(s16 *v) {
    s32 t = (BTLFX_EXPORTS(D_801ED534)->math_sin(D_801EDF48 + (v[1] << 8)) * D_801EDF44) >> 12;
    s32 x = v[0];

    v[0] = x + t;
}

INCLUDE_RODATA("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_headerPtr);

s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    union {
        struct {
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            VECTOR sc;
        } a;
        struct {
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            u8 pad[16];
            VECTOR sc;
        } b;
        struct {
            SVECTOR b0;
            SVECTOR b1;
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            u8 pad[16];
            VECTOR sc;
        } c;
    } u;
    Fx0d3Summon81 *sm;
    Pool6CEntry *p;
    Pool6CEntry *p2;
    s32 sv;
    s32 frame;
    s32 t;
    s32 frame_119;
    s32 a;
    s32 a_174;
    s32 a_281;
    s32 a_309;
    s32 a_457;
    s32 c;
    s32 c_315;
    s32 c_332;
    s32 c_498;
    s32 c_516;
    s32 k;
    s32 k_181;
    s32 k_286;
    s32 k_303;
    s32 i;
    s32 s;
    s32 s_282;
    s32 s_310;
    s32 r;
    u8 *f;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED534 = env;
        work->unk54 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 1);
        work->unk58 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xC), 1);
        work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xE), 1);
        work->unk60 = D_801EB050;
        work->unk8.vx = D_801EB040.vx;
        work->unk8.vy = D_801EB040.vy;
        work->unk8.vz = D_801EB040.vz;
        work->unk28.vy = 0x800;
        work->unk28.vx = 0;
        work->unk28.vz = 0;
        work->unk1C.vx = 0xF00;
        work->unk1C.vy = 0xF00;
        work->unk1C.vz = 0xF00;
        work->unk10.vx = D_801EB048.vx;
        work->unk10.vy = D_801EB048.vy;
        work->unk10.vz = D_801EB048.vz;
        work->unk10.vy -= 0x800;
        work->unk8.vy = work->unk10.vy;
        work->unk8.vx = work->unk10.vx;
        work->unk8.vz = work->unk10.vz;
        work->unk8.vy += 0x136;
        sm = (Fx0d3Summon81 *)env->unk4;
        ex->Hi_SetSummonMotion(sm->unk54->unk180[0], sm->unk60);
        ex->Hi_SetSummonMotFrame(((Fx0d3Summon81 *)env->unk4)->unk60, 0xA);
        return 0;
    } else {
    frame_119 = entry->unk0;
    switch (work->unk0) {
    case 0:
        t = (frame_119 << 12) / 66;
        if (frame_119 == 18) {
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->unk4)->unk54->unk180[1], ((Fx0d3Summon81 *)env->unk4)->unk60);
        }
        work->unk10.vy += 0xC00;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                               ((Fx0d3Summon81 *)env->unk4)->unk60, 1);
        work->unk10.vy -= 0xC00;
        if (frame_119 == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = -0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = -0x80;
            }
        }
        if (frame_119 == 1) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EC614;
                p->unk12 = 0x4A;
                p->unkC = 0x1C;
                p->unk22 = 0x100;
            }
        }
        if (frame_119 == 7) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
        }
        if (frame_119 < 0x23) {
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz - 0x200;
            rot.vx = 0;
            rot.vy = 0x800;
            rot.vz = frame_119 * 8;
            scale.vx = scale.vy = 0x555;
            scale.vz = 0x780;
            a_174 = (frame_119 << 12) / 35;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk60);
            a_174 >>= 2;
            ex->Hi_ModifyEffModelRGB(work->unk60, (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80,
                                     (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80, (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80);
            ex->func_800E6624(frame_119 * 16, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->func_800E6624(frame_119 * 8, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
        }
        if (frame_119 >= 0x18) {
            k_181 = frame_119 - 0x18;
            a = (k_181 << 12) / 42;
            env->unk28 = 0x80;
            f = (u8 *)ex->btlfx_getFileData(entry->unk8, 7);
            ex->func_800D3B6C(f, &work->unk8, k_181, ex->math_sin(a >> 2));
            env->unk28 = 0;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x15), frame_119);
        work->unk40.vy = work->unk10.vy;
        work->unk40.vx = work->unk10.vx;
        work->unk40.vz = work->unk10.vz;
        work->unk40.vy += 0x300;
        work->unk40.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
        env->unk2C->unk0[3].vx = work->unk40.vx;
        env->unk2C->unk0[3].vy = work->unk40.vy;
        env->unk2C->unk0[3].vz = work->unk40.vz;
        if (frame_119 >= 0x42) {
            work->unk0 = 1;
            entry->unk0 = -1;
            work->unk40.vx = D_801EB040.vx;
            work->unk40.vy = D_801EB040.vy;
            work->unk40.vz = D_801EB040.vz;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
        }
        break;
    case 1:
        if (frame_119 == 0) {
            ex->func_800E3C58();
            BTLFX_COPY_SVEC(&work->unk30, &D_801EB040);
            work->unk30.vy -= 0x1000;
            work->unk30.vz -= 0x200;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk30);
            if (p != NULL) {
                p->unk14 = (s32)D_801ECE34;
                p->unk12 = 0x40;
                p->unkC = 0x1C;
                p->unk22 = 0x80;
            }
            BTLFX_COPY_SVEC(&work->unk38, &D_801EB040);
            work->unk38.vy -= 0x1000;
            work->unk38.vz += 0xFF;
            work->unk4C = p2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk38);
            if (p2 != NULL) {
                p2->unk22 = 0x80;
            }
            work->unk50 = p2 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk38);
            if (p2 != NULL) {
                p2->unk22 = 0x80;
            }
        }
        if (work->unk4C != NULL && work->unk4C->unk30 == -1) {
            work->unk4C = NULL;
        }
        if (work->unk50 != NULL && work->unk50->unk30 == -1) {
            work->unk50 = NULL;
        }
        if (frame_119 >= 0x25) {
            work->unk0 = 2;
            entry->unk0 = -1;
            ex->func_800E3D54(work->unk4C);
            ex->func_800E3D54(work->unk50);
            work->unk40.vx = D_801EB040.vx;
            work->unk40.vy = D_801EB040.vy;
            work->unk40.vz = D_801EB040.vz;
            work->unk40.vy -= 0x200;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
        }
        break;
    case 2:
        if (frame_119 == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801EB040);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            work->unk30.vx = D_801EB040.vx;
            work->unk30.vy = D_801EB040.vy;
            work->unk30.vz = D_801EB040.vz;
            work->unk30.vy -= 0x200;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
        }
        if (frame_119 == 7) {
            ex->func_800E3C58();
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&D_801EB040);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
        }
        if (frame_119 >= 7) {
            rot.vy = frame_119 * 4;
            rot.vx = 0;
            rot.vz = 0;
            pos.vx = D_801EB040.vx;
            pos.vy = D_801EB040.vy;
            pos.vz = D_801EB040.vz;
            a_281 = ((frame_119 - 7) << 12) / 25;
            s_282 = a_281 >> 4;
            scale.vy = 0x555;
            scale.vx = scale.vz = s_282 + 0x555;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk54);
            k_286 = frame_119 * 7;
            rot.vy = k_286;
            scale.vx = scale.vz = s_282 + 0x400;
            scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk54);
            ex->Hi_ModifyEffModelRGB(work->unk54, -0x20, -0x20, -0x20);
            rot.vy = k_286;
            scale.vx = scale.vz = 0x400;
            scale.vy = (ex->math_sin(a_281 >> 2) / 2 + 0x800) >> 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk58);
            ex->func_800E6624(0x3E8 - frame_119 * 9, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(0x3E8 - k_286, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            ex->math_cos(a_281);
            ex->math_cos(a_281 << 1);
            ex->func_800D0B18((s32)(env->unk20 + 2), 0x42, 0x10, 3, 0x3C, 1, D_801EB09C, 0x18, frame_119 * 0x190, 0x186,
                              0x100);
        }
        k_303 = frame_119 - 7;
        if ((u32)k_303 < 20) {
            rot.vx = 0x400;
            pos.vx = D_801EB040.vx;
            pos.vy = D_801EB040.vy;
            pos.vz = D_801EB040.vz;
            a_309 = ((k_303 << 12) / 20) >> 2;
            s_310 = 0xC00 - (k_303 << 12) / 20;
            rot.vy = ex->math_sin(a_309) / 3;
            rot.vz = 0;
            scale.vx = scale.vy = (ex->math_sin(a_309) >> 6) + 0x249;
            scale.vz = 0x555;
            c_315 = ex->math_sin(a_309) / 15 + 2;
            sv = (s_310 >> 5) - 0x80;
            for (i = 0; i < 7; i++) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk5C);
                ex->Hi_ModifyEffModelRGB(work->unk5C, sv, sv, sv);
                pos.vy -= c_315;
                rot.vy = ((rot.vy * 5) >> 2) + (i << 10);
                scale.vx = scale.vy = (scale.vx * 9) >> 3;
            }
        }
        if (frame_119 == 5) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x64);
        }
        if (frame_119 == 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0x80, 0xFF);
        }
        if ((u32)(frame_119 - 7) < 8) {
            c_332 = 0xF0 - (((frame_119 - 7) * 0xF0) >> 3);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_332, c_332, c_332 >> 1);
        }
        if (frame_119 >= 0x20) {
            work->unk0 = 3;
            entry->unk0 = -1;
            work->unk40.vx = work->unk10.vx;
            work->unk40.vy = work->unk10.vy;
            work->unk40.vz = work->unk10.vz;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            work->unk28.vx = 0x80;
        }
        break;
    case 3:
        work->unk10.vy += 0xC00;
        t = (frame_119 << 12) / 23;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                               ((Fx0d3Summon81 *)env->unk4)->unk60, 1);
        work->unk10.vy -= 0xC00;
        rot.vx = 8;
        rot.vy = frame_119 * 12;
        rot.vz = 0;
        scale.vx = scale.vz = 0x555;
        pos.vx = D_801EB048.vx;
        pos.vy = D_801EB048.vy;
        pos.vz = D_801EB048.vz;
        pos.vy += 0x992;
        pos.vz += 0x9F6;
        scale.vy = ((t / 2 + 0x800) * 2) / 3;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk58);
        rot.vx = -8;
        rot.vy = -frame_119 * 4;
        rot.vz = 0;
        scale.vx = scale.vz = 0x400;
        scale.vy = (ex->math_sin(t >> 2) / 2 + 0x800) / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk58);
        ex->math_cos(t);
        ex->math_cos(t << 1);
        ex->func_800D0B18((s32)(env->unk20 + 2), 0x42, 0x10, 2, 0x5A, 1, D_801EB0AC, 0x18, frame_119 * 0x140, 0x186, 0x100);
        ex->func_800E6624(0x3E8 - frame_119 * 7, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
        D_801EDF44 = 4;
        D_801EDF48 = t >> 1;
        u.a.col.r = u.a.col.g = u.a.col.b = (ex->math_sin(t << 1) >> 7) - 0x73;
        u.a.sc.vx = u.a.sc.vy = u.a.sc.vz = 0x1020;
        u.a.rot.vx = 0;
        u.a.rot.vy = 0;
        u.a.rot.vz = 0;
        ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.a.rot, &u.a.sc, &u.a.col, 0,
                          (void (*)(SVECTOR *))func_801E90D4);
        if (frame_119 >= 0x17) {
            work->unk0 = 4;
            entry->unk0 = -1;
            work->unk28.vx = 0;
            work->unk40.vx = work->unk10.vx;
            work->unk40.vy = work->unk10.vy;
            work->unk40.vz = work->unk10.vz + 0x64;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->unk4)->unk54->unk180[2], ((Fx0d3Summon81 *)env->unk4)->unk60);
        }
        break;
    case 4:
        work->unk10.vy += 0xC00;
        t = (frame_119 << 12) / 14;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                               ((Fx0d3Summon81 *)env->unk4)->unk60, 1);
        work->unk10.vy -= 0xC00;
        if (frame_119 == 0) {
            work->unk30.vx = work->unk40.vx;
            work->unk30.vy = work->unk40.vy;
            work->unk30.vz = work->unk40.vz;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = 0x10;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = 0x10;
            }
        }
        D_801EDF44 = 4;
        D_801EDF48 = t >> 1;
        u.b.col.r = u.b.col.g = u.b.col.b = (ex->math_sin(t << 1) >> 7) - 0x73;
        u.b.sc.vx = u.b.sc.vy = u.b.sc.vz = 0x1020;
        u.b.rot.vx = 0;
        u.b.rot.vy = 0;
        u.b.rot.vz = 0;
        ex->func_800E6764(3, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.b.rot, &u.b.sc, &u.b.col, 0,
                          (void (*)(SVECTOR *))func_801E90D4);
        ex->math_cos(t);
        ex->math_cos(t << 1);
        ex->func_800D0B18((s32)(env->unk20 + 2), 0x42, 0x10, 2, 0x5A, 1, D_801EB0B8, 0x1D, frame_119 << 9, 0x186, 0x100);
        if (frame_119 >= 0xE) {
            work->unk0 = 5;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->unk4)->unk54->unk180[3], ((Fx0d3Summon81 *)env->unk4)->unk60);
            work->unk10.vx = 0;
            work->unk10.vy = 0;
            work->unk10.vz = 0;
            work->unk28.vx = 0;
            work->unk28.vy = 0x800;
            work->unk28.vz = 0;
        }
        break;
    case 5:
        t = (frame_119 << 12) / 60;
        ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                               ((Fx0d3Summon81 *)env->unk4)->unk60, 0);
        if (frame_119 == 0) {
            ex->func_800E3C58();
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk8);
            if (p != NULL) {
                p->unk24->unk0 = -1;
                *(s32 *)&p->unk24->unk3C = (s32)func_801E9050;
                p->unk22 = 0x20;
            }
        }
        rot.vx = 0;
        rot.vy = frame_119 * 8;
        rot.vz = 0;
        a_457 = t >> 2;
        scale.vx = scale.vz = a_457 + 0x800;
        pos.vx = D_801EB040.vx;
        pos.vy = D_801EB040.vy;
        pos.vz = D_801EB040.vz;
        scale.vy = (((t >> 3) + 0x800) * 2) / 3;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk54);
        ex->Hi_ModifyEffModelRGB(work->unk54, (ex->math_cos(a_457) >> 5) - 0x80, (ex->math_cos(a_457) >> 5) - 0x80,
                                 (ex->math_cos(a_457) >> 5) - 0x80);
        env->unk28 = 0x302;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        func_801E7720(&D_801EB040, &rot, 0x11C, 0, 0, 4, frame_119 & 0x1F, 0x190, 0xC, 0xC, a_457 + 0x1000, ex->math_cos(a_457), 1,
                      D_801EDF54, 0);
        env->unk28 = 0;
        ex->func_800E6624(0x3E8 - frame_119 * 3, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        env->unk28 = -0x80;
        if (frame_119 == 0) {
            for (i = 0; i < 6; i++) {
                ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->unk4)->unk60, D_801EB054[i].unk0, (u16 *)&u.c.b0);
                ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->unk4)->unk60, D_801EB054[i].unk4, (u16 *)&u.c.b1);
                func_801E8628(&u.c.b0, &u.c.b1, 0x3F3, 0, 0, 0, 0, 0, 0, 0, D_801EDAC4[i], 1, 0);
            }
        }
        for (i = 0; i < 6; i++) {
            ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->unk4)->unk60, D_801EB054[i].unk0, (u16 *)&u.c.b0);
            ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->unk4)->unk60, D_801EB054[i].unk4, (u16 *)&u.c.b1);
            func_801E8628(&u.c.b0, &u.c.b1, 0xB, 0x1C, 0x3D09, 0, 0x80, 0x40, 0x80, 0x80, D_801EDAC4[i], 1,
                          10000 - work->unk4);
        }
        env->unk28 = 0;
        ex->func_800E6624(10000 - work->unk4 * 4, 0x300, 0x180, 0x10, 0x80, 0x310, 0x180);
        u.c.rot.vx = 0;
        u.c.rot.vy = 0;
        u.c.rot.vz = 0;
        u.c.col.r = u.c.col.g = u.c.col.b = ((t * 0x4D) >> 12) + 0xF;
        u.c.sc.vx = u.c.sc.vy = u.c.sc.vz = 0x1080;
        ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.c.rot, &u.c.sc, &u.c.col, 1, NULL);
        if (frame_119 >= 0x30) {
            c_498 = (frame_119 - 0x30) * 0xFF / 12;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_498, c_498, c_498);
        }
        if (frame_119 >= 0x3C) {
            work->unk0 = 6;
            entry->unk0 = -1;
            work->unk40.vx = D_801EB048.vx;
            work->unk40.vy = D_801EB048.vy;
            work->unk40.vz = D_801EB048.vz;
            work->unk40.vy -= 0x400;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
        }
        break;
    case 6:
        t = (frame_119 << 12) / 87;
        if (frame_119 < 0x18) {
            c_516 = 0xFF - frame_119 * 0xFF / 24;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_516, c_516, c_516);
        }
        work->unk40.vx = D_801EB048.vx;
        work->unk40.vy = D_801EB048.vy;
        work->unk40.vz = D_801EB048.vz;
        work->unk40.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
        env->unk2C->unk0[3].vx = work->unk40.vx;
        env->unk2C->unk0[3].vy = work->unk40.vy;
        env->unk2C->unk0[3].vz = work->unk40.vz;
        if (frame_119 == 0) {
            work->unk30.vx = D_801EB048.vx;
            work->unk30.vy = D_801EB048.vy;
            work->unk30.vz = D_801EB048.vz - 0x100;
            work->unk30.vy -= 0x400;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = 0x10;
            }
        }
        if (frame_119 == 0x20) {
            work->unk38.vx = D_801EB048.vx;
            work->unk38.vy = D_801EB048.vy;
            work->unk38.vz = D_801EB048.vz;
            work->unk30.vz -= 0x100;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk38);
            if (p != NULL) {
                p->unk22 = 0x10;
            }
        }
        if (frame_119 >= 0x57) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->unk4++;
    }
    return 0;
}

s32 fx_0d3_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801ED534 = env;
        env->unk2C->unk52 = 0;
    } else {
        return 1;
    }
    return 0;
}

s32 fx_0d3_op87(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801ED534 = env;
    } else {
        frame = entry->unk0;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[1]), frame, &work[0]);
        env->unk2C->unk0[1].vx = work[0].vx;
        env->unk2C->unk0[1].vy = work[0].vy;
        env->unk2C->unk0[1].vz = work[0].vz;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[0]), frame, &work[1]);
        env->unk2C->unk0[0].vx = work[1].vx;
        env->unk2C->unk0[0].vy = work[1].vy;
        env->unk2C->unk0[0].vz = work[1].vz;
        if (frame >= D_801EB0C4[2]) {
            return 1;
        }
    }
    return 0;
}
