#include "fxmod/fx_1a7.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[0], D_801E8990[0], frame, frames);
}

s32 func_801E7748(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[1], D_801E8990[1], frame, frames);
}

s32 func_801E778C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[2], D_801E8990[2], frame, frames);
}

s32 func_801E77D0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[3], D_801E8990[3], frame, frames);
}

s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[4], D_801E8990[4], frame, frames);
}

s32 func_801E7858(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[5], D_801E8990[5], frame, frames);
}

s32 func_801E789C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[6], D_801E8990[6], frame, frames);
}

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[7], D_801E8990[7], frame, frames);
}

s32 func_801E7924(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7968(e, pos, D_801E8950[8], D_801E8990[8], frame, frames);
}

s32 func_801E7968(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8940);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C0, (s16 *)&D_801EA5D0, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E7AB4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[0], D_801E89B0[0], frame, frames);
}

s32 func_801E7AF8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[1], D_801E89B0[1], frame, frames);
}

s32 func_801E7B3C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[2], D_801E89B0[2], frame, frames);
}

s32 func_801E7B80(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[3], D_801E89B0[3], frame, frames);
}

s32 func_801E7BC4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[4], D_801E89B0[4], frame, frames);
}

s32 func_801E7C08(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[5], D_801E89B0[5], frame, frames);
}

s32 func_801E7C4C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[6], D_801E89B0[6], frame, frames);
}

s32 func_801E7C90(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[7], D_801E89B0[7], frame, frames);
}

s32 func_801E7CD4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return func_801E7D18(e, pos, D_801E8970[8], D_801E89B0[8], frame, frames);
}

s32 func_801E7D18(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8940);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C8, (s16 *)&D_801EA5D8, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a7_op80(s32 mode, Fx1a7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    MATRIX m2;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E8940 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk2C);
        D_801EA5D0.vx = work->unk2C.vx;
        D_801EA5D0.vy = work->unk2C.vy;
        D_801EA5D0.vz = work->unk2C.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801EA5E0);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E89D0;
            work->unk0->unk12 = 0x10;
            work->unk0->unkC = 0x60;
        }
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801EA5E0);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x190, (s32)&work->unk34);
        if (frame == 0x32) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&m2);
            ex->btlfx_matrixPointAhead(&m2, 0xC8, (s32)&D_801EA5C0);
            D_801EA5C0.vy -= 0xC8;
        }
        if (frame >= 0x32) {
            t = frame - 0x32;
            for (i = 0; i < 9; i++) {
                if (t == D_801E8874[i]) {
                    work->unk8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8860[i]), (s32)&D_801EA5E0);
                    if (work->unk8[i] != NULL) {
                        work->unk8[i]->unk14 = (s32)D_801E8FD0[i];
                        work->unk8[i]->unk12 = 1;
                        work->unk8[i]->unkC = 0x38;
                    }
                    if (work->unk8[i] != NULL) {
                        ((EffMotion *)work->unk8[i]->file)->unk8 = D_801E88B0[i];
                    }
                    D_801E8950[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    D_801E8990[i] = i * 10;
                } else if (t >= D_801E8874[i]) {
                    D_801E8950[i] += 0x200;
                    D_801E8990[i] += (i / 2 + 1) * 8;
                }
            }
        }
        if (frame >= 0 && frame < 0x18) {
            if (work->unk0 != NULL && work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk0, (s32)&work->unk34);
            if (work->unk4 != NULL && work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk4, (s32)&work->unk34);
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1a7_op81(s32 mode, Fx1a7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 i;
    s32 k;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        D_801E8940 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk34);
        D_801EA5D8.vx = work->unk34.vx;
        D_801EA5D8.vy = work->unk34.vy;
        D_801EA5D8.vz = work->unk34.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk3C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk44);
        D_801EA5C8.vx = work->unk3C.vx;
        D_801EA5C8.vy = work->unk3C.vy;
        D_801EA5C8.vz = work->unk3C.vz;
        work->unk54.vx = work->unk3C.vx;
        work->unk54.vy = work->unk3C.vy;
        work->unk54.vz = work->unk3C.vz;
        work->unk54.vy -= 0xC8;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk54);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E91D0;
            work->unk0->unk12 = 0x16;
            work->unk0->unkC = 0x38;
        }
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk44);
        ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->unk34, (s16 *)&work->unk4C);
        ex->btlfx_pointAhead((s16 *)&work->unk34, (s16 *)&work->unk4C, -0x190, (s16 *)&work->unk5C);
        D_801EA5D8.vx = work->unk5C.vx;
        D_801EA5D8.vy = work->unk5C.vy;
        D_801EA5D8.vz = work->unk5C.vz;
    } else {
        frame = entry->frame;
        if (frame >= 0x14) {
            t = frame - 0x14;
            for (i = 0; i < 9; i++) {
                if (t == D_801E889C[i]) {
                    work->unk8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8888[i]), (s32)&D_801EA5E0);
                    if (work->unk8[i] != NULL) {
                        work->unk8[i]->unk14 = (s32)D_801E96A0[i];
                        work->unk8[i]->unk12 = 1;
                        work->unk8[i]->unkC = 0xB0;
                    }
                    if (work->unk8[i] != NULL) {
                        ((EffMotion *)work->unk8[i]->file)->unk8 = D_801E88D4[i];
                    }
                    D_801E8970[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    D_801E89B0[i] = ex->btlfx_rand() % 200 + 0x64;
                } else if (t >= D_801E889C[i]) {
                    D_801E8970[i] += 0x80;
                    k = 0x78;
                    if (t - D_801E889C[i] >= 5) {
                        k = 0x28;
                    }
                    if (ex->btlfx_rand() & 1) {
                        D_801E89B0[i] += ex->btlfx_rand() % k;
                    } else {
                        D_801E89B0[i] -= ex->btlfx_rand() % k;
                    }
                }
            }
        }
        if (frame == 0x1C) {
            work->unk2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk34);
            if (work->unk2C != NULL) {
                work->unk2C->unk14 = (s32)D_801E9CD0;
                work->unk2C->unk12 = 0xD;
                work->unk2C->unkC = 0xB0;
            }
            work->unk30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->unk5C);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a7/nonmatchings/fx_1a7", fx_1a7_headerPtr);
