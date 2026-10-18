#include "fxmod/fx_1e9.h"

INCLUDE_ASM("asm/us/fx_1e9/nonmatchings/fx_1e9", fx_1e9_op80);

s32 func_801E7FCC(s32 mode, Fx1e9Spark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 9);
        pt = e->unk18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x14;
        ex->func_800E0014((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->unk16 == 0) {
            TRAIL(e)->unk0 = TRAIL(e)->unk2 = 8;
            TRAIL(e)->unk1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->unk18 - 4, 0);
        }
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1e9/nonmatchings/fx_1e9", func_801E810C);

INCLUDE_ASM("asm/us/fx_1e9/nonmatchings/fx_1e9", func_801E8118);

s32 fx_1e9_op82(s32 mode, Fx1e9Work82 *work, Unk800F78A4 *env, Fx1e9Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E7FCC;
        init->unk4 = 0x48;
    } else if (mode == 1) {
        D_801E8564 = env;
        func_801E810C();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801E8518 == 0) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->unk8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            func_801E8118(inst, &work->unk0, 0x80, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801E8518 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1e9/nonmatchings/fx_1e9", fx_1e9_headerPtr);
