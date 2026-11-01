#include "fxmod/fx_1fe.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_1fe_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
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
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_b();
                *(s32 *)&colBL = *(s32 *)&colBR;
                *(s32 *)&colTL = *(s32 *)&colTR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(0x8000000, otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, otz, tpage);
    }
    w--;
    h--;
    left = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_waveVertexX(s16 *vert) {
    s32 wave = (BTLFX_EXPORTS(fx_1fe_env)->math_sin(fx_1fe_wavePhase + (vert[1] << 8)) * fx_1fe_waveAmp) >> 12;
    s32 x = vert[0];

    vert[0] = x + wave;
}

s32 fx_1fe_op81(s32 mode, Fx1feWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    s32 frame;
    s32 t;
    s32 spin;
    s32 shade;
    s32 rise;
    s32 k;
    s32 grow;
    s32 unused0;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        work->state0 = 1;
        work->frame4 = 0;
        fx_1fe_env = env;
        ex->btlfx_initEffModels(fx_1fe_effModels, 7);
        work->model34 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
        work->model38 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        work->model3C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xF), 1);
        work->unk8.vx = fx_1fe_basePos0.vx;
        work->unk8.vy = fx_1fe_basePos0.vy;
        work->unk8.vz = fx_1fe_basePos0.vz;
        work->camTarget20.vx = fx_1fe_basePos0.vx;
        work->camTarget20.vy = fx_1fe_basePos0.vy;
        work->camTarget20.vz = fx_1fe_basePos0.vz;
        work->camTarget20.vy -= 0xA00;
        env->camera->points[3].vx = work->camTarget20.vx;
        env->camera->points[3].vy = work->camTarget20.vy;
        env->camera->points[3].vz = work->camTarget20.vz;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 1:
            rise = (frame << 12) / 37;
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->pos10, &fx_1fe_basePos0);
                work->pos10.vy -= 0x1000;
                work->pos10.vz -= 0x200;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = fx_1fe_op81Trails;
                    emitter->trailSlots = 0x40;
                    emitter->trailStride = 0x1C;
                    emitter->otZ = 0x80;
                }
            }
            work->camTarget20.vx = fx_1fe_basePos0.vx;
            work->camTarget20.vy = fx_1fe_basePos0.vy;
            work->camTarget20.vz = fx_1fe_basePos0.vz;
            work->camTarget20.vy = (s16)(work->camTarget20.vy - 0x200) - (ex->math_sin(rise >> 2) >> 1);
            env->camera->points[3].vx = work->camTarget20.vx;
            env->camera->points[3].vy = work->camTarget20.vy;
            env->camera->points[3].vz = work->camTarget20.vz;
            if (frame >= 0x25) {
                work->state0 = 2;
                entry->frame = -1;
                work->camTarget20.vx = fx_1fe_basePos0.vx;
                work->camTarget20.vy = fx_1fe_basePos0.vy;
                work->camTarget20.vz = fx_1fe_basePos0.vz;
                work->camTarget20.vy -= 0x200;
                env->camera->points[3].vx = work->camTarget20.vx;
                env->camera->points[3].vy = work->camTarget20.vy;
                env->camera->points[3].vz = work->camTarget20.vz;
            }
            break;
        case 2:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&fx_1fe_basePos0);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                work->pos10.vx = fx_1fe_basePos0.vx;
                work->pos10.vy = fx_1fe_basePos0.vy;
                work->pos10.vz = fx_1fe_basePos0.vz;
                work->pos10.vy -= 0x200;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 7) {
                ex->btlfx_initParticleSystem();
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&fx_1fe_basePos0);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame >= 7) {
                rot.vy = frame * 4;
                rot.vx = 0;
                rot.vz = 0;
                scale.vy = 0x555;
                pos.vx = fx_1fe_basePos0.vx;
                pos.vy = fx_1fe_basePos0.vy;
                pos.vz = fx_1fe_basePos0.vz;
                grow = ((frame - 7) << 12) / 25;
                scale.vx = scale.vz = (grow >> 4) + scale.vy;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model34);
                spin = frame * 7;
                rot.vy = spin;
                scale.vx = scale.vz = (grow >> 4) + 0x400;
                scale.vy = 0x800;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model34);
                ex->Hi_ModifyEffModelRGB(work->model34, -0x20, -0x20, -0x20);
                rot.vy = spin;
                scale.vx = scale.vz = 0x400;
                scale.vy = ((ex->math_sin(grow >> 2) / 2) + 0x800) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model38);
                ex->btlfx_drawScrollMove(1000 - frame * 9, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
                ex->btlfx_drawScrollMove(1000 - spin, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
                ex->math_cos(grow);
                ex->math_cos(grow * 2);
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0x42, 0x10, 3, 0x3C, 1, &fx_1fe_rippleColor, 0x18, frame * 400, 0x186, 0x100);
            }
            if ((u32)(frame - 7) < 0x14) {
                s32 pulse;
                s32 ang;
                s32 fade;

                rot.vx = 0x400;
                pos.vx = fx_1fe_basePos0.vx;
                pos.vy = fx_1fe_basePos0.vy;
                pos.vz = fx_1fe_basePos0.vz;
                pulse = ((frame - 7) << 12) / 20;
                ang = pulse >> 2;
                fade = 0xC00 - pulse;
                rot.vy = ex->math_sin(ang) / 3;
                rot.vz = 0;
                scale.vx = scale.vy = (ex->math_sin(ang) >> 6) + 0x249;
                scale.vz = 0x555;
                rise = (ex->math_sin(ang) / 15) + 2;
                shade = (fade >> 5) - 0x80;
                for (k = 0; k < 7; k++) {
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model3C);
                    ex->Hi_ModifyEffModelRGB(work->model3C, shade, shade, shade);
                    pos.vy -= rise;
                    rot.vy = ((rot.vy * 5) >> 2) + (k << 10);
                    scale.vx = scale.vy = (scale.vx * 9) >> 3;
                }
            }
            if (frame == 5) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x64);
            }
            if (frame == 6) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0, 0x80, 0xFF);
            }
            if ((u32)(frame - 7) < 8) {
                s32 tint = 0xF0 - (((frame - 7) * 0xF0) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint >> 1);
            }
            if (frame >= 0x18) {
                s32 tint = ((frame - 0x18) * 0xFF) >> 3;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
            }
            if (frame >= 0x20) {
                work->state0 = 6;
                entry->frame = -1;
                work->camTarget20.vx = fx_1fe_basePos1.vx;
                work->camTarget20.vy = fx_1fe_basePos1.vy;
                work->camTarget20.vz = fx_1fe_basePos1.vz;
                work->camTarget20.vy -= 0x400;
                env->camera->points[3].vx = work->camTarget20.vx;
                env->camera->points[3].vy = work->camTarget20.vy;
                env->camera->points[3].vz = work->camTarget20.vz;
            }
            break;
        case 6:
            t = (frame << 12) / 87;
            if (frame < 0x18) {
                s32 tint = 0xFF - (frame * 0xFF) / 24;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
            }
            work->camTarget20.vx = fx_1fe_basePos1.vx;
            work->camTarget20.vy = fx_1fe_basePos1.vy;
            work->camTarget20.vz = fx_1fe_basePos1.vz;
            work->camTarget20.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
            env->camera->points[3].vx = work->camTarget20.vx;
            env->camera->points[3].vy = work->camTarget20.vy;
            env->camera->points[3].vz = work->camTarget20.vz;
            if (frame == 0) {
                work->pos10.vx = fx_1fe_basePos1.vx;
                work->pos10.vy = fx_1fe_basePos1.vy;
                work->pos10.vz = fx_1fe_basePos1.vz - 0x100;
                work->pos10.vy -= 0x400;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x10;
                }
            }
            if (frame == 0x20) {
                work->pos18.vx = fx_1fe_basePos1.vx;
                work->pos18.vy = fx_1fe_basePos1.vy;
                work->pos18.vz = fx_1fe_basePos1.vz;
                work->pos10.vz -= 0x100;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x10;
                }
            }
            if (frame >= 0x57) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_1fe_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fe_env = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fe/nonmatchings/fx_1fe", fx_1fe_headerPtr);
