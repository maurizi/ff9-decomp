#include "fxmod/fx_0b3.h"

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E7704);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E7754);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E77DC);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E7890);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E78BC);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op80);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op81);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op83);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E8DF4);

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op84);

s32 func_801E9DB8(s32 a) {
    s32 v = BTLFX_EXPORTS(D_801ED5E0)->math_sin(a);

    if (v < 0) {
        v >>= 1;
    }
    return v;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", func_801E9DFC);

s32 func_801EA6C8(SVECTOR *v) {
    s32 i = D_801EB5D4;

    v->pad = 1;
    v->vx = D_801EE340[i].vx;
    v->vy = D_801EE340[i].vy;
    v->vz = D_801EE340[i].vz;
    i++;
    if (i >= 16) {
        i = 0;
    }
    D_801EB5D4 = i;
    return 0;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op85);

s32 fx_0b3_op86(s32 mode, Fx0b3Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *e;
    s32 i;

    if (mode == 0) {
        *arg4 = sizeof(Fx0b3Work86);
    } else if (mode == 1) {
        D_801ED5E0 = env;
        work->unk40 = env->unkC->unk24;
        for (i = 0; i < work->unk40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
                work->unk0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->unk0[i].vy -= 0x200;
                e = ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk0[i]);
                if (e != NULL) {
                    e->unk22 = ex->func_800E7CC4(i, 0x200);
                }
            }
        }
    } else if (entry->unk0 >= 0x2E) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_op87);

INCLUDE_RODATA("asm/us/fx_0b3/nonmatchings/fx_0b3", fx_0b3_headerPtr);
