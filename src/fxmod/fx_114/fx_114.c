#include "fxmod/fx_114.h"

s32 func_801E777C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[0], frame, n);
}

s32 func_801E77B0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[1], frame, n);
}

s32 func_801E77E4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[2], frame, n);
}

s32 func_801E7818(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E784C(pt, pos, &D_801EF268[3], frame, n);
}

s32 func_801E784C(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

void func_801E78AC(Fx114Path *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801EA6F4(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", fx_114_op80);

s32 fx_114_op81(s32 mode, Fx114Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        D_801EB768 = env;
        func_801EA808(&D_801ED0E8, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x89));
        work->unk46 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x85));
        work->unk44 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            ex->Hi_GetSummonBonePos(((Fx114Summon *)env->unk4)->unk60, 0x1B, (u16 *)&work->unk34);
            work->unk10.vx = work->unk34.vx;
            work->unk10.vy = work->unk34.vy;
            work->unk10.vz = work->unk34.vz;
            work->unk3C.vx = work->unk34.vx;
            work->unk3C.vy = work->unk34.vy;
            work->unk3C.vz = work->unk34.vz;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk34);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk3C);
            work->unk44 = 0xC;
            break;
        case 0xD:
            ex->Hi_FreeEffModel(work->unk46);
            ex->func_800E3D54(work->unk0);
            ex->func_800E3D54(work->unk4);
            break;
        }
        if (frame >= 1 && frame < 0xD) {
            func_801EA81C(&D_801ED0E8, frame - 1, &work->unk20);
            ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk46);
            ex->Hi_ModifyEffModelAbr(work->unk46, 1);
            if (work->unk44 >= 0) {
                work->unk30 = c = func_801EA604(0, -0xFF, 0xC - work->unk44, 0xC);
                ex->Hi_ModifyEffModelRGB(work->unk46, c, c, c);
                work->unk44--;
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_114/nonmatchings/fx_114", fx_114_headerPtr);

s32 fx_114_op82(s32 mode, Fx114Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    DR_MODE *dr;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0x320;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        D_801EB768 = env;
        work->unk66 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x2A), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->unk66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0x17, 0x3D00, 0x80, 0xE0);
        D_801ED078.unk10 = 0x60;
        D_801ED078.unk8 = 0;
        D_801ED078.unkA = 0;
        D_801ED078.unkC = 0x1000;
        D_801ED078.unkE = 0x1000;
        D_801ED078.unk12 = 0x30;
        work->unk60 = -1;
        work->unk62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x1C0, 0x100, 0x80, 0xA0, D_801ED1C8, 0, 0x168, 0xF6E);
            break;
        case 9:
            work->unk58.vx = -3;
            work->unk58.vy = 0;
            work->unk58.vz = 0x320;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk58);
            break;
        case 8:
            work->unk60 = 4;
            break;
        case 2:
            work->unk62 = 6;
            break;
        case 29:
            ex->Hi_FreeEffModel(work->unk66);
            break;
        case 18:
            ex->Hi_FreeEffModel(work->unk68);
            break;
        }
        if ((u32)frame < 0x1D) {
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk66);
            ex->Hi_ModifyEffModelRGB(work->unk66, 6, 6, 6);
        }
        if ((u32)(frame - 8) < 10) {
            D_801ED078.unkA = (frame - 8) * 2;
            D_801ED078.unkA = D_801ED078.unkA >= 10 ? 9 : D_801ED078.unkA;
            if (work->unk60 >= 0) {
                D_801ED078.unkC = func_801EA604(0, 0x1000, 4 - work->unk60, 4);
                work->unk60--;
            }
            if (work->unk62 >= 0) {
                D_801ED078.unkC = func_801EA604(0x1000, 0, 6 - work->unk62, 6);
                work->unk62--;
            }
            for (i = 0, buf = D_801EE658[0]; i < 13; i++) {
                func_801EA6DC(&D_801ED078.unk0, (u16 *)func_801EA8FC(ex->btlfx_getFileData(entry->unk8, 0x17), (u16)i));
                func_801E78AC(&D_801ED078.unk0, buf, 0x50, D_801ED078.unk8, D_801ED078.unkA, D_801ED078.unk10, D_801ED078.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, NULL, 0, D_801ED078.unkE, D_801ED078.unkC, 1,
                                    buf, 0);
                buf += 0x50;
            }
        }
        if ((u32)(t = frame - 0xE) < 4) {
            dr = (DR_MODE *)env->unk24;
            env->unk24 = (u8 *)(dr + 1);
            setlen(dr, 2);
            dr->code[0] = 0xE6000001;
            dr->code[1] = 0;
            addPrim(&env->unk20[1], dr);
            sc.vz = sc.vy = sc.vx = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vz = rot.vy = rot.vx = 0;
            ex->func_800E6764(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            env->unk2C->unk5E = 0x208 - t * t * 4;
        }
        work->unk64++;
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_114/nonmatchings/fx_114", fx_114_op83);

s32 fx_114_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801EB768 = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EA914(0xFF, 0, 0x10 - *work, 0x10, 1, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

void func_801EA5C0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EB768)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801EA604(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801EA65C(Fx114Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key12 *)data;
}

s32 func_801EA670(Fx114Track6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = -k->x;
    out->vy = k->y;
    out->vz = -k->z;
    return done;
}

void func_801EA6DC(Fx114Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx114Point *)data;
}

