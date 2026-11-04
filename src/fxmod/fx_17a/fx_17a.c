#include "fxmod/fx_17a.h"

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 radius, s32 bright, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr) {
    Unk800F78A4 *env = fx_17a_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    MATRIX m;
    SVECTOR quad[4];
    s32 otz;
    POLY_FT4 *prim;
    s32 ang;
    s32 ang1;
    s32 z0;
    s32 z1;
    s32 row;
    s32 tileV;
    s32 j;
    s32 tileU;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_FT4 *)env->prim;
    env->prim = (u8 *)(prim + 320);
    col.cd = abr == 0xFF ? 0x2C : 0x2E;
    col.r = (rgb->r * bright) >> 12;
    col.g = (rgb->g * bright) >> 12;
    col.b = (rgb->b * bright) >> 12;
    tpage |= TPAGE_ABR(abr);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    z1 = 0x866;
    z0 = 0;
    for (row = 0; row < 20; row++) {
        ang += dang;
        tileV = row & 0xF;
        for (j = 0; j < 16; j++) {
            quad[0].vx = (ex->math_cos(ang) * radius) >> 12;
            quad[0].vy = (ex->math_sin(ang) * radius) >> 12;
            ang1 = ang + 0x100;
            quad[1].vx = (ex->math_cos(ang1) * radius) >> 12;
            quad[1].vy = (ex->math_sin(ang1) * radius) >> 12;
            quad[0].vz = quad[1].vz = z0;
            ang += dang;
            quad[2].vx = (ex->math_cos(ang) * radius) >> 12;
            quad[2].vy = (ex->math_sin(ang) * radius) >> 12;
            ang1 = ang + 0x100;
            quad[3].vx = (ex->math_cos(ang1) * radius) >> 12;
            quad[3].vy = (ex->math_sin(ang1) * radius) >> 12;
            quad[2].vz = quad[3].vz = z1;
            ang -= dang;
            gte_ldv3c(quad);
            gte_rtpt();
            tileU = j & 3;
            /* two statements each: a twice-set u0/v0 is not loop-invariant, so v + rv * 8 stays in the loop */
            u0 = u;
            u0 += tileU * 32;
            v0 = v;
            v0 += tileV * 8;
            u1 = u0 + 0x20;
            if (tileU == 3) {
                u1 = u0 + 0x1F;
            }
            v1 = v0 + 8;
            if (tileV == 0xF) {
                v1 = v0 + 7;
            }
            gte_nclip();
            *(s32 *)&prim->r0 = *(s32 *)&col;
            gte_stopz(&otz);
            if (otz >= 0) {
                prim->u0 = prim->u2 = u0;
                prim->u1 = prim->u3 = u1;
                prim->v0 = prim->v1 = v0;
                prim->v2 = prim->v3 = v1;
                prim->tpage = tpage;
                prim->clut = clut;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                otz = 0xFFF;
                gte_ldv0(&quad[3]);
                gte_rtps();
                ex->btlfx_addPrim(0x09000000, otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
            ang += 0x100;
        }
        z1 += 0x866;
        z0 += 0x866;
    }
}

static s32 fx_17a_op80PoolTask(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk2 = 0;
        p->unk14.vx = ex->btlfx_rand();
        p->unk14.vy = ex->btlfx_rand();
        p->unk14.vz = 0;
        p->unk1C = ex->btlfx_rand() % 128 + 0x80;
        p->unk1E = 0xC0 - ex->btlfx_rand() % 32;
        p->unk24 = 0;
        p->unk20 = 0;
    } else if (p->state0 == FX_17A_PART_SPARK) {
        p->posC.vx += -0x80 - p->speedA;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_GLOW), &p->posC, NULL, p->scale4 * 3 >> 1, 0, 1, -1, 0, 0);
        if (frame >= 0x11) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17a/nonmatchings/fx_17a", fx_17a_headerPtr);

/* Matching notes: the variable split is what global-alloc needs. The spawn loop's sparkAng/dist are fresh block locals
 * (dist is set once, so sched1 launches its +0x180 next to the call; both are local-alloc'd), the spawn counter
 * spawn, the trail ring counter trail and case 3's phase are their own variables, the case 3 model loop subtracts
 * ringScale * 0x30 (a loop giv, not a counter variable), and the tint values reuse tint (a global pseudo, so the 0xFF
 * constant isn't tied to the result). */
