#include "fxmod/fx_04f.h"

s32 func_801E7704(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 a;
    s32 c;
    s32 s;
    s32 *q;
    s32 c2;

    if (mode == 1) {
        *it = 0;
    } else {
        a = ((frame << 12) / 9) >> 1;
        c = ex->math_sin(a);
        if (frame < 5) {
            c2 = a + 0x200;
            s = ((ex->math_sin((frame << 10) >> 2) * (work->unk4C >> 1)) >> 12) + c2;
        } else {
            c2 = a + 0x200;
            s = (work->unk4C >> 1) + *(q = &c2); /* the address taken changes c2's register (permuter) */
        }
        scale.vy = s;
        scale.vx = s;
        scale.vz = (work->unk50 * 7) >> 3;
        BTLFX_COPY_SVEC(&rot, &work->unk38);
        rot.vy += 0x800;
        ex->func_800CFD78((s16 *)&work->unk14, (s16 *)&rot, *it, (s16 *)&p);
        pos.vx = p.vx;
        pos.vy = p.vy;
        pos.vz = p.vz;
        *it -= 0x18;
        ex->Hi_DrawEffModel(&work->unk38, &pos, (s32)&scale, work->unk4);
        ex->Hi_ModifyEffModelRGB(work->unk4, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (frame >= 9) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_04f/nonmatchings/fx_04f", fx_04f_op80);

INCLUDE_RODATA("asm/us/fx_04f/nonmatchings/fx_04f", fx_04f_headerPtr);
