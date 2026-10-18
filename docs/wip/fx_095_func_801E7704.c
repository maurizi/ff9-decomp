/* Draft of fx_095 func_801E7704. Status: same instructions as the original (0 aligned mismatches ignoring
 * registers); register allocation only: global-alloc gives part/frame and scale/abr each other's registers (original:
 * ex s2, frame s3, part s4, abr s5, scale s6; here part s3, frame s4, scale s5, abr s6). Priorities (refs/live
 * length): frame 10/130 vs part 16/272, scale 7/73 vs abr 7/104. tools/equiv.py: equivalent (29/29 trials).
 * Levers that got here: math_cos before the abr/bright assignments in cases 0/2/3, a computed before abr/bright
 * in case 0 (keeps the 0x1000 copy), and (scale * 62) >> 12 into t. */
#include "fxmod/fx_095.h"
extern s32 D_801E882C[];
extern s32 D_801E885C[];
extern s32 D_801E888C[];

s32 func_801E7704(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx095Ctl *ctl) {

    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale;
    s32 a;
    s32 glow;
    s32 bright;
    s32 glowFrame;
    s32 abr;
    s32 t;
    s32 r;
    s32 x;
    s32 y;
    s32 *tab;

    if (mode == 1) {
        part->unk4 = 0;
        return 0;
    }
    bright = glow = a = scale = 0;
    glowFrame = 0;
    abr = scale;
    switch (part->unk4) {
    case 0:
        t = frame << 9;
        scale = ((ex->math_cos(t >> 2) * 3) >> 1) + 0x1000;
        a = part->unk0 + ((0x1000 - t) >> 1);
        abr = 1;
        bright = 0x1000;
        if (frame >= 8) {
            part->unk4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        scale = 0x1000;
        a = part->unk0;
        abr = 1;
        bright = scale;
        if (frame < 5) {
            t = frame << 10;
            glow = ex->math_cos(t >> 2);
            glowFrame = frame;
        }
        if (ctl->unkC != 0) {
            part->unk4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        t = frame << 8;
        scale = 0x1000;
        ex->math_cos(t >> 2);
        abr = 0xFF;
        bright = scale;
        a = part->unk0;
        if (frame >= 16) {
            part->unk4 = 3;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 3:
        t = frame << 9;
        scale = 0x1000;
        bright = ex->math_cos(t >> 2);
        abr = 1;
        a = part->unk0;
        if (frame >= 8) {
            return 1;
        }
        break;
    }
    t = (scale * 62) >> 12;
    a -= 0x400;
    x = ctl->unk8 + ((ex->math_cos(a) * t) >> 12);
    y = ctl->unkA + ((ex->math_sin(a) * t) >> 12);
    if (part->unk4 < 2) {
        tab = D_801E882C;
    } else {
        tab = D_801E888C;
    }
    ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, tab[part->unk8]), x, y, 0, abr, bright);
    if (glow != 0) {
        ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E885C[part->unk8]), x, y, 0, 1, glow);
        ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x2C), x, y, glowFrame, 1, 0x1000);
    }
    return 0;
}
