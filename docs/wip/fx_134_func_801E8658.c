/* Draft of fx_134 func_801E8658 (orb pool callback). Status: 4 instructions off (build/sdiff.py --count says 10,
 * 6 of them the jump table listing); tools/equiv.py agrees (62/62 trials, 76% coverage). Left: in case 4 sched2 puts
 * `sll a0,s6,6` (the cos argument) into the pos.vz load delay; the original uses the pos.vy one (case 3, the same
 * shape, matches). What got it here: mode 1 falling through to the common `return 0`, `if (frame >= N) return 1;`
 * per case, an unused SVECTOR for the 0xA0 frame, and `sin(a >> 1)` inline in case 2 (a separate `a >>= 1` moves
 * either the &it->unk14 argument or the shift).
 * Pass 2 (sched2 trace): sll a0 (insn 1313) depends only on the sra; it becomes ready at T-37 and is the only ready
 * insn at T-39, so it fills the pos.vz load delay. The original stalls there instead, so in its sched2 input the
 * sll must have had another successor among the pos.vy/pos.vz copies. A single-insn argument (frame << 4) does land
 * in the pos.vy delay; no source order of the *31 statements, the pos copies or the scale line (144 variants), and
 * no temporary for frame << 6 changes the two-insn case. The permuter found nothing in ~3800 iterations. fx_0e0
 * func_801E8628 (0.98 twin, other partition) has the same open instruction. */
#include "fxmod/fx_134.h"

typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* phase */
    s16 unk4; /* velocity */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* size index */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    Pool6CEntry *unk20;
} Fx134Orb; /* 0x24 */

typedef struct {
    u8 pad0[8];
    SVECTOR unk8; /* target */
    u8 pad10[0x5C - 0x10];
    s32 unk5C; /* models */
    u8 pad60[0x70 - 0x60];
    s32 unk70;
} Fx134Work80;

extern s32 D_801EC868[];
extern s32 D_801EF97C; /* catch radius */
extern s32 D_801EF980;

