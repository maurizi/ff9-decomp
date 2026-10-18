#include "fxmod/fx_184.h"

s32 fx_184_op80(s32 mode, Fx184Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x44;
    } else if (mode == 1) {
        D_801E7DC4 = env;
        ex->btlfx_initEffModels(D_801E7DD4, 1);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk10.vx = work->unk0.vx;
        work->unk10.vy = work->unk0.vy;
        work->unk10.vz = work->unk0.vz;
        work->unk10.vy = -0xFA0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
        work->unk18.vx = ex->ratan2(work->unk8.vy - work->unk10.vy, work->unk10.vz - work->unk8.vz);
        work->unk30.vx = 0x1000;
        work->unk30.vy = 0x1000;
        work->unk30.vz = 0x1000;
        ex->btlfx_setWeaponVisible(0x10, 0);
        work->unk40 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
    } else {
        frame = entry->unk0;
        if (frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->unk10, (s16 *)&work->unk8, frame << 10, (s16 *)&v);
            work->unk20.vx = v.vx;
            work->unk20.vy = v.vy;
            work->unk20.vz = v.vz;
        }
        ex->Hi_SetEffModelSlice(work->unk40, work->unk20.vy);
        ex->Hi_DrawSliceEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk40);
        if (frame >= 4) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_184_op81(s32 mode, Fx184Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7DC4 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unk4.vy = 0;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_184/nonmatchings/fx_184", fx_184_op82);

INCLUDE_ASM("asm/us/fx_184/nonmatchings/fx_184", func_801E7BC4);

void func_801E7CE0(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7DC4)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7D24(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_184/nonmatchings/fx_184", fx_184_headerPtr);
