/* Draft of fx_1e9 fx_1e9_op80. Status: about 48 strict mismatches: register allocation in the state-1 block (s2/s4)
 * and the scale.vy statement order. The permuter only got closer by making a local short. */
#include "fxmod/fx_1e9.h"

s32 fx_1e9_op80(s32 mode, Fx1e9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 alpha;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 c;
    Pool6CEntry *p;
    s32 z;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E8564 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk20 = 0;
        work->unk4 = 0;
        D_801E8518 = 0;
        ex->btlfx_initEffModels(D_801E8574, 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xB7, 0x3D80, 0, 0);
    } else {
        alpha = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 8;
            a = t >> 2;
            s = ex->math_sin(a) * 0x10FF;
            env->unk28 = 0x7C;
            s >>= 12;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, s, 0, 1, t);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, s, 0, 1, t);
            scale.vx = scale.vz = 0x174;
            scale.vy = ex->math_sin(a) / 6;
            alpha = t;
            if (frame >= 0x10) {
                work->unk0 = 1;
                entry->unk0 = -1;
                env->unk0->pad10[0] = 0;
                ex->func_800E806C(0x10, 0);
                work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
                if (work->unk8 != NULL) {
                    work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                work->unk1C = 0;
            }
            break;
        case 1:
            t = ((++work->unk1C % 10) << 12) / 10;
            s = (ex->math_sin(t) >> 4) + 0x10FF;
            env->unk28 = 0x7C;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, s, 0, 1, 0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, 0x1000);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, s, 0, 1, 0x1000);
            scale.vx = scale.vz = 0x174;
            scale.vy = (ex->math_sin(t) >> 7) + 0x2AA;
            alpha = 0x1000;
            if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            if (frame != 0 && t == 0 && env->unk0->pad10[0] != 0) {
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
            }
            break;
        case 2:
            t = (frame << 12) / 47;
            c = 0x1000 - t;
            s = t + 0x10FF;
            env->unk28 = 0x7C;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, s, 0, 1, c);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, c);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, s, 0, 1, c);
            if (frame < 0x10) {
                a = (frame << 8) >> 2;
                alpha = ex->math_cos(a);
                scale.vx = scale.vz = (ex->math_sin(a) >> 3) + 0x174;
                scale.vy = ex->math_cos(a) / 6;
            }
            if (frame == 3) {
                ex->func_800E8554(0, 0x10);
                work->unk20 = 0;
            }
            if (frame >= 0x2F) {
                D_801E8518 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            rot.vx = 0;
            pos.vz = work->unkC.vz;
            rot.vz = 0;
            z = work->unk4;
            rot.vy = z << 5;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (alpha >> 5) - 0x80, (alpha >> 5) - 0x80, (alpha >> 5) - 0x80);
            scale.vy >>= 1;
            scale.vx = scale.vz = scale.vx * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (alpha >> 5) - 0x80, (alpha >> 5) - 0x80, (alpha >> 5) - 0x80);
        }
        ex->func_800E6624(work->unk4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
        work->unk4++;
    }
    return 0;
}
