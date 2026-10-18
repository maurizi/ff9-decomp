#include "fxmod/fx_09b.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7808(pt, pos, &D_801E9A34[0], frame, n);
}

s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7808(pt, pos, &D_801E9A34[1], frame, n);
}

s32 func_801E776C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7808(pt, pos, &D_801E9A34[2], frame, n);
}

s32 func_801E77A0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7808(pt, pos, &D_801E9A34[3], frame, n);
}

s32 func_801E77D4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7808(pt, pos, &D_801E9A34[4], frame, n);
}

s32 func_801E7808(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_09b_op80(s32 mode, Fx09bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        s32 n = *arg4;
        D_801E8164 = env;
        work->unk6C = n;
        if (n >= 5) {
            return 1;
        }
        if (n == 0) {
            ex->btlfx_initEffModels(D_801E8174, 5);
        }
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk54);
        work->unk64.vx = work->unk54.vx;
        work->unk64.vy = work->unk54.vy;
        work->unk64.vz = work->unk54.vz;
        if (ex->btlfx_rand() & 1) {
            work->unk64.vx += ex->btlfx_rand() % 100;
        } else {
            work->unk64.vx -= ex->btlfx_rand() % 100;
        }
        if (ex->btlfx_rand() & 1) {
            work->unk64.vy += ex->btlfx_rand() % 100;
        } else {
            work->unk64.vy -= ex->btlfx_rand() % 100;
        }
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk3C);
        work->unk5C.vx = work->unk3C.vx;
        work->unk5C.vy = work->unk3C.vy;
        work->unk5C.vz = work->unk3C.vz;
        env->unk2C->unk0[0].vx = work->unk3C.vx;
        env->unk2C->unk0[0].vy = work->unk3C.vy;
        env->unk2C->unk0[0].vz = work->unk3C.vz;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk14.vz = 0;
        work->unk1C.vx = 0;
        work->unk1C.vy = 0;
        work->unk1C.vz = 0;
        work->unk2C.vx = 0x1000;
        work->unk2C.vy = 0x1000;
        work->unk2C.vz = 0x1000;
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 1);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk3C);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E80F4[work->unk6C]),
                                       (s32)&D_801E9A64[work->unk6C]);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E8564[work->unk6C];
            work->unk4->unk12 = 0xE;
            work->unk4->unkC = 0x4C;
        }
        if (work->unk4 != NULL) {
            ((EffMotion *)work->unk4->unk0)->unk8 = D_801E8108[work->unk6C];
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 2:
            ex->btlfx_aimAngles((s16 *)&work->unk3C, (s16 *)&work->unk64, (s16 *)&ang);
            ex->func_800CFD78((s16 *)&work->unk64, (s16 *)&ang, -0xC8, (s16 *)&work->unk44);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk44);
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 4:
            work->unk4C.vx = work->unk64.vx;
            work->unk4C.vy = work->unk64.vy;
            work->unk4C.vz = work->unk64.vz;
            work->unk4C.vy = 0;
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk64);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk4C);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 6:
            ex->func_800E9930(0x13, entry->unk8, -1);
            ex->func_800E9930(0x14, entry->unk8, -1);
            ex->func_800E9930(0x15, entry->unk8, -1);
            break;
        case 8:
            ex->func_800E3D54(work->unk4);
            break;
        }
        if ((u32)frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->unk5C, (s16 *)&work->unk64, (frame << 12) / 5,
                              (s16 *)&D_801E9A34[work->unk6C]);
            work->unk1C.vx = D_801E9A34[work->unk6C].vx;
            work->unk1C.vy = D_801E9A34[work->unk6C].vy;
            work->unk1C.vz = D_801E9A34[work->unk6C].vz;
            ex->Hi_DrawEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, work->unk70);
            if (frame == 5 && work->unk6C != 4) {
                func_801E805C(0);
            }
        }
        if (frame >= 0x28) {
            if (work->unk6C == 4) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            return 1;
        }
    }
    return 0;
}

void func_801E7F6C(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8164);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E7FD8(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8164);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E805C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8164)->btlfx_isUnitActive(unit)) {
        func_801E7F6C(unit, 2);
    }
}

void func_801E80A8(s32 unit) {
    if (BTLFX_EXPORTS(D_801E8164)->btlfx_isUnitActive(unit)) {
        func_801E7F6C(unit, 3);
    }
}

INCLUDE_RODATA("asm/us/fx_09b/nonmatchings/fx_09b", fx_09b_headerPtr);
