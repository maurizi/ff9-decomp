#include "fxmod/fx_088.h"

s32 fx_088_op80(s32 mode, Fx088Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7DA0 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->func_800DC818(0x10, 0x17, (u16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7DB0;
            work->unk0->unk12 = 0x19;
            work->unk0->unkC = 0x30;
        }
        ex->btlfx_aimAngles((s16 *)&work->unk8, &env->unkC->unk38, (s16 *)&work->unk18);
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk18.vx;
                work->unk0->unk5C.vy = work->unk18.vy;
                work->unk0->unk5C.vz = work->unk18.vz;
            }
        }
        if (frame == 0xC) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&env->unkC->unk38);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = 0x20;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_088_op81(s32 mode, Fx088Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        work->unk8.vx = env->unkC->unk38;
        work->unk8.vy = env->unkC->unk3A;
        D_801E7DA0 = env;
        work->unk8.vz = env->unkC->unk3C;
    } else {
        frame = entry->unk0;
        return func_801E7984(0x10, frame, 0xF, &work->unk8, &work->unk0) != 0;
    }
    return 0;
}

s32 func_801E7984(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7DA0);
    SVECTOR rot;
    SVECTOR pos;

    ex->btlfx_getUnitPos(unit, (s16 *)&pos);
    if (frame == 0) {
        ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += delta->vx / n;
    rot.vy += delta->vy / n;
    rot.vz += delta->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

s32 func_801E7B00(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7DA0);
    SVECTOR from;
    SVECTOR rot;
    SVECTOR to;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)&from);
        ex->btlfx_getUnitPos(target, (s16 *)&to);
        ex->btlfx_aimAngles((s16 *)&from, (s16 *)&to, (s16 *)ang);
        if (relative) {
            ex->btlfx_getUnitRot(unit, (s16 *)&rot);
            ang->vx -= rot.vx;
            ang->vy -= rot.vy;
            ang->vz -= rot.vz;
            if (ang->vy >= 0x800) {
                ang->vy = 0x1000 - ang->vy;
            } else if (ang->vy < -0x7FF) {
                ang->vy = ang->vy + 0x1000;
            }
        }
        if (turnBack == 1) {
            ang->vy += 0x800;
        }
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += ang->vx / n;
    rot.vy += ang->vy / n;
    rot.vz += ang->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

INCLUDE_RODATA("asm/us/fx_088/nonmatchings/fx_088", fx_088_headerPtr);