s32 fx_17a_op80(s32 mode, Fx17aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    Fx17aPart *spark;
    s32 frame;
    s32 t;
    s32 glow;
    s32 gridScale;
    s32 level;
    s32 i;
    s32 j;
    s32 glowAng;
    s32 ringSize;
    s32 ringScale;
    s32 sparkBright;
    s32 k;
    s32 rise;
    s32 lineFrame;
    s32 endSize;
    s32 introSize;
    s32 blade;
    s32 spinAng;
    s32 pulseAng;
    s32 cylAng;
    s32 tint;
    s32 phase;
    s32 trail;
    s32 spawn;

    if (mode == 0) {
        init->workSize = 0xB4;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_17a_op80PoolTask;
        init->poolCount = 0x30;
    } else if (mode == 1) {
        work->state0 = 2;
        work->frame4 = 0;
        fx_17a_env = env;
        ex->btlfx_initEffModels(fx_17a_effModels, 6);
        work->model80 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MADEEN_RAINBOW_DOME), 0xBA, 0x3FC0, 0, 0);
        work->model84 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MADEEN_RAINBOW_ARCH), 0xBA, 0x3EC0, 0, 0x80);
        work->model88 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WINGS), 0x3A, 0x3E81, 0, 0x80);
        work->model8C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MADEEN_INFINITY_LOOP), 0x138, 0xFFFF, 0, 0x80);
        BTLFX_COPY_SVEC(&work->posC, &fx_17a_centerPos);
        work->rot4C.vx = 0;
        work->rot4C.vy = 0;
        work->rot4C.vz = 0;
        work->pos34.vx = work->posC.vx;
        work->pos34.vy = work->posC.vy;
        work->pos34.vz = work->posC.vz;
        work->pos14.vx = work->posC.vx;
        work->pos14.vy = work->posC.vy;
        work->pos14.vz = work->posC.vz - 0x2555;
        work->pos1C.vx = work->posC.vx;
        work->pos1C.vy = work->posC.vy;
        work->pos1C.vz = work->posC.vz - 0x1C00;
    } else {
        glow = 0;
        gridScale = 0;
        frame = entry->frame;
        {
        switch (work->state0) {
        case 2:
            env->camera->points[3].vx = work->posC.vx;
            env->camera->points[3].vy = work->posC.vy;
            env->camera->points[3].vz = work->posC.vz;
            work->rot4C.vx += 0x20;
            work->rot4C.vy += 0x10;
            t = (frame << 12) / 18;
            work->scale40.vx = work->scale40.vy = work->scale40.vz = (0x1000 - t) * 2;
            glow = (t * 0xAA) >> 12;
            ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model80);
            ex->Hi_ModifyEffModelRGB(work->model80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->rot4C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model80);
            ex->Hi_ModifyEffModelRGB(work->model80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->rot4C.vz -= 0x400;
            env->otZ = 0x100;
            introSize = ex->math_sin(t >> 2) * 2;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &work->posC, NULL, introSize, frame, 1, -1);
            env->otZ = 0;
            if (frame >= 0x12) {
                work->state0 = 3;
                entry->frame = -1;
                for (blade = 0; blade < 4; blade++) {
                    work->rot90[blade].vx = 0x800 - blade * 0x180;
                    work->rot90[blade].vy = blade * 0xC0;
                    work->rot90[blade].vz = blade * 0x60;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_STREAKS), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        case 3: {
            VECTOR sc;

            glow = 0xAA;
            work->rot4C.vx += 0x20;
            work->rot4C.vy += 0x10;
            t = (frame << 12) / 24;
            gridScale = (t >> 4) + 0x80;
            if (frame < 0xC) {
                ringScale = 0x6AA;
            } else if (frame < 0x14) {
                ringScale = 0x6AA - ((frame - 0xC) * 0x6AA) / 8;
            } else {
                ringScale = 0;
            }
            if (ringScale != 0) {
                work->scale40.vx = work->scale40.vy = work->scale40.vz = ringScale;
                ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model80);
                work->rot4C.vz += 0x400;
                ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model80);
                work->rot4C.vz -= 0x400;
            }
            ringScale = ringScale * 5 + 0x1000;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &work->posC, NULL, ringScale, frame, 1, -1);
            if ((u32)(frame - 0xF) < 6) {
                ringScale = 0x1000 - (((frame - 0xF) << 12) / 6);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_RING), &work->pos14, NULL, ringScale, frame, 1, 0x1000);
            }
            if (frame >= 0xC) {
                phase = ((frame - 0xC) << 10) / 12;
                ringScale = ex->math_sin(phase) * 2 + 0x2000;
                i = ex->math_sin(phase);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_VIOLET_RING), &work->pos14, NULL, ringScale, 0, 1, i);
            }
            if (frame >= 0xB) {
                rise = ((frame - 0xA) << 12) / 14;
                glowAng = rise >> 2;
                sc.vx = sc.vy = sc.vz = (rise * 0xB) / 12;
                for (ringScale = 0; ringScale < 4; ringScale++) {
                    work->rot90[ringScale].vz = (s16)(work->rot90[ringScale].vz - 0x18) - ringScale * 0x30;
                    ex->Hi_DrawEffModel(&work->rot90[ringScale], (VECTOR *)&work->pos34, (s32)&sc, work->model84);
                    sc.vx = sc.vy = sc.vz = (sc.vx * 0x12) >> 4;
                    ex->Hi_ModifyEffModelRGB(work->model84, ((ex->math_sin(glowAng) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(glowAng) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(glowAng) * 2) / 3 >> 5) - 0x80);
                }
            }
            if (frame == 0xC) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_FAN_EMITTER), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            i = (ex->math_sin(t >> 2) * 0x46) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i >> 2, i >> 1, i);
            if (frame >= 0x15) {
                i = ((frame - 0x14) * 0xFF) / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i, i, i);
            }
            if (frame >= 0x11) {
                lineFrame = frame - 0x14;
                i = ex->math_sin(lineFrame << 8);
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_GLOW_LINE_2), &work->posC, lineFrame, i);
            }
            if (frame >= 0x18) {
                work->state0 = 4;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos24, &fx_17a_centerPos);
                work->pos24.vy += 0x400;
                env->camera->points[3].vx = work->pos24.vx;
                env->camera->points[3].vy = work->pos24.vy;
                env->camera->points[3].vz = work->pos24.vz;
                work->pos34.vx = fx_17a_centerPos.vx;
                work->pos34.vy = fx_17a_centerPos.vy;
                work->pos34.vz = fx_17a_centerPos.vz;
                BTLFX_COPY_SVEC(&work->posC, &fx_17a_centerPos);
                BTLFX_COPY_SVEC(&work->pos14, &fx_17a_centerPos);
                emitter = work->emitB0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_FAN_EMITTER), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        }
        case 4:
            BTLFX_COPY_SVEC(&work->pos24, &fx_17a_centerPos);
            t = (frame << 12) / 10;
            work->pos24.vy += (0x1000 - t) >> 2;
            env->camera->points[3].vx = work->pos24.vx;
            glow = 0xF0;
            env->camera->points[3].vy = work->pos24.vy;
            gridScale = 0x300;
            env->camera->points[3].vz = work->pos24.vz;
            work->rot4C.vx += 0x20;
            work->rot4C.vy += 0x10;
            if (frame < 0x10) {
                s32 bright;

                level = (frame << 12) >> 4;
                ringSize = ex->math_sin((frame << 12) >> 6) >> 1;
                work->scale40.vx = work->scale40.vy = work->scale40.vz = ringSize;
                ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model80);
                bright = ((0x1000 - level) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model80, bright, bright, bright);
            }
            env->otZ = 0x100;
            level = ex->math_cos(t >> 2);
            ringSize = ex->math_sin(t >> 2) + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &work->posC, NULL, ringSize, frame, 1, -1);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_RING), &work->posC, NULL, ringSize, frame, 1, level);
            env->otZ = 0;
            if (frame < 6) {
                tint = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
            }
            if (frame >= 7) {
                tint = (frame - 7) * 0x55;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, tint, tint, tint);
            }
            if (frame >= 0xA) {
                work->state0 = 5;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emitB0);
                work->rot6C.vy = 0x400;
                work->rot4C.vy = 0x400;
                work->rot6C.vx = 0;
                work->rot6C.vz = 0;
                work->scale60.vx = 0x1000;
                work->scale60.vy = 0x1000;
                work->scale60.vz = 0x1000;
                work->rot4C.vx = 0;
                work->rot4C.vz = 0;
                work->scale40.vx = 0x333;
                work->scale40.vy = 0x333;
                work->scale40.vz = 0x333;
                work->pos34.vx = work->posC.vx;
                work->pos34.vy = work->posC.vy;
                work->pos34.vz = work->posC.vz;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_STREAKS), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_SPARKLES), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0xFF);
            }
            break;
        case 5: {
            SVECTOR dir;
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;
            SVECTOR ang;
            SVECTOR at;
            SVECTOR ang2;
            SVECTOR at2;
            CVECTOR col2;
            SVECTOR rot2;
            VECTOR sc2;
            s32 frameQ12;
            s32 trailRadius;
            s32 trailScale;

            frameQ12 = frame << 12;
            env->otZ = 0x100;
            t = frameQ12 / 51;
            pulseAng = t >> 2;
            ringScale = ex->math_cos(pulseAng);
            ringSize = ex->math_sin(pulseAng) + 0xC00;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &work->posC, NULL, ringSize, frame, 1, 0xB86);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &work->posC, NULL, ringSize / 2, frame, 1, -1);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_TEAL_RING), &work->posC, NULL, (ringSize * 2) / 3, frame, 1, ringScale);
            ringSize = ex->math_sin(pulseAng) * 2 + 0x3000;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_VIOLET_RING), &work->posC, NULL, ringSize, 0, 1, -1);
            env->otZ = 0;
            if (frame < 0x20) {
                spinAng = frameQ12 >> 7;
                ringSize = (ex->math_sin(spinAng) * 3) >> 1;
                i = ex->math_cos(spinAng);
                dir.vx = 0x200;
                dir.vy = 0x280;
                dir.vz = work->frame4 * 32;
                dir.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_TEXTURED_RING), &work->posC, &dir, ringSize, 0, 1, i);
                dir.vy += 0x800;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_TEXTURED_RING), &work->posC, &dir, ringSize, 0, 1, i);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 0xFF, 0xA0, 0, 0xFF);
            } else if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 6) {
                i = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i >> 1, i >> 1, i);
            }
            work->rot6C.vz += 0x18;
            ex->btlfx_sampleGradient(fx_17a_cylColors, &col, frame);
            cylAng = t >> 2;
            fxm_drawTexCylinder(&work->posC, &work->rot6C, (VECTOR *)&work->scale60, 0x1C00, 0x1800, &col, 0x17, 0x3E80, 0, 0,
                          ((ex->math_sin(cylAng) * 0x58) >> 12) + 0x40, 0xFF);
            ex->btlfx_drawScrollMove(work->frame4 * 7, 0x1C0, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            if (frame < 0x19) {
                work->rot4C.vz += 0xC;
                level = 0x1000 - ((frame << 12) / 25);
                ex->Hi_DrawEffModel(&work->rot4C, (VECTOR *)&work->pos34, (s32)&work->scale40, work->model88);
                ex->Hi_ModifyEffModelRGB(work->model88, ((ex->math_sin(level >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(level >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(level >> 2) - 0x800) >> 5) - 0x80);
            }
            rot.vx = 0x980;
            rot.vy = 0x400;
            rot.vz = 0;
            sc.vx = sc.vy = sc.vz = ex->math_sin(cylAng) + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->pos34, (s32)&sc, work->model8C);
            ex->Hi_ModifyEffModelRGB(work->model8C, (((ex->math_cos(cylAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(cylAng) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(cylAng) >> 1) + 0x800) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_MADEEN_BACKDROP), work->frame4);
            ex->btlfx_drawScrollMove(work->frame4 * 10, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
            sparkBright = (ex->math_cos(cylAng) >> 1) + 0x800;
            if (frame < 0x18) {
                level = ((ex->math_sin((frame << 10) / 24) * 0x76C) >> 12) + 0x200;
            } else {
                level = 0x96C;
            }
            ang.vx = (ex->math_sin(t >> 2) * 2) / 3;
            ang.vy = work->frame4 << 3;
            ang.vz = 0;
            for (i = 0, spinAng = work->frame4 * 0xC; i < 0x80; i++, spinAng += 0x20) {
                ex->btlfx_offsetPoint(&work->posC, (s32)&at, 0, (XY32 *)&ang, level, spinAng);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_GLOW), &at, NULL, 0x2800, 0, 1, sparkBright, 0,
                                        0);
                ang.vx += 0x40;
                ang.vy -= 0x80;
            }
            trailScale = (ex->math_cos(t >> 2) >> 1) + 0x1000;
            if (frame < 0x18) {
                trailRadius = (ex->math_sin((frame << 10) / 24) >> 2) + 0x6A4;
            } else {
                trailRadius = 0xAA4;
            }
            for (trail = 0; trail < 2; trail++) {
                if (frame == 0) {
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_TRAIL), (s32)&work->posC,
                                        (s32 *)&work->posC, -1, trailScale, ex->math_cos(t >> 2), 1, (u8 *)&fx_17a_trails[trail], 1);
                }
                {
                    SVECTOR *pp = fx_17a_trails[trail].points4;

                    for (k = 0; k < 0x40; k++) {
                        pp->vy -= 0x40;
                        pp++;
                    }
                }
            }
            ang2.vx = ex->math_sin(work->frame4 << 7) >> 4;
            ang2.vy = work->frame4 * 0x52 - 0x180;
            ang2.vz = 0;
            for (k = 0, j = work->frame4 * 0x5C; k < 2; k++, j += 0x800) {
                ex->btlfx_offsetPoint(&work->posC, (s32)&at2, 0, (XY32 *)&ang2, trailRadius, j);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_TRAIL), (s32)&at2, (s32 *)&work->posC, 0, trailScale,
                                    (ex->math_sin(t >> 1) >> 1) + 0x60E, 1, (u8 *)&fx_17a_trails[k], 1);
            }
            if (frame < 0x18) {
                k = ex->math_cos((frame << 10) / 24);
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_GLOW_LINE), &work->posC, frame, k);
            }
            if (frame >= 8) {
                for (spawn = 0; spawn < 3; spawn++) {
                    spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (spark != NULL) {
                        s32 dist;
                        s32 sparkAng;
                        spark->state0 = FX_17A_PART_SPARK;
                        BTLFX_COPY_SVEC(&spark->posC, &work->posC);
                        sparkAng = ex->btlfx_rand();
                        dist = (ex->btlfx_rand() & 0x1FF) + 0x180;
                        spark->posC.vx = (s16)(spark->posC.vx - 0x100) + ex->btlfx_randCentered(0x200);
                        spark->posC.vy += (ex->math_cos(sparkAng) * dist) >> 12;
                        spark->posC.vz += (ex->math_sin(sparkAng) * dist) >> 12;
                        spark->speedA = ex->btlfx_rand() % 70;
                        spark->scale4 = (ex->btlfx_rand() & 0xFFF) + 0x1000;
                    }
                }
            }
            if (frame == 0x2B) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_FAN_EMITTER), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0x2C) {
                rise = (frame - 0x2B) << 12;
                k = rise >> 7;
                if (k >= 0x100) {
                    k = 0xFF;
                }
                rot2.vz = 0x20;
                sc2.vy = sc2.vx = (rise >> 5) + 0x1000;
                sc2.vz = 0x1000;
                col2.r = col2.g = col2.b = k;
                rot2.vx = 0;
                rot2.vy = 0;
                ex->btlfx_drawScreenGrid(2, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot2, &sc2, &col2, 1, 0);
            }
            if (frame >= 0x33) {
                work->state0 = 6;
                entry->frame = -1;
                ex->btlfx_poolClear(entry->pool);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MADEEN_BLUE_SPARKLES), (s32)&fx_17a_centerPos);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        }
        case 6:
            BTLFX_COPY_SVEC(&work->pos24, &fx_17a_centerPos);
            env->camera->points[3].vx = work->pos24.vx;
            env->camera->points[3].vy = work->pos24.vy;
            t = frame << 8;
            env->camera->points[3].vz = work->pos24.vz;
            env->otZ = 0x180;
            endSize = ex->math_cos(t >> 2) + 0x400;
            level = ex->math_cos(t >> 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_MADEEN_WHITE_RING), &fx_17a_centerPos, NULL, endSize, 0, 1, level);
            env->otZ = 0;
            level = (ex->math_cos(t >> 2) * 0xFF) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, level, level, level);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (glow != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            col.r = col.g = col.b = glow;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vy = sc.vx = gridScale + 0x1020;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(0x200, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
        }
        work->frame4++;
    }
    return 0;
}
