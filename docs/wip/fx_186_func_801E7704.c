/* Draft of fx_186 func_801E7704. Status: 13 strict mismatches: the constant 1 / alpha registers (s4/s3) are swapped,
 * and r - 0x2BC is computed in place after the sqrt call. */
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
                o->unk6 = r - 0x2BC + ex->btlfx_sqrt(x * x + z * z, 0x14) / 2;
                o->unkA = o->unkC.vz / 4 + ex->btlfx_randCentered(0x80);
            }
            break;
        case 1:
            o->unkC.vx += o->unk8 >> 2;
            o->unkC.vz += o->unkA >> 2;
            o->unk8 = o->unk8 * 7 >> 3;
            o->unkA = o->unkA * 7 >> 3;
            o->unkC.vy += o->unk6 / 5;
            o->unk6 += 12;
            t = (frame << 12) / 40;
            a -= t;
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
    }
    return 0;
}
