#include "fxmod/fx_057.h"

static void fx_057_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 rot, s32 scale, s32 alpha, s32 blend, s32 t,
                   s32 n) {
    Unk800F78A4 *env = fx_057_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = fx_057_task;
    u8 *buf = (u8 *)ex->btlfx_getParticleTrail(pt, e);
    s32 s;
    s32 d;
    SVECTOR v;

    if (t == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), (s32)pos, NULL, -1, 0, 0, 0, buf, 0);
    }
    if (t < n - 4) {
        s = 0x1000;
    } else {
        s = 0x1000 - (((t - (n - 4)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), (s32)pos, (s32 *)e->origin, t,
                      0x1C00 - (t << 12) / n, s, 1, buf, 1);
    if (pt->slot == 1) {
        if (fx_057_emitter0 != NULL) {
            ex->btlfx_moveEmitterTo((s32)fx_057_emitter0, (s32)pos);
        }
        BTLFX_COPY_SVEC(&fx_057_headPos, pos);
        if (t < 0x1D) {
            env->camera->points[3].vx = pos->vx;
            env->camera->points[3].vy = pos->vy;
            env->camera->points[3].vz = pos->vz;
        } else {
            d = t - 0x1D;
            if (d < 0x20) {
                ex->btlfx_lerpVec((s16 *)pos, (s16 *)&fx_057_camRest, 0x1000 - ex->math_cos(d << 5), (s16 *)&v);
            } else {
                BTLFX_COPY_SVEC(&v, &fx_057_camRest);
            }
            env->camera->points[3].vx = v.vx;
            env->camera->points[3].vy = v.vy;
            env->camera->points[3].vz = v.vz;
        }
    }
}

s32 fx_057_op80(s32 mode, Fx057Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    VECTOR sv;
    SVECTOR unused; 
    Pool6CEntry *p;
    s32 frame;
    s32 alpha;
    s32 scale;
    s32 t;
    s32 c;
    s32 k;

    if (mode == 0) {
        *args = 0x38;
    } else if (mode == 1) {
        fx_057_env = env;
        fx_057_fade = 0;
        fx_057_task = entry;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->state0 = 0;
        work->frame4 = 0;
        BTLFX_COPY_SVEC(&fx_057_camRest, &work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_057_targetPos);
        work->emit2C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
        if (work->emit2C != NULL) {
            work->emit2C->otZ = 0x18;
        }
        if (work->emit2C != NULL) {
            work->emit2C->trails = (s32)fx_057_trails;
            work->emit2C->trailSlots = 0x10;
            work->emit2C->trailStride = 0x184;
        }
        if (work->emit2C != NULL) {
            work->emit2C->elems->draw = (void (*)())fx_057_op80ElemDraw;
        }
        fx_057_emitter0 = work->emit30 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->pos24);
        if (work->emit30 != NULL) {
            work->emit30->otZ = 0x18;
        }
        work->emit34 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos24);
        if (work->emit34 != NULL) {
            work->emit34->otZ = 0x10;
        }
        ex->btlfx_initEffModels(fx_057_effModels, 3);
        work->model8 =
            ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x19), 0x137, 0xFFFF, 0, 0x80);
        work->modelC = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x28));
    } else {
        alpha = 0;
        frame = entry->frame;
        scale = 0;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 22;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
                work->emit30 = NULL;
            }
            if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
                work->emit34 = NULL;
            }
            alpha = 0x1000;
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = alpha;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = alpha;
            }
            scale = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < 4) {
                c = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 1, c, c);
            }
            if (frame >= 22) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 20;
            alpha = 0x1000 - t;
            scale = 0x1000;
            fx_057_fade = t;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
                work->emit30 = NULL;
            }
            if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
                work->emit34 = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = alpha;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = alpha;
            }
            if (frame >= 20) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit30);
                ex->btlfx_stopEmitter(work->emit34);
                fx_057_emitter0 = NULL;
            }
            break;
        case 2:
            fx_057_fade = 0x1000;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (frame == 19) {
                fx_057_emitter1 = p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
                if (fx_057_emitter1 != NULL) {
                    fx_057_emitter1->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
            }
            if (frame >= 22) {
                ex->btlfx_stopEmitter(work->emit2C);
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.pad = 0;
            rot.vy = 0;
            rot.vx = 0;
            rot.vz = -work->frame4 * 20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &fx_057_headPos, &rot, scale, 0, 1, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x11), &fx_057_headPos, &rot, 0xE00, 0, 1, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &fx_057_headPos, &rot, 0x1000, 0, 1, alpha);
        }
        if (fx_057_fade != 0) {
            sv.vx = 0x1000;
            sv.vy = 0x1000;
            sv.vz = 0x1000;
            ang.vx = 0;
            ang.vy = -0x400;
            ang.vz = 0;
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos18, (s32)&sv, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->model8, (fx_057_fade >> 5) - 0x80, (fx_057_fade >> 5) - 0x80,
                                     (fx_057_fade >> 5) - 0x80);
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos18, (s32)&sv, work->modelC);
            ex->Hi_ModifyEffModelAbr(work->modelC, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->modelC, (fx_057_fade >> 5) - 0x80, (fx_057_fade >> 5) - 0x80,
                                     (fx_057_fade >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x18), work->frame4);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos10, NULL, 0x1600, 0, 1, fx_057_fade);
            env->otZ = 0;
            ang.pad = 1;
            ang.vx = -0x200;
            ang.vy = work->frame4 * 0x18 + 0x580;
            ang.vz = 0;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->pos10, &ang, 0x16000, 0, 1,
                              fx_057_fade);
            k = ex->math_cos(((work->frame4 - 22) << 10) / 42);
            ang.vx = -0x180;
            ang.vy = work->frame4 * 32 + 0x180;
            ang.vz = work->frame4 * 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos10, &ang, k * 4, 0, 1,
                              fx_057_fade);
            ang.vx = 0x200;
            ang.vy = 0x200 - work->frame4 * 32;
            ang.vz = -work->frame4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos10, &ang, (k * 8 + k * 4) >> 1, 0, 1,
                              fx_057_fade);
            ang.vx = 0x100;
            ang.vy = -0x180 - work->frame4 * 0x18;
            ang.vz = -work->frame4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos10, &ang, (k * 16 + k * 4) >> 1, 0, 1,
                              fx_057_fade);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1F), work->frame4);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), work->frame4);
        work->frame4++;
    }
    return 0;
}

