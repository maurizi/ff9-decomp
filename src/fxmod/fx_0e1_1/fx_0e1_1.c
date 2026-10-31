#include "fxmod/fx_0e1_1.h"

s32 fx_0e1_1_op80(s32 mode, Fx0e11Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 t;
    s32 c;
    s32 d;
    u16 b;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        work->pos8.vx = fx_0e1_1_origin.vx;
        work->pos8.vy = fx_0e1_1_origin.vy;
        work->pos8.vz = fx_0e1_1_origin.vz;
        fx_0e1_1_env = env;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
        if (p != NULL) {
            p->otZ = 0x20;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (p != NULL) {
            p->otZ = 0x100;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
        if (p != NULL) {
            p->otZ = 0x12C;
        }
        work->unk10 = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame >= 0x24) {
                t = ((frame - 0x24) * 255) / 16;
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 1, t, t, t);
            }
            if (frame < 0x14) {
                c = ex->math_cos((frame << 10) / 20);
                env->otZ = 0x80;
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->pos8, frame, c);
                env->otZ = 0;
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), frame);
            if (frame >= 0x34) {
                ex->btlfx_initParticleSystem();
                v.vx = fx_0e1_1_camPos.vx;
                v.vy = fx_0e1_1_camPos.vy - 0x800;
                v.vz = fx_0e1_1_camPos.vz;
                work->camPos24.vx = v.vx;
                work->camPos24.vy = v.vy;
                work->camPos24.vz = v.vz;
                env->camera->points[2].vx = v.vx;
                env->camera->points[2].vy = v.vy;
                env->camera->points[2].vz = v.vz;
                work->emit30 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                work->emit2C = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos1C);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                work->pos1C.vx = v.vx;
                work->pos1C.vy = v.vy;
                work->pos1C.vz = v.vz;
                work->pos1C.vy -= 0x300;
                work->swayAmp34 = 0x200;
                work->alpha3C = 0x1000;
                work->state0 = 1;
                work->fadeOut38 = 0;
                BTLFX_COPY_SVEC(&work->pos14, &work->pos1C);
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame >= 0x35 && env->state->partyReload == 0) {
                work->fadeOut38 = 1;
            }
            work->swayAmp34 -= 8;
            if (work->swayAmp34 < 0) {
                work->swayAmp34 = 0;
            }
            if (frame < 0x41) {
                d = ex->math_cos(frame << 5);
                b = fx_0e1_1_camPos.vy;
                d = ((((d * 0x800) >> 12) + 0x1000) >> 1) - 0x200;
                work->camPos24.vy = b - d;
            }
            work->pos1C.vx = work->camPos24.vx;
            work->pos1C.vy = work->camPos24.vy;
            work->pos1C.vz = work->camPos24.vz;
            env->camera->points[2].vx = work->camPos24.vx;
            env->camera->points[2].vy = work->camPos24.vy;
            env->camera->points[2].vz = work->camPos24.vz;
            env->camera->points[2].vx += (ex->math_sin(work->frame4 << 7) * work->swayAmp34) >> 12;
            if (frame < 0x18) {
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 1, 0xFF - (frame * 255) / 24, 0xFF - (frame * 255) / 24,
                                         0xFF - (frame * 255) / 24);
            }
            if (frame < 0x20) {
                ex->btlfx_drawRippleGrid((s32)&env->ot[2], -frame * 2, 4, 1, 0x78, 1, &fx_0e1_1_rippleRgb, 0x18, work->frame4 << 8, 0x100, 0);
            }
            if (work->emit2C->state == BTLFX_EMIT_DONE) {
                work->alpha3C = 0;
            }
            if (work->fadeOut38 != 0) {
                if (work->alpha3C > 0) {
                    work->alpha3C -= 0x40;
                    if (work->emit2C != NULL) {
                        work->emit2C->alpha = work->alpha3C;
                    }
                    if (work->emit30 != NULL) {
                        work->emit30->alpha = work->alpha3C;
                    }
                } else {
                    work->alpha3C = 0;
                    ex->btlfx_stopEmitter(work->emit2C);
                    ex->btlfx_stopEmitter(work->emit30);
                    work->state0 = 0;
                    return 1;
                }
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_0e1_1_env = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e1_1/nonmatchings/fx_0e1_1", fx_0e1_1_headerPtr);