s32 func_801E8658(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work80 *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR d;
    SVECTOR old;
    SVECTOR t;
    SVECTOR pad;
    Pool6CEntry *e;
    s32 k;
    s32 a;
    s32 c;
    s32 i;

    if (mode == 1) {
        it->unk2 = 0;
        it->unk14.vx = ex->btlfx_rand();
        it->unk14.vy = ex->btlfx_rand();
        it->unk14.vz = 0;
        it->unk1C = ex->btlfx_rand() % 64;
        it->unk1E = ex->btlfx_rand() % 32;
        it->unk20 = NULL;
    } else {
    switch (it->unk0) {
    case 0:
    case 1:
        k = 0;
        switch (it->unk2) {
        case 0:
            it->unk14.vx += it->unk1C;
            it->unk14.vy += it->unk1E;
            old.vx = it->unkC.vx;
            old.vy = it->unkC.vy;
            old.vz = it->unkC.vz;
            it->unkC.vx += it->unk4;
            it->unkC.vy += it->unk6;
            it->unkC.vz += it->unk8;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vx = scale.vy = scale.vz = D_801EC868[it->unkA];
            ex->Hi_SetEffModelOffset(parent->unk5C, 0x40);
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            d.vx = parent->unk8.vx - it->unkC.vx;
            d.vy = parent->unk8.vy - it->unkC.vy;
            d.vz = parent->unk8.vz - it->unkC.vz;
            if (frame < 8) {
                k = (frame << 12) >> 3;
            } else {
                k = 0x1000;
            }
            if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < D_801EF97C * D_801EF97C * D_801EF97C) {
                it->unk2 = 1;
                ((s16 *)it)[-1] = -1;
                it->unk20 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&it->unkC);
                if (e != NULL) {
                    e->unk22 = 0x20;
                }
            }
            break;
        case 1:
            scale.vx = scale.vy = scale.vz = D_801EC868[it->unkA];
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            ex->Hi_SetEffModelOffset(parent->unk5C, 0x40);
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            ex->btlfx_aimAngles((s16 *)&parent->unk8, (s16 *)&it->unkC, (s16 *)&ang);
            ang.pad = 1;
            if (it->unk20 != NULL) {
                it->unk20->unk5C.vx = ang.vx;
                it->unk20->unk5C.vy = ang.vy;
                it->unk20->unk5C.vz = ang.vz;
            }
            if (frame < 4) {
                k = 0x1000 - ((frame << 12) >> 2);
            }
            it->unk4 = it->unk4 * 2 / 3;
            it->unk6 = it->unk6 * 2 / 3;
            it->unk8 = it->unk8 * 2 / 3;
            env->unk28 = 0x1F4;
            if (frame < 8) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &it->unkC, NULL, 0x3000, frame, 1,
                                  ex->math_cos(frame << 7));
            }
            env->unk28 = 0x20;
            if (frame < 8) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &it->unkC, &ang, 0x2000, frame, 1,
                                  ex->math_cos(frame << 7));
            }
            env->unk28 = 0;
            if (D_801EF980 != 0) {
                return 1;
            }
            break;
        }
        if (k != 0) {
            env->unk28 = 0x180;
            BTLFX_COPY_SVEC(&t, &it->unkC);
            t.vx -= it->unk4;
            t.vy -= it->unk6;
            t.vz -= it->unk8;
            for (i = 0; i < 12; i++) {
                t.vx -= it->unk4;
                t.vy -= it->unk6;
                t.vz -= it->unk8;
                func_801E7B38(&t, NULL, 0x600, k, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
            }
            env->unk28 = 0;
        }
        break;
    case 2:
        a = (frame << 12) / 24;
        scale.vx = scale.vy = (ex->math_sin(a >> 2) >> 1) + 0x800;
        scale.vz = it->unkA + 0x200;
        it->unk14.vz -= 0x40;
        it->unkC.vy -= 0x4D;
        pos.vx = it->unkC.vx;
        pos.vy = it->unkC.vy;
        pos.vz = it->unkC.vz;
        ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk70);
        ex->Hi_ModifyEffModelRGB(parent->unk70, (ex->math_sin(a >> 1) >> 5) - 0x80, (ex->math_sin(a >> 1) >> 5) - 0x80,
                                 (ex->math_sin(a >> 1) >> 5) - 0x80);
        if (frame >= 24) {
            return 1;
        }
        break;
    case 3:
        it->unk14.vx += it->unk1C * 2;
        it->unk14.vy += it->unk1E * 2;
        it->unkC.vx += it->unk4;
        it->unkC.vy += it->unk6;
        it->unkC.vz += it->unk8;
        pos.vx = it->unkC.vx;
        pos.vy = it->unkC.vy;
        pos.vz = it->unkC.vz;
        scale.vz = ((it->unkA * ex->math_cos((frame << 8) >> 2)) >> 12) * 2;
        scale.vx = scale.vy = scale.vz / 3;
        ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
        ex->Hi_ModifyEffModelRGB(parent->unk5C, -0x4B, -0x4B, -0x4B);
        if (frame >= 16) {
            return 1;
        }
        break;
    case 4:
        it->unk14.vx += it->unk1C;
        it->unk14.vy += it->unk1E;
        it->unkC.vx += it->unk4;
        it->unkC.vy += it->unk6;
        it->unkC.vz += it->unk8;
        it->unk4 = (it->unk4 * 31) >> 5;
        it->unk6 = (it->unk6 * 31) >> 5;
        it->unk8 = (it->unk8 * 31) >> 5;
        pos.vx = it->unkC.vx;
        pos.vy = it->unkC.vy;
        pos.vz = it->unkC.vz;
        scale.vz = ((it->unkA * ex->math_cos((frame << 6) >> 2)) >> 12) * 2;
        scale.vx = scale.vy = scale.vz >> 1;
        ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
        ex->Hi_ModifyEffModelRGB(parent->unk5C, 0, -0x4B, -0x4B);
        if (frame >= 0x40) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
