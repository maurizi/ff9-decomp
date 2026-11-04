#include "fxmod/fx_048.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_048_env;
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
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ringPos;
    VECTOR pos;
    VECTOR pillarScale;
    SVECTOR pillarRot;
    SVECTOR segRot;
    VECTOR segPos;
    VECTOR segScale;
    SVECTOR sparkPos;
    Pool6CEntry *emitter;
    s32 ringAng;
    s32 frame;
    s32 alpha;
    s32 size;
    s32 t;
    s32 segT;
    s32 flash;
    s32 i;
    s32 sparkCount;
    s32 ang;
    s32 grow;
    s32 coil;
    s32 tmp;
    s32 segAng;
    s32 ringT;

    if (mode == 0) {
        *args = 0x38;
    } else if (mode == 1) {
        work->pos0.vx = env->targets->centerX;
        fx_048_env = env;
        work->pos0.vy = env->targets->centerY;
        work->pos0.vz = env->targets->centerZ;
        work->pos0.vy -= 0x200;
        BTLFX_COPY_SVEC(&work->pos8, &work->pos0);
        work->pos8.vy = 0;
        ex->btlfx_initEffModels(fx_048_effModels, 6);
        work->model14[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_COLUMN_SEG_0), 1);
        work->model14[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_COLUMN_SEG_1), 1);
        work->model14[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_COLUMN_SEG_2), 1);
        work->model14[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_COLUMN_SEG_3), 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_FLAME_PILLAR), 0x37, 0x3D06, 0x80, 0x80);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_FLAME_DISC), 0x37, 0x3D06, 0x80, 0x80);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_ORBIT_TRAIL), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->trails = (s32)fx_048_trails;
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
            size = 0x1800;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            rot.vx = 0x400;
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_GLOW), &work->pos8, NULL, 0x1000, 0, 1, t);
            env->otZ = 0;
            if (frame >= 0x18) {
                work->state10 = 1;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_FLAME_BURST), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            break;
        case 1:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0x800, 1, (t * 70) >> 12, 0, 0);
            if (frame == 4) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_EMBERS), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_GOLD_SPARKS_2), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_GOLD_SPARKS), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            alpha = ex->math_cos(t >> 2);
            env->otZ = 0x200;
            size = (t * 2) / 3 + 0x555;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_GLOW), &work->pos8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->otZ = 0;
            if (frame >= 8) {
                work->state10 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 46;
            if (frame < 4) {
                flash = ex->math_cos(frame << 8) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, flash >> 12, flash >> 13, flash >> 15);
            }
            if (frame < 0x20) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + OT_LAST, 0xFF,
                                         ((ex->math_cos(frame << 5) / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0,
                                         0);
            }
            {
            s32 spin;
            s32 pillarOn;
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

            spin = frame << 4;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.pad = 1;
            rot.vz = spin;
            pillarOn = frame < 0x1D;
            if (pillarOn) {
                ringT = (frame << 12) / 29;
                ringAng = ringT >> 2;
                alpha = ex->math_cos(ringAng);
                ringPos.vx = work->pos8.vx;
                ringPos.vy = work->pos8.vy;
                size = ringT + 0x1000;
                ringPos.vz = work->pos8.vz;
                ringPos.vy -= 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_PILLAR_RING), &ringPos, &rot, ex->math_sin(ringAng) * 2 + 0x1800,
                                  0, 1, ex->math_cos(ringAng));
            }
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_GLOW), &work->pos8, NULL,
                              ex->math_cos(t >> 2) / 2 * 3 + 0x2000, 0, 1, ex->math_cos(t >> 2));
            env->otZ = 0;
            if (pillarOn) {
                ex->btlfx_drawScrollMove(spin, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
                tmp = ((frame << 12) / 29) >> 2;
                pillarScale.vx = pillarScale.vz = ex->math_sin(tmp) / 4 + 0x80;
                pillarScale.vy = ex->math_sin(tmp) * 3 / 2 + 0x1000;
                pillarRot.vx = 0;
                pillarRot.vy = frame * 24;
                pillarRot.vz = 0;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                pillarScale.vx = pillarScale.vz = ex->math_sin(tmp) / 2 + 0x200;
                pillarScale.vy = (ex->math_cos(tmp) * 3 / 2 + 0x1000) * 3 / 2;
                ex->Hi_DrawEffModel(&pillarRot, &pos, (s32)&pillarScale, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(tmp) >> 5) - 0x80, (ex->math_cos(tmp) >> 5) - 0x80,
                                         (ex->math_cos(tmp) >> 5) - 0x80);
            }
            if (frame == 2) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_FLAME_BURST_2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 0xE) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_SMOKE_BURST), (s32)work);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 3) {
                emitter = work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_CONE_GLOW), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                emitter = work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRAGA_CONE_SPARKS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
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
                segT = (frame << 12) / 36;
                ang = -frame << 7;
                wobbleAng = -frame * 0x19B;
                segRot.vx = 0x800;
                radiusFlare = 0;
                segRot.vy = frame * 24;
                segRot.vz = 0;
                grow = (segT << 7) >> 12;
                coilStep = grow + 0x80;
                coilRadius = grow + 0x100;
                if (frame < 0x10) {
                    radiusBase = 0x4E2;
                    radiusTop = (frame * 0x4E2) / 16;
                    wobble = 0xB9;
                } else {
                    tmp = ((frame - 0x10) << 10) / 20;
                    radiusBase = ex->math_sin(tmp) / 4 + 0x4E2;
                    radiusTop = ((ex->math_sin(tmp) * 3) >> 2) + 0x4E2;
                    radiusFlare = tmp / 3;
                    wobble = (tmp * 2) / 3 + 0xB9;
                }
                tmp = segT >> 2;
                segHeight = (segT >> 5) + 0xE00;
                dy = frame * 6 + 8;
                bright = ex->math_cos(tmp) / 2;
                if (frame < 6) {
                    riseDist = (ex->math_sin((frame << 10) / 6) * 0xB86) >> 12;
                } else {
                    riseDist = 0xA28 - tmp;
                }
                sparkCount = 0;
                for (i = 0; i < 0x12; i++) {
                    segT = (i << 12) / 18;
                    segPos.vx = work->pos8.vx;
                    segPos.vy = work->pos8.vy;
                    segPos.vz = work->pos8.vz;
                    segPos.vy -= 0x200 + ((segT * riseDist) >> 12);
                    ang += coilStep;
                    coil = (coilRadius * segT) >> 12;
                    segPos.vx += (ex->math_sin(ang) * coil) >> 12;
                    segPos.vz += (ex->math_cos(ang) * coil) >> 12;
                    segAng = segT >> 2;
                    wobbleAng += 0x180;
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
                    ex->Hi_ModifyEffModelRGB(work->model14[i & 3], ((bright * ex->math_cos(segAng)) >> 17) - 0x80, ((bright * (0x1000 - segT)) >> 17) - 0x80, ((bright * (0x1000 - segT)) >> 17) - 0x80);
                    if (!(ex->btlfx_rand() & 3) && sparkCount < 3) {
                        env->otZ = 0x100;
                        sparkPos.vx = segPos.vx;
                        sparkPos.vy = segPos.vy;
                        sparkPos.vz = segPos.vz;
                        sparkPos.vx += ex->btlfx_randCentered(0x140);
                        sparkPos.vy += ex->btlfx_randCentered(0x100);
                        sparkPos.vz += ex->btlfx_randCentered(0x140);
                        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FIRAGA_COLUMN_SPARK), &sparkPos, NULL, 0x7000, 0, 1, bright);
                        sparkCount++;
                        env->otZ = 0;
                    }
                }
            }
            }
            if (frame >= 0x2E) {
                work->state10 = 3;
                entry->frame = -1;
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
            fxm_drawTexelQuads(&work->pos8, &rot, 0x117, 0, 0x30, 2, work->frame34 & 7, 0x100, 0x10, 0x10, (size * 2) / 3, alpha, 1,
                          fx_048_texels, 0);
        }
        work->frame34++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_048/nonmatchings/fx_048", fx_048_headerPtr);
