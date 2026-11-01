#include "fxmod/fx_08f.h"

static void fxm_randDiscPoints(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *p;
    s32 *prev;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_08f_env);
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

static void fxm_drawStreakField(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pt, CVECTOR *col, s32 p, s32 abr) {
    Unk800F78A4 *env = fx_08f_env;
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
    ex->math_rotMatrixYXZ(src, &m);
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

s32 fx_08f_op80(s32 mode, Fx08fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR rot2;
    CVECTOR streakCol;
    CVECTOR gridCol;
    SVECTOR rot3;
    VECTOR scale2;
    SVECTOR spriteRot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 fadeIn;
    s32 glowT;
    s32 level;
    s32 near;
    s32 spriteOfs;
    s32 camZ;
    s32 camY;
    s32 d;
    s32 camShift;
    s32 shake;
    s32 bright;
    s32 spriteFrame;
    s32 spriteY;
    s32 otz;
    s32 spriteX;
    s32 spriteAlpha;
    s32 spinAlpha;
    s32 t;
    s32 approach;
    s32 dist;

    if (mode == 0) {
        *args = sizeof(Fx08fWork80);
    } else if (mode == 1) {
        fx_08f_env = env;
        work->state0 = 1;
        work->frame4 = 0;
        fxm_randDiscPoints(0x180, 0x400, fx_08f_streakPts);
        work->unk8[0] = 0xA0;
        work->unk8[1] = 0x78;
        work->unk8[2] = 0;
        work->unk8[3] = 0x78;
        work->unk8[4] = 0xC8;
        work->unk8[5] = 0x78;
        ex->btlfx_initEffModels(fx_08f_effModels, 8);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x11), 0x9A, 0x3E00, 0, 0);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 4), 0x139, 0xFFFF, 0, 0x80);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x1A), 0xB9, 0x3D40, 0, 0);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC), 0x3C, 0x3D04, 0, 0x80);
        work->model34 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x17));
        work->model38 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x18));
        work->model3C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x19));
        work->model40 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xF), 0x97, 0x3D80, 0, 0);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 1:
            level = (frame << 11) / 49;
            near = frame < 8;
            pos.vx = 0;
            pos.vz = 0x800 - level;
            pos.vy = level - 0x800;
            fadeIn = (frame << 12) / 49;
            if (!near) {
                shake = frame - 8;
                pos.vx += ex->btlfx_randCentered(shake * 3);
                pos.vy += ex->btlfx_randCentered(shake * 2);
                pos.vz += ex->btlfx_randCentered(shake * 2);
            }
            rot.vx = 0x200;
            rot.vz = -frame << 6;
            rot.vy = 0;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            work->pos14.vx = pos.vx;
            work->pos14.vy = pos.vy;
            work->pos14.vz = pos.vz - 0x46;
            work->pos14.vy += 0x1D;
            env->camera->points[3].vx = pos.vx;
            env->camera->points[3].vy = pos.vy;
            env->camera->points[3].vz = pos.vz;
            camZ = (u16)env->camera->points[3].vz - 0x100;
            camShift = (frame * 0x384) / 49;
            env->camera->points[3].vz = camZ + camShift;
            camY = (u16)env->camera->points[3].vy + 0x180;
            env->camera->points[3].vy = camY - camShift;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
            ex->btlfx_drawScrollMove(work->frame4 * 8, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model40);
            ex->Hi_ModifyEffModelRGB(work->model40, (level >> 5) - 0x80, (level >> 5) - 0x80, (level >> 5) - 0x80);
            if (!near) {
                glowT = ((frame - 8) << 12) / 41;
                rot.vx = 0x200;
                rot.vy = 0;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
                ex->Hi_ModifyEffModelAbr(work->model28, 1);
                ex->Hi_ModifyEffModelRGB(work->model28, (glowT >> 5) - 0x80, (glowT >> 5) - 0x80, (glowT >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), work->frame4);
            }
            otz = -0x64;
            streakCol.r = 0x20;
            streakCol.g = 0x80;
            streakCol.b = 0xFF;
            rot2.vy = -0x400;
            rot2.vx = 0x200;
            rot2.vz = 0;
            if (frame < 0x18) {
                otz = -0x46;
            }
            fxm_drawStreakField((u32 *)(env->ot + 8), 0x180, 0x80, otz, &rot2, fx_08f_streakPts, &streakCol,
                          ex->math_cos((frame << 10) / 49) * 3 / 4 + 0x400, 1);
            if (frame == 2) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos14);
                work->emit44 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x5C;
                }
            }
            if (frame >= 0x19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((((frame - 0x18) << 12) / 25) << 6) >> 12, 0, 0);
            }
            spriteOfs = (frame & 7) * 0x3C;
            spriteX = 0x1B8 - spriteOfs;
            spriteY = 0x140 - spriteOfs;
            ex->btlfx_drawSpriteAtOt(2, (u8 *)ex->btlfx_getFileData(entry->file, 0x10), spriteX + 0x64, spriteY + 0x78, 0, 1, fadeIn);
            spriteOfs = ((frame + 5) % 10) * 0x3C;
            spriteX = 0x1B8 - spriteOfs;
            spriteY = 0x140 - spriteOfs;
            ex->btlfx_drawSpriteAtOt(2, (u8 *)ex->btlfx_getFileData(entry->file, 0xE), spriteX - 0x40, spriteY + 0x78, 0, 1, fadeIn);
            if (frame >= 0x31) {
                ex->btlfx_stopEmitter(work->emit44);
                work->camPos1C.vx = 0;
                work->camPos1C.vy = -0x140;
                work->camPos1C.vz = 0;
                env->camera->points[3].vx = work->camPos1C.vx;
                env->camera->points[3].vy = work->camPos1C.vy;
                env->camera->points[3].vz = work->camPos1C.vz;
                env->camera->points[3].vx += 0x1800;
                env->camera->points[3].vx -= 0x1000;
                ex->btlfx_startScreenFade(1, 1);
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            approach = (frame << 12) / 26;
            dist = (0x1000 - approach) * 3 / 2;
            pos.vz = work->camPos1C.vx + dist;
            pos.vy = work->camPos1C.vy - dist;
            rot.vx = 0x200;
            pos.vx = 0;
            rot.vy = 0;
            rot.vz = -frame << 6;
            level = 0x2000 - ex->math_sin(approach >> 2) * 2;
            scale.vx = level;
            scale.vy = level;
            scale.vz = level;
            work->pos14.vx = pos.vx;
            work->pos14.vy = pos.vy;
            work->pos14.vz = pos.vz;
            if (frame < 0x19) {
                env->camera->points[3].vx = pos.vx;
                env->camera->points[3].vy = pos.vy;
                env->camera->points[3].vz = pos.vz;
            }
            if (frame == 7) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1B), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x5C;
                }
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, level >> 1, 0, 1, 0x1000);
            env->otZ = 0;
            if (frame >= 13) {
                bright = ((ex->math_sin((((frame - 12) << 12) / 12) >> 2) << 6) >> 12) + 0x20;
                gridCol.g = bright >> 1;
                gridCol.r = bright;
                gridCol.b = bright >> 2;
                scale2.vx = 0x1200;
                scale2.vy = 0x1200;
                rot3.vz = 0;
                rot3.vy = 0;
                rot3.vx = 0;
                scale2.vz = 0x1000;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x78, 0x46, 0x38, 4, 4, 2, 2, &rot3, &scale2, &gridCol, 1, NULL);
            }
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), 0, NULL, -1, 0x3000, 0, 1,
                                    fx_08f_trail, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), (s32)&work->pos14, NULL, 0,
                                ex->math_cos(approach >> 2) * 2 + 0x1000, ex->math_cos(approach >> 2) / 2 + 0x800, 1, fx_08f_trail, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), work->frame4);
            if (frame >= 9) {
                spriteRot.vx = 0;
                spriteRot.vy = 0;
                spriteRot.vz = (frame << 5) + 0x7E8;
                spriteRot.pad = 0;
                spinAlpha = ex->math_cos((frame - 8) << 6);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->pos14, &spriteRot, 0x3000,
                                      frame - 12, 1, spinAlpha);
            }
            if (frame >= 13) {
                spriteFrame = frame - 12;
                spriteAlpha = ex->math_cos((spriteFrame << 10) / 12);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->pos14, NULL, 0x3000, spriteFrame,
                                      1, spriteAlpha);
            }
            if (frame >= 0x18) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            t = (frame << 12) / 6;
            work->pos14.vz -= 0x40;
            work->pos14.vy += 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), (s32)&work->pos14, NULL, 0,
                                ex->math_cos(t >> 2), (0x1000 - t) / 2, 1, fx_08f_trail, 0);
            if (frame >= 6) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        {
            /* dead struct initializer: only a CLOBBER of q, which never gets a register; its spill slot is the
               original's otherwise unused frame word after the fadeIn / near slots (frame 0xD0) */
            struct { s32 spriteX; } q = { frame };
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08f/nonmatchings/fx_08f", fx_08f_headerPtr);
