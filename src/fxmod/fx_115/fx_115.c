#include "fxmod/fx_115.h"

s32 fx_115_op80(s32 mode, Fx115Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0xA4;
    } else if (mode == 1) {
        D_801E7BB8 = env;
        ex->btlfx_initEffModels(D_801E7BC8, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
        work->unk30.vx = 0x100;
        work->unk30.vy = 0x100;
        work->unk30.vz = 0x100;
        work->unk18.vx += 0x800;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
        work->unkA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 1);
        work->unkA2 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 1);
    } else {
        frame = entry->unk0;
        if (frame < 8) {
            t = (frame << 12) / 6;
            ex->btlfx_lerpVec((s16 *)&work->unk0, (s16 *)&work->unk8, t, (s16 *)&pos);
            work->unk20.vx = pos.vx;
            work->unk20.vy = pos.vy;
            work->unk20.vz = pos.vz;
            ex->math_sin(t >> 1);
        }
        ex->Hi_DrawEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unkA0);
        for (i = 0; i < 4; i++) {
            if (frame >= i + 1) {
                c = -(i * 20);
                ex->Hi_DrawEffModel(&work->unk40[i], &work->unk60[i], (s32)&work->unk30, work->unkA2);
                ex->Hi_ModifyEffModelAbr(work->unkA2, 3);
                ex->Hi_ModifyEffModelRGB(work->unkA2, c, c, c);
            }
        }
        work->unk40[3].vx = work->unk40[2].vx;
        work->unk40[3].vy = work->unk40[2].vy;
        work->unk40[3].vz = work->unk40[2].vz;
        work->unk60[3].vx = work->unk60[2].vx;
        work->unk60[3].vy = work->unk60[2].vy;
        work->unk60[3].vz = work->unk60[2].vz;
        work->unk40[2].vx = work->unk40[1].vx;
        work->unk40[2].vy = work->unk40[1].vy;
        work->unk40[2].vz = work->unk40[1].vz;
        work->unk60[2].vx = work->unk60[1].vx;
        work->unk60[2].vy = work->unk60[1].vy;
        work->unk60[2].vz = work->unk60[1].vz;
        work->unk40[1].vx = work->unk40[0].vx;
        work->unk40[1].vy = work->unk40[0].vy;
        work->unk40[1].vz = work->unk40[0].vz;
        work->unk60[1].vx = work->unk60[0].vx;
        work->unk60[1].vy = work->unk60[0].vy;
        work->unk60[1].vz = work->unk60[0].vz;
        work->unk40[0].vx = work->unk18.vx;
        work->unk40[0].vy = work->unk18.vy;
        work->unk40[0].vz = work->unk18.vz;
        work->unk60[0].vx = work->unk20.vx;
        work->unk60[0].vy = work->unk20.vy;
        work->unk60[0].vz = work->unk20.vz;
        if (frame >= 6) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_115_op81(s32 mode, Fx115Work80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7BB8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_115/nonmatchings/fx_115", fx_115_headerPtr);
