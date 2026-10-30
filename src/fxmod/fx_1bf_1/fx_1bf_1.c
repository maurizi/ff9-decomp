#include "fxmod/fx_1bf_1.h"

s32 func_801EC704(SVECTOR *v, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    s32 a = ex->btlfx_rand();

    v->pad = 0;
    v->vy = 0;
    v->vx = (ex->math_cos(a) * D_801EFDD8) >> 12;
    v->vz = (ex->math_sin(a) * D_801EFDD8) >> 12;
    rot->vy = a;
    return 0;
}

s32 func_801EC7C8(s32 mode, Fx1bf1Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1bf1RockInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 t;
    s32 s;

    if (mode == 1) {
        o->unk2 = 0;
        o->unk1C = ex->btlfx_rand() % 64;
        o->unk1E = ex->btlfx_rand() % 32;
    } else {
        switch (o->unk0) {
        case 0:
            switch (o->unk2) {
            case 0:
                pos.vx = o->unkC.vx;
                pos.vy = o->unkC.vy;
                pos.vz = o->unkC.vz;
                pos.vx += ex->btlfx_randCentered(frame + 0x4D);
                pos.vy += ex->btlfx_randCentered(frame * 2 + 0x4D);
                s = o->unkA;
                scale.vx = scale.vz = s;
                scale.vy = s * 3;
                ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
                ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
                if (D_801EF25C < o->unk22) {
                    o->unk2 = 1;
                    ((s16 *)o)[-1] = -1;
                }
                break;
            case 1:
                t = (frame << 12) / 24;
                if (o->unkC.vx > 0) {
                    o->unk1C -= (0xE10 - o->unkC.vz) / 300;
                } else {
                    o->unk1C += (o->unkC.vz + 0xE10) / 300;
                }
                o->unk14.vx += o->unk1C;
                o->unk14.vy += o->unk1E;
                o->unkC.vx += o->unk4;
                o->unkC.vy += o->unk6;
                o->unkC.vz += o->unk8;
                pos.vx = o->unkC.vx;
                pos.vy = o->unkC.vy;
                pos.vz = o->unkC.vz;
                s = ((0x1000 - ex->math_sin(t >> 2)) * o->unkA) >> 12;
                scale.vx = scale.vz = s;
                scale.vy = s * 3;
                ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
                ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
                if (frame >= 0x18) {
                    return 1;
                }
                break;
            }
            break;
        case 1:
            t = frame << 8;
            o->unk14.vx += o->unk1C;
            o->unk14.vy += o->unk1E;
            o->unkC.vx += o->unk4;
            o->unkC.vy += o->unk6;
            o->unkC.vz += o->unk8;
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            s = ((0x1000 - t) * o->unkA) >> 12;
            scale.vx = scale.vz = s;
            scale.vy = s * 3;
            ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
            ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
            if (frame >= 0x10) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_1bf_1_op80(s32 mode, Fx1bf1Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR rot;
    SVECTOR p60;
    s32 frame;
    s32 t;
    s32 b;
    s32 s9C;
    s32 c3;
    s32 m6;
    s32 y35;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 c4;
    s32 c5;
    s32 s;
    s32 q;
    s32 d;
    s32 e;
    s32 k;
    s32 i;
    s32 y;
    s32 a;
    Pool6CEntry *p;
    Fx1bf1Rock *o;
    s16 *rp;
    s32 sh1;
    s32 sh2;
    s32 y2;
    s32 x2;
    Fx1bf1Rock *o2;
    Fx1bf1Rock *o1;

    if (mode == 0) {
        arg4->workSize = 0xA4;
        arg4->elemSize = 0x24;
        arg4->elemFn = (s32)func_801EC7C8;
        arg4->poolCount = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EF2D8 = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(D_801EF2E8, 0xE);
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8A), 0);
        work->unk74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x88), 0);
        work->unk78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
        work->unk7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x91), 1);
        work->unk80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x89), 1);
        work->unk84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x90), 0);
        work->unk88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x92), 0);
        work->unk8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x93), 0);
        work->unk90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8F), 1);
        work->unk94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8C), 1);
        work->unk98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x94), 1);
        work->unk9C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        work->unkA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8E), 1);
        work->unk34.vx = 0;
        work->unk34.vy = 0x320;
        work->unk34.vz = 0;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        work->unk28[0] = D_801EF254.vx;
        work->unk28[1] = D_801EF254.vy;
        work->unk28[2] = D_801EF254.vz;
        work->unk44[1] = 0x1800;
        work->unk44[2] = 0x1800;
        work->unk44[0] = 0x1800;
        BTLFX_COPY_SVEC(&work->unk8, &D_801EF254);
        work->unk8.vy -= 0x200;
        work->unk18.vx = D_801EF254.vx;
        work->unk18.vy = D_801EF254.vy;
        work->unk18.vz = D_801EF254.vz;
        work->unk18.vy -= 0x300;
        env->camera->points[3].vx = work->unk18.vx;
        env->camera->points[3].vy = work->unk18.vy;
        env->camera->points[3].vz = work->unk18.vz;
    } else {
        b = 0;
        s9C = 0;
        c3 = 0;
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            t = frame << 8;
            if (frame < 16) {
                c0 = 0xFF - ((frame * 0xFF) >> 4);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c0, c0 * 2 / 3, c0 / 2);
            }
            work->unk18.vy -= 4 + ((ex->math_cos(t >> 2) * 10) >> 12);
            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            env->camera->points[3].vz = work->unk18.vz;
            b = 0x1000;
            if (frame >= 16) {
                work->unk0 = 1;
                entry->frame = -1;
                work->unk5C.vx = -0x400;
                work->unk5C.vy = 0x400;
                work->unk5C.vz = 0;
                work->unk50[0] = work->unk8.vx;
                work->unk50[1] = work->unk8.vy;
                work->unk50[2] = work->unk8.vz;
                BTLFX_COPY_SVEC(&D_801EFDE0, &work->unk8);
                BTLFX_COPY_SVEC(&D_801EFDE8, &work->unk8);
                D_801EFDE8.vy -= 0xDAC;
            }
            break;
        case 1: {
            VECTOR v;

            t = frame << 8;
            if (frame < 6) {
                d = (frame << 12) / 6;
                s = d >> 2;
                q = d;
            } else {
                d = ((frame - 6) << 12) / 10;
                s = 0xC00 - ((d << 11) >> 13);
                q = 0x1000;
                if (frame == 6) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0x80, 0x80);
                } else {
                    c1 = (d << 7) >> 12;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c1, c1, c1 * 2 / 3);
                }
                e = d >> 2;
                c3 = (d << 6) >> 12;
                sc.vx = sc.vz = ex->math_sin(e) * 2 + 0x3000;
                sc.vy = ex->math_sin(e) * 4;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                rot.vx = 0;
                rot.vy = d >> 3;
                rot.vz = 0;
                s9C = 0x300;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk7C);
                ex->Hi_ModifyEffModelRGB(work->unk7C, (ex->math_cos(e) >> 5) - 0x80, (ex->math_cos(e) >> 5) - 0x80,
                                         (ex->math_cos(e) >> 5) - 0x80);
                ex->btlfx_drawScrollMove(frame * 12, 0x340, 0x100, 0x10, 0x40, 0x350, 0x100);
            }
            v.vx = v.vy = s;
            v.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)work->unk50, (s32)&v, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, (q >> 5) - 0x80, (q >> 5) - 0x80, (q >> 5) - 0x80);
            if (frame == 6) {
                work->unk64 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk8);
                if (work->unk64 != NULL) {
                    work->unk64->otZ = 0x80;
                }
            }
            work->unk18.vy -= 3;
            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            b = 0x1001 - t;
            env->camera->points[3].vz = work->unk18.vz;
            if (frame >= 16) {
                work->unk0 = 2;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                work->unk10.vy -= 0x800;
                work->unk68 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk10);
                if (work->unk68 != NULL) {
                    work->unk68->otZ = 0x80;
                }
                work->unk6C = NULL;
            }
            break;
        }
        case 2: {
            VECTOR v;
            SVECTOR cam;
            SVECTOR cam2;
            s32 w;
            s32 m;
            s32 z;
            s32 g;
            s32 h;

            t = (frame << 12) / 70;
            if (frame < 5) {
                q = 0x800;
                m = (frame << 12) / 5;
                if (work->unk68 != NULL && work->unk68->state == -1) {
                    work->unk68 = NULL;
                }
                BTLFX_COPY_SVEC(&p60, &work->unk8);
                p60.vy -= ((0x1000 - m) << 11) >> 12;
                ex->btlfx_moveEmitterTo((s32)work->unk68, (s32)&p60);
                sc.vx = sc.vz = q * 4;
                rot.vx = 0;
                rot.vy = m >> 3;
                rot.vz = 0;
                pos.vx = p60.vx;
                pos.vy = p60.vy;
                pos.vz = p60.vz;
                sc.vy = (q * (m + 0x400)) >> 8;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk7C);
                ex->btlfx_drawScrollMove(frame * 12, 0x340, 0x100, 0x10, 0x40, 0x350, 0x100);
                if (frame == 1 || frame == 3) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x60, 0x10);
                }
            } else {
                w = frame - 5;
                m = (w << 12) / 65;
                q = ((0x1000 - m) * 0x1800) >> 12;
                if (frame < 21) {
                    g = 0x1000 - ex->math_cos((w << 10) >> 4);
                    pos.vx = work->unk8.vx;
                    pos.vy = work->unk8.vy;
                    pos.vz = work->unk8.vz;
                    h = g >> 2;
                    pos.vy -= h;
                    rot.vx = 0;
                    rot.vy = frame << 5;
                    rot.vz = 0;
                    sc.vx = sc.vz = (g * 5) >> 2;
                    sc.vy = 0x1000;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unkA0);
                    ex->Hi_ModifyEffModelRGB(work->unkA0, ((ex->math_cos(h) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(h) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(h) - 0x800) >> 5) - 0x80);
                }
                if (frame == 5) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk8);
                    if (p != NULL) {
                        p->otZ = 0x100;
                    }
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
                    if (p != NULL) {
                        p->otZ = 0x100;
                    }
                    BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                    work->unk10.vy -= 0x400;
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk10);
                    if (p != NULL) {
                        p->tail->unk0 = -1;
                        *(s32 *)&p->tail->unk3C = (s32)func_801EC704;
                        p->otZ = 0x100;
                    }
                } else if (frame < 21) {
                    c2 = 0x80 - ((w << 7) >> 4);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 16), 1, c2, c2, c2 >> 2);
                    c3 = c2 >> 1;
                    s9C = 0x400;
                }
            }
            v.vx = v.vy = q;
            v.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)work->unk50, (s32)&v, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk18.vy -= 2;
            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            b = ((0x1000 - t) >> 1) + 1;
            env->camera->points[3].vz = work->unk18.vz;
            if (frame >= 6) {
                m6 = frame - 6;
                sc.vx = sc.vy = sc.vz = m6 * 0x60;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                pos.vy -= 0x400;
                rot.vy = frame << 2;
                rot.vx = 0;
                rot.vz = 0;
                D_801EFDD8 = (sc.vx * 0xA28) >> 12;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk80);
                ex->btlfx_drawScrollMove(frame * 8, 0x360, 0x100, 0x20, 0x80, 0x360, 0x180);
                z = m6 * 0x122B / 64;
                sc.vx = sc.vy = sc.vz = z;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                rot.vx = 0;
                rot.vy = frame * 8;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk90);
                ex->Hi_ModifyEffModelRGB(work->unk90, -0x30, -0x30, -0x30);
                rot.vy = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk94);
                ex->Hi_ModifyEffModelRGB(work->unk94, -0x20, -0x20, -0x20);
                sc.vx = sc.vy = sc.vz = z / 2;
                rot.vx = 0;
                rot.vy = frame << 4;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk90);
                ex->btlfx_drawScrollMove(frame * ((t >> 9) + 12), 0x200, 0x180, 0x20, 0x80, 0x220, 0x180);
                rot.vx = (frame << 4) + 0x200;
                rot.vy = frame << 5;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = z;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk98);
                ex->Hi_ModifyEffModelRGB(work->unk98, -0x20, -0x20, -0x20);
                rot.vx = (frame << 5) - 0x200;
                rot.vy = frame << 6;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = z * 2 / 3;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk98);
                ex->btlfx_drawScrollMove(frame * ((t >> 8) + 8), 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
            }
            if (frame >= 35) {
                env->otZ = 0x100;
                y35 = ((frame - 35) << 12) / 35;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xC), &work->unk8, NULL, y35 + 0xAAA, 0, 1, y35);
                env->otZ = 0;
            }
            if (frame >= 58) {
                c4 = (frame - 58) * 0xFF / 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 256), 1, c4, c4, c4 * 2 / 3);
            }
            if (frame == 41) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk8);
                if (p != NULL) {
                    p->otZ = 0x100;
                }
            }
            if (frame >= 6 && frame < 14) {
                sh1 = ex->math_cos(((frame - 6) << 10) >> 3) >> 7;
                BTLFX_COPY_SVEC(&cam, &work->unk18);
                cam.vy += ex->btlfx_randCentered(sh1);
                cam.vx += ex->btlfx_randCentered(sh1);
                env->camera->points[3].vx = cam.vx;
                env->camera->points[3].vy = cam.vy;
                env->camera->points[3].vz = cam.vz;
            }
            if (frame >= 35) {
                sh2 = 0x1000 - ex->math_cos(((frame - 35) << 10) / 35);
                sh2 >>= 6;
                BTLFX_COPY_SVEC(&cam2, &work->unk18);
                cam2.vy += ex->btlfx_randCentered(sh2);
                cam2.vx += ex->btlfx_randCentered(sh2);
                env->camera->points[3].vx = cam2.vx;
                env->camera->points[3].vy = cam2.vy;
                env->camera->points[3].vz = cam2.vz;
            }
            if (frame == 58) {
                BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                work->unk10.vz -= 0x5DC;
                work->unk10.vy -= 0x1AE;
                work->unk6C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk10);
                if (work->unk6C != NULL) {
                    work->unk6C->otZ = 0x200;
                }
            }
            if (work->unk6C != NULL && work->unk6C->state == -1) {
                work->unk6C = NULL;
            }
            if (frame >= 70) {
                work->unk0 = 3;
                entry->frame = -1;
            }
            break;
        }
        case 3: {
            s32 t;
            s32 n;
            s32 n5;
            s32 nz;

            t = (frame << 12) / 41;
            if (work->unk6C != NULL) {
                if (work->unk6C->state == -1) {
                    work->unk6C = NULL;
                }
                if (work->unk6C != NULL) {
                    work->unk6C->otZ = -10 - ((t << 5) >> 12);
                }
            }
            if (frame < 42) {
                if (frame == 0) {
                    for (i = 0, rp = D_801EF260, y = 0; i < 24; rp++, i++, y -= 0x18) {
                        o = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (o != NULL) {
                            n = (i << 7) + 0x400;
                            o->unkC.vx = work->unk8.vx + *rp * 3 / 4;
                            o->unkC.vz = work->unk8.vz + y;
                            o->unkC.vy = work->unk8.vy + 0x300;
                            o->unk14.vx = 0x400;
                            o->unk14.vy = 0x400;
                            o->unk14.vz = 0;
                            o->unk0 = 0;
                            n5 = n * 5;
                            o->unkA = n5 >> 2;
                            o->unk22 = y;
                            o->unk20 = i % 3;
                            o->unk4 = ex->btlfx_randCentered(0xAA);
                            o->unk8 = ex->btlfx_randCentered(0xAA);
                            o->unk6 = ex->btlfx_randCentered(0x80) - ((n5 << 6) >> 12);
                            o->unk1C = 0;
                            o->unk1E = 0;
                        }
                    }
                }
                D_801EF25C = -((t * 0x241) >> 12);
                if (!(frame & 1)) {
                    o1 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (o1 != NULL) {
                        a = (0x1000 - ex->math_cos(t >> 2)) * 3 / 4 + 0x400;
                        nz = -(t >> 3);
                        o1->unkC.vx = work->unk8.vx + ex->btlfx_randCentered(a >> 1) * 5;
                        o1->unkC.vz = work->unk8.vz + nz;
                        o1->unkC.vy = work->unk8.vy + 0x200;
                        o1->unk14.vx = 0x400;
                        o1->unk0 = 1;
                        o1->unk14.vy = 0;
                        o1->unk14.vz = 0;
                        o1->unkA = a * 3 / 2;
                        o1->unk20 = ex->btlfx_rand() % 3;
                        o1->unk1C = ex->btlfx_randCentered(0x40);
                        o1->unk1E = ex->btlfx_randCentered(0x40);
                        o1->unk4 = ex->btlfx_randCentered(0x80);
                        o1->unk8 = ex->btlfx_randCentered(0x80);
                        o1->unk6 = ex->btlfx_randCentered(0x80) - ((a << 9) >> 12);
                    }
                }
                if (frame == 40) {
                    for (i = 0; i < 2; i++) {
                        o2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (o2 != NULL) {
                            o2->unkC.vx = work->unk8.vx + i * 0x280 - 0x140;
                            o2->unkC.vz = work->unk8.vz + 0x2BC;
                            o2->unkC.vy = work->unk8.vy + 0xBB8;
                            o2->unk0 = 1;
                            o2->unkA = 0x4000;
                            o2->unk1C = 0x32 - i * 0x64;
                            o2->unk1E = 0x18;
                            o2->unk14.vx = 0x400;
                            o2->unk14.vy = 0x400;
                            o2->unk14.vz = 0;
                            o2->unk20 = 0;
                            o2->unk4 = 0;
                            o2->unk8 = 0;
                            o2->unk6 = -0x11A;
                        }
                    }
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 512), 1, 0xFF, 0xFF, 0xFF);
            } else {
                c5 = 0xFF - (frame - 41) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 512), 1, c5, c5, c5);
            }
            c3 = ((t << 6) >> 12) + 0x20;
            s9C = (t >> 3) + 0x100;
            b = 0;
            if (frame >= 49) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_initParticleSystem();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->otZ = 0;
        if (c3 != 0) {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = c3;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            s2.vx = s2.vy = s2.vz = s9C + 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
        }
        if (b != 0) {
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk44, work->unk74);
            k = (b >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk74, k, k, k);
            work->unk3C.vy += 1;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)work->unk28, (s32)work->unk44, work->unk70);
            ex->Hi_ModifyEffModelRGB(work->unk70, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}

