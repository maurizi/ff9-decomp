#include "fxmod/fx_0b2.h"

void func_801E7704(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E90A0);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = arg10;
        arg6 += arg7 + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 25);
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0, NULL, arg8, 0x1000, arg9, 1, (u8 *)trail, 0);
}

INCLUDE_ASM("asm/us/fx_0b2/nonmatchings/fx_0b2", func_801E78FC);

INCLUDE_ASM("asm/us/fx_0b2/nonmatchings/fx_0b2", fx_0b2_op80);

INCLUDE_RODATA("asm/us/fx_0b2/nonmatchings/fx_0b2", fx_0b2_headerPtr);
