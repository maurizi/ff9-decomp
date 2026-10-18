/* Draft of func_801E80F4 (module fx_0df). Status: 230/232 aligned instructions with ccprobe; one register differs: (s16)p->unk2 lands in v0 instead of a1 before the mult.
 * Declarations it needs that are not yet in include/fxmod/fx_0df.h are repeated below. */
#include "fxmod/fx_0df.h"

s32 func_801E80F4(s32 mode, Fx0dfPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0dfWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 t;
    s32 b;
    s32 c;
    s32 r;
    s32 g;
    s32 k;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
    t = (frame << 12) / 12;
    p->unk14 += p->unk16;
    func_801E80B0(&p->unk4, &rot);
    rot.vz = p->unk14;
    pos.vx = p->unk4.vx;
    pos.vy = p->unk4.vy;
    pos.vz = p->unk4.vz;
    k = ex->math_sin(t >> 2);
    k *= 3;
    scale.vx = scale.vy = scale.vz = ((p->unk2 * k) >> 13) + (p->unk2 >> 1);
    if (frame < 3) {
        b = ((frame + 1) << 12) / 3;
    } else {
        b = ex->math_cos(((frame - 3) << 10) / 9);
    }
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
    ex->Hi_ModifyEffModelRGB(work->unk28, (b >> 5) - 0x80, (b >> 5) - 0x80, (b >> 5) - 0x80);
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk2C);
    ex->Hi_ModifyEffModelRGB(work->unk2C, ((b * 2 - 0x1000) >> 5) - 0x80, ((b * 2 - 0x1000) >> 5) - 0x80,
                             ((b * 2 - 0x1000) >> 5) - 0x80);
    if (frame < 8) {
        p->unkC.vz += 0xFF;
        scale.vx = scale.vy = scale.vx / 5;
        scale.vz = scale.vx / 7;
        ex->Hi_DrawEffModel(&p->unkC, &pos, (s32)&scale, work->unk20);
        c = frame << 10;
        c >>= 3;
        r = (ex->math_cos(c) >> 5) - 0x80;
        g = (ex->math_cos(c) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk20, r, g, (ex->math_cos(c) >> 5) - 0x80);
    }
    env->unk28 = 0x200;
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &p->unk4, 0, (p->unk2 * 3) >> 1, 0, 1, b >> 1, 0, 0);
    env->unk28 = 0;
    if (frame >= 0xC) {
        return 1;
    }
    }
    return 0;
}
