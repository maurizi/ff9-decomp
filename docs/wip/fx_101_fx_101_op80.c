/* Draft of fx_101_op80 (module fx_101). Status: 358/591 aligned instructions with ccprobe; register allocation in the long middle part.
 * Declarations it needs that are not yet in include/fxmod/fx_101.h are repeated below. */
#include "fxmod/fx_101.h"

s32 fx_101_op80(s32 mode, Fx101Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 b;
    s32 frame;
    s32 t0;
    s32 a0;
    s32 t1;
    s32 a1;
    s32 t2;
    s32 a2;
    s32 a;
    s32 c;
    s32 n;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E85FC = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk20 = 0;
        work->unk4 = 0;
        D_801E85B0 = 0;
        ex->btlfx_initEffModels(D_801E860C, 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xB7, 0x3D80, 0, 0);
    } else {
    b = 0;
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        t0 = frame << 8;
        a0 = (ex->math_sin(t0 >> 2) * 0x10FF) >> 12;
        env->unk28 = 0x7C;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a0, 0, 1, t0);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, t0);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a0, 0, 1, t0);
        scale.vx = scale.vz = 0x174;
        scale.vy = ex->math_sin(t0 >> 2) / 6;
        b = t0;
        if (frame >= 0x10) {
            work->unk0 = 1;
            entry->unk0 = -1;
            ((Fx101BattleState *)env->unk0)->unk10 = 0;
            ex->func_800E806C(0x10, 0);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            work->unk1C = 0;
        }
        break;
    case 1:
        n = ++work->unk1C;
        t1 = ((n % 10) << 12) / 10;
        a1 = (ex->math_sin(t1) >> 4) + 0x10FF;
        env->unk28 = 0x7C;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a1, 0, 1, 0x1000);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, 0x1000);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a1, 0, 1, 0x1000);
        scale.vx = scale.vz = 0x174;
        scale.vy = (ex->math_sin(t1) >> 7) + 0x2AA;
        b = 0x1000;
        if (work->unk8 != NULL && work->unk8->unk30 == -1) {
            work->unk8 = NULL;
        }
        if (frame != 0 && t1 == 0 && ((Fx101BattleState *)env->unk0)->unk10 != 0) {
            work->unk0 = 2;
            entry->unk0 = -1;
            env->unk2C->unk52 = 0;
            ex->func_800E806C(0x10, 0x80);
            ex->func_800E3D54(work->unk8);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
        }
        break;
    case 2:
        t2 = (frame << 12) / 47;
        c = 0x1000 - t2;
        env->unk28 = 0x7C;
        a2 = t2 + 0x10FF;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a2, 0, 1, c);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, c);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a2, 0, 1, c);
        if (frame < 0x10) {
            t2 = (frame << 8) >> 2;
            b = ex->math_cos(t2);
            scale.vx = scale.vz = (ex->math_sin(t2) >> 3) + 0x174;
            scale.vy = ex->math_cos(t2) / 6;
        }
        if (frame == 3 && ex->func_800E8554(1, 0x10) == 0) {
            work->unk20 = 1;
        }
        if (work->unk20 != 0 && frame >= 0x12) {
            ex->func_800E8554(0, 0x10);
            work->unk20 = 0;
        }
        if (frame >= 0x2F) {
            D_801E85B0 = 1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    if (b != 0) {
        pos.vx = work->unkC.vx;
        pos.vy = work->unkC.vy;
        rot.vx = 0;
        pos.vz = work->unkC.vz;
        rot.vz = 0;
        rot.vy = work->unk4 << 5;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        a = (b >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk24, a, a, a);
        scale.vy >>= 1;
        scale.vx = scale.vz = scale.vx * 3 / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        ex->Hi_ModifyEffModelRGB(work->unk24, a, a, a);
    }
    ex->func_800E6624(work->unk4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
    work->unk4++;
    }
    return 0;
}
