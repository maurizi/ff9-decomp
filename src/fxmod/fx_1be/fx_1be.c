#include "fxmod/fx_1be.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FD8);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        r = 0x1000 - ((k * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        r = (k * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - r, (s16 *)out);
}

s32 func_801E7840(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FD8);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1be_op80(s32 mode, Fx1beWork80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR aim;
    VECTOR sc;
    SVECTOR ang;
    SVECTOR pos;
    SVECTOR rot2;
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    Fx1beUnit *u;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 r;
    s32 a;
    s32 c;

    if (mode == 0) {
        *arg4 = 0xDC;
    } else if (mode == 1) {
        D_801E8FD8 = env;
        ex->btlfx_initEffModels(D_801E91E8, 5);
        {
            u8 *b = &w->unk0[31];
            for (i = 31; i >= 0; i--, b--) {
                *b = 0;
            }
        }
        for (i = 15; i >= 0; i--) {
            w->unk20[i] = NULL;
        }
        D_801E95D4 = 8;
        D_801E95D8 = 8;
        D_801E95D0 = 2;
        for (i = 0; i < env->targets->targetCount; i++) {
            D_801E95E8[i].unk34 = ex->btlfx_rand() & 7;
            D_801E95E8[i].unk30 = 0x28;
            D_801E95E8[i].unk28.vx = ex->btlfx_randRange(-0x20, 0x20);
            D_801E95E8[i].unk28.vy = ex->btlfx_randRange(-0x40, 0x40);
            D_801E95E8[i].unk28.vz = ex->btlfx_randRange(-0x20, 0x20);
            ex->btlfx_getUnitScale(i, &D_801E95E8[i].unk8);
            ex->btlfx_getUnitRot(i, (s16 *)&D_801E95E8[i].unk0);
            ex->btlfx_getUnitRot(i, (s16 *)&D_801E95E8[i].unk20);
            ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E95E8[i].unk18);
        }
        for (i = 0; i < 0x40; i++) {
            r = ex->btlfx_rand() & 0xFFF;
            c = (ex->btlfx_rand() & 0xDFF) + 0xB00;
            D_801E8FE8[i].vx = (ex->math_sin(c) * r) >> 12;
            D_801E8FE8[i].vz = ((ex->math_cos(c) * r) >> 12) + 0x418;
            D_801E8FE8[i].vy = 0;
        }
        w->unkD4.vx = 0;
        w->unkD4.vy = 0;
        w->unkD4.vz = 0x418;
        w->unkCC.vx = 0;
        w->unkCC.vy = -0x2000;
        w->unkCC.vz = 0x418;
        w->unk60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xC), 0xFF);
        w->unk74.vx = 0x1800;
        w->unk74.vy = 0x1800;
        w->unk74.vz = 0x1800;
        w->unk84.vx = 0;
        w->unk84.vy = 0;
        w->unk84.vz = 0;
        w->unk64.vx = 0;
        w->unk64.vy = -0x2000;
        w->unk64.vz = 0x418;
        w->unk8C.vz = 0x418;
        w->unk8C.vx = 0;
        w->unk8C.vy = -0x2000;
        w->unkAC.vx = 0x1A80;
        w->unkAC.vy = 0x1A80;
        w->unkAC.vz = 0x1A80;
        w->unkA8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        w->unk98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xE), 1);
        w->unk94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        w->unkA4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 1);
    } else {
        frame = entry->frame;
        env->camera->points[1].vx = 0;
        env->camera->points[1].vy = -0x800;
        env->camera->points[1].vz = 0x418;
        env->camera->points[3].vx = w->unk64.vx;
        env->camera->points[3].vy = w->unk64.vy;
        env->camera->points[3].vz = w->unk64.vz;
        switch (frame) {
        case 0:
            w->unk0[1] = 0x28;
            w->unk0[0xD] = 0x20;
            w->unk0[0] = 1;
            w->unk0[0x14] = 1;
            w->unk0[4] = 0x10;
            break;
        case 0x18:
            w->unk20[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&w->unkBC);
            w->unk20[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&w->unkC4);
            if (w->unk20[0] != NULL) {
                w->unk20[0]->unk22 = 0x200;
            }
            if (w->unk20[1] != NULL) {
                w->unk20[1]->unk22 = 0x200;
            }
            w->unk0[0xF] = 0x20;
            w->unk0[7] = 0x10;
            w->unk0[5] = 1;
            w->unk0[0x14] = 1;
            w->unk0[0xC] = 0x30;
            p = w->unk20[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&w->unkD4);
            if (p != NULL) {
                *(s32 *)&p->file->unk8 = (s32)func_801E8E10;
            }
            break;
        case 0x30:
            w->unk0[0xE] = 0x10;
            break;
        case 0x40:
            w->unk0[9] = 0x3C;
            w->unk0[0xA] = 0x10;
            w->unk0[0x12] = frame;
            w->unk0[6] = 1;
            ex->btlfx_stopEmitter(w->unk20[2]);
            w->unk74.vx = 0x555;
            w->unk74.vy = 0x555;
            w->unk74.vz = 0x555;
            w->unk64.vx = 0;
            w->unk64.vy = -0x800;
            w->unk64.vz = 0x418;
            w->unk8C.vx = 0;
            w->unk8C.vy = -0x800;
            w->unk8C.vz = 0x418;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&w->unk8C);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            break;
        case 0x50:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&w->unk8C);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&w->unk8C);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&w->unk8C);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 0x54:
            w->unk0[3] = 8;
            break;
        case 0x5C:
            w->unk0[0] = 0;
            w->unk0[5] = 0;
            w->unk0[0x14] = 0;
            break;
        case 0x2C:
            w->unk0[0xB] = 0x10;
            break;
        case 0x7C:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitScale(i, (s32 *)&D_801E95E8[i].unk8);
                ex->btlfx_setUnitRot(i, (s16 *)&D_801E95E8[i].unk0);
            }
            w->unk0[0x10] = 8;
            break;
        case 0xAC:
            return 1;
        }
        for (i = 0; i < 16; i++) {
            if (w->unk20[i] != NULL && w->unk20[i]->state == -1) {
                w->unk20[i] = NULL;
            }
        }
        if (w->unk0[1] != 0) {
            w->unk0[1]--;
            w->unk64.vy = func_801E7840(2, 0x28, w->unk0[1], -0x2000, -0x1000);
            w->unk8C.vy = w->unk64.vy;
        }
        if (w->unk0[3] != 0) {
            w->unk0[3]--;
            w->unk74.vx = w->unk74.vz = w->unk74.vy = func_801E7840(1, 8, w->unk0[3], 0x555, 0);
        }
        if (w->unk0[0] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xF), frame);
            ex->Hi_DrawEffModel(&w->unk84, &w->unk64, (s32)&w->unk74, w->unk60);
        }
        if (w->unk0[0xD] != 0) {
            w->unk0[0xD]--;
            ex->Hi_DrawEffModel(&w->unk84, &w->unk64, (s32)&w->unkAC, w->unkA8);
            if (w->unk0[0xD] < 0x10) {
                a = (w->unk0[0xD] - 0x10) * 0x10;
                ex->Hi_ModifyEffModelRGB(w->unkA8, a, a, a);
            }
        }
        if (w->unk0[0xF] != 0) {
            w->unk0[0xF]--;
            ex->btlfx_moveEmitterTo((s32)w->unk20[0], (s32)&w->unk8C);
            ex->btlfx_moveEmitterTo((s32)w->unk20[1], (s32)&w->unk8C);
        }
        if (w->unk0[0xE] != 0) {
            if (--w->unk0[0xE] != 0) {
                if (w->unk20[0] != NULL) {
                    w->unk20[0]->unk20 = w->unk0[0xE] << 8;
                }
                if (w->unk20[1] != NULL) {
                    w->unk20[1]->unk20 = w->unk0[0xE] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(w->unk20[0]);
                ex->btlfx_stopEmitter(w->unk20[1]);
            }
        }
        if (w->unk0[5] != 0) {
            aim.vx = w->unk64.vx;
            aim.vy = w->unk64.vy;
            aim.vz = w->unk64.vz;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&aim, (s16 *)&w->unk9C);
            ex->Hi_DrawEffModel(&w->unk9C, &w->unk64, (s32)&w->unk74, w->unk94);
            if (w->unk0[6] != 0) {
                ex->Hi_DrawEffModel(&w->unk9C, &w->unk64, (s32)&w->unk74, w->unk98);
                ex->Hi_DrawEffModel(&w->unk84, &w->unk64, (s32)&w->unk74, w->unkA4);
            }
        }
        if (w->unk0[7] != 0) {
            w->unk0[7]--;
            a = -(w->unk0[7] * 0x10);
            ex->Hi_ModifyEffModelRGB(w->unk94, a, a, a);
        }
        if (w->unk0[0xC] != 0) {
            w->unk0[0xC]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                u = &D_801E95E8[i];
                if (u->unk34 == 0) {
                    if (u->unk30 != 0) {
                        u->unk30--;
                        func_801E7704(1, 0x28, u->unk30, &D_801E95E8[i].unk18, &w->unk8C, &ang);
                        ex->btlfx_setUnitPos(i, (s16 *)&ang);
                        sc.vx = sc.vy = sc.vz = func_801E7840(1, 0x28, u->unk30, u->unk8.vy, 0);
                        ex->btlfx_setUnitScale(i, (s32 *)&sc);
                        u->unk20.vx += u->unk28.vx;
                        u->unk20.vy += u->unk28.vy;
                        u->unk20.vz += u->unk28.vz;
                        ex->btlfx_setUnitRot(i, (s16 *)&D_801E95E8[i].unk20);
                    }
                } else {
                    u->unk34--;
                }
            }
        }
        if (w->unk0[9] != 0) {
            a = 0xFFF;
            w->unk0[9]--;
            if (w->unk0[0xA] != 0) {
                w->unk0[0xA]--;
                a = (0x10 - w->unk0[0xA]) << 8;
            }
            if (w->unk0[0xB] != 0) {
                w->unk0[0xB]--;
                a = w->unk0[0xB] << 8;
            }
            ex->btlfx_aimAngles((s16 *)&w->unk8C, (s16 *)&env->camera->eye, (s16 *)&rot2);
            pos.vz = 0xC18;
            pos.vx = 0;
            pos.vy = -0x1200;
            rot2.vz = 0;
            rot2.vx += 0xA0;
            func_801E89A8(&pos, &rot2, a, 0x19, 0x3D03, 1);
            pos.vy = 0x200;
            rot2.vz = 0x800;
            rot2.vx -= 0x140;
            func_801E89A8(&pos, &rot2, a, 0x19, 0x3D03, 1);
        }
        if (w->unk0[0x10] != 0) {
            w->unk0[0x10]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&D_801E95E8[i].unk18);
                D_801E95E8[i].unk18.vy -= ex->math_sin(w->unk0[0x10] << 7) >> 1;
                ex->btlfx_setUnitPos(i, (s16 *)&D_801E95E8[i].unk18);
                if (w->unk0[0x10] == 0) {
                    D_801E95E8[i].unk18.vy = 0;
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&D_801E95E8[i].unk18);
                    if (p != NULL) {
                        p->unk22 = -0x100;
                    }
                }
            }
        }
        if (w->unk0[0x12] != 0) {
            w->unk0[0x12]--;
            D_801E95D8 = func_801E7840(1, 0x40, w->unk0[0x12], 4, 0x14);
        }
        D_801E95D4 += D_801E95D8;
        if (w->unk0[5] != 0) {
            ex->btlfx_drawScrollMove(D_801E95D4, 0x2C0, 0x100, 0x38, 0x70, 0x2C0, 0x170);
        }
        if (w->unk0[0x14] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - D_801E95D4, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (w->unk0[9] != 0) {
            ex->btlfx_drawScrollMove(frame * 8, 0x240, 0x140, 0x18, 0x60, 0x240, 0x1A0);
        }
        if (w->unk0[4] != 0) {
            w->unk0[4]--;
            col.r = col.g = col.b = w->unk0[4] * 8;
            scale.vz = scale.vy = scale.vx = w->unk0[4] * 0x10 + 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
    }
    return 0;
}

