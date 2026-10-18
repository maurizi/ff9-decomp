#include "fxmod/fx_18a.h"

INCLUDE_ASM("asm/us/fx_18a/nonmatchings/fx_18a", fx_18a_op80);

s32 func_801E844C(s32 mode, Fx18aSpark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 12) / 12;
        pt = e->unk18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x1A;
        ex->func_800E0014((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->unk16 == 0) {
            TRAIL(e)->unk0 = TRAIL(e)->unk2 = 0xE;
            TRAIL(e)->unk1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->unk18 - 4, 0);
        }
        if (frame >= 0xC) {
            return 1;
        }
    }
    return 0;
}

void func_801E85A4(void) {
    D_801E90D4 = 0;
}

void func_801E85B0(Fx18aInst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx18aTrail *tr = &D_801E8B04[D_801E90D4];
    SVECTOR *pt = &tr->unk4[14];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A24);
    SVECTOR v;
    Fx18aSpark *e;
    s32 i;
    s32 t;

    tr->unk0 = tr->unk2 = 0xE;
    tr->unk1 = 0;
    for (i = 0; i < 15; i++, pt--) {
        t = 0x1000 - (i << 12) / 14;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 3), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->unk10);
        if (e != NULL) {
            e->unk14 = D_801E90D4;
            e->unk16 = 0xE - i;
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
    D_801E90D4 = (D_801E90D4 + 1) % 12;
}

s32 fx_18a_op82(s32 mode, Fx18aWork82 *work, Unk800F78A4 *env, Fx18aInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    s32 frame;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E844C;
        init->unk4 = 0xB4;
    } else if (mode == 1) {
        D_801E8A24 = env;
        func_801E85A4();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        frame = inst->unk0;
        c = frame < 0xE6;
        if (c) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->unk8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            func_801E85B0(inst, &work->unk0, frame + 0x40, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (!c) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18a/nonmatchings/fx_18a", fx_18a_headerPtr);
