#include "fxmod/fx_07e.h"

void func_801E7704(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR a;
    DVECTOR b;
    DVECTOR c;
    DVECTOR d;
    DVECTOR e;
    DVECTOR f;
    DVECTOR g;
    DVECTOR h;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env = D_801E8AB8;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    s32 flag = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 t;
    s32 ang;
    s32 k;
    DVECTOR *p;
    s32 r;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n * m);
    t = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        t = 1;
    }
    c0.cd = t != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        tp0 = getTPage(2, 0, src->x, src->y);
        tp1 = getTPage(2, 0, src->x + 0x80, src->y);
        sx = src->x;
        sy = src->y;
        src->x = env->drawEnv->clip.x + (x - (src->w >> 1));
        src->y = env->drawEnv->clip.y + (y - (src->h >> 1));
        move = (u32 *)env->prim;
        env->prim = (u8 *)move + 0x18;
        ex->SetDrawMove(move, src, sx, sy);
        cx = (src->w >> 1) + (sx & 0x3F);
        cy = (src->h >> 1) + (sy & 0xFF);
    } else {
        cx = x;
        cy = y;
        tp0 = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tp1 = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    p = pts[1];
    tp0 <<= 16;
    tp1 <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        p[i].vx = cx;
        k = i * 4 + 2;
        *(s16 *)((u8 *)p + k) = cy;
    }
    step = 0x1000 / n;
    r = 0;
    for (j = 1; j <= m; j++) {
        t = ex->btlfx_lerpRatio(from, to, j - 1, m);
        i = 0;
        ang = 0;
        c0.r = c0.g = c0.b = t;
        t = ex->btlfx_lerpRatio(from, to, j, m);
        c1.r = c1.g = c1.b = t;
        r += dr;
        for (; i <= n; i++, ang += step) {
            *(s32 *)&a = *(s32 *)&pts[0][i];
            *(s32 *)&e = *(s32 *)&pts[1][i];
            if (i < n) {
                b.vx = (ex->math_cos(ang) * r) >> 12;
                b.vy = (ex->math_sin(ang) * r) >> 12;
                f.vx = b.vx + cx;
                f.vy = b.vy + cy;
            } else {
                *(s32 *)&b = *(s32 *)&pts[0][0];
                *(s32 *)&f = *(s32 *)&pts[1][0];
            }
            if (f.vx < 0) {
                f.vx = 0;
            }
            if (f.vx >= 0x140) {
                f.vx = 0x13F;
            }
            if (f.vy < 0) {
                f.vy = 0;
            }
            if (f.vy >= 0xF0) {
                f.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&f;
            if (i != 0) {
                if (e.vx < 0x100 && f.vx < 0x100 && g.vx < 0x100 && h.vx < 0x100) {
                    t = tp0;
                    flag = 0;
                } else {
                    t = tp1;
                    flag = 1;
                    e.vx -= 0x80;
                    f.vx -= 0x80;
                    g.vx -= 0x80;
                    h.vx -= 0x80;
                }
            }
            if (i < n && fn != NULL) {
                fn(r, j, ang, i, &b);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&b;
            a.vx += x;
            a.vy += y;
            b.vx += x;
            b.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = e.vx | (e.vy << 8);
                *(s32 *)&prim->u1 = f.vx | (f.vy << 8) | t;
                *(s32 *)&prim->u2 = g.vx | (g.vy << 8);
                *(s32 *)&prim->u3 = h.vx | (h.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&c0;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                *(s32 *)&prim->r3 = *(s32 *)&c1;
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->x0 = *(s32 *)&a;
                *(s32 *)&prim->x1 = *(s32 *)&b;
                *(s32 *)&prim->x2 = *(s32 *)&c;
                *(s32 *)&prim->x3 = *(s32 *)&d;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (flag) {
                e.vx += 0x80;
                f.vx += 0x80;
                g.vx += 0x80;
                h.vx += 0x80;
            }
            *(s32 *)&c = *(s32 *)&a;
            *(s32 *)&d = *(s32 *)&b;
            *(s32 *)&g = *(s32 *)&e;
            *(s32 *)&h = *(s32 *)&f;
        }
    }
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

s32 func_801E7EB8(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx07eWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR out;
    SVECTOR rot;
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    s32 s;
    s32 c;
    s32 n1;
    s32 n2;
    s32 w0;
    s32 w1;
    s32 (*fn)(SVECTOR *, VECTOR *, s32, s32);

    if (mode == 1) {
        part->unk4 = 0;
        return 0;
    }
    s = work->unk14.vz;
    c = part->unk0;
    w0 = ((s32 *)&work->unk14)[0];
    w1 = ((s32 *)&work->unk14)[1];
    ((XY32 *)&ang)[0].w = w0;
    ((XY32 *)&ang)[1].w = w1;
    s += c;
    ex->func_800D044C(&work->unkC, (s32)&out, 0, (XY32 *)&ang, work->unk1C, s);
    pos.vx = out.vx;
    pos.vy = out.vy;
    pos.vz = out.vz;
    scale.vx = scale.vy = scale.vz = work->unk20;
    BTLFX_COPY_SVEC(&rot, &ang);
    s += 0x400;
    rot.vz = s;
    switch (part->unk4) {
    case 0:
        s = frame << 9;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        ex->Hi_ModifyEffModelRGB(work->unk28, (s >> 5) - 0x80, (s >> 5) - 0x80, (s >> 5) - 0x80);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 5), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 3), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        if (frame >= 8) {
            part->unk4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        ex->Hi_ModifyEffModelRGB(work->unk28, 0, 0, 0);
        if (work->unk24 != 0) {
            part->unk4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        s = frame << 8;
        n1 = ex->math_cos(s >> 2);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        ex->Hi_ModifyEffModelAbr(work->unk28, 1);
        n2 = (n1 >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk28, n2, n2, n2);
        if (frame >= 16) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_07e_op80(s32 mode, Fx07eWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx07ePart *part;
    s32 frame;
    s32 t;
    s32 r;

    if (mode == 0) {
        ((EffInit *)arg4)->workSize = sizeof(Fx07eWork80);
        ((EffInit *)arg4)->elemSize = sizeof(Fx07ePart);
        ((EffInit *)arg4)->elemFn = (s32)func_801E7EB8;
        ((EffInit *)arg4)->poolCount = 0xB;
    } else if (mode == 1) {
        D_801E8AB8 = env;
        arg4 = (s32 *)*arg4;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk1C = 0x2EE;
        work->unk4 = 0;
        work->unk0 = 0;
        work->unk8 = 0;
        work->unk24 = 0;
        work->unk20 = 0x8000;
        if (arg4 != NULL) {
            work->unk14.vx = 0x200;
        } else {
            work->unk14.vx = -0x200;
        }
        do {
            work->unk14.vy = 0;
        } while (0);
        work->unk14.vz = 0;
        ex->btlfx_initEffModels(D_801E8AC8, 1);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xC), 0xBB, 0x3E40, 0,
                                                 0x80);
    } else {
        frame = entry->frame;
        work->unk14.vz += 0x18;
        work->unk14.vy = work->unk8 * 0x14;
        work->unk14.vx -= 0xC;
        switch (work->unk0) {
        case 0:
            t = (work->unk4 << 12) / 11;
            part = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (part != NULL) {
                part->unk8 = work->unk4;
                part->unk0 = t;
            }
            if (++work->unk4 >= 11) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 22;
            r = 0x2EE - ((t * 0x2CE) >> 12);
            work->unk1C = r;
            work->unk20 = ((((r << 12) / 750) << 15) >> 12);
            if (frame >= 22) {
                work->unk0 = 2;
                entry->frame = -1;
                work->unk24 = 1;
            }
            break;
        case 2:
            if (frame >= 17) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk8++;
    }
    return 0;
}

void func_801E8498(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8AB8);
    s32 x;
    s32 y;
    s32 t;

    t = ((0x3C - r) << 12) / 60;
    t = (t * D_801E8B90) >> 12;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT r1;
    RECT r2;
    SVECTOR *pos;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = sizeof(Fx07eWork81);
    } else if (mode == 1) {
        D_801E8AB8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk8 = ex->func_800E7CC4(0, 0x20);
        work->unk0 = 0;
    } else {
        frame = entry->frame;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        pos = &work->unkC;
        gte_ldv0(pos);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 12;
            env->otZ = work->unk8;
            s = ex->math_sin(t >> 2);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), pos, NULL, 0x2000, 0, 1, s, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 2), pos, NULL, 0x2000, 0, 2, s);
            env->otZ = 0;
            if (frame >= 12) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            r1.x = 0x1C0;
            r1.y = 0x180;
            r1.w = 0x7A;
            r1.h = 0x7A;
            t = frame << 8;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r1, func_801E8498);
            D_801E8B90 = ex->math_sin(t >> 2) >> 1;
            env->otZ = work->unk8;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), pos, NULL, 0x2000, 0, 1, 0x1000, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 2), pos, NULL, 0x2000, 0, 2, 0x1000);
            env->otZ = 0;
            if (frame >= 16) {
                work->unk0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            r2.x = 0x1C0;
            r2.y = 0x180;
            r2.w = 0x7A;
            r2.h = 0x7A;
            t = (frame << 12) / 12;
            func_801E7704(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &r2, func_801E8498);
            a = t >> 2;
            D_801E8B90 = ex->math_cos(a) >> 1;
            env->otZ = work->unk8;
            c = 0x1000 - ex->math_sin(a);
            s = ex->math_cos(a);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), pos, NULL, c * 2, 0, 1, s, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 2), pos, NULL, c * 2, 0, 2, s);
            env->otZ = 0;
            if (frame >= 12) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07e/nonmatchings/fx_07e", fx_07e_headerPtr);
