#include "fxmod/fx_0ed.h"

INCLUDE_ASM("asm/us/fx_0ed/nonmatchings/fx_0ed", func_801E7704);

INCLUDE_ASM("asm/us/fx_0ed/nonmatchings/fx_0ed", func_801E7B74);

INCLUDE_ASM("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_op80);

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        SVECTOR v;
        CVECTOR c;
    } u;
    SVECTOR rot;
    VECTOR sc;
    s32 tmp;
    s32 frame;
    s32 r;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk4 = 0xA;
        work->unk0 = 0;
        work->unkC = (BtlUnit *)-1;
        work->unk8 = 0;
        D_801E9224 = env;
        ((Fx0edBattleState *)env->unk0)->unk10 = 0;
    } else {
        frame = entry->unk0;
        c = 0;
        switch (work->unk0) {
        case 0:
            do {
                r = ex->func_800E82CC(0, (s16 *)&work->unk10, &work->unkC, &tmp);
                if (r == -2) {
                    work->unkC = (BtlUnit *)-1;
                }
            } while (r == -2 || r == -1);
            env->unk2C->unk0[0].vx = work->unk10.vx;
            env->unk2C->unk0[0].vy = work->unk10.vy;
            env->unk2C->unk0[0].vz = work->unk10.vz;
            u.v.vx = work->unk10.vx;
            u.v.vy = work->unk10.vy;
            u.v.vz = work->unk10.vz;
            if (u.v.vz < 0) {
                u.v.vz += 0x640;
            } else {
                u.v.vz -= 0x640;
            }
            u.v.vy -= 0x200;
            env->unk2C->unk0[1].vx = u.v.vx;
            env->unk2C->unk0[1].vy = u.v.vy;
            env->unk2C->unk0[1].vz = u.v.vz;
            ex->func_800E9930(3, entry->unk8, -1);
            if (++work->unk8 >= 0x10 && r == 1) {
                work->unk0 = 2;
            } else {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            break;
        case 1:
            env->unk28 = 0x80;
            ex->func_800D198C((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk10, 0, 0xFF, -1);
            env->unk28 = 0;
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            if (frame >= work->unk4) {
                work->unk0 = 0;
                work->unk4--;
            }
            break;
        case 2:
            ((Fx0edBattleState *)env->unk0)->unk10 = 1;
            env->unk2C->unk52 = 0;
            return 1;
        }
        if (c != 0) {
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            u.c.r = u.c.g = u.c.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(6, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &u.c, 1, 0);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_headerPtr);
