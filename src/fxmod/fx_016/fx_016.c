#include "fxmod/fx_016.h"

s32 fx_016_op80(s32 mode, Fx016Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 r;
    s32 a;
    s32 m;
    s32 v;
    s32 q;
    s32 w;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E8358 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        ex->btlfx_initEffModels(D_801E8368, 8);
        work->unk14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0x39, 0x3D00, 0, 0x80);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x39, 0x3D00, 0, 0x80);
        work->unk1C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x39, 0x3D00, 0, 0x80);
        work->unk20 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x39, 0x3D00, 0, 0x80);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x39, 0x3D00, 0, 0x80);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x3B, 0x3D01, 0, 0x80);
        work->unk2C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0x3B, 0x3D01, 0, 0x80);
        work->unk30 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 9), 0x39, 0x3D00, 0, 0x80);
        r = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (r < 0x400) {
            r = 0x6A4;
        }
        if (r > 0x1800) {
            r = 0x1800;
        }
        D_801E89A8 = r;
        work->unk34 = r;
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            if (frame < 24) {
                a = (frame << 12) / 24;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                v = (((ex->math_sin(a >> 2) / 10) + 0x199) * work->unk34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                m = (0x1000 - a) * 4;
                rot.vy = -frame * 0x20;
                scale.vy = (v * 3) >> 2;
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
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
                ex->Hi_ModifyEffModelRGB(work->unk30, (ex->math_sin(a >> 2) >> 5) - 0x80,
                                         (ex->math_sin(a >> 2) >> 5) - 0x80, (ex->math_sin(a >> 2) >> 5) - 0x80);
            } else {
                /* m (not q) takes the division: the copy q = m >> 2 into a block-local pseudo (s2) gives m a
                 * global-alloc preference for s2, which it also gets in the frame < 24 branch. The scale value
                 * here is its own block-local variable w. */
                m = ((frame - 24) << 12) / 26;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                w = (work->unk34 * 0x332) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                scale.vz = w;
                scale.vx = w;
                scale.vy = (w * 3) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
                q = m >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_cos(q) >> 5) - 0x80,
                                         (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
                ex->Hi_ModifyEffModelRGB(work->unk30, (ex->math_cos(q) >> 5) - 0x80,
                                         (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80);
            }
            ex->btlfx_drawScrollMove(frame, 0x240, 0x180, 0x10, 0x40, D_801E82B0[frame % 23 + 1].unk0,
                              D_801E82B0[frame % 23 + 1].unk2);
            if (frame >= 24) {
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                scale.vx = scale.vz = (work->unk34 << 11) >> 12;
                a = ((frame - 24) << 12) / 26;
                scale.vy = (((ex->math_sin(a >> 2) / 4) + 0x2AA) * work->unk34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x40;
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->unk28, work->unk2C, 0x1000 - a);
                ex->Hi_ModifyEffModelRGB(work->unk28, (ex->math_sin(a >> 1) >> 5) - 0x80,
                                         (ex->math_sin(a >> 1) >> 5) - 0x80, (ex->math_sin(a >> 1) >> 5) - 0x80);
            }
            if (frame == 16) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame == 1) {
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk4);
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

s32 fx_016_op81(s32 mode, Fx016Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E8358 = env;
        work->unk40 = env->targets->targetCount;
        for (i = 0; i < work->unk40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
            work->unk44 = ((-0x140 - (ex->btlfx_rand() & 0x1F)) * D_801E89A8) >> 12;
            ex->func_800DC818(i, 0, (u16 *)&v);
            work->unk48[i] = -v.vy;
        }
    } else {
        frame = entry->unk0;
        if (frame < 0x20) {
            t = frame << 7;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk44 * ex->math_sin(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        } else {
            t = (frame - 0x20) << 10;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk48[i] * t) >> 12;
                v.vy += (work->unk44 * ex->math_cos(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        }
        if (frame >= 0x24) {
            for (i = 0; i < work->unk40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_016/nonmatchings/fx_016", fx_016_headerPtr);
