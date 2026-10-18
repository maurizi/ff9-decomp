#include "fxmod/fx_186.h"

s32 func_801E7704(s32 mode, Fx186Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 a;
    s32 t;
    s32 c;
    s16 x;
    s16 z;
    s32 r;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk6 = ex->btlfx_rand() % 1000 - 400;
        o->unk14.vx = 0;
        o->unk14.vy = 0;
        o->unk14.vz = 0;
        o->unk1E = 0;
        o->unk1C = 0;
    } else {
        scale.vx = 0x2000;
        scale.vy = 0x2000;
        scale.vz = 0x2000;
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        a = 0x1000;
        o->unk14.vx += o->unk1C;
        o->unk14.vy += o->unk1E;
        switch (o->unk0) {
        case 0:
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            t = frame << 7;
            pos.vy = pos.vy - ((ex->math_sin(t >> 2) * o->unk6) >> 12) + ((frame & 1) * 2 - 1) * (t >> 7);
            if (frame >= 0x20) {
                o->unk0 = 1;
                ((s16 *)o)[-1] = -1;
                o->unkC.vx -= o->unk6;
                o->unk1C = ex->btlfx_randCentered(8);
                o->unk1E = ex->btlfx_randCentered(8);
                o->unk8 = o->unkC.vx / 4 + ex->btlfx_randCentered(0x80);
                r = ex->btlfx_randCentered(0x100);
                x = o->unkC.vx / 7;
                z = o->unkC.vz / 7;
                o->unk6 = (s16)(r - 0x2BC) + ex->btlfx_sqrt(x * x + z * z, 0x14) / 2;
                o->unkA = o->unkC.vz / 4 + ex->btlfx_randCentered(0x80);
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            a -= t;
            o->unkC.vx += o->unk8 >> 2;
            o->unkC.vz += o->unkA >> 2;
            o->unk8 = o->unk8 * 7 >> 3;
            o->unkA = o->unkA * 7 >> 3;
            o->unkC.vy += o->unk6 / 5;
            o->unk6 += 12;
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
        ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, o->unk20);
        if (a < 0x1000) {
            if (a < 0x800) {
                ex->Hi_ModifyEffModelAbr(o->unk20, 1);
            }
            c = (a >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(o->unk20, c, c, c);
        }
        /* Dead test, removed after reload: the extra reference ranks a above the constant 1 in global-alloc. */
        if (a != 0) {
            t = 0;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_186/nonmatchings/fx_186", fx_186_op80);

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
            /* The dead conditional store splits the block, so n lives across blocks and is left to global-alloc.
             * That leaves two local qtys here, which local-alloc sorts properly (its hand-written three-qty sort
             * compares qty numbers instead of positions), giving %hi(D_801E9214) v0 and env->unkC v1. */
            if (n != 0) k = 0;
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

INCLUDE_RODATA("asm/us/fx_186/nonmatchings/fx_186", fx_186_headerPtr);