s32 fx_057_op81(s32 mode, Fx057Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR ang;
    SVECTOR out;
    SVECTOR v;
    SVECTOR out2;
    Pool6CEntry *p;
    s32 t4;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 i;
    s32 j;
    s32 r;
    s32 step;
    s32 cs;
    s32 q;
    s32 c;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_057_env = env;
        work->targetOtZ8 = ex->btlfx_addRadiusSixth(0, 0);
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0;
        p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->posC);
        if (p != NULL) {
            p->otZ = work->targetOtZ8 + 0x40;
        }
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        t = (frame << 12) / 7;
        s = ex->math_sin(t >> 2);
        env->otZ = work->targetOtZ8 + 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->posC, NULL, 0x13000, frame, 1, s);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &work->posC, NULL, (t >> 1) + 0x3000, 0, 1,
                          s);
        env->otZ = 0;
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->posC, work->frame4, ex->math_sin(t >> 2));
        if (frame >= 7) {
            work->state0 = 1;
            entry->frame = -1;
            work->model28 =
                ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x27), 0xB9, 0x3D80, 0x80, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0xFF);
        }
        break;
    case 1:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        env->otZ = work->targetOtZ8 + 0x40;
        t = (frame << 12) / 7;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &work->posC, &d, 0x3000, frame, 1, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x23), &work->posC, NULL, t + 0x13000, frame, 1,
                          0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x22), &work->posC, NULL, (t >> 1) + 0x3800, 0, 1,
                          0x1000);
        env->otZ = 0;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->posC, work->frame4, 0x1000);
        if (frame >= 7) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            work->state0 = 2;
            entry->frame = -1;
            BTLFX_COPY_SVEC(&work->posC, &fx_057_targetPos);
            work->pos1C.vx = work->posC.vx;
            work->pos1C.vy = work->posC.vy;
            work->pos1C.vz = work->posC.vz;
            BTLFX_COPY_SVEC(&work->pos14, &work->posC);
            work->pos14.vy -= 0xC00;
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = work->targetOtZ8 + 0x40;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = work->targetOtZ8 + 0x40;
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->posC);
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x26), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = work->targetOtZ8 + 0x80;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2A), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = work->targetOtZ8 + 0x46;
            }
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&work->pos14);
            if (p != NULL) {
                p->otZ = 0x46;
            }
        }
        break;
    case 2:
        t = frame << 7;
        env->otZ = 0;
        if (frame < 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, (0xFF - frame * 0xFF / 6) >> 1,
                                     0xFF - frame * 0xFF / 6, 0xFF - frame * 0xFF / 6);
        }
        if (frame == 7) {
            p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2B), (s32)&work->posC);
            if (p != NULL) {
                p->otZ = 0x46;
            }
        }
        cs = ex->math_cos(t >> 2);
        r = 0x80;
        i = 0;
        step = ((t >> 7) + 4) * 3;
        ang.vx = work->frame4 * 128;
        ang.vy = work->frame4 * 32;
        q = (t / 40 + 10) * 2;
        do {
            ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&ang, r, (s16 *)&out);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &out, NULL, 0x1000, (frame + 7) & 0xF, 1,
                              (cs * ex->math_sin((i << 11) >> 7)) >> 12, 0, 0);
            i++;
            r += step;
            ang.vx += q * 2;
            ang.vy += q;
        } while (i < 0x2E);
        c = ex->math_cos(t >> 2);
        t4 = ex->math_sin(t >> 2) * 0x92E / 4096 + 0x200;
        v.vx = 0;
        v.vy = 0;
        v.vz = 0;
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                v.vx = work->frame4 * 0x18 + 0x200;
            } else {
                v.vx = -0x200 - work->frame4 * 0x18;
            }
            a = work->frame4 * 0x29 + (j << 7);
            for (i = 0; i < 0x10; i++, a += 0x100) {
                ex->btlfx_offsetPoint(&work->posC, (s32)&out2, 0, (XY32 *)&v, t4, a);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1D), &out2, NULL, 0x1800, frame & 0xF, 1,
                                  c, 0, 0);
            }
        }
        if (frame >= 0x20) {
            work->state0 = 3;
            entry->frame = -1;
        }
        break;
    case 3:
        if (frame >= 0xF) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_057/nonmatchings/fx_057", fx_057_headerPtr);
