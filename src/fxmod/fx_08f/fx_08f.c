#include "fxmod/fx_08f.h"

void func_801E7704(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *p;
    s32 *prev;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A04);
    s32 i;
    s32 rad;
    s32 a;

    p = pts;
    prev = (s32 *)&p[n];
    for (i = 0; i < n; i++, prev++, p++) {
        *prev = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        p->vx = (ex->math_cos(a) * rad) >> 12;
        p->vy = (ex->math_sin(a) * rad) >> 12;
        p->vz = ex->btlfx_rand() % 1280 - 0x100;
    }
}

void func_801E7858(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pt, CVECTOR *col, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E8A04;
    CVECTOR c;
    MATRIX m;
    s32 hsave;
    u32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    s32 *prev;
    SVECTOR *pts;
    s32 live;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->prim;
    env->prim = (u8 *)(prim + n);
    if (col == NULL) {
        c.r = c.g = c.b = 0;
    } else {
        gte_DpqColorFar_nst(col, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ReadGeomScreen(&hsave);
    gte_SetGeomScreen(h);
    ex->func_800316D8(src, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    pts = pt;
    prev = (s32 *)&pts[n];
    for (i = 0; i < n; i++, prev++, pts++, prim++) {
        live = *prev != 0x7F7F7F7F;
        gte_ldv0(pts);
        gte_rtps();
        pts->vz -= dz;
        if (dz > 0 && pts->vz < -0x100) {
            pts->vz = 0x400;
            *prev = 0x7F7F7F7F;
        } else if (dz < 0 && pts->vz > 0x300) {
            pts->vz = -0x200;
            *prev = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&otz);
            if (otz < 0x200 && live) {
                gte_DpqColorFar_nst(&c, ((0x200 - otz) << 12) >> 9);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->x1 = *prev;
                *(s32 *)&prim->r1 = 0;
                prim->code = abr != 0xFF ? 0x52 : 0x50;
                ex->btlfx_linkPrimAbr(0x4000000, ot, prim, abr);
            }
            *prev = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(hsave);
}

s32 fx_08f_op80(s32 mode, Fx08fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR rot2;
    CVECTOR c;
    CVECTOR c2;
    SVECTOR rot3;
    VECTOR scale2;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 t2;
    s32 s;
    s32 near;
    s32 k;
    s32 k2;
    s32 k3;
    s32 d;
    s32 d2;
    s32 n;
    s32 w2;
    s32 w3;
    s32 y;
    s32 y0;
    s32 x;
    s32 c3;
    s32 c4;
    s32 t3;
    s32 u;
    s32 w;

    if (mode == 0) {
        *arg4 = sizeof(Fx08fWork80);
    } else if (mode == 1) {
        D_801E8A04 = env;
        work->unk0 = 1;
        work->unk4 = 0;
        func_801E7704(0x180, 0x400, D_801E9054);
        work->unk8[0] = 0xA0;
        work->unk8[1] = 0x78;
        work->unk8[2] = 0;
        work->unk8[3] = 0x78;
        work->unk8[4] = 0xC8;
        work->unk8[5] = 0x78;
        ex->btlfx_initEffModels(D_801E8A14, 8);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x9A, 0x3E00, 0, 0);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x139, 0xFFFF, 0, 0x80);
        work->unk2C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), 0xB9, 0x3D40, 0, 0);
        work->unk30 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0x3C, 0x3D04, 0, 0x80);
        work->unk34 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x17));
        work->unk38 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x18));
        work->unk3C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19));
        work->unk40 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x97, 0x3D80, 0, 0);
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 1:
            s = (frame << 11) / 49;
            near = frame < 8;
            pos.vx = 0;
            pos.vz = 0x800 - s;
            pos.vy = s - 0x800;
            t = (frame << 12) / 49;
            if (!near) {
                n = frame - 8;
                pos.vx += ex->btlfx_randCentered(n * 3);
                pos.vy += ex->btlfx_randCentered(n * 2);
                pos.vz += ex->btlfx_randCentered(n * 2);
            }
            rot.vx = 0x200;
            rot.vz = -frame << 6;
            rot.vy = 0;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            work->unk14.vx = pos.vx;
            work->unk14.vy = pos.vy;
            work->unk14.vz = pos.vz - 0x46;
            work->unk14.vy += 0x1D;
            env->camera->points[3].vx = pos.vx;
            env->camera->points[3].vy = pos.vy;
            env->camera->points[3].vz = pos.vz;
            k2 = (u16)env->camera->points[3].vz - 0x100;
            d2 = (frame * 0x384) / 49;
            env->camera->points[3].vz = k2 + d2;
            k3 = (u16)env->camera->points[3].vy + 0x180;
            env->camera->points[3].vy = k3 - d2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->func_800E6624(work->unk4 * 8, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk40);
            ex->Hi_ModifyEffModelRGB(work->unk40, (s >> 5) - 0x80, (s >> 5) - 0x80, (s >> 5) - 0x80);
            if (!near) {
                t2 = ((frame - 8) << 12) / 41;
                rot.vx = 0x200;
                rot.vy = 0;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
                ex->Hi_ModifyEffModelAbr(work->unk28, 1);
                ex->Hi_ModifyEffModelRGB(work->unk28, (t2 >> 5) - 0x80, (t2 >> 5) - 0x80, (t2 >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), work->unk4);
            }
            y0 = -0x64;
            c.r = 0x20;
            c.g = 0x80;
            c.b = 0xFF;
            rot2.vy = -0x400;
            rot2.vx = 0x200;
            rot2.vz = 0;
            if (frame < 0x18) {
                y0 = -0x46;
            }
            func_801E7858((u32 *)(env->ot + 8), 0x180, 0x80, y0, &rot2, D_801E9054, &c,
                          ex->math_cos((frame << 10) / 49) * 3 / 4 + 0x400, 1);
            if (frame == 2) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk14);
                work->unk44 = p;
                if (p != NULL) {
                    p->unk22 = 0x5C;
                }
            }
            if (frame >= 0x19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((((frame - 0x18) << 12) / 25) << 6) >> 12, 0, 0);
            }
            k = (frame & 7) * 0x3C;
            x = 0x1B8 - k;
            y = 0x140 - k;
            ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), x + 0x64, y + 0x78, 0, 1, t);
            k = ((frame + 5) % 10) * 0x3C;
            x = 0x1B8 - k;
            y = 0x140 - k;
            ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), x - 0x40, y + 0x78, 0, 1, t);
            if (frame >= 0x31) {
                ex->func_800E3D54(work->unk44);
                work->unk1C.vx = 0;
                work->unk1C.vy = -0x140;
                work->unk1C.vz = 0;
                env->camera->points[3].vx = work->unk1C.vx;
                env->camera->points[3].vy = work->unk1C.vy;
                env->camera->points[3].vz = work->unk1C.vz;
                env->camera->points[3].vx += 0x1800;
                env->camera->points[3].vx -= 0x1000;
                ex->btlfx_startScreenFade(1, 1);
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 26;
            w = (0x1000 - u) * 3 / 2;
            pos.vz = work->unk1C.vx + w;
            pos.vy = work->unk1C.vy - w;
            rot.vx = 0x200;
            pos.vx = 0;
            rot.vy = 0;
            rot.vz = -frame << 6;
            s = 0x2000 - ex->math_sin(u >> 2) * 2;
            scale.vx = s;
            scale.vy = s;
            scale.vz = s;
            work->unk14.vx = pos.vx;
            work->unk14.vy = pos.vy;
            work->unk14.vz = pos.vz;
            if (frame < 0x19) {
                env->camera->points[3].vx = pos.vx;
                env->camera->points[3].vy = pos.vy;
                env->camera->points[3].vz = pos.vz;
            }
            if (frame == 7) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x5C;
                }
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, s >> 1, 0, 1, 0x1000);
            env->unk28 = 0;
            if (frame >= 13) {
                w2 = ((ex->math_sin((((frame - 12) << 12) / 12) >> 2) << 6) >> 12) + 0x20;
                c2.g = w2 >> 1;
                c2.r = w2;
                c2.b = w2 >> 2;
                scale2.vx = 0x1200;
                scale2.vy = 0x1200;
                rot3.vz = 0;
                rot3.vy = 0;
                rot3.vx = 0;
                scale2.vz = 0x1000;
                ex->func_800E6764(1, 0xA0, 0x78, 0x46, 0x38, 4, 4, 2, 2, &rot3, &scale2, &c2, 1, NULL);
            }
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0, NULL, -1, 0x3000, 0, 1,
                                    D_801EA254, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk14, NULL, 0,
                                ex->math_cos(u >> 2) * 2 + 0x1000, ex->math_cos(u >> 2) / 2 + 0x800, 1, D_801EA254, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), work->unk4);
            if (frame >= 9) {
                v.vx = 0;
                v.vy = 0;
                v.vz = (frame << 5) + 0x7E8;
                v.pad = 0;
                c4 = ex->math_cos((frame - 8) << 6);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unk14, &v, 0x3000,
                                      frame - 12, 1, c4);
            }
            if (frame >= 13) {
                w3 = frame - 12;
                c3 = ex->math_cos((w3 << 10) / 12);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unk14, NULL, 0x3000, w3,
                                      1, c3);
            }
            if (frame >= 0x18) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            t3 = (frame << 12) / 6;
            work->unk14.vz -= 0x40;
            work->unk14.vy += 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk14, NULL, 0,
                                ex->math_cos(t3 >> 2), (0x1000 - t3) / 2, 1, D_801EA254, 0);
            if (frame >= 6) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        {
            /* dead struct initializer: only a CLOBBER of q, which never gets a register; its spill slot is the
               original's otherwise unused frame word after the t / near slots (frame 0xD0) */
            struct { s32 x; } q = { frame };
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08f/nonmatchings/fx_08f", fx_08f_headerPtr);
