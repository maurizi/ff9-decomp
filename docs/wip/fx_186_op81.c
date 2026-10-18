/* Draft of fx_186 fx_186_op81. Status: 8 aligned mismatches with registers, 0 ignoring them: at the start of mode 1
 * the original gives %hi(D_801E9214) v0 and env->unkC v1 (here the other way round: local-alloc's tie-break by birth
 * order, sched1 hoists the env->unkC load above the lui; all 59 orders of the five statements tried).
 * Levers that got here: the unit-count test as 'if (n <= 0) return 0' + do/while (gives blez: the loop's own
 * entry test would be simplified to beqz from the u8's nonzero bits), an s16 n, a separate counter k for the mode-1
 * loop (i then has the priority of the case loops only), the unk4 copy through two temporaries (both loads first). */
#include "fxmod/fx_186.h"

s32 fx_186_op81(s32 mode, Fx186Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 k;
    s32 t;
    s32 w0;
    s32 w1;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801E9214 = env;
        {
            s32 n = env->unkC->unk24;
            k = 0;
            work->unk0 = 0;
            work->unk104 = n;
            if (n <= 0) {
                return 0;
            }
        }
        do {
            ex->btlfx_getUnitHomePos(k, (s16 *)&work->unk4[k]);
            ex->btlfx_getUnitRot(k, (s16 *)&work->unk44[k]);
            ((s32 *)&work->unkC4[k])[0] = ((s32 *)&work->unk44[k])[0];
            ((s32 *)&work->unkC4[k])[1] = ((s32 *)&work->unk44[k])[1];
            work->unk84[k].vx = 0x2000 / ((ex->btlfx_rand() % 2 + 1) * 40);
            work->unk84[k].vy = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk84[k].vz = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->unk108 = -0x578 - (ex->btlfx_rand() & 0x3FF);
            k++;
        } while (k < work->unk104);
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 7;
            for (i = 0; i < work->unk104; i++) {
                w1 = ((s32 *)&work->unk4[i])[1];
                w0 = ((s32 *)&work->unk4[i])[0];
                ((s32 *)&p)[1] = w1;
                ((s32 *)&p)[0] = w0;
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
                w1 = ((s32 *)&work->unk4[i])[1];
                w0 = ((s32 *)&work->unk4[i])[0];
                ((s32 *)&p)[1] = w1;
                ((s32 *)&p)[0] = w0;
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
