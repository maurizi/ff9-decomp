#include "fxmod/fx_05a.h"

s32 fx_05a_op80(s32 mode, Fx05aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;
    s32 h;
    s32 r;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        D_801E7B10 = env;
        ex->btlfx_initEffModels(D_801E7B20, 1);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7BF0;
            work->unk0->unk12 = 0xA;
            work->unk0->unkC = 0xB0;
        }
        work->unk18.vx = 0;
        work->unk18.vy = 0;
        work->unk18.vz = 0;
        work->unk20.vx = work->unk8.vx;
        work->unk20.vy = work->unk8.vy;
        work->unk20.vz = work->unk8.vz;
        if (ex->btlfx_isEnemyUnit(0)) {
            s = 0x155;
            h = ex->func_800E738C(0);
            r = ex->btlfx_getUnitRadius(0);
            if (h > 0x190) {
                s += (h - 0x190) / 4;
            }
            if (r > 0x320) {
                s += (r - 0x320) / 4;
            }
            work->unk10.vy -= (s - 0x155) * 3;
            if (s > 0x400) {
                s = 0x400;
            }
            work->unk30.vx = s;
            work->unk30.vy = s;
            work->unk30.vz = s;
        } else {
            work->unk30.vx = 0x155;
            work->unk30.vy = 0x155;
            work->unk30.vz = 0x155;
        }
        work->unk44 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0x117, 0, 0, 0xC0);
        work->unk40 = -0x14;
        work->unk42 = 0xFF;
    } else {
        frame = entry->unk0;
        if (frame == 0x19) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame >= 0x19) {
            ex->Hi_DrawEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk44);
            ex->Hi_ModifyEffModelAbr(work->unk44, 1);
            work->unk18.vy += 0x40;
            t = frame - 0x19;
            if (frame >= 0x2D) {
                ex->Hi_ModifyEffModelRGB(work->unk44, work->unk40, work->unk40, work->unk40);
                work->unk40 -= 4;
            }
            ex->func_800D3030((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), t);
        }
        if (frame >= 0x19 && frame < 0x21) {
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, work->unk42, work->unk42, work->unk42);
            work->unk42 -= 0x1F;
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05a/nonmatchings/fx_05a", fx_05a_headerPtr);
