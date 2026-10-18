/* Draft of fx_186 fx_186_op81. Status: about 15 mismatches besides register numbering. The unit-count test after the
 * lbu is blez in the original but beqz here (gcc knows the u8 is non-negative); not found how to keep the signed test. */
#include "fxmod/fx_186.h"

s32 fx_186_op81(s32 mode, Fx186Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801E9214 = env;
        t = env->unkC->unk24;
        work->unk0 = 0;
        work->unk104 = t;
        for (i = 0; i < work->unk104; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk4[i]);
            ex->btlfx_getUnitRot(i, (s16 *)&work->unk44[i]);
            ((s32 *)&work->unkC4[i])[0] = ((s32 *)&work->unk44[i])[0];
            ((s32 *)&work->unkC4[i])[1] = ((s32 *)&work->unk44[i])[1];
            work->unk84[i].vx = 0x2000 / ((ex->btlfx_rand() % 2 + 1) * 40);
            work->unk84[i].vy = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk84[i].vz = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk108 = -0x578 - (ex->btlfx_rand() & 0x3FF);
        }
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 7;
            for (i = 0; i < work->unk104; i++) {
                ((s32 *)&p)[1] = ((s32 *)&work->unk4[i])[1];
                ((s32 *)&p)[0] = ((s32 *)&work->unk4[i])[0];
                p.vy -= (t >> 4) + (ex->btlfx_rand() & 0x7F);
                ex->btlfx_setUnitPos(i, (s16 *)&p);
            }
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            for (i = 0; i < work->unk104; i++) {
                ((s32 *)&p)[1] = ((s32 *)&work->unk4[i])[1];
                ((s32 *)&p)[0] = ((s32 *)&work->unk4[i])[0];
                p.vy += (work->unk108 * ex->math_sin(t >> 1)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&p);
                work->unkC4[i].vx += work->unk84[i].vx;
                work->unkC4[i].vy += work->unk84[i].vy;
                work->unkC4[i].vz += work->unk84[i].vz;
                ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
            }
            if (frame >= 0x28) {
                for (i = 0; i < work->unk104; i++) {
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unk44[i]);
                    ex->btlfx_getUnitHomePos(i, (s16 *)&p);
                    ex->btlfx_setUnitPos(i, (s16 *)&p);
                }
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            t = (frame << 12) / 7;
            for (i = 0; i < work->unk104; i++) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), &work->unk4[i], &rot, 0x1000, frame, 1,
                                  ex->math_cos(t >> 2));
            }
            if (frame >= 7) {
                return 1;
            }
            break;
        }
    }
    return 0;
}
