#include "fxmod/fx_083.h"

INCLUDE_RODATA("asm/us/fx_083/nonmatchings/fx_083", fx_083_headerPtr);

s32 fx_083_op80(s32 mode, Fx083Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 tex[8];
    Pool6CEntry *e;
    s32 frame;
    s32 i;
    s32 r;
    s32 n;
    s32 t;

    if (mode == 0) {
        *arg4 = sizeof(Fx083Work);
    } else if (mode == 1) {
        fx_083_env = env;
        ex->btlfx_initEffModels(fx_083_effModels, 0x14);
        for (i = 9; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        tex[0] = tex[2] = 0xF8;
        tex[1] = 0x3DC0;
        tex[3] = 0x3D80;
        tex[4] = 0;
        tex[6] = 0;
        tex[5] = 0x80;
        tex[7] = 0;
        for (i = 0; i < 4; i++) {
            work->model28[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0xFA, 0x3DC0,
                                                        0x80, 0x80);
            work->ringRot38[i].vy = i << 10;
            work->ringRot38[i].vx = 0x100;
            work->ringRot38[i].vz = 0;
            ex->btlfx_getUnitPos(0, (s16 *)&work->emitStartPos180[i]);
        }
        work->ringScale98.vx = 0x333;
        work->ringScale98.vy = 0x333;
        work->ringScale98.vz = 0x333;
        for (i = 0; i < 5; i++) {
            work->modelB0[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 1), 0xB8, 0x3D40,
                                                        0xC0, 0);
            work->pillarOnC4[i] = 0;
            work->pillarFadeD8[i] = -1;
        }
        work->radius168 = ex->btlfx_getUnitRadius(0);
        work->height16C = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos160);
        work->pos160.vy = -0x80;
        work->pillarOnC4[0] = 1;
        work->rot140.vx = 0;
        work->rot140.vy = 0;
        work->rot140.vz = 0;
        work->fade13C = 0;
        work->orbit1C0 = 0;
        work->center158.vx = work->pos160.vx;
        work->center158.vy = work->pos160.vy - work->height16C;
        work->center158.vz = work->pos160.vz;
        work->pillarScale148.vx = work->radius168;
        work->pillarScale148.vz = work->radius168;
        work->radius168 += 0x100 + (work->radius168 >> 2);
        /* Store order AC, vy, vx, vz: sets the local-alloc lifetimes so vx takes a3 (original register choice). */
        work->ringHeightAC = work->center158.vy;
        work->pillarPosEC[0].vy = work->center158.vy;
        work->pillarPosEC[0].vx = work->center158.vx;
        work->pillarPosEC[0].vz = work->center158.vz;
        work->pillarScale148.vy = work->height16C / 3;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            work->timers0[2] = 0x50;
            work->timers0[0] = 0x10;
            break;
        case 0x8:
            work->timers0[6] = 0x3C;
            work->fade13C = 0x10;
            break;
        case 0x10:
            work->timers0[4] = 8;
            break;
        case 0x14:
            work->timers0[3] = 0x20;
            work->timers0[7] = 0x20;
            work->emit170[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->emitStartPos180[0]);
            work->emit170[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->emitStartPos180[1]);
            work->emit170[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->emitStartPos180[2]);
            work->emit170[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->emitStartPos180[3]);
            work->timers0[8] = 0x30;
            break;
        case 0x30:
            e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos160);
            if (e != NULL) {
                e->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos160);
            if (e != NULL) {
                e->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos160);
            if (e != NULL) {
                e->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            break;
        case 0x34:
            work->timers0[1] = 0x10;
            break;
        case 0x37:
            work->pillarFadeD8[4] = 8;
            break;
        case 0x38:
            work->pillarFadeD8[3] = 8;
            break;
        case 0x39:
            work->pillarFadeD8[2] = 8;
            break;
        case 0x3A:
            work->pillarFadeD8[1] = 8;
            break;
        case 0x3B:
            work->pillarFadeD8[0] = 8;
            break;
        }
        for (i = 0; i < 4; i++) {
            work->ringRot38[i].vy += 0x40;
        }
        work->orbit1C0 += 0x10;
        if (work->timers0[3] != 0) {
            work->timers0[3]--;
            work->ringHeightAC = ex->btlfx_lerp(work->pos160.vy, work->center158.vy, work->timers0[3] << 7);
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            work->ringOffsetA8 = 0x1000 - ((ex->math_sin((0x10 - work->timers0[0]) << 6) << 12) >> 12);
        }
        if (work->timers0[1] != 0) {
            work->timers0[1]--;
            work->ringOffsetA8 = 0x1000 - ((ex->math_sin(work->timers0[1] << 6) << 12) >> 12);
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            for (i = 0; i < 4; i++) {
                work->ringPos58[i].vx =
                    work->center158.vx + ((ex->math_sin(work->ringRot38[i].vy) * (work->radius168 + work->ringOffsetA8)) >> 12);
                work->ringPos58[i].vz =
                    work->center158.vz + ((ex->math_cos(work->ringRot38[i].vy) * (work->radius168 + work->ringOffsetA8)) >> 12);
                work->ringPos58[i].vy = work->ringHeightAC - ((ex->math_sin(0x200) * work->ringOffsetA8) >> 12);
                ex->Hi_DrawEffModel(&work->ringRot38[i], &work->ringPos58[i], (s32)&work->ringScale98, work->model28[i]);
            }
            env->camera->points[0].vx = work->ringPos58[0].vx;
            env->camera->points[0].vy = work->ringPos58[0].vy;
            env->camera->points[0].vz = work->ringPos58[0].vz;
            env->camera->pointRots[0].vx = 0x100;
            env->camera->pointRots[0].vy = work->ringRot38[0].vy;
            env->camera->pointRots[0].vz = 0;
        }
        if (work->timers0[8] != 0) {
            work->timers0[8]--;
            for (i = 0; i < 4; i++) {
                t = i << 10;
                work->emitPos1A0[i].vx = work->center158.vx + ((ex->math_sin(t + work->orbit1C0) *
                                                         (work->radius168 + work->ringOffsetA8 + 0x200)) >> 12);
                work->emitPos1A0[i].vz = work->center158.vz + ((ex->math_cos(t + work->orbit1C0) *
                                                         (work->radius168 + work->ringOffsetA8 + 0x200)) >> 12);
                work->emitPos1A0[i].vy = work->ringPos58[0].vy;
                if (work->emit170[i] != NULL && work->emit170[i]->state == BTLFX_EMIT_DONE) {
                    work->emit170[i] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit170[i], (s32)&work->emitPos1A0[i]);
            }
        }
        if (work->timers0[4] != 0) {
            work->timers0[4]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, fx_083_texAnimSrc1[work->timers0[4] >> 1].x0,
                              fx_083_texAnimSrc1[work->timers0[4] >> 1].y4);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            ex->btlfx_drawScrollMove(0, 0x2C0, 0x180, 0x40, 0x40, fx_083_texAnimSrc2[work->timers0[5]].x0, fx_083_texAnimSrc2[work->timers0[5]].y4);
        } else if (frame >= 0x15 && !(ex->btlfx_rand() & 7)) {
            work->timers0[5] = 4;
        }
        if (work->timers0[7] != 0) {
            work->pillarPosEC[0].vy = work->ringHeightAC;
            if (!(work->timers0[7] & 7) && (n = work->timers0[7] >> 3) > 0) {
                work->pillarOnC4[n] = 1;
                work->pillarPosEC[n].vx = work->pillarPosEC[0].vx;
                work->pillarPosEC[n].vy = work->pillarPosEC[0].vy;
                work->pillarPosEC[n].vz = work->pillarPosEC[0].vz;
            }
            work->timers0[7]--;
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->center158, (s16 *)&work->rot140);
            work->rot140.vx = 0;
            work->rot140.vz = 0;
            for (i = 0; i < 5; i++) {
                if (work->pillarOnC4[i] != 0) {
                    ex->Hi_DrawEffModel(&work->rot140, &work->pillarPosEC[i], (s32)&work->pillarScale148, work->modelB0[i]);
                    if (work->pillarFadeD8[i] > 0) {
                        work->pillarFadeD8[i]--;
                        r = (work->pillarFadeD8[i] << 5) - 0x100;
                        ex->Hi_ModifyEffModelRGB(work->modelB0[i], r, r, r);
                        if (work->pillarFadeD8[i] == 0) {
                            work->pillarOnC4[i] = 0;
                        }
                    }
                    if (i == 0 && work->fade13C != 0) {
                        work->fade13C--;
                        r = -(work->fade13C * 0x10);
                        ex->Hi_ModifyEffModelRGB(work->modelB0[0], r, r, r);
                    }
                }
            }
        }
        if (frame >= 0x5C) {
            return 1;
        }
        ex->btlfx_drawScrollMove(0x2710 - frame * 4, 0x260, 0x100, 0x20, 0xF0, 0x240, 0x100);
    }
    return 0;
}
