#include "fxmod/fx_079.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_079_env);
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

INCLUDE_RODATA("asm/us/fx_079/nonmatchings/fx_079", fx_079_headerPtr);

s32 fx_079_op80(s32 mode, Fx079Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    SVECTOR pos;
    VECTOR cur;
    VECTOR d;
    Fx079Strip *tr;
    Fx079Pt *pt;
    Pool6CEntry *p;
    s32 i;
    s32 frame;
    s32 j;
    s32 n = 11;
    s32 a;
    s32 b;
    s32 r;
    u8 *strip;
    s32 w1;
    s32 w2;

    if (mode == 0) {
        *arg4 = 0xA8;
    } else if (mode == 1) {
        fx_079_env = env;
        fx_079_emitter = NULL;
        ex->btlfx_initEffModels(fx_079_effModels, 3);
        for (i = 15; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            fx_079_strips[i].timer11C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_079_strips[i], 0);
        }
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x137,
                                                         0xFFFF, 0, 0);
        work->models40[0].rot4.vx = 0x800;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = 0x5C;
        work->models40[0].scaleC.vy = 0x5C;
        work->models40[0].scaleC.vz = 0x5C;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = 0x50;
        work->models40[1].scaleC.vy = 0x50;
        work->models40[1].scaleC.vz = 0x50;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 6));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = 0x66;
        work->models40[2].scaleC.vy = 0x66;
        work->models40[2].scaleC.vz = 0x66;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_bodyPos);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_emitStartPos);
        ex->btlfx_getUnitHomePos(0, (s16 *)&fx_079_homePos);
        ex->btlfx_getUnitScale(0, &fx_079_unitScale);
        fx_079_unitHeight = ex->btlfx_getUnitHeight(0);
        work->pos94.vx = fx_079_curBodyPos.vx = fx_079_bodyPos.vx;
        work->pos94.vy = fx_079_bodyPos.vy;
        work->pos94.vz = fx_079_curBodyPos.vz = fx_079_bodyPos.vz;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&fx_079_bonePos);
        switch (frame) {
        case 0:
            work->timers0[9] = 1;
            work->timers0[1] = 1;
            work->timers0[0] = 0x24;
            work->timers0[4] = 0x10;
            break;
        case 8:
            work->timers0[6] = 0x10;
            break;
        case 16:
            fx_079_emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&fx_079_emitStartPos);
            break;
        case 24:
            work->timers0[8] = 4;
            break;
        case 28:
            work->timers0[5] = 0x10;
            work->timers0[7] = 0x10;
            break;
        case 40:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_079_bodyPos);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 44:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 0xFF, 0xFF, 0xFF, 0xFF);
            work->timers0[1] = 0;
            break;
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            work->pos94.vx = fx_079_bodyPos.vx;
            work->pos94.vy = fx_079_bodyPos.vy + fxm_easeLerp(0, 8, work->timers0[2], 0, -0x200);
            work->pos94.vz = fx_079_bodyPos.vz;
        }
        if (work->timers0[4] != 0) {
            work->timers0[4]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x5C, 0x18A);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x50, 0x143);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[4], 0x66, 0x199);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x18A, 0);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x143, 0);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, 0x10, work->timers0[5], 0x199, 0);
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            if (fx_079_unitHeight > 0x200) {
                scale.vx = scale.vy = scale.vz =
                    fxm_easeLerp(2, 0x10, work->timers0[6], fx_079_unitScale.vy, fx_079_unitScale.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vy = fx_079_homePos.vy + fxm_easeLerp(2, 0x10, work->timers0[6], 0, -0x200);
            pos.vx = fx_079_homePos.vx;
            pos.vz = fx_079_homePos.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[8] != 0) {
            work->timers0[8]--;
            pos.vy = fx_079_homePos.vy - 0x200;
            pos.vx = fx_079_homePos.vx;
            pos.vz = fx_079_homePos.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[7] != 0) {
            if (--work->timers0[7] != 0) {
                if (fx_079_unitHeight > 0x200) {
                    scale.vx = scale.vy = scale.vz =
                        fxm_easeLerp(2, 0x10, work->timers0[7], fx_079_unitScale.vy >> 1, fx_079_unitScale.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = fx_079_homePos.vy + fxm_easeLerp(2, 0x10, work->timers0[7], -0x200, 0);
                pos.vx = fx_079_homePos.vx;
                pos.vz = fx_079_homePos.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&fx_079_unitScale);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_curBodyPos);
        fx_079_emitPos.vx = fx_079_curBodyPos.vx;
        fx_079_emitPos.vy = fx_079_curBodyPos.vy;
        fx_079_emitPos.vz = fx_079_curBodyPos.vz;
        if (fx_079_emitter != NULL && fx_079_emitter->state == BTLFX_EMIT_DONE) {
            fx_079_emitter = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)fx_079_emitter, (s32)&fx_079_emitPos);
        if (work->timers0[1] != 0) {
            work->pos94.vx = fx_079_curBodyPos.vx;
            work->pos94.vy = fx_079_curBodyPos.vy;
            work->pos94.vz = fx_079_curBodyPos.vz;
            ex->btlfx_aimAngles((s16 *)&fx_079_curBodyPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, 1);
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            for (i = 0; i < 5; i++) {
                tr = &fx_079_strips[i];
                if (tr->timer11C > 0) {
                    tr->timer11C--;
                    tr->unk0 = tr->unk2 = n - 1;
                    tr->unk1 = 0;
                    cur.vx = fx_079_bonePos.vx << 12;
                    cur.vy = fx_079_bonePos.vy << 12;
                    cur.vz = fx_079_bonePos.vz << 12;
                    d.vx = ((work->pos94.vx + tr->endX5C - fx_079_bonePos.vx) << 12) / n;
                    d.vy = ((work->pos94.vy + tr->endY60 - fx_079_bonePos.vy) << 12) / n;
                    d.vz = ((work->pos94.vz + tr->endZ64 - fx_079_bonePos.vz) << 12) / n;
                    strip = (u8 *)tr;
                    pt = tr->pts4;
                    for (j = 0; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->offsetY6C[j] + (cur.vy >> 12);
                        pt->unk6 = tr->timer11C << 2;
                        cur.vx += d.vx;
                        cur.vy += d.vy;
                        cur.vz += d.vz;
                        tr->offsetY6C[j] += tr->speedYC4[j];
                    }
                    if (tr->timer11C < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, frame, 0x1000,
                                            tr->timer11C << 10, 1, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), 0, NULL, frame, 0x1000,
                                            0x1000, 1, strip, 0);
                    }
                } else if (tr->timer11C < 0) {
                    tr->timer11C++;
                } else if (work->timers0[0] >= 5) {
                    tr->timer11C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    a = ex->btlfx_rand() & 0xFFF;
                    b = ex->btlfx_rand() & 0xFFF;
                    r = (ex->math_cos(a) * work->models40[0].scaleC.vx) >> 12;
                    tr->endY60 = r;
                    tr->endX5C = (ex->math_sin(b) * abs(r)) >> 12;
                    tr->endZ64 = (ex->math_cos(b) * abs(r)) >> 12;
                    tr->speedYC4[0] = 0;
                    tr->offsetY6C[0] = 0;
                    for (; j < n - 1; j++) {
                        w2 = j * 8 + 16;
                        w1 = -w2;
                        tr->offsetY6C[j] = ex->btlfx_randRange(-(j * 16) - 32, j * 16 + 32);
                        tr->speedYC4[j] = ex->btlfx_randRange(w1, w2);
                    }
                    tr->speedYC4[j] = 0;
                    tr->offsetY6C[j] = 0;
                }
            }
        }
        if (work->timers0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->timers0[9]);
            work->timers0[9]++;
        }
        return frame >= 0x5A;
    }
    return 0;
}
