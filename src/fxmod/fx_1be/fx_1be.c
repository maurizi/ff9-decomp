#include "fxmod/fx_1be.h"

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1be_env);
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

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1be_env);
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
        fx_1be_env = env;
        ex->btlfx_initEffModels(fx_1be_effModels, 5);
        {
            u8 *b = &w->counters0[31];
            for (i = 31; i >= 0; i--, b--) {
                *b = 0;
            }
        }
        for (i = 15; i >= 0; i--) {
            w->emit20[i] = NULL;
        }
        fx_1be_scrollPos = 8;
        fx_1be_scrollSpeed = 8;
        fx_1be_unusedScroll = 2;
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_1be_units[i].timer34 = ex->btlfx_rand() & 7;
            fx_1be_units[i].timer30 = 0x28;
            fx_1be_units[i].spin28.vx = ex->btlfx_randRange(-0x20, 0x20);
            fx_1be_units[i].spin28.vy = ex->btlfx_randRange(-0x40, 0x40);
            fx_1be_units[i].spin28.vz = ex->btlfx_randRange(-0x20, 0x20);
            ex->btlfx_getUnitScale(i, &fx_1be_units[i].scale8);
            ex->btlfx_getUnitRot(i, (s16 *)&fx_1be_units[i].rot0);
            ex->btlfx_getUnitRot(i, (s16 *)&fx_1be_units[i].rot20);
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1be_units[i].pos18);
        }
        for (i = 0; i < 0x40; i++) {
            r = ex->btlfx_rand() & 0xFFF;
            c = (ex->btlfx_rand() & 0xDFF) + 0xB00;
            fx_1be_op80ParticleEnds[i].vx = (ex->math_sin(c) * r) >> 12;
            fx_1be_op80ParticleEnds[i].vz = ((ex->math_cos(c) * r) >> 12) + 0x418;
            fx_1be_op80ParticleEnds[i].vy = 0;
        }
        w->posD4.vx = 0;
        w->posD4.vy = 0;
        w->posD4.vz = 0x418;
        w->unkCC.vx = 0;
        w->unkCC.vy = -0x2000;
        w->unkCC.vz = 0x418;
        w->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xC), 0xFF);
        w->scale74.vx = 0x1800;
        w->scale74.vy = 0x1800;
        w->scale74.vz = 0x1800;
        w->rot84.vx = 0;
        w->rot84.vy = 0;
        w->rot84.vz = 0;
        w->pos64.vx = 0;
        w->pos64.vy = -0x2000;
        w->pos64.vz = 0x418;
        w->pos8C.vz = 0x418;
        w->pos8C.vx = 0;
        w->pos8C.vy = -0x2000;
        w->scaleAC.vx = 0x1A80;
        w->scaleAC.vy = 0x1A80;
        w->scaleAC.vz = 0x1A80;
        w->modelA8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        w->model98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xE), 1);
        w->model94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        w->modelA4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 1);
    } else {
        frame = entry->frame;
        env->camera->points[1].vx = 0;
        env->camera->points[1].vy = -0x800;
        env->camera->points[1].vz = 0x418;
        env->camera->points[3].vx = w->pos64.vx;
        env->camera->points[3].vy = w->pos64.vy;
        env->camera->points[3].vz = w->pos64.vz;
        switch (frame) {
        case 0:
            w->counters0[1] = 0x28;
            w->counters0[0xD] = 0x20;
            w->counters0[0] = 1;
            w->counters0[0x14] = 1;
            w->counters0[4] = 0x10;
            break;
        case 0x18:
            w->emit20[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&w->posBC);
            w->emit20[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&w->posC4);
            if (w->emit20[0] != NULL) {
                w->emit20[0]->otZ = 0x200;
            }
            if (w->emit20[1] != NULL) {
                w->emit20[1]->otZ = 0x200;
            }
            w->counters0[0xF] = 0x20;
            w->counters0[7] = 0x10;
            w->counters0[5] = 1;
            w->counters0[0x14] = 1;
            w->counters0[0xC] = 0x30;
            p = w->emit20[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&w->posD4);
            if (p != NULL) {
                p->file->m.module.update = fx_1be_op80ParticleMotion;
            }
            break;
        case 0x30:
            w->counters0[0xE] = 0x10;
            break;
        case 0x40:
            w->counters0[9] = 0x3C;
            w->counters0[0xA] = 0x10;
            w->counters0[0x12] = frame;
            w->counters0[6] = 1;
            ex->btlfx_stopEmitter(w->emit20[2]);
            w->scale74.vx = 0x555;
            w->scale74.vy = 0x555;
            w->scale74.vz = 0x555;
            w->pos64.vx = 0;
            w->pos64.vy = -0x800;
            w->pos64.vz = 0x418;
            w->pos8C.vx = 0;
            w->pos8C.vy = -0x800;
            w->pos8C.vz = 0x418;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&w->pos8C);
            if (p != NULL) {
                p->otZ = 0x400;
            }
            break;
        case 0x50:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&w->pos8C);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&w->pos8C);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&w->pos8C);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            break;
        case 0x54:
            w->counters0[3] = 8;
            break;
        case 0x5C:
            w->counters0[0] = 0;
            w->counters0[5] = 0;
            w->counters0[0x14] = 0;
            break;
        case 0x2C:
            w->counters0[0xB] = 0x10;
            break;
        case 0x7C:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitScale(i, (s32 *)&fx_1be_units[i].scale8);
                ex->btlfx_setUnitRot(i, (s16 *)&fx_1be_units[i].rot0);
            }
            w->counters0[0x10] = 8;
            break;
        case 0xAC:
            return 1;
        }
        for (i = 0; i < 16; i++) {
            if (w->emit20[i] != NULL && w->emit20[i]->state == BTLFX_EMIT_DONE) {
                w->emit20[i] = NULL;
            }
        }
        if (w->counters0[1] != 0) {
            w->counters0[1]--;
            w->pos64.vy = fxm_easeLerp(2, 0x28, w->counters0[1], -0x2000, -0x1000);
            w->pos8C.vy = w->pos64.vy;
        }
        if (w->counters0[3] != 0) {
            w->counters0[3]--;
            w->scale74.vx = w->scale74.vz = w->scale74.vy = fxm_easeLerp(1, 8, w->counters0[3], 0x555, 0);
        }
        if (w->counters0[0] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xF), frame);
            ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scale74, w->model60);
        }
        if (w->counters0[0xD] != 0) {
            w->counters0[0xD]--;
            ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scaleAC, w->modelA8);
            if (w->counters0[0xD] < 0x10) {
                a = (w->counters0[0xD] - 0x10) * 0x10;
                ex->Hi_ModifyEffModelRGB(w->modelA8, a, a, a);
            }
        }
        if (w->counters0[0xF] != 0) {
            w->counters0[0xF]--;
            ex->btlfx_moveEmitterTo((s32)w->emit20[0], (s32)&w->pos8C);
            ex->btlfx_moveEmitterTo((s32)w->emit20[1], (s32)&w->pos8C);
        }
        if (w->counters0[0xE] != 0) {
            if (--w->counters0[0xE] != 0) {
                if (w->emit20[0] != NULL) {
                    w->emit20[0]->alpha = w->counters0[0xE] << 8;
                }
                if (w->emit20[1] != NULL) {
                    w->emit20[1]->alpha = w->counters0[0xE] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(w->emit20[0]);
                ex->btlfx_stopEmitter(w->emit20[1]);
            }
        }
        if (w->counters0[5] != 0) {
            aim.vx = w->pos64.vx;
            aim.vy = w->pos64.vy;
            aim.vz = w->pos64.vz;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&aim, (s16 *)&w->rot9C);
            ex->Hi_DrawEffModel(&w->rot9C, &w->pos64, (s32)&w->scale74, w->model94);
            if (w->counters0[6] != 0) {
                ex->Hi_DrawEffModel(&w->rot9C, &w->pos64, (s32)&w->scale74, w->model98);
                ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scale74, w->modelA4);
            }
        }
        if (w->counters0[7] != 0) {
            w->counters0[7]--;
            a = -(w->counters0[7] * 0x10);
            ex->Hi_ModifyEffModelRGB(w->model94, a, a, a);
        }
        if (w->counters0[0xC] != 0) {
            w->counters0[0xC]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                u = &fx_1be_units[i];
                if (u->timer34 == 0) {
                    if (u->timer30 != 0) {
                        u->timer30--;
                        fxm_easeLerpSvec(1, 0x28, u->timer30, &fx_1be_units[i].pos18, &w->pos8C, &ang);
                        ex->btlfx_setUnitPos(i, (s16 *)&ang);
                        sc.vx = sc.vy = sc.vz = fxm_easeLerp(1, 0x28, u->timer30, u->scale8.vy, 0);
                        ex->btlfx_setUnitScale(i, (s32 *)&sc);
                        u->rot20.vx += u->spin28.vx;
                        u->rot20.vy += u->spin28.vy;
                        u->rot20.vz += u->spin28.vz;
                        ex->btlfx_setUnitRot(i, (s16 *)&fx_1be_units[i].rot20);
                    }
                } else {
                    u->timer34--;
                }
            }
        }
        if (w->counters0[9] != 0) {
            a = 0xFFF;
            w->counters0[9]--;
            if (w->counters0[0xA] != 0) {
                w->counters0[0xA]--;
                a = (0x10 - w->counters0[0xA]) << 8;
            }
            if (w->counters0[0xB] != 0) {
                w->counters0[0xB]--;
                a = w->counters0[0xB] << 8;
            }
            ex->btlfx_aimAngles((s16 *)&w->pos8C, (s16 *)&env->camera->eye, (s16 *)&rot2);
            pos.vz = 0xC18;
            pos.vx = 0;
            pos.vy = -0x1200;
            rot2.vz = 0;
            rot2.vx += 0xA0;
            fx_1be_drawGradientGrid(&pos, &rot2, a, 0x19, 0x3D03, 1);
            pos.vy = 0x200;
            rot2.vz = 0x800;
            rot2.vx -= 0x140;
            fx_1be_drawGradientGrid(&pos, &rot2, a, 0x19, 0x3D03, 1);
        }
        if (w->counters0[0x10] != 0) {
            w->counters0[0x10]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1be_units[i].pos18);
                fx_1be_units[i].pos18.vy -= ex->math_sin(w->counters0[0x10] << 7) >> 1;
                ex->btlfx_setUnitPos(i, (s16 *)&fx_1be_units[i].pos18);
                if (w->counters0[0x10] == 0) {
                    fx_1be_units[i].pos18.vy = 0;
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&fx_1be_units[i].pos18);
                    if (p != NULL) {
                        p->otZ = -0x100;
                    }
                }
            }
        }
        if (w->counters0[0x12] != 0) {
            w->counters0[0x12]--;
            fx_1be_scrollSpeed = fxm_easeLerp(1, 0x40, w->counters0[0x12], 4, 0x14);
        }
        fx_1be_scrollPos += fx_1be_scrollSpeed;
        if (w->counters0[5] != 0) {
            ex->btlfx_drawScrollMove(fx_1be_scrollPos, 0x2C0, 0x100, 0x38, 0x70, 0x2C0, 0x170);
        }
        if (w->counters0[0x14] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - fx_1be_scrollPos, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (w->counters0[9] != 0) {
            ex->btlfx_drawScrollMove(frame * 8, 0x240, 0x140, 0x18, 0x60, 0x240, 0x1A0);
        }
        if (w->counters0[4] != 0) {
            w->counters0[4]--;
            col.r = col.g = col.b = w->counters0[4] * 8;
            scale.vz = scale.vy = scale.vx = w->counters0[4] * 0x10 + 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
    }
    return 0;
}

void fx_1be_drawGradientGrid(SVECTOR *pos, SVECTOR *rot, s32 n, s32 tpage, s32 clut, s32 abr) {
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

    env = fx_1be_env;
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
        t.vx = fx_1be_gridColors[j].vx;
        t.vy = fx_1be_gridColors[j].vy;
        t.vz = fx_1be_gridColors[j].vz;
        c0.r = (t.vx * k0) >> 12;
        c0.g = (t.vy * k0) >> 12;
        c0.b = (t.vz * k0) >> 12;
        t.vx = fx_1be_gridColors[j + 1].vx;
        t.vy = fx_1be_gridColors[j + 1].vy;
        t.vz = fx_1be_gridColors[j + 1].vz;
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

static s32 fx_1be_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;
    SVECTOR from;
    SVECTOR *key;

    key = &fx_1be_op80ParticleEnds[(e->slot - 1) & 0x3F];
    from.vx = 0;
    from.vy = -0x800;
    from.vz = 0x418;
    fxm_easeLerpSvec(1, 0x1000, ((0x1000000 / frames) * frame) >> 12, &from, key, &v);
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
