#include "fxmod/fx_0cb.h"

INCLUDE_ASM("asm/us/fx_0cb/nonmatchings/fx_0cb", func_801E7704);

void func_801E7EB8(s16 *a, s16 *b) {
    BTLFX_EXPORTS(D_801E8670)->btlfx_aimAngles((s16 *)&D_801E8670->unk2C->unk54, a, b);
}

s32 func_801E7EFC(s32 mode, Fx0cbPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx0cbWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 c;
    s32 s;
    s32 a;
    s32 m;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        a = frame << 9;
        p->unk14 += p->unk16;
        func_801E7EB8((s16 *)&p->unk4, (s16 *)&rot);
        rot.vz = p->unk14;
        pos.vx = p->unk4.vx;
        pos.vy = p->unk4.vy;
        pos.vz = p->unk4.vz;
        m = ex->math_sin(a >> 2);
        m *= 3;
        s = p->unk2;
        scale.vx = scale.vy = scale.vz = ((s * m) >> 13) + (s >> 1);
        if (frame < 3) {
            c = ((frame + 1) << 12) / 3;
        } else {
            c = ex->math_cos(((frame - 3) << 10) / 5);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk20);
        s = c >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk20, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        s = c >> 6;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk24, s, s, s);
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0cb/nonmatchings/fx_0cb", fx_0cb_op80);

INCLUDE_RODATA("asm/us/fx_0cb/nonmatchings/fx_0cb", fx_0cb_headerPtr);
