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
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk8.vx = D_801ECD70.vx;
        work->unk8.vy = D_801ECD70.vy;
        work->unk8.vz = D_801ECD70.vz;
        D_801ECDD0 = env;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x20;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x100;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
        if (p != NULL) {
            p->unk22 = 0x12C;
        }
        work->unk10 = 0;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame >= 0x24) {
                t = ((frame - 0x24) * 255) / 16;
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 1, t, t, t);
            }
            if (frame < 0x14) {
                c = ex->math_cos((frame << 10) / 20);
                env->otZ = 0x80;
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->unk8, frame, c);
                env->otZ = 0;
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), frame);
            if (frame >= 0x34) {
                ex->btlfx_initParticleSystem();
                v.vx = D_801ECD78.vx;
                v.vy = D_801ECD78.vy - 0x800;
                v.vz = D_801ECD78.vz;
                work->unk24.vx = v.vx;
                work->unk24.vy = v.vy;
                work->unk24.vz = v.vz;
                env->camera->points[2].vx = v.vx;
                env->camera->points[2].vy = v.vy;
                env->camera->points[2].vz = v.vz;
                work->unk30 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk2C = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk1C.vx = v.vx;
                work->unk1C.vy = v.vy;
                work->unk1C.vz = v.vz;
                work->unk1C.vy -= 0x300;
                work->unk34 = 0x200;
                work->unk3C = 0x1000;
                work->unk0 = 1;
                work->unk38 = 0;
                BTLFX_COPY_SVEC(&work->unk14, &work->unk1C);
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame >= 0x35 && ((Fx0e11BattleState *)env->state)->unk22 == 0) {
                work->unk38 = 1;
            }
            work->unk34 -= 8;
            if (work->unk34 < 0) {
                work->unk34 = 0;
            }
            if (frame < 0x41) {
                d = ex->math_cos(frame << 5);
                b = D_801ECD78.vy;
                d = ((((d * 0x800) >> 12) + 0x1000) >> 1) - 0x200;
                work->unk24.vy = b - d;
            }
            work->unk1C.vx = work->unk24.vx;
            work->unk1C.vy = work->unk24.vy;
            work->unk1C.vz = work->unk24.vz;
            env->camera->points[2].vx = work->unk24.vx;
            env->camera->points[2].vy = work->unk24.vy;
            env->camera->points[2].vz = work->unk24.vz;
            env->camera->points[2].vx += (ex->math_sin(work->unk4 << 7) * work->unk34) >> 12;
            if (frame < 0x18) {
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 1, 0xFF - (frame * 255) / 24, 0xFF - (frame * 255) / 24,
                                         0xFF - (frame * 255) / 24);
            }
            if (frame < 0x20) {
                ex->btlfx_drawRippleGrid((s32)&env->ot[2], -frame * 2, 4, 1, 0x78, 1, &D_801ECD80, 0x18, work->unk4 << 8, 0x100, 0);
            }
            if (work->unk2C->state == -1) {
                work->unk3C = 0;
            }
            if (work->unk38 != 0) {
                if (work->unk3C > 0) {
                    work->unk3C -= 0x40;
                    if (work->unk2C != NULL) {
                        work->unk2C->unk20 = work->unk3C;
                    }
                    if (work->unk30 != NULL) {
                        work->unk30->unk20 = work->unk3C;
                    }
                } else {
                    work->unk3C = 0;
                    ex->btlfx_stopEmitter(work->unk2C);
                    ex->btlfx_stopEmitter(work->unk30);
                    work->unk0 = 0;
                    return 1;
                }
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801ECDD0 = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e1_1/nonmatchings/fx_0e1_1", fx_0e1_1_headerPtr);
