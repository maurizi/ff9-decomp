#include "fxmod/fx_1b2.h"

s32 fx_1b2_op80(s32 mode, Fx1b2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        work->pos18.vx = 0;
        work->pos18.vy = 0;
        work->pos18.vz = -0x44C;
        work->rot10.vx = 0;
        work->rot10.vy = 0;
        work->rot10.vz = 0;
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->pos54.vx = work->pos18.vx;
        work->pos54.vy = work->pos18.vy;
        work->pos54.vz = work->pos18.vz;
        fx_1b2_env = env;
        fxm_vecPathInit(&fx_1b2_vecPath, (u8 *)ex->btlfx_getFileData(entry->file, 1));
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0x117, 0, 0, 0);
        work->rot3C.vx = -0x200;
        work->rot3C.vy = 0;
        work->rot3C.vz = 0;
        work->count64 = 0;
        work->index68 = 0;
        work->timer5C = 1;
        work->timer60 = -1;
        work->emit4 = NULL;
        work->emit8 = NULL;
    } else {
        frame = entry->frame;
        if (!(frame & 0xF)) {
            work->timer5C = 0xB;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos54);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos54);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x200;
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x180;
            }
            for (i = 0; i < env->targets->targetCount; i++) {
                if (work->count64 != 2) {
                    fxm_startHitMotionIfActive(i);
                }
            }
            work->count64++;
            if (!(frame & 0xF)) {
                ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->pos44);
                ex->btlfx_getUnitBonePos(0x10, 0x29, (u16 *)&work->pos4C);
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos44);
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos4C);
                if (work->emit4 != NULL) {
                    work->emit4->trails = (s32)fx_1b2_op80Trails0[work->index68];
                    work->emit4->trailSlots = 0x3C;
                    work->emit4->trailStride = 0x28;
                }
                if (work->emit8 != NULL) {
                    work->emit8->trails = (s32)fx_1b2_op80Trails1[work->index68];
                    work->emit8->trailSlots = 0x3C;
                    work->emit8->trailStride = 0x28;
                }
                work->index68++;
            }
        }
        if (work->timer5C >= 0) {
            t = 0xB - work->timer5C;
            if (t == 8) {
                work->timer60 = 4;
            }
            if (work->timer60 >= 0) {
                work->fade38 = fxm_lerpRatio(0, -0x80, 4 - work->timer60, 4);
                work->timer60--;
            }
            fxm_vecPathGet(&fx_1b2_vecPath, t, &work->scale28);
            ex->Hi_DrawEffModel(&work->rot10, &work->pos18, (s32)&work->scale28, work->model6C);
            ex->Hi_ModifyEffModelAbr(work->model6C, 1);
            if (work->timer60 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model6C, work->fade38, work->fade38, work->fade38);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), t);
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fxm_isMotionDone(i) != 0) {
                    fxm_startUnitMotion(i, 0);
                }
            }
            work->timer5C--;
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot3C.vx;
                work->emit4->rot.vy = work->rot3C.vy;
                work->emit4->rot.vz = work->rot3C.vz;
            }
        }
        if (work->emit8 != NULL) {
            if (work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (work->emit8 != NULL) {
                work->emit8->rot.vx = work->rot3C.vx;
                work->emit8->rot.vy = work->rot3C.vy;
                work->emit8->rot.vz = work->rot3C.vz;
            }
        }
        return frame >= 0x2C;
    }
    return 0;
}

s32 fx_1b2_op81(s32 mode, Fx1b2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_1b2_env = env;
        ex->btlfx_initEffModels(fx_1b2_effModels, 3);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 6:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->posC);
            break;
        case 9:
            ex->btlfx_getUnitBonePos(0x10, 0xC, (u16 *)&work->pos14);
            ex->btlfx_getUnitBonePos(0x10, 0x29, (u16 *)&work->pos1C);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos14);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos1C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_1b2_op81Trails0;
                work->emit4->trailSlots = 0xA;
                work->emit4->trailStride = 0x80;
            }
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_1b2_op81Trails1;
                work->emit8->trailSlots = 0xA;
                work->emit8->trailStride = 0x80;
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1b2_op82(s32 mode, Fx1b2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->pos8.vz = -0x44C;
        work->rot0.vx = 0;
        work->rot0.vy = 0;
        work->rot0.vz = 0;
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->scale18.vx = 0x1000;
        work->scale18.vy = 0x1000;
        work->scale18.vz = 0x1000;
        fx_1b2_env = env;
        work->model34 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC), 0x119, 0, 0, 0);
        work->timer2C = -1;
        work->timer30 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer2C = 8;
            break;
        case 0x28:
            work->timer30 = 0xA;
            break;
        }
        if (frame >= 0 && frame < 0x32) {
            if (work->timer2C >= 0) {
                work->fade28 = fxm_lerpRatio(-0x80, 0, 8 - work->timer2C, 8);
                work->timer2C--;
            }
            if (work->timer30 >= 0) {
                work->fade28 = fxm_lerpRatio(0, -0x80, 0xA - work->timer30, 0xA);
                work->timer30--;
            }
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale18, work->model34);
            ex->Hi_ModifyEffModelAbr(work->model34, 1);
            if (work->timer2C > 0 || work->timer30 >= 0) {
                c = work->fade28 + 6;
                ex->Hi_ModifyEffModelRGB(work->model34, c, c, c);
            } else {
                ex->Hi_ModifyEffModelRGB(work->model34, 6, 6, 6);
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1b2_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1b2_env = env;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            ex->btlfx_drawBgOffscreen(0x240, 0x100, 0x80, 0x80, fx_1b2_bgOt, 0, -0x3E8, 0x1770);
        }
        return frame >= 5;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1b2_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_vecPathInit(Fx1b2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1b2Point *)data;
}

static s32 fxm_vecPathGet(Fx1b2Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->count2 - 1;
    s32 done = 1;
    Fx1b2Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->keys4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1b2_env)->btlfx_drawScreenTint((u32 *)(fx_1b2_env->ot + ot), arg4, c, c, c);
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b2_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b2_env);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1b2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1b2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_headerPtr);
