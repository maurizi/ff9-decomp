#include "fxmod/fx_0cb.h"

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
    Unk800F78A4 *env = D_801E8670;
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

void func_801E7EB8(s16 *a, s16 *b) {
    BTLFX_EXPORTS(D_801E8670)->btlfx_aimAngles((s16 *)&D_801E8670->camera->eye, a, b);
}

s32 func_801E7EFC(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 c;
    s32 s;
    s32 a;
    s32 m;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        a = frame << 9;
        p->unk14 += p->unk16;
        func_801E7EB8((s16 *)&p->unk4, (s16 *)&rot);
        rot.vz = p->unk14;
        pos.vx = p->unk4.vx;
        pos.vy = p->unk4.vy;
        pos.vz = p->unk4.vz;
        m = ex->math_sin(a >> 2);
        m *= 3;
        s = p->unk2;
        scale.vx = scale.vy = scale.vz = ((s * m) >> 13) + (s >> 1);
        if (frame < 3) {
            c = ((frame + 1) << 12) / 3;
        } else {
            c = ex->math_cos(((frame - 3) << 10) / 5);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
        s = c >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk20, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        s = c >> 6;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk24, s, s, s);
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0cb_op80(s32 mode, Fx0cbWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR off;
    Fx0cbPart *part;
    s32 frame;
    s32 t;
    s32 a;
    s32 c;
    u8 *data;
    s32 u;
    s32 r1;
    s32 sc;
    s32 q;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0cbWork);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0cbPart);
        ((EffInit *)arg4)->unk14 = (s32)func_801E7EFC;
        ((EffInit *)arg4)->unk4 = 6;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8670 = env;
        work->unk8 = ex->func_800E7CC4(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz;
        ex->btlfx_initEffModels(D_801E8680, 3);
        work->unk20 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x12), 1);
        work->unk24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 1);
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 50;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0, 0x40, 0x40);
            } else if (frame < 7) {
                u = 0x78 - (frame - 1) * 0x14;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, u, u, u >> 1);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), work->unk4);
            ex->func_800E6624(frame * 8, 0x240, 0x100, 0x38, 0x70, 0x278, 0x100);
            if (frame < 0x20) {
                if (frame == frame / 3 * 3) {
                    a = frame << 7;
                    r1 = (a >> 4) + 0x100;
                    sc = 0xAAA - (a >> 1);
                    part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&v);
                        if (frame != 0) {
                            ex->func_800CFCA8(&v, r1, (s32)&off);
                        } else {
                            off.vz = 0;
                            off.vy = 0;
                            off.vx = 0;
                        }
                        part->unk4.vx = work->unkC.vx + off.vx;
                        part->unk4.vz = work->unkC.vz + off.vy;
                        part->unk4.vy = work->unkC.vy + off.vz;
                        part->unk2 = sc;
                        ex->btlfx_randRotation(&part->unkC);
                    }
                }
            }
            v.vy = 0;
            v.vx = 0;
            v.vz = 0;
            v.pad = 0;
            q = t >> 2;
            c = ex->math_cos(q);
            env->unk28 = work->unk8 + 0x46;
            data = (u8 *)ex->btlfx_getFileData(entry->unk8, 5);
            ex->func_800D23D8(data, &work->unkC, &v, ex->math_cos(q) / 3 + 0x2AA, 0, 1, c);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xF), work->unk4);
            v.vx = 0x200;
            v.vy = 0x590;
            v.vz = 0x80;
            v.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unkC, &v, t / 2 + 0x555, 0, 1, c);
            v.vx = 0x600;
            v.vy = 0x990;
            v.vz = 0x80;
            v.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unkC, &v, t / 2 + 0x555, 0, 1, c);
            env->unk28 = 0;
            if (frame >= 0x32) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cb/nonmatchings/fx_0cb", fx_0cb_headerPtr);
