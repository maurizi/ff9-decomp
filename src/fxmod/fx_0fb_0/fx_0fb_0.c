#include "fxmod/fx_0fb_0.h"

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E7720);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E7B90);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E809C);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E8540);

INCLUDE_RODATA("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_op80);

s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801EB3AC = env;
        work->unk104 = env->unkC->unk24;
        work->unk0 = 0;
        for (i = 0; i < work->unk104; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk4[i]);
                work->unk108 = ex->btlfx_randCentered(0x400) - 0x3000;
                ex->btlfx_getUnitRot(i, (s16 *)&work->unk44[i]);
                BTLFX_COPY_SVEC(&work->unkC4[i], &work->unk44[i]);
                work->unk84[i].vx = ex->btlfx_randCentered(0x100);
                work->unk84[i].vy = ex->btlfx_randCentered(0x180);
                work->unk84[i].vz = ex->btlfx_randCentered(0x20);
            }
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 46;
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    BTLFX_COPY_SVEC(&v, &work->unk4[i]);
                    v.vy += (ex->math_sin(t >> 2) * work->unk108) >> 12;
                    ex->btlfx_setUnitPos(i, (s16 *)&v);
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (frame >= 0x2E) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } else {
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (D_801EB348 != 0) {
                for (i = 0; i < work->unk104; i++) {
                    if (ex->btlfx_isEnemyUnit(i) != 0) {
                        ex->btlfx_setUnitRot(i, (s16 *)&work->unk44[i]);
                        ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                        ex->btlfx_setUnitPos(i, (s16 *)&v);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_0fb_0_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        v.vx = env->unkC->unk38;
        v.vy = env->unkC->unk3A;
        v.vz = env->unkC->unk3C;
        v.vy -= 0x230;
        env->unk2C->unk0[2].vx = v.vx;
        env->unk2C->unk0[2].vy = v.vy;
        env->unk2C->unk0[2].vz = v.vz;
        D_801EB3AC = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
