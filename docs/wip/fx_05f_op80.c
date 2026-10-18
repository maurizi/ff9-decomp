/* Draft of fx_05f op80, the twin of fx_016 op80 (see docs/wip/fx_016_op80.c). Status: ~117 aligned
 * mismatches, register allocation and scheduling. Fix fx_016 first and port the result. */
#include "fxmod/fx_05f.h"

s32 fx_05f_op80(s32 mode, Fx05fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 a;
    s32 m;
    s32 v;
    s32 q;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unkC.vx = env->unkC->unk38;
        work->unkC.vy = env->unkC->unk3A;
        work->unkC.vz = env->unkC->unk3C;
        D_801E8320 = env;
        work->unkC.vy = 0;
        BTLFX_COPY_SVEC(&work->unk4, &work->unkC);
        work->unk4.vy -= 0x200;
        ex->btlfx_initEffModels(D_801E8330, 8);
        work->unk14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 6), 0x39, 0x3D00, 0, 0x80);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x39, 0x3D00, 0, 0x80);
        work->unk1C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0x39, 0x3D00, 0, 0x80);
        work->unk20 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x39, 0x3D00, 0, 0x80);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x39, 0x3D00, 0, 0x80);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x3B, 0x3D01, 0, 0x80);
        work->unk2C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0x3B, 0x3D01, 0, 0x80);
        work->unk30 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 9), 0x39, 0x3D00, 0, 0x80);
    } else {
    frame = entry->unk0;
    if (work->unk0 == 0) {
    if (frame < 24) {
        a = (frame << 12) / 24;
        pos.vx = work->unkC.vx;
        pos.vy = work->unkC.vy;
        pos.vz = work->unkC.vz;
        v = ex->math_sin(a >> 2) / 10 + 0x333;
        rot.vz = 0;
        rot.vx = 0;
        m = (0x1000 - a) * 4;
        rot.vy = -frame * 0x20;
        scale.vy = v / 2;
        scale.vz = v;
        scale.vx = v;
        if (m > 0x3000) {
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk14, work->unk18, m - 0x3000);
        } else if (m > 0x2000) {
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk18, work->unk1C, m - 0x2000);
        } else if (m > 0x1000) {
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk1C, work->unk20, m - 0x1000);
        } else {
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk20, work->unk24, m);
        }
        scale.vz = scale.vx = (scale.vx * 5) >> 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
        a >>= 2;
        ex->Hi_ModifyEffModelRGB(work->unk30, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                 (ex->math_sin(a) >> 5) - 0x80);
    } else {
        q = ((frame - 24) << 12) / 26 >> 2;
        pos.vx = work->unkC.vx;
        pos.vy = work->unkC.vy;
        scale.vz = 0x4CC;
        scale.vx = 0x4CC;
        scale.vy = 0x266;
        rot.vz = 0;
        rot.vx = 0;
        rot.vy = -frame * 0x20;
        pos.vz = work->unkC.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80,
                                 (ex->math_cos(q) >> 5) - 0x80);
        scale.vz = scale.vx = (scale.vx * 5) >> 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
        ex->Hi_ModifyEffModelRGB(work->unk30, (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80,
                                 (ex->math_cos(q) >> 5) - 0x80);
    }
    scale.vz = scale.vx = (scale.vx * 4) / 5;
    ex->func_800E6624(frame, 0x240, 0x180, 0x10, 0x40, D_801E8278[frame % 23 + 1].unk0,
                      D_801E8278[frame % 23 + 1].unk2);
    if (frame >= 24) {
        pos.vx = work->unkC.vx;
        pos.vy = work->unkC.vy;
        scale.vz = 0xE00;
        scale.vx = 0xE00;
        pos.vz = work->unkC.vz;
        a = ((frame - 24) << 12) / 26;
        scale.vy = ex->math_sin(a >> 2) / 4 + 0x400;
        rot.vy = -frame * 0x40;
        rot.vz = 0;
        rot.vx = 0;
        ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk28, work->unk2C, 0x1000 - a);
        a >>= 1;
        ex->Hi_ModifyEffModelRGB(work->unk28, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                 (ex->math_sin(a) >> 5) - 0x80);
    }
    if (frame == 16) {
        p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    }
    if (frame == 1) {
        p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x40);
        }
    }
    if (frame >= 50) {
        ex->btlfx_clearEffModels();
        return 1;
    }
    }
    }
    return 0;
}
