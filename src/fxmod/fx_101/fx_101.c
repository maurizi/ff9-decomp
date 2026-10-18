#include "fxmod/fx_101.h"

INCLUDE_ASM("asm/us/fx_101/nonmatchings/fx_101", fx_101_op80);

s32 func_801E8048(s32 mode, Fx101Spark *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        p->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 9);
        pt = p->unk18;
        bias[0] = 0;
        bias[1] = 0;
        pt->pad = 0x14;
        ex->func_800E0014((Pool6CEntry *)p, 0, (SVECTOR *)bias, pt);
        if (p->unk16 == 0) {
            ((Fx101Bolt *)((s32 *)p->unk18 - 4))->unk0 = ((Fx101Bolt *)((s32 *)p->unk18 - 4))->unk2 = 0xB;
            ((Fx101Bolt *)((s32 *)p->unk18 - 4))->unk1 = 0;
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, 0, 5, 0x1000, a, 1,
                              (u8 *)p->unk18 - 4, 0);
        }
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

void func_801E8188(void) {
    D_801E89FC = 0;
}

void func_801E8194(Unk80100C78Entry *entry, SVECTOR *origin, s32 ang, SVECTOR *from, SVECTOR *to) {
    Fx101Bolt *bolt = &D_801E86DC[D_801E89FC];
    SVECTOR *pt = &bolt->unk4[11];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E85FC);
    SVECTOR p;
    Fx101Spark *s;
    s32 i;
    s32 t;

    bolt->unk2 = 0xB;
    bolt->unk0 = 0xB;
    bolt->unk1 = 0;
    for (i = 0; i < 12; i++, pt--) {
        t = 0x1000 - (i << 12) / 11;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        p.vx += ex->btlfx_randCentered(0x18);
        p.vy += ex->btlfx_randCentered(0x18);
        t = ang + ex->btlfx_randCentered(0xF) + (ex->math_sin(t >> 1) >> 4);
        ex->func_800CFD78((s16 *)origin, (s16 *)&p, t, (s16 *)pt);
        s = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (s != NULL) {
            s->unk14 = D_801E89FC;
            s->unk16 = 0xB - i;
            s->unk18 = pt;
            s->unk0 = pt->vx << 12;
            s->unk4 = pt->vy << 12;
            s->unk8 = pt->vz << 12;
            s->unkC = 0;
            s->unkE = 0;
            s->unk10 = 0;
            s->unkC = ((pt->vx - origin->vx) >> 3) + ex->btlfx_randCentered(5);
            s->unkE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0xF);
            s->unk10 = ((pt->vz - origin->vz) >> 3) + ex->btlfx_randCentered(5);
        }
    }
    D_801E89FC++;
    D_801E89FC %= 8;
}

s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR prev;

    if (mode == 0) {
        cfg->unk0 = 0x10;
        cfg->unk8 = 0x1C;
        cfg->unk14 = func_801E8048;
        cfg->unk4 = 0x60;
    } else if (mode == 1) {
        D_801E85FC = env;
        func_801E8188();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801E85B0 == 0) {
            BTLFX_COPY_SVEC(&prev, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->unk8.vy += 0x500 + (ex->btlfx_rand() & 0x7F);
            func_801E8194(entry, &work->unk0, 0x9B, &prev, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801E85B0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_101/nonmatchings/fx_101", fx_101_headerPtr);