s32 func_801EA6F4(Fx114Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx114Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

void func_801EA758(Fx114Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key12 *)data;
}

s32 func_801EA76C(Fx114Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    if (rot != NULL) {
        rot->vx = k->unk6;
        rot->vy = k->unk8;
        rot->vz = k->unkA;
    }
    if (pos != NULL) {
        pos->vx = -k->unk0;
        pos->vy = k->unk2;
        pos->vz = -k->unk4;
    }
    return done;
}

void func_801EA808(Fx114Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx114Key6 *)data;
}

s32 func_801EA81C(Fx114Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx114Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

void func_801EA880(Fx114Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx114Point *)data;
}

s32 func_801EA898(Fx114Path *path, s32 t, VECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx114Point *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &path->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

s32 func_801EA8FC(s32 base, s32 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801EA914(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801EB768)->btlfx_drawScreenTint((u32 *)(D_801EB768->unk20 + ot), arg4, c, c, c);
}

void func_801EA9AC(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EF2B8[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EF6B8[i] = 0;
    }
}

void func_801EA9F8(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801EF2B8[i].unk6 == -1) {
            D_801EF2B8[i].unk6 = 0;
            D_801EF2B8[i].unk0 = data;
            D_801EF2B8[i].unk4 = start;
            D_801EF2B8[i].unkA = dst;
            D_801EF2B8[i].unkC = 0;
            D_801EF2B8[i].unkE = c;
            D_801EF2B8[i].unk8 = ch;
            return;
        }
    }
}

void func_801EAA54(s32 ch) {
    Unk800F78A4 *env = D_801EB768;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801EF2B8[i].unk8 == ch) {
            if (D_801EF2B8[i].unk6 == 0 && D_801EF2B8[i].unk4 == D_801EF6B8[ch]) {
                D_801EF2B8[i].unk6 = *D_801EF2B8[i].unk0;
                D_801EF2B8[i].unk0++;
            }
            if (D_801EF2B8[i].unk6 > 0) {
                D_801EF2B8[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801EF2B8[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801EF2B8[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801EF2B8[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801EF2B8[i].unk0[2];
                        if (D_801EF2B8[i].unkC < D_801EF2B8[i].unkE) {
                            D_801EF2B8[i].unk0 += 3;
                            D_801EF2B8[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EF6B8[ch]++;
}

void func_801EABB4(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801EB768;
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
                gte_rtps_bm();
                *(s32 *)&c0 = *(s32 *)&c1;
                *(s32 *)&c2 = *(s32 *)&c3;
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

void func_801EB154(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801EB768;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}
