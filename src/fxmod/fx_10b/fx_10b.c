#include "fxmod/fx_10b.h"

s32 fx_10b_op80(s32 mode, Fx10bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 t;
    s32 i;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        D_801E7B74 = env;
        ex->btlfx_initEffModels(D_801E7B84, 2);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
        work->unk30.vx = 0x1000;
        work->unk30.vy = 0x1000;
        work->unk30.vz = 0x1000;
        work->unk18.vx += 0x800;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
        work->unk72 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xC), 0x97, 0x3D40, 0, 0);
    } else {
        frame = entry->frame;
        if (frame < 8) {
            t = (frame << 12) / 6;
            ex->btlfx_lerpVec((s16 *)&work->unk0, (s16 *)&work->unk8, t, (s16 *)&pos);
            work->unk20.vx = pos.vx;
            work->unk20.vy = pos.vy;
            work->unk20.vz = pos.vz;
            work->unk20.vy -= (ex->math_sin(t >> 1) * 200) >> 12;
        }
        ex->Hi_DrawEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk70);
        for (i = 0; i < 2; i++) {
            if (frame >= i + 1) {
                c = -(i * 16);
                ex->Hi_DrawEffModel(&work->unk40[i], &work->unk50[i], (s32)&work->unk30, work->unk72);
                ex->Hi_ModifyEffModelAbr(work->unk72, 3);
                ex->Hi_ModifyEffModelRGB(work->unk72, c, c, c);
            }
        }
        work->unk40[1].vx = work->unk40[0].vx;
        work->unk40[1].vy = work->unk40[0].vy;
        work->unk40[1].vz = work->unk40[0].vz;
        work->unk50[1].vx = work->unk50[0].vx;
        work->unk50[1].vy = work->unk50[0].vy;
        work->unk50[1].vz = work->unk50[0].vz;
        work->unk40[0].vx = work->unk18.vx;
        work->unk40[0].vy = work->unk18.vy;
        work->unk40[0].vz = work->unk18.vz;
        work->unk50[0].vx = work->unk20.vx;
        work->unk50[0].vy = work->unk20.vy;
        work->unk50[0].vz = work->unk20.vz;
        if (frame >= 6) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_10b_op81(s32 mode, Fx10bWork80_0bd *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7B74 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_10b/nonmatchings/fx_10b", fx_10b_headerPtr);
