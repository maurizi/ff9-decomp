#include "fxmod/fx_1a0.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[0] * 2;

    return func_801E789C(e, pos, &D_801E8238[0], t, frame, frames);
}

s32 func_801E7748(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[1] * 2;

    return func_801E789C(e, pos, &D_801E8238[1], t, frame, frames);
}

s32 func_801E778C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[2] * 2;

    return func_801E789C(e, pos, &D_801E8238[2], t, frame, frames);
}

s32 func_801E77D0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[3] * 2;

    return func_801E789C(e, pos, &D_801E8238[3], t, frame, frames);
}

s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[4] * 2;

    return func_801E789C(e, pos, &D_801E8238[4], t, frame, frames);
}

s32 func_801E7858(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = D_801E8268[5] * 2;

    return func_801E789C(e, pos, &D_801E8238[5], t, frame, frames);
}

s32 func_801E789C(Pool30Entry *e, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E7E94(path, t, &v);
    v.vx += D_801E8280.vx;
    v.vy += D_801E8280.vy;
    v.vz += D_801E8280.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a0_op80(s32 mode, Fx1a0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1a0Bone *b;
    MATRIX m;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        D_801E7F88 = env;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            for (i = 0; i < 6; i++) {
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E8288[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk22 = 0x20;
                }
            }
        }
        if (frame >= 0 && frame < 0xC) {
            b = D_801E7EF8;
            for (i = 0; i < 6; i++, b++) {
                ex->func_800DC8AC(0x10, b->unk0, (BtlBone *)&m);
                ex->func_800CFE88(&m, b->unk2, (s32)&work->unk18[i]);
                if (work->unk0[i] != NULL && work->unk0[i]->unk30 == -1) {
                    work->unk0[i] = NULL;
                }
                ex->func_800E3D20((s32)work->unk0[i], (s32)&work->unk18[i]);
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

s32 fx_1a0_op81(s32 mode, Fx1a0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        s32 a = arg4[0];
        s32 b = arg4[1];

        D_801E7F88 = env;
        work->unk14 = a;
        work->unk18 = b;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        D_801E8280.vx = work->unkC.vx;
        D_801E8280.vy = work->unkC.vy;
        D_801E8280.vz = work->unkC.vz;
        func_801E7E7C(&D_801E8238[work->unk14], (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E7F1C[work->unk14]));
    } else {
        frame = entry->unk0;
        D_801E8268[work->unk14] = frame;
        if (frame == 0) {
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E7F10[work->unk14]),
                                           (s32)&D_801E82B8[work->unk14]);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7F98[work->unk14];
                work->unk0->unk12 = 1;
                work->unk0->unkC = 0x70;
            }
            if (work->unk0 != NULL) {
                ((EffMotion *)work->unk0->unk0)->unk8 = D_801E7F28[work->unk14];
            }
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x20;
            }
            if (work->unk18 != 0) {
                work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
                work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
                if (work->unk4 != NULL) {
                    work->unk4->unk22 = 0x80;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk22 = 0x80;
                }
            }
        }
        return frame >= 0x14;
    }
    return 0;
}

s32 fx_1a0_op82(s32 mode, Fx1a0Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7F88 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk4);
    } else {
        return entry->unk0 >= 0x14;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7E7C);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7E94);

INCLUDE_RODATA("asm/us/fx_1a0/nonmatchings/fx_1a0", fx_1a0_headerPtr);