void func_801EE6BC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801EF2D8;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    a = cnt;
    trail->unk0 = trail->unk2 = a - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < a; i++, pt++) {
        t = 0x1000 - (i << 12) / (a - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++ / 3;
        h = (ex->math_sin((t >> 1) >> 1) * h) >> 12;
        t >>= 1;
        h += ex->btlfx_lerp(h0, h1, ex->math_sin(t));
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->otZ = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->otZ = 0;
}

void func_801EE968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *path, Fx1bf1Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801EE6BC(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801EEA80(s32 mode, Fx1bf1Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 i;
    s32 u;
    s32 kind;
    s32 a;
    Fx1bf1Trail *tr;
    Fx1bf1Trail *tr2;

    if (mode != 1) {
        kind = b->unk11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 8;
            c = ex->math_cos(t >> 2);
            i = b->unk10;
            u = b->unk15;
            tr = (Fx1bf1Trail *)&D_801EFDF8[i];
            tr2 = (Fx1bf1Trail *)&D_801F00B8[i];
            if (frame == 0) {
                u >>= 1;
            }
            b->unk14 += 3;
            k = frame < 3;
            if (b->unk11 == 0) {
                a = 0x1000 - t;
                func_801EE6BC(entry, 0xB, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x3E8,
                              b->unk14 * 16, 0xF0, frame, c, a * u / 1500, 0);
                func_801EE968(entry, 0x12, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
                if (frame >= 0x10) {
                    return 1;
                }
            } else {
                t += frame << 9;
                c = 0x1000 - t;
                func_801EE6BC(entry, 0x11, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x12C,
                              b->unk14 << 6, 0x14A, frame, c, (0x1000 - t / 2) * u / 1500, 0);
                func_801EE968(entry, 0x12, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (c * u) >> 12);
                if (frame >= 5) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 fx_1bf_1_op83(s32 mode, Fx1bf1Work83 *work, Unk800F78A4 *env, Fx1bf1Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 k;
    s32 frame;
    s32 r;
    s32 a;
    Fx1bf1Bolt *b;

    if (mode == 0) {
        init->workSize = 0xC;
        init->elemSize = 0x1C;
        init->elemFn = (s32)func_801EEA80;
        init->poolCount = 5;
    } else if (mode == 1) {
        n = init->workSize;
        D_801EF2D8 = env;
        work->unk0 = D_801F0198[0];
        p = D_801F0198[0];
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0xD2);
            *p++ = ex->btlfx_randCentered(0x1C3);
            *p++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        k = work->unk8;
        frame = inst->unk0;
        switch (k) {
        case 0:
            if (frame >= 5) {
                return 1;
            }
            work->unk4++;
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDE0)[0];
                ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDE0)[1];
                ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDE8)[0];
                ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDE8)[1];
                b->unk11 = 0;
                b->unk10 = work->unk4 % 5;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0x4E;
                b->unk14 = work->unk4 * 0x33 + ex->btlfx_randCentered(7);
            }
            break;
        case 1:
            if (frame < 0x1E) {
                work->unk4++;
                r = 0xD48 - frame * 0xD48 / 28;
                b = ex->btlfx_poolSpawn(inst->unk10);
                if (b != NULL) {
                    a = (frame << 11) / 3 + 0x400;
                    ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDE8)[0];
                    ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDE8)[1];
                    ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDE0)[0];
                    ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDE0)[1];
                    b->unk0.vx += (ex->math_sin(a) * r) >> 12;
                    b->unk0.vz += (ex->math_cos(a) * r) >> 12;
                    b->unk11 = k;
                    b->unk8.vy += 0x400;
                    b->unk10 = work->unk4 % 5;
                    b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    b->unk12 = ex->btlfx_rand() % 15;
                    b->unk13 = ex->btlfx_rand() % 15;
                    b->unk15 = 0x3E - frame;
                    b->unk14 = work->unk4 * 0x33 + ex->btlfx_randCentered(7);
                }
            }
            if (frame >= 0x1E) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_headerPtr);
