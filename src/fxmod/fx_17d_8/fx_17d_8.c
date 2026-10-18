#include "fxmod/fx_17d_8.h"

s32 func_801E7718(s32 mode, Fx17d8Obj *w, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx17d8Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 a;
    s32 c;

    if (mode == 1) {
        w->unk0 = 0;
    } else {
        a = (frame << 7) >> 2;
        c = (0x1000 - ex->math_cos(a)) * D_801E8FCC;
        pos.vx = w->unk4;
        pos.vy = w->unk6;
        pos.vz = w->unk8;
        pos.vx -= c >> 12;
        scale.vx = scale.vy = scale.vz = ((0x1080 - ex->math_sin(a)) * w->unk2) >> 12;
        ex->Hi_DrawEffModel(&w->unkC, &pos, (s32)&scale, inst->unk78);
        ex->Hi_ModifyEffModelRGB(inst->unk78, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                 (ex->math_sin(a) >> 5) - 0x80);
        if (frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_8/nonmatchings/fx_17d_8", fx_17d_8_headerPtr);

INCLUDE_ASM("asm/us/fx_17d_8/nonmatchings/fx_17d_8", fx_17d_8_op80);
