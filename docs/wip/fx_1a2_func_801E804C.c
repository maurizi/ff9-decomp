/* Draft of fx_1a2 func_801E804C. Status: 8 mismatches in the state-0 block: t = frame << 9 is scheduled before the
 * func_800CFD78 call, and v1/a0 are swapped around the pos.vy update. */
#include "fxmod/fx_1a2.h"

s32 func_801E804C(s32 mode, Fx1a2Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1a2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    VECTOR pos;
    SVECTOR rot;
    s32 t;
    s32 d;
    s32 y;

    if (mode == 1) {
        o->unk0 = 0;
        o->unk4 = (ex->btlfx_rand() & 0x7F) + 100;
    } else {
    d = 0;
    switch (o->unk8) {
    case 0:
        t = frame << 9;
        ex->func_800CFD78((s16 *)&o->unkC, (s16 *)&o->unk14, (o->unk1C << 5) >> 12, (s16 *)&o->unkC);
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (o->unk2 * (0x1000 - t)) >> 12;
        t >>= 1;
        pos.vy -= (ex->math_sin(t) * o->unk4) >> 12;
        d = -(ex->math_sin(t) >> 2);
        if (frame >= 8) {
            o->unk8 = 1;
            ((s16 *)o)[-1] = -1;
            o->unk2 = 0;
            o->unk6 = (ex->btlfx_rand() & 3) + 1;
        }
        break;
    case 1:
        pos.vx = o->unkC.vx;
        pos.vy = o->unkC.vy;
        pos.vz = o->unkC.vz;
        pos.vy -= (ex->btlfx_rand() & 0xF) + o->unk2;
        if (frame >= o->unk6) {
            o->unk8 = 0;
            ((s16 *)o)[-1] = -1;
            if (o->unk1C < 0x1800) {
                o->unk14.vy += ex->btlfx_randCentered(0x400);
            }
            o->unk4 = ex->btlfx_rand() % 160 + 0x118;
            if (++o->unk0 >= 10) {
                return 1;
            }
        }
        break;
    }
    rot.vx = o->unk14.vx;
    rot.vy = o->unk14.vy;
    rot.vz = o->unk14.vz;
    rot.vx += 0x400;
    rot.vx += d;
    rot.vy += 0x800;
    scale.vx = scale.vy = scale.vz = o->unk1C;
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, inst->unk48);
    }
    return 0;
}
