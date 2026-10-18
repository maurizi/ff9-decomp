#include "fxmod/fx_112.h"

void func_801E7718(s32 x, s32 y, s32 w, s32 h, s32 dx, s32 dy) {
    Unk800F78A4 *env = D_801E8050;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;

    r.x = env->unk18->disp.x + x - (w >> 1);
    r.y = env->unk18->disp.y + y - (h >> 1);
    r.w = w;
    r.h = h;
    ex->MoveImage(&r, dx, dy);
}

INCLUDE_RODATA("asm/us/fx_112/nonmatchings/fx_112", fx_112_headerPtr);

INCLUDE_ASM("asm/us/fx_112/nonmatchings/fx_112", fx_112_op80);
