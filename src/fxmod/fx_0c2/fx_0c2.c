#include "fxmod/fx_0c2.h"

s32 fx_0c2_op80(s32 mode, Fx0c2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E847C = env;
        ex->func_800DC8AC(0x10, 0x17, &bone);
        ex->func_800CFE88((MATRIX *)&bone, 0x64, (s32)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

s32 func_801E7804(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E847C)->btlfx_lerpVec((s16 *)&D_801E8484, (s16 *)&D_801E848C, (frame << 12) / 9, (s16 *)pos);
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c2_op81(s32 mode, Fx0c2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        s32 n = *arg4;
        work->unk4C = n;
        D_801E847C = env;
        if (n >= env->targets->targetCount) {
            func_801E8420(env, 7, 1);
            return 1;
        }
        func_801E8420(env, 7, 0);
        func_801E8298(0x10, 0x1C);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitBodyPos(work->unk4C, 0, (s16 *)&work->unk1C);
        ex->btlfx_getUnitRot(work->unk4C, (s16 *)&work->unk24);
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            ex->func_800DC818(0x10, 0x17, (u16 *)&work->unk34);
            D_801E8484.vx = work->unk34.vx;
            D_801E8484.vy = work->unk34.vy;
            D_801E8484.vz = work->unk34.vz;
            ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
            ex->func_800CFD78((s16 *)&work->unkC, (s16 *)&work->unk14, 0x12C, (s16 *)&work->unk2C);
            ex->btlfx_aimAngles((s16 *)&work->unkC, (s16 *)&work->unk1C, (s16 *)&ang);
            ex->func_800CFD78((s16 *)&work->unk1C, (s16 *)&ang, 0, (s16 *)&D_801E848C);
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk2C);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&D_801E8494);
            if (work->unk4 != NULL) {
                ((EffMotion *)work->unk4->unk0)->unk8 = func_801E7804;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            ex->btlfx_aimAngles((s16 *)&work->unk34, (s16 *)&work->unk1C, (s16 *)&work->unk44);
            work->unk3C.vx = work->unk44.vx;
            work->unk3C.vy = work->unk44.vy;
            work->unk3C.vz = work->unk44.vz;
        }
        if (frame == 0xD) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk1C);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(work->unk4C, 0x80);
            }
        }
        switch (frame) {
        case 5:
            ex->func_800E9930(7, entry->unk8, -1);
            ex->func_800E9930(8, entry->unk8, -1);
            ex->func_800E9930(9, entry->unk8, -1);
            break;
        case 0xD:
            ex->func_800E9930(0xE, entry->unk8, -1);
            ex->func_800E9930(0xF, entry->unk8, -1);
            ex->func_800E9930(0x10, entry->unk8, -1);
            break;
        }
        if (frame >= 4) {
            if (work->unk0 != NULL) {
                if (work->unk0->unk30 == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk5C.vx = work->unk3C.vx;
                    work->unk0->unk5C.vy = work->unk3C.vy;
                    work->unk0->unk5C.vz = work->unk3C.vz;
                }
            }
            if (work->unk4 != NULL) {
                if (work->unk4->unk30 == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk44.vx;
                    work->unk4->unk5C.vy = work->unk44.vy;
                    work->unk4->unk5C.vz = work->unk44.vz;
                }
            }
        }
        if (func_801E8304(0x10)) {
            func_801E8298(0x10, 9);
        }
        if (frame >= 0x14) {
            func_801E8420(env, 7, 1);
            return 1;
        }
    }
    return 0;
}

s32 fx_0c2_op82(s32 mode, Fx0c2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        D_801E847C = env;
        work->unk8.vz = env->targets->centerZ;
    } else {
        frame = entry->unk0;
        return func_801E7EC4(0x10, frame, 0xF, &work->unk8, &work->unk0) != 0;
    }
    return 0;
}

s32 fx_0c2_op83(s32 mode, Fx0c2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        s32 n = *arg4;
        work->unk10 = n;
        D_801E847C = env;
        if (n >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(n, (s16 *)&work->unk8);
    } else {
        frame = entry->unk0;
        return func_801E8040(0x10, work->unk10, frame, 4, 0, 1, &work->unk0) != 0;
    }
    return 0;
}

s32 func_801E7EC4(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E847C);
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

s32 func_801E8040(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E847C);
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

void func_801E8298(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E847C);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E8304(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E847C);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E8388(s32 unit) {
    if (BTLFX_EXPORTS(D_801E847C)->btlfx_isUnitActive(unit)) {
        func_801E8298(unit, 2);
    }
}

void func_801E83D4(s32 unit) {
    if (BTLFX_EXPORTS(D_801E847C)->btlfx_isUnitActive(unit)) {
        func_801E8298(unit, 3);
    }
}

void func_801E8420(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_0c2/nonmatchings/fx_0c2", fx_0c2_headerPtr);
