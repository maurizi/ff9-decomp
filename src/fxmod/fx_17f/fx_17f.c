#include "fxmod/fx_17f.h"

void func_801E7704(SVECTOR *v, s32 i) {
    v->vy -= D_801E7C60[i];
    if (v->vy < 0) {
        v->vy = 0;
    }
    if (v->vy < 0x1E) {
        v->vx = v->vx * 0x5C / 90;
        v->vz = (s16)(v->vz - 0xC8) * 0x5C / 90 + 0xC8;
    }
}

s32 fx_17f_op80(s32 mode, Fx17fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR ang;
    s32 frame;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7C50 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk20);
        if (ex->btlfx_isEnemyUnit(0x10) != 0) {
            ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
            ex->btlfx_getUnitScale(0x10, (VECTOR *)work->unk0);
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
            ex->func_800E8980(0);
            ex->btlfx_fadeUnitById(0x10, -1);
            work->unk1C = (BtlUnit9C8 *)ex->btlfx_swapModelData(0xE, D_801E88E0);
            ex->btlfx_fadeUnitById(0x11, 0x1000);
            ex->btlfx_fadeUnitById(0x11, -2);
            ex->btlfx_setUnitRot(0x11, (s16 *)&work->unk14);
            ex->btlfx_setUnitScale(0x11, work->unk0);
            ex->btlfx_aimAngles((s16 *)&work->unkC, (s16 *)&work->unk20, (s16 *)&ang);
            ex->func_800CFD78((s16 *)&work->unkC, (s16 *)&ang, -0x104, (s16 *)&work->unkC);
            ex->btlfx_setUnitPos(0x11, (s16 *)&work->unkC);
        }
        work->unk20.vx = (work->unk20.vx + work->unkC.vx) / 2;
        work->unk20.vy = (work->unk20.vy + work->unkC.vy) / 2 - 0x11C;
        work->unk20.vz = (work->unk20.vz + work->unkC.vz) / 2;
        for (i = 0; i < 0x2BC; i++) {
            D_801E7C60[i] = (ex->btlfx_rand() & 7) + 4;
        }
    } else {
        frame = entry->unk0;
        ex->btl_forEachModelVertex(work->unk1C, func_801E7704);
        if (frame >= 0x37) {
            c = 0x80 - (frame - 0x37) * 8;
            if (c < 0x10) {
                c = 0x10;
            }
            ex->btlfx_fadeUnitById(0x11, c);
        }
        if (frame == 3) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        if (frame == 6) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        if (frame == 0x1C) {
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unkC);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
        }
        work->unk20.vy += 5;
        if (frame >= 0x47) {
            ex->btlfx_fadeUnitById(0x11, -1);
            ex->btlfx_swapModelData(0xE, D_801E88E0);
            work->unkC.vx = 0;
            work->unkC.vy = 0;
            work->unkC.vz = 0x3A98;
            ex->btlfx_setUnitPos(0x11, (s16 *)&work->unkC);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17f/nonmatchings/fx_17f", fx_17f_headerPtr);
