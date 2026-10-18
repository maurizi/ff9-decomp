/* Draft of fx_1b7 fx_1b7_op80. Status: 5 mismatches. In the 8-model loop the original recomputes (b >> 5) every
 * iteration (only b = t + 0x1000 is hoisted); here loop.c hoists b >> 5. */
#include "fxmod/fx_1b7.h"

s32 fx_1b7_op80(s32 mode, Fx1b7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 sp;
    Pool6CEntry *p;
    u8 *buf;
    s32 frame;
    s32 t;
    s32 a;
    s32 c;
    s32 r;
    s32 g;
    s32 b;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&pos);
        pos.vz -= 0x258;
        work->unk8[0] = pos.vx;
        work->unk8[1] = pos.vy;
        work->unk8[2] = pos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitScale(0x10, work->unk1C);
        work->unk14.vy += 0x800;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk28);
        work->unk28.vy -= 0x180;
        work->unk30.vx = work->unk28.vx;
        work->unk30.vy = work->unk28.vy;
        work->unk30.vz = work->unk28.vz;
        work->unk30.vy = 0;
        work->unk38.vx = work->unk30.vx;
        work->unk38.vy = work->unk30.vy;
        work->unk38.vz = work->unk30.vz;
        work->unk38.vy = -0x280;
        pos.vy -= 0x80;
        env->unk2C->unk0[3].vx = pos.vx;
        env->unk2C->unk0[3].vy = pos.vy;
        D_801E87E4 = entry;
        env->unk2C->unk0[3].vz = pos.vz;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk40 = NULL;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 7) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E8E40;
                    p->unk12 = 0x20;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0xB) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x1D) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x27) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x10) {
                work->unk40 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (work->unk40 != NULL && work->unk40->unk30 == -1) {
                work->unk40 = NULL;
            }
            if (frame == 1 || frame == 0xF || frame == 0x14 || frame == 0x24) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 2, 0x40, 0x80, 0x80);
            }
            if (frame == 2 || frame == 0x10 || frame == 0x15 || frame == 0x25) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x64, 0x64, 0x40);
            }
            if (frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0xFF, 0xFF, 0xFF);
            }
            if (frame >= 0x2C) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk40);
                ex->func_800E806C(0x10, 0);
                ex->func_800E87C4(0x6E);
                ex->btlfx_initEffModels(D_801E8800, 8);
                for (i = 0; i < 8; i++) {
                    work->unk44[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8788[i]), 0);
                }
                buf = ((Fx1b7Summon *)env->unk4)->unk54->unk50 + 0x1B800;
                func_801E7704((Fx1b7Split *)buf, (SVECTOR (*)[3])(buf + 0xDBD8));
                for (i = 0; i < 8; i++) {
                    func_801E774C(work->unk44[i]);
                }
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0xC0, 0xAA, 0x20);
            }
            break;
        case 1:
            t = (frame << 12) / 130;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, 0x60, 0x55, 0x10);
            }
            if ((u32)(frame - 4) < 0x18) {
                c = 0xFF - ((frame - 4) * 0xFF) / 24;
                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 2, 1, c, c, c >> 1);
            }
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk38);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk30);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EA3C0;
                    p->unk12 = 0x1C;
                    p->unkC = 0xAC;
                    p->unk22 = 0x140;
                }
            }
            if (frame > 0) {
                a = t >> 2;
                b = t + 0x1000;
                r = (ex->math_cos(a) * 2) >> 5;
                g = (ex->math_cos(a) * 3 / 2) >> 5;
                for (i = 0; i < 8; i++) {
                    ex->Hi_DrawEffModel(&work->unk14, (VECTOR *)work->unk8, (s32)work->unk1C, work->unk44[i]);
                    ex->Hi_ModifyEffModelRGB(work->unk44[i], r - 0x80, g - 0x80, (b >> 5) - 0x80);
                }
                D_801E87F0 = 0x898 - ((t << 12) >> 12);
                FX1B7_SPAD_STACK(&sp);
                func_801E7868();
                func_801E7ACC();
                FX1B7_RESET_STACK(&sp);
            }
            if (frame >= 0x82) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}
