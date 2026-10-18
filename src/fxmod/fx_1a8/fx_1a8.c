#include "fxmod/fx_1a8.h"

void func_801E7704(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E97CC);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, D_801E97D4[0]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(D_801E97D4[0], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(D_801E97D4[0], c, c, c);
    }
}

void func_801E7828(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 arg5, s32 arg6, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E97CC);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, D_801E97D4[1]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(D_801E97D4[1], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(D_801E97D4[1], c, c, c);
    }
}

INCLUDE_ASM("asm/us/fx_1a8/nonmatchings/fx_1a8", func_801E7950);

INCLUDE_ASM("asm/us/fx_1a8/nonmatchings/fx_1a8", fx_1a8_op80);

INCLUDE_RODATA("asm/us/fx_1a8/nonmatchings/fx_1a8", fx_1a8_headerPtr);
