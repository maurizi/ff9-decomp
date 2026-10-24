#include "fxmod/fx_1bf_2.h"

s32 func_801E7718(s32 mode, Fx1bf2Obj *w, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1bf2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 a;
    s32 c;

    if (mode == 1) {
        w->unk0 = 0;
    } else {
        a = (frame << 7) >> 2;
        c = (0x1000 - ex->math_cos(a)) * D_801E8FCC;
        pos.vx = w->unk4;
        pos.vy = w->unk6;
        pos.vz = w->unk8;
        pos.vx -= c >> 12;
        scale.vx = scale.vy = scale.vz = ((0x1080 - ex->math_sin(a)) * w->unk2) >> 12;
        ex->Hi_DrawEffModel(&w->unkC, &pos, (s32)&scale, inst->unk78);
        ex->Hi_ModifyEffModelRGB(inst->unk78, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                 (ex->math_sin(a) >> 5) - 0x80);
        if (frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_2/nonmatchings/fx_1bf_2", fx_1bf_2_headerPtr);

s32 fx_1bf_2_op80(s32 mode, Fx1bf2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    /* function-scope temporaries for the matrix restores: multiply set, so sched1 does not keep each load next to
     * its store (the original hoists the first restore's loads above the save) */
    s32 r0, r1, r2, r3;
    s32 w0, w1, w2, w3;
    VECTOR sc;
    CVECTOR cv;
    SVECTOR sv;
    VECTOR vv;
    CVECTOR cv2;
    SVECTOR sv2;
    VECTOR vv2;
    VECTOR vv3;
    Fx1bf2Obj *o;
    Fx1bf2Obj *o2;
    Pool6CEntry *p;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 t;
    s32 h2;
    s32 g2;
    s32 h3;
    s32 g3;
    s32 c;
    s32 v;

    if (mode == 0) {
        init->unk0 = 0xBC;
        init->unk8 = 0x14;
        init->unk14 = (s32)func_801E7718;
        init->unk4 = 0x10;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8A44 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801E8A54, 7);
        work->unk64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xd), 0);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xf), 1);
        work->unk6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x5), 1);
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1), 1);
        work->unk74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2), 1);
        work->unk78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x10), 1);
        BTLFX_COPY_SVEC(&work->unk8, &D_801E89F4);
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk3C.vx = work->unk3C.vz = work->unk3C.vy = 0x1000;
        work->unk28.vx = work->unk8.vx;
        work->unk28.vy = work->unk8.vy;
        work->unk28.vz = work->unk8.vz;
        work->unk20.vx = D_801E89F4.vx;
        work->unk20.vy = D_801E89F4.vy;
        work->unk20.vz = D_801E89F4.vz;
        env->camera->points[2].vx = work->unk20.vx;
        env->camera->points[2].vy = work->unk20.vy;
        env->camera->points[2].vz = work->unk20.vz;
        work->unk5C.vx = 0;
        work->unk5C.vy = 0x400;
        work->unk5C.vz = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            n1 = (((0x1000 - ex->math_sin(t >> 3)) << 6) >> 12) - 0x2C;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xb), n1, 0x78, 0, 0xFF, t);
            work->unk20.vx -= 0x2E;
            env->camera->points[2].vx = work->unk20.vx;
            env->camera->points[2].vy = work->unk20.vy;
            env->camera->points[2].vz = work->unk20.vz;
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk5C, -0x968, (s16 *)&work->unk54);
                work->unk48.vx = work->unk54.vx;
                work->unk48.vy = work->unk54.vy;
                work->unk48.vz = work->unk54.vz;
            }
            break;
        case 1:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, ((t - 0x800) >> 5) - 0x80, ((t - 0x800) >> 5) - 0x80,
                                     ((t - 0x800) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            work->unk20.vx -= (ex->math_cos(t >> 2) * 0x2E) >> 12;
            env->camera->points[2].vx = work->unk20.vx;
            env->camera->points[2].vy = work->unk20.vy;
            env->camera->points[2].vz = work->unk20.vz;
            D_801E8FCC = 0x400;
            if (frame % 3 == 0) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = 0x400;
                    o->unkC.vy = 0x400;
                    o->unkC.vz = 0;
                    o->unk4 = work->unk48.vx;
                    o->unk6 = work->unk48.vy;
                    o->unk8 = work->unk48.vz;
                    o->unk2 = 0x1000;
                }
            }
            env->unk28 = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk54, NULL, t >> 1, 0, 1, t);
            env->unk28 = 0;
            v = (((0x1000 - ex->math_sin((t >> 3) + 0x200)) << 6) >> 12) - 0x2C;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xb), v, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x20) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            h2 = t / 2;
            ex->Hi_ModifyEffModelRGB(work->unk68, ((0x800 - h2) >> 5) - 0x80, ((0x800 - h2) >> 5) - 0x80,
                                     ((0x800 - h2) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            g2 = ((0x1000 - h2) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, g2, g2, g2);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            sc.vx = sc.vy = (0x1000 - ex->math_sin(t >> 2)) / 3 + 0xF5;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk70);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk74);
            D_801E8FCC = (t >> 1) + 0x400;
            if (frame % 3 == 0) {
                o2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o2 != NULL) {
                    o2->unkC.vx = 0x400;
                    o2->unkC.vy = 0x400;
                    o2->unkC.vz = 0;
                    o2->unk4 = work->unk48.vx;
                    o2->unk6 = work->unk48.vy;
                    o2->unk8 = work->unk48.vz;
                    o2->unk2 = 0x1000;
                }
            }
            if (frame == 1) {
                work->unk10.vx = work->unk54.vx;
                work->unk10.vy = work->unk54.vy;
                work->unk10.vz = work->unk54.vz;
                work->unk18.vx = work->unk54.vx;
                work->unk18.vy = work->unk54.vy;
                work->unk18.vz = work->unk54.vz;
                work->unk18.vx -= 0xC00;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x7), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk2C = (s32)&work->unk18;
                    p->unk22 = 0x80;
                }
            }
            cv.r = cv.g = cv.b = 0x84;
            vv.vy = vv.vx = 0x1020;
            sv.vx = 0;
            sv.vy = 0;
            sv.vz = 0;
            vv.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv, &vv, &cv, 0, NULL);
            env->unk28 = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk54, NULL, (t >> 1) + 0x800, 0, 1,
                              ex->math_cos(t >> 2));
            env->unk28 = 0;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xb), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            h3 = t / 2;
            ex->Hi_ModifyEffModelRGB(work->unk68, (-h3 >> 5) - 0x80, (-h3 >> 5) - 0x80, (-h3 >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk6C);
            g3 = ((0x800 - h3) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, g3, g3, g3);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            n2 = t >> 2;
            sc.vx = sc.vy = ((0x1000 - ex->math_sin(n2)) * 0xF5) >> 12;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk70);
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk48, (s32)&sc, work->unk74);
            D_801E8FCC = n2 + 0xC00;
            if (!(frame & 3) && frame < 0x18) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = 0x400;
                    o->unkC.vy = 0x400;
                    o->unkC.vz = 0;
                    o->unk4 = work->unk48.vx;
                    o->unk6 = work->unk48.vy;
                    o->unk8 = work->unk48.vz;
                    o->unk2 = 0x1000 - t;
                }
            }
            if (frame == 0x16) {
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->viewMatrix)[0];
                    q1 = ((s32 *)env->viewMatrix)[1];
                    q2 = ((s32 *)env->viewMatrix)[2];
                    q3 = ((s32 *)env->viewMatrix)[3];
                    ((s32 *)&work->unk7C)[0] = q0;
                    ((s32 *)&work->unk7C)[1] = q1;
                    ((s32 *)&work->unk7C)[2] = q2;
                    ((s32 *)&work->unk7C)[3] = q3;
                }
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->viewMatrix)[4];
                    q1 = ((s32 *)env->viewMatrix)[5];
                    q2 = ((s32 *)env->viewMatrix)[6];
                    q3 = ((s32 *)env->viewMatrix)[7];
                    ((s32 *)&work->unk7C)[4] = q0;
                    ((s32 *)&work->unk7C)[5] = q1;
                    ((s32 *)&work->unk7C)[6] = q2;
                    ((s32 *)&work->unk7C)[7] = q3;
                }
            }
            cv2.r = cv2.g = cv2.b = 0x84;
            vv2.vy = vv2.vx = 0x1020;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv2.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv2, &cv2, 0, NULL);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xb), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk20.vx = work->unk8.vx;
                work->unk20.vy = work->unk8.vy;
                work->unk20.vz = work->unk8.vz;
                work->unk20.vy -= 0x200;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            break;
        case 4:
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->viewMatrix)[0];
                q1 = ((s32 *)env->viewMatrix)[1];
                q2 = ((s32 *)env->viewMatrix)[2];
                q3 = ((s32 *)env->viewMatrix)[3];
                ((s32 *)&work->unk9C)[0] = q0;
                ((s32 *)&work->unk9C)[1] = q1;
                ((s32 *)&work->unk9C)[2] = q2;
                ((s32 *)&work->unk9C)[3] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->viewMatrix)[4];
                q1 = ((s32 *)env->viewMatrix)[5];
                q2 = ((s32 *)env->viewMatrix)[6];
                q3 = ((s32 *)env->viewMatrix)[7];
                ((s32 *)&work->unk9C)[4] = q0;
                ((s32 *)&work->unk9C)[5] = q1;
                ((s32 *)&work->unk9C)[6] = q2;
                ((s32 *)&work->unk9C)[7] = q3;
            }
            r0 = ((s32 *)&work->unk7C)[0];
            r1 = ((s32 *)&work->unk7C)[1];
            r2 = ((s32 *)&work->unk7C)[2];
            r3 = ((s32 *)&work->unk7C)[3];
            ((s32 *)env->viewMatrix)[0] = r0;
            ((s32 *)env->viewMatrix)[1] = r1;
            ((s32 *)env->viewMatrix)[2] = r2;
            ((s32 *)env->viewMatrix)[3] = r3;
            r0 = ((s32 *)&work->unk7C)[4];
            r1 = ((s32 *)&work->unk7C)[5];
            r2 = ((s32 *)&work->unk7C)[6];
            r3 = ((s32 *)&work->unk7C)[7];
            ((s32 *)env->viewMatrix)[4] = r0;
            ((s32 *)env->viewMatrix)[5] = r1;
            ((s32 *)env->viewMatrix)[6] = r2;
            ((s32 *)env->viewMatrix)[7] = r3;
            t = (frame << 12) / 20;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk64);
            ex->Hi_ModifyEffModelAbr(work->unk64, 1);
            c = 0x1000 - t;
            ex->Hi_ModifyEffModelRGB(work->unk64, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            w0 = ((s32 *)&work->unk9C)[0];
            w1 = ((s32 *)&work->unk9C)[1];
            w2 = ((s32 *)&work->unk9C)[2];
            w3 = ((s32 *)&work->unk9C)[3];
            ((s32 *)env->viewMatrix)[0] = w0;
            ((s32 *)env->viewMatrix)[1] = w1;
            ((s32 *)env->viewMatrix)[2] = w2;
            ((s32 *)env->viewMatrix)[3] = w3;
            w0 = ((s32 *)&work->unk9C)[4];
            w1 = ((s32 *)&work->unk9C)[5];
            w2 = ((s32 *)&work->unk9C)[6];
            w3 = ((s32 *)&work->unk9C)[7];
            ((s32 *)env->viewMatrix)[4] = w0;
            ((s32 *)env->viewMatrix)[5] = w1;
            ((s32 *)env->viewMatrix)[6] = w2;
            ((s32 *)env->viewMatrix)[7] = w3;
            cv2.r = cv2.g = cv2.b = 0x84;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv3.vy = vv3.vx = 0x1020;
            vv3.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv3, &cv2, 0, NULL);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xb), -0x2C, 0x78, 0, 1, c);
            if (frame >= 0x14) {
                ex->btlfx_poolClear(entry->unk10);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(work->unk4 * 4, 0x2C0, 0x100, 0x20, 0x80, 0x2E0, 0x100);
        ex->btlfx_drawScrollMove(0x2710 - work->unk4 * 4, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x3), work->unk4);
        work->unk4++;
    }
    return 0;
}