void func_801E89A8(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr) {
    SVECTOR v[4];
    SVECTOR t;
    MATRIX m;
    Fx1beUV uv0;
    Fx1beUV uv1;
    Fx1beUV uv2;
    Fx1beUV uv3;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env;
    BtlfxExports *ex;
    POLY_GT4 *p;
    s32 i;
    s32 j;
    s32 k0;
    s32 k1;

    env = D_801E8FD8;
    ex = BTLFX_EXPORTS(env);
    uv1.u = uv3.u = 0x5F;
    uv0.v = uv1.v = 0x40;
    uv0.u = uv2.u = 0;
    uv2.v = uv3.v = 0x9F;
    v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    p = (POLY_GT4 *)env->prim;
    tpage |= (abr & 3) << 5;
    env->prim = (u8 *)(p + 32);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    n /= 8;
    k0 = 0;
    k1 = n;
    for (j = 0; j < 8; j++) {
        t.vx = D_801E8F38[j].vx;
        t.vy = D_801E8F38[j].vy;
        t.vz = D_801E8F38[j].vz;
        c0.r = (t.vx * k0) >> 12;
        c0.g = (t.vy * k0) >> 12;
        c0.b = (t.vz * k0) >> 12;
        t.vx = D_801E8F38[j + 1].vx;
        t.vy = D_801E8F38[j + 1].vy;
        t.vz = D_801E8F38[j + 1].vz;
        c1.r = (t.vx * k1) >> 12;
        c1.g = (t.vy * k1) >> 12;
        c1.b = (t.vz * k1) >> 12;
        k0 += n;
        k1 += n;
        v[0].vz = v[1].vz = j * 0xA00 - 0x2800;
        v[2].vz = v[3].vz = j * 0xA00 - 0x1E00;
        for (i = 0; i < 4; i++) {
            v[0].vx = v[2].vx = i * 0x1400 - 0x2800;
            v[1].vx = v[3].vx = i * 0x1400 - 0x1400;
            gte_ldv3c(v);
            gte_rtpt();
            p->code = (abr != 0xFF) ? 0x3E : 0x3C;
            p->r0 = p->r1 = c0.r;
            p->g0 = p->g1 = c0.g;
            p->b0 = p->b1 = c0.b;
            p->r2 = p->r3 = c1.r;
            p->g2 = p->g3 = c1.g;
            p->b2 = p->b3 = c1.b;
            gte_stsxy3(&p->x0, &p->x1, &p->x2);
            gte_ldv0(&v[3]);
            gte_rtps();
            *(u16 *)&p->u0 = *(u16 *)&uv0;
            *(u16 *)&p->u1 = *(u16 *)&uv1;
            *(u16 *)&p->u2 = *(u16 *)&uv2;
            *(u16 *)&p->u3 = *(u16 *)&uv3;
            p->clut = clut;
            p->tpage = tpage;
            gte_stsxy(&p->x3);
            ex->btlfx_addPrim(0x0C000000, 0, p);
            p++;
        }
    }
}

s32 func_801E8E10(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;
    SVECTOR from;
    SVECTOR *key;

    key = &D_801E8FE8[(e->unk1F - 1) & 0x3F];
    from.vx = 0;
    from.vy = -0x800;
    from.vz = 0x418;
    func_801E7704(1, 0x1000, ((0x1000000 / frames) * frame) >> 12, &from, key, &v);
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = v.vx << 12;
    e->pos.vy = v.vy << 12;
    e->pos.vz = v.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1be/nonmatchings/fx_1be", fx_1be_headerPtr);
