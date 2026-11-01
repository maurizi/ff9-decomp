#include "fxmod/fx_062.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_062_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_RODATA("asm/us/fx_062/nonmatchings/fx_062", fx_062_headerPtr);

s32 fx_062_op80(s32 mode, Fx062Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR cur;
    VECTOR b;
    s32 frame;
    u8 *strip;
    s32 n;
    Fx062Strip *tr;
    Fx062Pt *pt;
    s32 i;
    s32 j;
    s32 r;
    s32 s;
    s32 t;
    s32 u;
    s32 k;

    if (mode == 0) {
        *arg4 = 0xD4;
    } else if (mode == 1) {
        fx_062_env = env;
        ex->btlfx_initEffModels(fx_062_effModels, 4);
        for (i = 15; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            fx_062_strips[i].timer118 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_062_strips[i], 0);
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&fx_062_targetHome);
        ex->btlfx_getUnitScale(0, &fx_062_targetScale);
        fx_062_targetHeight = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        env->camera->points[0].vx = work->pos94.vx = fx_062_targetBodyPos.vx;
        env->camera->points[0].vz = work->pos94.vz = fx_062_targetBodyPos.vz;
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x137,
                                                         0xFFFF, 0, 0x80);
        work->models40[0].rot4.vx = 0x800;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = 0;
        work->models40[0].scaleC.vy = 0;
        work->models40[0].scaleC.vz = 0;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = 0;
        work->models40[1].scaleC.vy = 0;
        work->models40[1].scaleC.vz = 0;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = 0;
        work->models40[2].scaleC.vy = 0;
        work->models40[2].scaleC.vz = 0;
        work->modelA4 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xA), 0x17A, 0xFFFF, 0,
                                                 0x80);
        work->posB0.vx = fx_062_targetHome.vx;
        work->posB0.vy = 0;
        work->posB0.vz = fx_062_targetHome.vz;
        work->rotA8.vx = 0;
        work->rotA8.vy = 0;
        work->rotA8.vz = 0;
        work->scaleC0.vx = 0x249;
        work->scaleC0.vy = 0;
        work->scaleC0.vz = 0x249;
        ex->Hi_SetEffModelOffset(work->modelA4, 0x20);
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x280, 0x100, 0x70, 0x70, fx_062_bgOt, fx_062_targetHome.vx, fx_062_targetHome.vz, -0x1000);
            break;
        case 1:
            work->timers0[1] = 1;
            work->timers0[2] = 0x10;
            work->timers0[0] = 0x30;
            work->timers0[7] = 0x10;
            break;
        case 9:
            work->timers0[4] = 1;
            work->timers0[5] = 8;
            break;
        case 0x11:
            work->timers0[9] = 0x19;
            break;
        case 0x2A:
            work->timers0[3] = 0x10;
            work->timers0[8] = 0x10;
            work->timers0[6] = 8;
            break;
        case 0x3A:
            work->timers0[1] = 0;
            work->timers0[4] = 0;
            break;
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x170);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x143);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x199);
        }
        if (work->timers0[3] != 0) {
            work->timers0[3]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[3], 0x170, 0);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[3], 0x143, 0);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[3], 0x199, 0);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            work->scaleC0.vy += 0x40;
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            work->scaleC0.vy -= 0x40;
        }
        if (work->timers0[7] != 0) {
            work->timers0[7]--;
            if (fx_062_targetHeight > 0x200) {
                scale.vx = scale.vy = scale.vz =
                    fxm_easeLerp(2, 0x10, work->timers0[7], fx_062_targetScale.vy, fx_062_targetScale.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vx = fx_062_targetHome.vx;
            pos.vz = fx_062_targetHome.vz;
            pos.vy = fx_062_targetHome.vy + fxm_easeLerp(2, 0x10, work->timers0[7], 0, -0x200) +
                ((ex->math_sin(frame << 8) * (0x10 - work->timers0[7])) >> 10);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        }
        if (work->timers0[9] != 0) {
            work->timers0[9]--;
            pos.vx = fx_062_targetHome.vx;
            pos.vz = fx_062_targetHome.vz;
            k = ((ex->math_sin(frame << 8) * 16) >> 10) - 0x200;
            pos.vy = fx_062_targetHome.vy + k;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[8] != 0) {
            if (--work->timers0[8] != 0) {
                if (fx_062_targetHeight > 0x200) {
                    scale.vx = scale.vy = scale.vz =
                        fxm_easeLerp(2, 0x10, work->timers0[8], fx_062_targetScale.vy >> 1, fx_062_targetScale.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = fx_062_targetHome.vy + fxm_easeLerp(2, 0x10, work->timers0[8], -0x200, 0) +
                    ((ex->math_sin(frame << 8) * work->timers0[8]) >> 10);
                pos.vx = fx_062_targetHome.vx;
                pos.vz = fx_062_targetHome.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&fx_062_targetScale);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        }
        env->camera->points[0].vy = work->pos94.vy = fx_062_targetBodyPos.vy;
        if (work->timers0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&fx_062_targetBodyPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, 1);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), frame);
        }
        if (work->timers0[4] != 0) {
            ex->Hi_DrawEffModel(&work->rotA8, &work->posB0, (s32)&work->scaleC0, work->modelA4);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 9), frame);
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            for (n = 0; n < 5; n++) {
                tr = &fx_062_strips[n];
                if (tr->timer118 != 0) {
                    tr->timer118--;
                    tr->lastSlot0 = tr->count2 = 10;
                    tr->head1 = 0;
                    s = work->models40[0].scaleC.vx;
                    tr->angle104 += tr->spinB4;
                    tr->angle108 += tr->spinB8;
                    pt = tr->pts4;
                    r = (ex->math_sin(tr->angle104) * s) >> 12;
                    tr->x10C = (ex->math_cos(tr->angle108) * r) >> 12;
                    tr->z110 = (ex->math_sin(tr->angle108) * r) >> 12;
                    pt->vy = tr->pos5C.vy = work->pos94.vy + ((ex->math_cos(tr->angle104) * s) >> 12);
                    strip = (u8 *)tr;
                    pt->vx = tr->pos5C.vx = work->pos94.vx + tr->x10C;
                    pt->vz = tr->pos5C.vz = work->pos94.vz + tr->z110;
                    pt->halfWidth6 = tr->width114;
                    tr->pos64.vy = 0;
                    tr->pos64.vx = work->pos94.vx + (tr->x10C >> 4);
                    tr->pos64.vz = work->pos94.vz + (tr->z110 >> 4);
                    ex->btlfx_aimAngles((s16 *)&fx_062_targetBodyPos, (s16 *)&tr->pos5C, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        b.vx = (u * u) >> 6;
                        b.vy = (t * u + u * 4) >> 6;
                        b.vz = (t * t) >> 6;
                        tr->jitter6C[j].vx += tr->jitterVelBC[j];
                        tr->jitter6C[j].vz += tr->jitterVelE0[j];
                        cur.vx = tr->jitter6C[j].vx +
                            ((tr->pos5C.vx * b.vx + tr->pos64.vx * b.vz +
                              (work->pos94.vx + ((ex->math_sin(ang.vy) * -tr->pos5C.vy) >> 10) * b.vy)) >> 12);
                        pt->vy = (tr->pos5C.vy * b.vx + tr->pos64.vy * b.vz + (work->pos94.vy >> 1) * b.vy) >> 12;
                        cur.vz = tr->jitter6C[j].vz +
                            ((tr->pos5C.vz * b.vx + tr->pos64.vz * b.vz +
                              (work->pos94.vz + ((ex->math_cos(ang.vy) * -tr->pos5C.vy) >> 10) * b.vy)) >> 12);
                        pt->vx = cur.vx + tr->jitter6C[j].vx;
                        pt->vz = cur.vz + tr->jitter6C[j].vz;
                        pt->halfWidth6 = tr->width114;
                    }
                    pt->vx = tr->pos64.vx;
                    pt->vy = tr->pos64.vy;
                    pt->vz = tr->pos64.vz;
                    pt->halfWidth6 = tr->width114;
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                        tr->timer118 << 9, 1, strip, 0);
                } else if (work->timers0[0] >= 9) {
                    tr->timer118 = ex->btlfx_randRange(2, 8);
                    tr->angle104 = ex->btlfx_randRange(0x280, 0x600);
                    tr->angle108 = ex->btlfx_rand() & 0xFFF;
                    tr->spinB4 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->spinB8 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->width114 = ex->btlfx_randRange(8, 0x20);
                    for (j = 0; j < 9; j++) {
                        tr->jitter6C[j].vz = 0;
                        tr->jitter6C[j].vx = 0;
                        tr->jitterVelBC[j] = ex->btlfx_randRange(-0x20, 0x20);
                        tr->jitterVelE0[j] = ex->btlfx_randRange(-0x20, 0x20);
                    }
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}
