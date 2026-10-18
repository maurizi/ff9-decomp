#include "fxmod/fx_0e4.h"

s32 fx_0e4_op80(s32 mode, Fx0e4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 m;
    s32 t;
    s16 n;
    s32 f;
    s32 d;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        D_801E7D48 = env;
        ex->btlfx_initEffModels(D_801E7D58, 3);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk34);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk3C);
        work->unk8.vx = work->unk3C.vx;
        work->unk8.vy = work->unk3C.vy;
        work->unk8.vz = work->unk3C.vz;
        work->unk10.vx = work->unk34.vx;
        work->unk10.vy = work->unk34.vy;
        work->unk10.vz = work->unk34.vz;
        ex->func_800CFD78((s16 *)&work->unk34, (s16 *)&work->unk3C, ex->btlfx_getUnitRadius(0) + 0x190, (s16 *)&pos);
        work->unk10.vx = pos.vx;
        work->unk10.vy = pos.vy;
        work->unk10.vz = pos.vz;
        work->unk64 = 0;
        work->unk20.vx = 0x5000;
        work->unk20.vy = 0x6000;
        work->unk20.vz = 0x5000;
        for (i = 0; i < 3; i++) {
            work->unk68[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E7CF8[i]), 1);
        }
        work->unk54.vx = 0x400;
        work->unk5E = 0x1000;
        work->unk60 = 8;
        work->unk54.vy = 0;
        work->unk54.vz = 0;
        work->unk5C = 0;
        work->unk62 = -1;
        work->unk4C.vx = work->unk10.vx;
        work->unk4C.vy = work->unk10.vy;
        work->unk4C.vz = work->unk10.vz;
    } else {
        frame = entry->unk0;
        if (frame == 0xC) {
            work->unk62 = 0x10;
        }
        if (frame >= 0 && frame < 0x1C) {
            f = frame;
            if (work->unk60 >= 0) {
                work->unk10.vy = func_801E7CA0(-0xFA0, 0, 8 - work->unk60, 8);
                if (--work->unk60 == 0) {
                    work->unk44.vx = work->unk10.vx;
                    work->unk44.vy = work->unk10.vy;
                    work->unk44.vz = work->unk10.vz;
                    work->unk44.vy = 0;
                    work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk44);
                    work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk34);
                }
            }
            work->unk5C += 0x200;
            n = work->unk5C;
            if (n > 0x1000) {
                n = 0x1000;
            }
            work->unk5C = n;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk4C, &work->unk54, work->unk5C,
                              f, 2, work->unk5E);
            switch (work->unk64) {
            case 0:
                if (f < 6) {
                    m = 0;
                    ex->Hi_DrawEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, m);
                } else if (f >= 6 && f < 9) {
                    d = 0x1000 - ((f - 6) << 12) / 3;
                    t = d > 0x1000 ? 0x1000 : d;
                    ex->Hi_DrawMorphEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, work->unk68[0],
                                             work->unk68[1], t);
                } else {
                    m = 1;
                    ex->Hi_SetEffModelSlice(m, work->unk10.vy);
                    ex->Hi_DrawSliceEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, m);
                }
                env->unk2C->unk0[0].vx = work->unk10.vx;
                env->unk2C->unk0[0].vy = work->unk10.vy;
                env->unk2C->unk0[0].vz = work->unk10.vz;
                if (f >= 4 && frame < 0xC) {
                    work->unk20.vy -= 0x400;
                }
                break;
            case 1:
                m = 2;
                ex->Hi_SetEffModelSlice(m, work->unk10.vy);
                ex->Hi_DrawSliceEffModel(&work->unk8, &work->unk10, (s32)&work->unk20, m);
                break;
            }
            if (work->unk62 >= 0) {
                work->unk30 = func_801E7CA0(0, -0x80, 0x10 - work->unk62, 0x10);
                work->unk62--;
                ex->Hi_ModifyEffModelAbr(m, 1);
                ex->Hi_ModifyEffModelRGB(m, work->unk30, work->unk30, work->unk30);
                work->unk10.vy += 0xA;
                work->unk20.vy -= 0x100;
                work->unk5E -= 0x100;
            }
        }
        if (frame >= 0x41) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7C5C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7D48)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7CA0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0e4/nonmatchings/fx_0e4", fx_0e4_headerPtr);
