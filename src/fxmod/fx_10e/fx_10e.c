#include "fxmod/fx_10e.h"

s32 fx_10e_op80(s32 mode, Fx10eWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x8C;
    } else if (mode == 1) {
        D_801E7BAC = env;
        ex->btlfx_initEffModels(D_801E7BBC, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->func_800DC818(0x10, 0x17, (u16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
        work->unk30.vx = 0x1000;
        work->unk30.vy = 0x1000;
        work->unk30.vz = 0x1000;
        work->unk18.vx += 0x800;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
        work->unk88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 1);
        work->unk8A = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0x97, 0x3D40, 0, 0);
    } else {
        frame = entry->unk0;
        if (frame < 0xA) {
            t = frame << 9;
            ex->btlfx_lerpVec((s16 *)&work->unk0, (s16 *)&work->unk8, t, (s16 *)&pos);
            work->unk20.vx = pos.vx;
            work->unk20.vy = pos.vy;
            work->unk20.vz = pos.vz;
            work->unk20.vy -= (ex->math_sin(t >> 1) * 400) >> 12;
            work->unk18.vx += 0x200;
        }
        ex->Hi_DrawEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk88);
        for (i = 0; i < 3; i++) {
            if (frame >= i + 1) {
                c = -(i * 16);
                ex->Hi_DrawEffModel(&work->unk40[i], &work->unk58[i], (s32)&work->unk30, work->unk8A);
                ex->Hi_ModifyEffModelAbr(work->unk8A, 3);
                ex->Hi_ModifyEffModelRGB(work->unk8A, c, c, c);
            }
        }
        work->unk40[2].vx = work->unk40[1].vx;
        work->unk40[2].vy = work->unk40[1].vy;
        work->unk40[2].vz = work->unk40[1].vz;
        work->unk58[2].vx = work->unk58[1].vx;
        work->unk58[2].vy = work->unk58[1].vy;
        work->unk58[2].vz = work->unk58[1].vz;
        work->unk40[1].vx = work->unk40[0].vx;
        work->unk40[1].vy = work->unk40[0].vy;
        work->unk40[1].vz = work->unk40[0].vz;
        work->unk58[1].vx = work->unk58[0].vx;
        work->unk58[1].vy = work->unk58[0].vy;
        work->unk58[1].vz = work->unk58[0].vz;
        work->unk40[0].vx = work->unk18.vx;
        work->unk40[0].vy = work->unk18.vy;
        work->unk40[0].vz = work->unk18.vz;
        work->unk58[0].vx = work->unk20.vx;
        work->unk58[0].vy = work->unk20.vy;
        work->unk58[0].vz = work->unk20.vz;
        if (frame >= 8) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_10e_op81(s32 mode, Fx10eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7BAC = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_10e/nonmatchings/fx_10e", fx_10e_headerPtr);
