/* Draft of fx_026 func_801E98BC (pool callback of op81: the six barrier pieces and the core). Status: 61 aligned
 * mismatches. Structure matches; the case-0 block schedules the D_801EC5E0 load and the v.vx sign extension
 * differently, and case 2 (frame >= 14) computes the fade into a1 directly where the original goes through s0. Note
 * the extra unused SVECTOR: the original frame has 8 bytes more than the locals need. */
#include "fxmod/fx_026.h"

s32 func_801E98BC(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    SVECTOR unused; /* never used, but it takes a stack slot */
    s32 model;
    s32 model2;
    s32 c;
    Pool6CEntry *p;
    s32 f;
    s32 g;

    if (mode == 1) {
        it->unk0 = 0;
        it->pad4[1] = 0;
    } else if (it->unk2 < 6) {
        switch (it->unk0) {
        case 0:
            func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, D_801EA828[it->unk2]), frame, &v);
            v.vx += work->unk8.vx;
            v.vy += work->unk8.vy;
            pos.vy = v.vy;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            pos.vx = v.vx;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            v.vz += work->unk8.vz;
            pos.vz = v.vz;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EC5E0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, D_801EC5DC);
            if (frame >= 3) {
                it->unk0 = 1;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0x20, 0x46);
                it->unkC.vx = D_801EA840[it->unk2].vx;
                it->unkC.vy = D_801EA840[it->unk2].vy;
                it->unkC.vz = D_801EA840[it->unk2].vz;
                it->unkC.vx += work->unk8.vx;
                it->unkC.vy += work->unk8.vy;
                it->unkC.vz += work->unk8.vz;
                it->unkC.vy = 0;
                p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&it->unkC);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            return 0;
        case 1:
            if (frame - 5 + it->unk2 <= 0) {
                model = work->unk18[it->unk2][0];
                model2 = -1;
            } else {
                if (frame < 16) {
                    model = work->unk18[it->unk2][D_801EA870[frame]];
                } else {
                    model = work->unk18[it->unk2][2];
                }
                if (frame < 16) {
                    model2 = -1;
                } else {
                    model2 = work->unk6C[it->unk2];
                }
            }
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            if (model2 >= 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model2);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model);
            if (it->unk2 < 5) {
                g = D_801EA81C;
            } else {
                g = D_801EA824;
            }
            if (g != 0) {
                it->unk0 = 2;
                ((s16 *)it)[-1] = -1;
                func_801E912C(it->unk2, 0x14, (Fx026Shatter *)((u8 *)env->unk30 + D_801EA7EC[it->unk2].unk0),
                              (Fx026Tri *)((u8 *)env->unk30 + D_801EA7EC[it->unk2].unk4));
                func_801E9170(it->unk2, work->unk18[it->unk2][2]);
                return 0;
            }
            break;
        case 2:
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk18[it->unk2][2]);
            if (frame >= 14) {
                f = 0x1000 - ((frame - 14) << 12) / 6;
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = (f / 2 >> 5) - 0x80;
            } else {
                ex->Hi_ModifyEffModelAbr(work->unk18[it->unk2][2], 1);
                c = -0x40;
            }
            ex->Hi_ModifyEffModelRGB(work->unk18[it->unk2][2], c, c, c);
            func_801E92EC(it->unk2);
            func_801E9564(it->unk2);
            if (frame >= 20) {
                return 1;
            }
            break;
        }
    } else {
    switch (it->unk0) {
    case 0:
        pos.vx = work->unk8.vx;
        pos.vy = work->unk8.vy;
        pos.vz = work->unk8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
        if (frame < 8) {
            ex->Hi_ModifyEffModelRGB(work->unk6C[6], ((frame << 12) >> 9) - 0x80, ((frame << 12) >> 9) - 0x80,
                                     ((frame << 12) >> 9) - 0x80);
        }
        if (D_801EA820 != 0) {
            it->unk0 = 1;
            ((s16 *)it)[-1] = -1;
            func_801E912C(it->unk2, 0x57, D_801EDA0C, D_801EF00C);
            func_801E9170(it->unk2, work->unk6C[6]);
            return 0;
        }
        break;
    case 1:
        pos.vx = work->unk8.vx;
        pos.vy = work->unk8.vy;
        pos.vz = work->unk8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk6C[6]);
        if (frame >= 55) {
            ex->Hi_ModifyEffModelRGB(work->unk6C[6], ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80);
        }
        func_801E92EC(6);
        func_801E9564(6);
        if (frame >= 87) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
