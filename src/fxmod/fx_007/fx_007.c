#include "fxmod/fx_007.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_007_env;
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
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * hh);
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

s32 fx_007_op80(s32 mode, Fx007Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 alpha;
    s32 size;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x38;
    } else if (mode == 1) {
        Pool6CEntry *emitter;

        fx_007_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_initEffModels(fx_007_effModels, 6);
        work->model14[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x16), 1);
        work->model14[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x17), 1);
        work->model14[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x18), 1);
        work->model14[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x19), 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x37, 0x3D06, 0x80, 0x80);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 9), 0x37, 0x3D06, 0x80, 0x80);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->trails = (s32)fx_007_trails;
            emitter->trailSlots = 0x12;
            emitter->trailStride = 0x24;
            emitter->otZ = 0x20;
        }
        work->emit2C = NULL;
        work->emit30 = NULL;
        work->frame34 = 0;
        work->state10 = 0;
    } else {
        alpha = 0;
        size = 0;
        frame = entry->frame;
        switch (work->state10) {
        case 0:
            t = (frame << 12) / 24;
            alpha = t / 3;
            size = 0xAAA;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            rot.vx = 0x400;
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos8, NULL, 0x1000, 0, 1, t);
            env->otZ = 0;
            if (frame >= 0x18) {
                Pool6CEntry *emitter;

                work->state10 = 1;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            break;
        case 1:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0x800, 1, (t * 0x46) >> 12, 0, 0);
            if (frame == 4) {
                Pool6CEntry *emitter;

                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            alpha = ex->math_cos(t >> 2);
            env->otZ = 0x200;
            size = t / 3 + 0x555;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->otZ = 0;
            if (frame >= 8) {
                work->state10 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 46;
            {
                SVECTOR ringPos;
                VECTOR pos;
                VECTOR pillarScale;
                SVECTOR pillarRot;
                SVECTOR segRot;
                VECTOR segPos;
                VECTOR segScale;
                SVECTOR sparkPos;
                s32 spin;
                s32 pillarOn;
                s32 fade;
                s32 flash;
                s32 i;
                s32 sparkCount;

                if (frame < 4) {
                    flash = ex->math_cos(frame << 8) * 0xFF;
                    ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, flash >> 12, flash >> 13, flash >> 15);
                }
                if (frame < 0x20) {
                    fade = ex->math_cos(frame << 5);
                    ex->btlfx_drawScreenTint((u32 *)env->ot + OT_LAST, 0xFF,
                                             ((fade / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0, 0);
                }
                spin = frame * 16;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.pad = 1;
                rot.vz = spin;
                pillarOn = frame < 0x1D;
                if (pillarOn) {
                    s32 ringT;
                    s32 ringAng;
                    u8 *data;

                    ringT = (frame << 12) / 29;
                    ringAng = ringT >> 2;
                    alpha = ex->math_cos(ringAng);
                    ringPos.vx = work->pos8.vx;
                    size = ringT / 2 + 0x800;
                    ringPos.vy = work->pos8.vy;
                    ringPos.vz = work->pos8.vz;
                    ringPos.vy -= 0x400;
                    data = (u8 *)ex->btlfx_getFileData(entry->file, 0xF);
                    ex->btlfx_drawRings(data, &ringPos, &rot, ex->math_sin(ringAng) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(ringAng));
                }
                env->otZ = 0x200;
                {
                    u8 *data = (u8 *)ex->btlfx_getFileData(entry->file, 0xB);
                    s32 glowScale = ex->math_cos(t >> 2) / 2 * 2 + 0x2000;

                    ex->btlfx_drawRings(data, &work->pos8, NULL, glowScale, 0, 1, ex->math_cos(t >> 2));
                }
                env->otZ = 0;
                if (pillarOn) {
                    s32 pillarAng;

                    ex->btlfx_drawScrollMove(spin, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
                    pillarAng = ((frame << 12) / 29) >> 2;
                    pillarScale.vx = pillarScale.vz = ex->math_sin(pillarAng) / 4 + 0x80;
                    pillarScale.vy = ex->math_sin(pillarAng) * 3 / 2 + 0x1000;
                    pillarRot.vx = 0;
                    pillarRot.vy = frame * 0x18;
                    pillarRot.vz = 0;
                    pos.vx = work->pos8.vx;
                    pos.vy = work->pos8.vy;
                    pos.vz = work->pos8.vz;
                    pillarScale.vx = pillarScale.vz = ex->math_sin(pillarAng) / 3 + 0x180;
                    pillarScale.vy = ex->math_cos(pillarAng) * 3 / 2 + 0x1000;
                    ex->Hi_DrawEffModel(&pillarRot, &pos, (s32)&pillarScale, work->model24);
                    ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(pillarAng) >> 5) - 0x80, (ex->math_cos(pillarAng) >> 5) - 0x80,
                                             (ex->math_cos(pillarAng) >> 5) - 0x80);
                }
                if (frame == 2) {
                    Pool6CEntry *emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos8);

                    if (emitter != NULL) {
                        emitter->otZ = 0x100;
                    }
                }
                if (frame == 0xE) {
                    Pool6CEntry *emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)work);

                    if (emitter != NULL) {
                        emitter->otZ = 0x100;
                    }
                }
                if (frame == 3) {
                    work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos8);
                    if (work->emit2C != NULL) {
                        work->emit2C->otZ = 0x100;
                    }
                    work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos8);
                    if (work->emit30 != NULL) {
                        work->emit30->otZ = 0x100;
                    }
                }
                segRot.vx = 0x400;
                segRot.vy = 0;
                segRot.vz = 0;
                if (work->emit2C != NULL) {
                    if (work->emit2C->state == BTLFX_EMIT_DONE) {
                        work->emit2C = NULL;
                    }
                    if (work->emit2C != NULL) {
                        work->emit2C->rot.vx = segRot.vx;
                        work->emit2C->rot.vy = segRot.vy;
                        work->emit2C->rot.vz = segRot.vz;
                    }
                }
                if (work->emit30 != NULL) {
                    if (work->emit30->state == BTLFX_EMIT_DONE) {
                        work->emit30 = NULL;
                    }
                    if (work->emit30 != NULL) {
                        work->emit30->rot.vx = segRot.vx;
                        work->emit30->rot.vy = segRot.vy;
                        work->emit30->rot.vz = segRot.vz;
                    }
                }
                if (frame < 0x24) {
                    s32 radiusBase;
                    s32 radiusTop;
                    s32 radiusFlare;
                    s32 segHeight;
                    s32 riseDist;
                    s32 dy;
                    s32 coilRadius;
                    s32 coilStep;
                    s32 bright;
                    s32 wobbleAng;
                    s32 wobble;
                    s32 ang;
                    s32 tmp;
                    s32 segT;
                    s32 segAng;
                    s32 grow;

                    segT = (frame << 12) / 36;
                    ang = -frame * 0x80;
                    segRot.vx = 0x800;
                    wobbleAng = -frame * 0x19B;
                    radiusFlare = 0;
                    segRot.vy = frame * 0x18;
                    segRot.vz = 0;
                    grow = (segT << 7) >> 12;
                    coilStep = grow + 0x80;
                    coilRadius = grow + 0x100;
                    if (frame < 0x10) {
                        radiusBase = 0x2BC;
                        wobble = 0xAA;
                        radiusTop = (frame * 0x2BC) / 12;
                    } else {
                        tmp = ((frame - 0x10) << 10) / 20;
                        radiusBase = ex->math_sin(tmp) / 4 + 0x2BC;
                        radiusTop = ((ex->math_sin(tmp) * 3) >> 2) + 0x2BC;
                        wobble = tmp / 2 + 0xAA;
                        radiusFlare = tmp / 3;
                    }
                    tmp = segT >> 2;
                    segHeight = (segT >> 5) + 0x9C0;
                    dy = frame * 6 + 8;
                    bright = ex->math_cos(tmp) / 2;
                    if (frame < 6) {
                        riseDist = (ex->math_sin((frame << 10) / 6) * 0x960) >> 12;
                    } else {
                        riseDist = 0x960 - tmp;
                    }
                    for (sparkCount = 0, i = 0; i < 18; i++) {
                        segT = (i << 12) / 18;
                        segPos.vx = work->pos8.vx;
                        segPos.vy = work->pos8.vy;
                        segPos.vz = work->pos8.vz;
                        segPos.vy -= 0x200;
                        segPos.vy -= (segT * riseDist) >> 12;
                        ang += coilStep;
                        tmp = (coilRadius * segT) >> 12;
                        segPos.vx += (ex->math_sin(ang) * tmp) >> 12;
                        segAng = segT >> 2;
                        wobbleAng += 0x180;
                        segPos.vz += (ex->math_cos(ang) * tmp) >> 12;
                        tmp = ex->btlfx_lerp(radiusBase, radiusTop, 0x1000 - ex->math_cos(segAng)) + ((ex->math_sin(wobbleAng) * wobble) >> 12);
                        segScale.vy = segHeight;
                        segScale.vx = segScale.vz = tmp;
                        if (segT > 0x800) {
                            tmp = segT - 0x800;
                            tmp >>= 1;
                            segScale.vx += ((0x1000 - ex->math_cos(tmp)) * radiusFlare) >> 12;
                            segScale.vz += ((0x1000 - ex->math_cos(tmp)) * radiusFlare) >> 12;
                        }
                        segRot.vy += dy;
                        ex->Hi_DrawEffModel(&segRot, &segPos, (s32)&segScale, work->model14[i & 3]);
                        ex->Hi_ModifyEffModelRGB(work->model14[i & 3], ((bright * ex->math_cos(segAng)) >> 17) - 0x80,
                                                 ((bright * (0x1000 - segT)) >> 17) - 0x80, ((bright * (0x1000 - segT)) >> 17) - 0x80);
                        if (!(ex->btlfx_rand() & 3) && sparkCount < 3) {
                            env->otZ = 0x100;
                            sparkPos.vx = segPos.vx;
                            sparkPos.vy = segPos.vy;
                            sparkPos.vz = segPos.vz;
                            sparkPos.vx += ex->btlfx_randCentered(0x140);
                            sparkPos.vy += ex->btlfx_randCentered(0x100);
                            sparkPos.vz += ex->btlfx_randCentered(0x140);
                            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &sparkPos, NULL, 0x7000, 0, 1, bright);
                            sparkCount++;
                            env->otZ = 0;
                        }
                    }
                }
                if (frame >= 0x2E) {
                    work->state10 = 3;
                    entry->frame = -1;
                }
            }
            break;
        case 3:
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.vx = 0x400;
            rot.vz = frame << 8;
            rot.pad = 1;
            rot.vy = 0;
            fxm_drawTexelQuads(&work->pos8, &rot, 0x117, 0, 0x30, 2, work->frame34 & 7, 0x100, 0x10, 0x10, size, alpha, 1,
                          fx_007_texels, 0);
        }
        work->frame34++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_007/nonmatchings/fx_007", fx_007_headerPtr);
