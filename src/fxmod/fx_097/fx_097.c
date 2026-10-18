#include "fxmod/fx_097.h"

void func_801E7720(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801E9DB4;
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

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E7CBC);

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9DB4)->func_800CFD78(a, b, -((D_801E9D50[i] * 0x455) >> 9), out);
}

s32 func_801E82F4(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx097Item *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else if (it->unk2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &D_801E9CE8[it->unk4];
        switch (it->unk0) {
        case 0:
            if (it->unk8 == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&it->unk18);
                it->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 > 0) {
                it->unk0 = 1;
                it->unk8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->unk8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 >= 4) {
                it->unk0 = 2;
                it->unk8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->unkC.vx;
                v38.vy = it->unkC.vy;
                v38.vz = it->unkC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->func_800CFD78((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->unk8, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->func_800D23D8((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->unk78 != 0) {
                it->unk8 = 0;
                if (it->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->unk18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (q != NULL) {
                        q->unk2 = 1;
                        q->unk0 = n;
                        q->unk4 = k;
                        q->unk8 = 0;
                        q->unk2C = work->unk20[n];
                        func_801E8284((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->unk20, p);
                        q->unkC.vx = v48.vx << 4;
                        q->unkC.vy = v48.vy << 4;
                        q->unkC.vz = v48.vz << 4;
                        q->unk28 = ex->btlfx_randCentered(0x84);
                        q->unk2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->func_800CFCA8(&v58, t, (s32)&q->unk18);
                        q->unk18.vx += ex->btlfx_randCentered(0x200);
                        q->unk18.vy += ex->btlfx_randCentered(0x200);
                        q->unk18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->unk2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->unkC.vx += it->unk18.vx;
        it->unkC.vy += it->unk18.vy;
        it->unkC.vz += it->unk18.vz;
        it->unk18.vy += 0x40;
        it->unk20.vx += it->unk28;
        it->unk20.vy += it->unk2A;
        c = (c * 3) >> 1;
        if (it->unk0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->unkC.vx >> 4;
        pos.vy = it->unkC.vy >> 4;
        pos.vz = it->unkC.vz >> 4;
        ex->Hi_DrawEffModel(&it->unk20, &pos, (s32)&scale, it->unk2C);
        ex->Hi_ModifyEffModelAbr(it->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(it->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->unk8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_097/nonmatchings/fx_097", fx_097_headerPtr);

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", fx_097_op80);
