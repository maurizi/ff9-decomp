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

void func_801E8118(Fx1e9Inst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx1e9Trail *tr = &D_801E8644[D_801E88A4];
    SVECTOR *pt = &tr->unk4[8];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8564);
    SVECTOR v;
    Fx1e9Spark *e;
    s32 i;
    s32 t;

    tr->unk0 = tr->unk2 = 8;
    tr->unk1 = 0;
    for (i = 0; i < 9; i++, pt--) {
        t = 0x1000 - (i << 9);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 5), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->unk10);
        if (e != NULL) {
            e->unk14 = D_801E88A4;
            e->unk16 = 8 - i;
            e->unk18 = pt;
            e->unk0 = pt->vx << 12;
            e->unk4 = pt->vy << 12;
            e->unk8 = pt->vz << 12;
            e->unkC = 0;
            e->unkE = 0;
            e->unk10 = 0;
            e->unkC = ((pt->vx - origin->vx) >> 4) + ex->btlfx_randCentered(0x20);
            e->unkE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0x20);
            e->unk10 = ((pt->vz - origin->vz) >> 4) + ex->btlfx_randCentered(0x20);
        }
    }
    D_801E88A4++;
    D_801E88A4 %= 8;
}

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
