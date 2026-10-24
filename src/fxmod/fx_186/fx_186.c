#include "fxmod/fx_186.h"

s32 func_801E7704(s32 mode, Fx186Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 a;
    s32 t;
    s32 c;
    s16 x;
    s16 z;
    s32 r;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk6 = ex->btlfx_rand() % 1000 - 400;
        o->unk14.vx = 0;
        o->unk14.vy = 0;
        o->unk14.vz = 0;
        o->unk1E = 0;
        o->unk1C = 0;
    } else {
        scale.vx = 0x2000;
        scale.vy = 0x2000;
        scale.vz = 0x2000;
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        a = 0x1000;
        o->unk14.vx += o->unk1C;
        o->unk14.vy += o->unk1E;
        switch (o->unk0) {
        case 0:
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            t = frame << 7;
            pos.vy = pos.vy - ((ex->math_sin(t >> 2) * o->unk6) >> 12) + ((frame & 1) * 2 - 1) * (t >> 7);
            if (frame >= 0x20) {
                o->unk0 = 1;
                ((s16 *)o)[-1] = -1;
                o->unkC.vx -= o->unk6;
                o->unk1C = ex->btlfx_randCentered(8);
                o->unk1E = ex->btlfx_randCentered(8);
                o->unk8 = o->unkC.vx / 4 + ex->btlfx_randCentered(0x80);
                r = ex->btlfx_randCentered(0x100);
                x = o->unkC.vx / 7;
                z = o->unkC.vz / 7;
                o->unk6 = (s16)(r - 0x2BC) + ex->btlfx_sqrt(x * x + z * z, 0x14) / 2;
                o->unkA = o->unkC.vz / 4 + ex->btlfx_randCentered(0x80);
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            a -= t;
            o->unkC.vx += o->unk8 >> 2;
            o->unkC.vz += o->unkA >> 2;
            o->unk8 = o->unk8 * 7 >> 3;
            o->unkA = o->unkA * 7 >> 3;
            o->unkC.vy += o->unk6 / 5;
            o->unk6 += 12;
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
        ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, o->unk20);
        if (a < 0x1000) {
            if (a < 0x800) {
                ex->Hi_ModifyEffModelAbr(o->unk20, 1);
            }
            c = (a >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(o->unk20, c, c, c);
        }
        /* Dead test, removed after reload: the extra reference ranks a above the constant 1 in global-alloc. */
        if (a != 0) {
            t = 0;
        }
    }
    return 0;
}

s32 fx_186_op80(s32 mode, Fx186Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 tint;
    s32 frame;
    s32 t;

    if (mode == 0) {
        init->unk0 = 0x4C;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801E7704;
        init->unk4 = 0xA;
    } else if (mode == 1) {
        work->unk0 = 3;
        work->unk4 = 0;
        D_801E9214 = env;
        ex->btlfx_initEffModels(D_801E9224, 0xC);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk44 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
        work->unk48 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1B), 1);
        env->camera->points[3].vx = work->unk8.vx;
        env->camera->points[3].vy = work->unk8.vy;
        env->camera->points[3].vz = work->unk8.vz;
        env->camera->points[3].vy -= 0x100;
    } else {
        tint = 0;
        frame = entry->frame;
        switch (work->unk0) {
        case 3:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, D_801E8E14, work->unk8.vx, work->unk8.vz, 0xC00);
            work->unk0 = 0;
            entry->frame = -1;
            break;
        case 0:
            /* gcc 2.8 frees block-scoped stack slots only at the end of a switch; the original reuses each case's
             * slots for the next case, which this one-label switch reproduces. */
            switch (frame) {
            default: {
                RECT rect;
                VECTOR pos;
                VECTOR scale;
                SVECTOR rot;
                SVECTOR rot2;
                s32 a;
                s32 b;
                s32 i;
                s32 k;
                s32 n;
                s16 *q;
                s16 *r;

                t = frame << 8;
                env->camera->points[3].vx = work->unk8.vx;
                env->camera->points[3].vy = work->unk8.vy;
                env->camera->points[3].vz = work->unk8.vz;
                env->camera->points[3].vy = (s16)(env->camera->points[3].vy - 0x100) - (t >> 7) * ((frame & 1) * 2 - 1);
                if (frame == 1) {
                    for (r = D_801E9B84, k = 0; k < 0x60; k++, r++) {
                        *r = -1;
                    }
                    rect.x = 0x200;
                    rect.y = 0x180;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)D_801E9B84);
                    rect.x = 0x27C;
                    rect.y = 0x180;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)D_801E9B84 + 0x10);
                    rect.x = 0x240;
                    rect.y = 0x1EC;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)D_801E9B84 + 0x30);
                }
                b = (t + 0x1000) >> 2;
                scale.vz = ((t >> 1) + 0x1000) >> 2;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                pos.vy -= 0x180;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame * 16;
                for (i = 0; i < 6; i++) {
                    scale.vx = scale.vy = (b * ((ex->math_sin((i << 10) / 5) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk44);
                    ex->Hi_ModifyEffModelRGB(work->unk44, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                             (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80);
                    rot.vz += 0x258;
                }
                if (frame >= -4) {
                    s32 d;

                    d = ((frame + 4) << 12) / 20;
                    d >>= 2;
                    scale.vz = 0x2000;
                    scale.vx = 0x2000;
                    scale.vy = ex->math_sin(d) * 2 + 0x1000;
                    rot.vz = 0;
                    rot.vy = 0;
                    rot.vx = 0;
                    pos.vz = 0;
                    pos.vx = 0;
                    pos.vy = 0x5AE;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk48);
                    ex->Hi_ModifyEffModelAbr(work->unk48, 1);
                    ex->Hi_ModifyEffModelRGB(work->unk48, (ex->math_sin(d) >> 5) - 0x80, (ex->math_sin(d) >> 5) - 0x80,
                                             (ex->math_sin(d) >> 5) - 0x80);
                    ex->btlfx_drawScrollMove(frame * 16, 0x2C0, 0x180, 0x20, 0x80, 0x2E0, 0x180);
                }
                if (frame >= 9) {
                    rot2.vx = 0x400;
                    rot2.vy = 0;
                    rot2.vz = 0;
                    rot2.pad = 1;
                    a = (frame - 8) << 9;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->unk8, &rot2,
                                      ex->math_sin(a >> 2), 0,
                                      1, ex->math_sin(a >> 2));
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), &work->unk8, &rot2,
                                      ex->math_sin(a >> 2) >> 1, 0, 1, ex->math_sin(a >> 2));
                }
                if (frame >= 0x10) {
                    work->unk0 = 1;
                    entry->frame = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
                    for (n = 0, q = D_801E9B84, k = 0; k < 0x60; k++, q++) {
                        if (*q == 0) {
                            n++;
                        }
                    }
                    for (i = 0; i < 10; i++) {
                        if (n != 0) {
                            work->unk1C[i] =
                                ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8DB8[i]), 0);
                        } else {
                            work->unk1C[i] =
                                ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8DA4[i]), 0);
                        }
                    }
                }
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                VECTOR pos;
                VECTOR scale;
                SVECTOR rot;
                SVECTOR rot2;
                s32 t;
                s32 a;
                s32 b;
                s32 c;
                s32 i;
                s32 j;
                Fx186Rock *o;

                env->camera->points[3].vx = work->unk8.vx;
                env->camera->points[3].vy = work->unk8.vy;
                env->camera->points[3].vz = work->unk8.vz;
                env->camera->points[3].vy = (s16)(env->camera->points[3].vy - 0x100) - ((frame & 1) * 2 - 1) * 16;
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                    work->unk10.vy -= 0x180;
                    work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->unk10);
                    if (work->unk18 != NULL) {
                        work->unk18->unk22 = 0x100;
                    }
                    for (j = 0; j < 25; j++) {
                        o = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (o != NULL) {
                            o->unk20 = work->unk1C[j];
                            o->unkC.vx = work->unk8.vx;
                            o->unkC.vy = work->unk8.vy + 0x5AE;
                            o->unkC.vz = work->unk8.vz;
                        }
                    }
                }
                if (work->unk18 != NULL && work->unk18->unk30 == -1) {
                    work->unk18 = NULL;
                }
                if (frame < 4) {
                    c = 0xFF - ((frame * 0xFF) >> 2);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c, c, c);
                }
                if (frame < 0x18) {
                    t = (frame << 12) / 24;
                    scale.vz = t * 2 + 0x600;
                    b = (t >> 2) + 0x800;
                    pos.vx = work->unk8.vx;
                    a = t >> 2;
                    pos.vy = work->unk8.vy;
                    pos.vz = work->unk8.vz;
                    pos.vy += -0x180 - (t >> 1);
                    rot.vx = -0x400;
                    rot.vy = 0;
                    rot.vz = frame * 16 + 0x100;
                    for (i = 0; i < 6; i++) {
                        scale.vx = scale.vy = (b * ((ex->math_sin((i << 10) / 5) >> 1) + 0x800)) >> 12;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk44);
                        ex->Hi_ModifyEffModelRGB(work->unk44, (ex->math_cos(a) >> 5) - 0x80,
                                                 (ex->math_cos(a) >> 5) - 0x80,
                                                 (ex->math_cos(a) >> 5) - 0x80);
                        rot.vz += 0x258;
                    }
                }
                if (frame < 0x10) {
                    rot2.vx = 0x400;
                    rot2.pad = 1;
                    rot2.vy = 0;
                    rot2.vz = 0;
                    env->unk28 = 0x100;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), &work->unk8, &rot2,
                                      (ex->math_sin((frame << 8) >> 2) >> 1) + 0x800, 0, 1,
                                      ex->math_cos((frame << 8) >> 2));
                    env->unk28 = 0;
                }
                if (frame >= 0x20) {
                    work->unk0 = 2;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->unk18);
                }
            }
            }
            break;
        case 2: {
            s32 c;
            Pool6CEntry *p;

            t = (frame << 12) / 40;
            tint = 0x89;
            if (frame == 1) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            env->camera->points[3].vx = work->unk8.vx;
            env->camera->points[3].vy = work->unk8.vy;
            env->camera->points[3].vz = work->unk8.vz;
            env->camera->points[3].vy =
                (s16)(env->camera->points[3].vy - 0x100) - ((0x1000 - t) >> 7) * ((frame & 1) * 2 - 1);
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0x80, 0x80, 0x80);
            } else if (frame < 9) {
                c = 0x80 - (((frame - 1) << 7) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c, c, c);
            }
            if (frame >= 0x28) {
                work->unk0 = 3;
                return 1;
            }
            break;
        }
        }
        if (tint != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR scale;

            col.r = col.g = col.b = tint;
            rot.vz = -8;
            scale.vx = 0x1020;
            scale.vy = 0x1020;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            ex->btlfx_drawScreenGrid(2, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &scale, &col, 0, 0);
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_186_op81(s32 mode, Fx186Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 k;
    s32 t;
    s32 w0;
    s32 w1;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801E9214 = env;
        {
            s32 n = env->targets->targetCount;
            /* The dead conditional store splits the block, so n lives across blocks and is left to global-alloc.
             * That leaves two local qtys here, which local-alloc sorts properly (its hand-written three-qty sort
             * compares qty numbers instead of positions), giving %hi(D_801E9214) v0 and env->targets v1. */
            if (n != 0) k = 0;
            k = 0;
            work->unk0 = 0;
            work->unk104 = n;
            if (n <= 0) {
                return 0;
            }
        }
        do {
            ex->btlfx_getUnitHomePos(k, (s16 *)&work->unk4[k]);
            ex->btlfx_getUnitRot(k, (s16 *)&work->unk44[k]);
            ((s32 *)&work->unkC4[k])[0] = ((s32 *)&work->unk44[k])[0];
            ((s32 *)&work->unkC4[k])[1] = ((s32 *)&work->unk44[k])[1];
            work->unk84[k].vx = 0x2000 / ((ex->btlfx_rand() % 2 + 1) * 40);
            work->unk84[k].vy = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk84[k].vz = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk108 = -0x578 - (ex->btlfx_rand() & 0x3FF);
            k++;
        } while (k < work->unk104);
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            t = frame << 7;
            for (i = 0; i < work->unk104; i++) {
                w1 = ((s32 *)&work->unk4[i])[1];
                w0 = ((s32 *)&work->unk4[i])[0];
                ((s32 *)&p)[1] = w1;
                ((s32 *)&p)[0] = w0;
                p.vy -= (t >> 4) + (ex->btlfx_rand() & 0x7F);
                ex->btlfx_setUnitPos(i, (s16 *)&p);
            }
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            for (i = 0; i < work->unk104; i++) {
                w1 = ((s32 *)&work->unk4[i])[1];
                w0 = ((s32 *)&work->unk4[i])[0];
                ((s32 *)&p)[1] = w1;
                ((s32 *)&p)[0] = w0;
                p.vy += (work->unk108 * ex->math_sin(t >> 1)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&p);
                work->unkC4[i].vx += work->unk84[i].vx;
                work->unkC4[i].vy += work->unk84[i].vy;
                work->unkC4[i].vz += work->unk84[i].vz;
                ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
            }
            if (frame >= 0x28) {
                for (i = 0; i < work->unk104; i++) {
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unk44[i]);
                    ex->btlfx_getUnitHomePos(i, (s16 *)&p);
                    ex->btlfx_setUnitPos(i, (s16 *)&p);
                }
                work->unk0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            t = (frame << 12) / 7;
            for (i = 0; i < work->unk104; i++) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->unk4[i], &rot, 0x1000, frame, 1,
                                  ex->math_cos(t >> 2));
            }
            if (frame >= 7) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_186/nonmatchings/fx_186", fx_186_headerPtr);
