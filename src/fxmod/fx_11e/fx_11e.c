#include "fxmod/fx_11e.h"

s32 fx_11e_op80(s32 mode, Fx11eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR p;
    SVECTOR a;
    s32 frame;
    s32 t;
    s32 n;
    s32 m;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        n = *arg4;
        work->unk58 = n;
        D_801E7FD0 = env;
        if (n >= env->targets->targetCount) {
            func_801E7F70(env, 7, 1);
            return 1;
        }
        func_801E7F70(env, 7, 0);
        ex->func_800E9930(7, entry->unk8, -1);
        ex->func_800E9930(8, entry->unk8, -1);
        ex->func_800E9930(9, entry->unk8, -1);
        if (D_801E7F84 == 0) {
            ex->btlfx_initEffModels(D_801E7FE0, 8);
            D_801E7F84 = mode;
        }
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk50);
        ex->btlfx_getUnitBodyPos(work->unk58, 0, &work->unk48.vx);
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unk1C.vx = 0;
        work->unk1C.vy = 0;
        work->unk1C.vz = 0;
        work->unkC.vx = work->unk48.vx;
        work->unkC.vy = work->unk48.vy;
        work->unkC.vz = work->unk48.vz;
        work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
        work->unk5E = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk30 = ex->btlfx_rand() % 2;
        work->unk34 = ex->btlfx_rand() % 2;
        work->unk38 = ex->btlfx_rand() % 400 + 200;
        work->unk3C = ex->btlfx_rand() % 0x1000;
        work->unk40 = ex->btlfx_rand() % 16;
        work->unk44 = ex->btlfx_rand() % 16;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk50);
    } else {
        frame = entry->unk0;
        if ((u32)(frame - 0x1E) < 0x28) {
            t = frame - 0x1E;
            ex->btlfx_getUnitBodyPos(work->unk58, 0, &v.vx);
            work->unkC.vx = v.vx;
            work->unkC.vz = v.vz;
            work->unkC.vx += work->unk38 * ex->math_sin(work->unk3C) / 4096;
            work->unkC.vz += work->unk38 * ex->math_cos(work->unk3C) / 4096;
            if (t >= 0x19) {
                work->unkC.vy += -200 - ex->btlfx_rand() % 40;
                work->unk38 += 200 + ex->btlfx_rand() % 100;
            } else {
                if (ex->btlfx_rand() & 1) {
                    work->unkC.vy += ex->btlfx_rand() % 20;
                } else {
                    work->unkC.vy -= ex->btlfx_rand() % 20;
                }
                if (work->unk30 != 0) {
                    work->unk38 += ex->btlfx_rand() % 20;
                } else {
                    work->unk38 -= ex->btlfx_rand() % 20;
                }
                if (work->unk34 != 0) {
                    work->unk3C += ex->btlfx_rand() % 256;
                } else {
                    work->unk3C -= ex->btlfx_rand() % 256;
                }
                p.vx = work->unkC.vx;
                p.vy = work->unkC.vy;
                p.vz = work->unkC.vz;
                ex->btlfx_aimAngles(&v.vx, &p.vx, &a.vx);
                work->unk4.vx = a.vx;
                work->unk4.vy = a.vy;
                work->unk4.vz = a.vz;
            }
            if (t == 0x18) {
                work->unk4.vy += 0x800;
            }
            if (--work->unk40 == 0) {
                work->unk30 ^= 1;
                work->unk40 = ex->btlfx_rand() % 16;
            }
            if (--work->unk44 == 0) {
                work->unk34 ^= 1;
                work->unk44 = ex->btlfx_rand() % 16;
            }
            if (work->unk1C.vx < 0x1000) {
                work->unk1C.vx += 0x80;
                work->unk1C.vy += 0x80;
                work->unk1C.vz += 0x80;
            }
            if (t & 1) {
                m = work->unk5C;
            } else {
                m = work->unk5E;
            }
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, m);
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk2C = (s32)&work->unk48;
            }
        }
        if (frame == 3) {
            func_801E7F70(env, 7, 1);
        }
        return frame >= 0x50;
    }
    return 0;
}

void func_801E7F70(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_11e/nonmatchings/fx_11e", fx_11e_headerPtr);
