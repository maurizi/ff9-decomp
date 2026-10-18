/* Draft of fx_05e op80 (summon: model rises out of a slice, then ribbons from three bones). Identical body: fx_0ed
 * op80; fx_09a op80 is 0.74 similar. Status: 264 aligned mismatches, 32 ignoring registers. Pass 2 levers: one
 * if / else if / else with a single return 0 (mode 0 and 1 join the common return), case 2 computes t = frame << 12
 * after the summon draw and k = t / 24 from it, case 3's angle shares t, the ribbon decay loops go through
 * sg = D_801E9744 / D_801E9854 with ((SVECTOR *)sg)[i * 2 + 1] for the second edge point, and the D_801E8860 index
 * is i = (... > 0x800) != (... > 0x800) as its own statement. With these env/one land in s6/s5 as in the original.
 * Open: p (s0 here, s1 in the original), ex (s1 / s2), state (s2 / s1), t (s5 / s0) in global-alloc order; the
 * original addresses the decay loops' second point as base + (8 + 16 * i) with an integer giv. */
#include "fxmod/fx_05e.h"

s32 fx_05e_op80(s32 mode, Fx05eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        VECTOR pos;
        SVECTOR sv[2];
    } u;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    s32 file;
    Pool6CEntry *p;
    s32 frame;
    s32 state;
    s32 t;
    s32 k;
    s32 s;
    s32 i;
    s32 v;
    Fx05eSeg *sg;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8E74 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk10);
        work->unk10.vy &= 0xFFF;
        if (work->unk10.vy - 0x400U > 0x800) {
            work->unk10.vy = 0;
        } else {
            work->unk10.vy = 0x800;
        }
        ex->func_800CFCA8(&work->unk10, ex->btlfx_getUnitRadius(0) + 0x78, (s32)&u.sv[0]);
        work->unk18.vx = work->unk8.vx + u.sv[0].vx;
        work->unk18.vy = work->unk8.vy + u.sv[0].vy;
        work->unk18.vz = work->unk8.vz + u.sv[0].vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        BTLFX_COPY_SVEC(&u.sv[1], &work->unk18);
        u.sv[1].vy -= 0x140;
        env->unk2C->unk0[0].vx = u.sv[1].vx;
        env->unk2C->unk0[0].vy = u.sv[1].vy;
        env->unk2C->unk0[0].vz = u.sv[1].vz;
        ex->func_800CFCA8(&work->unk10, -0xD2, (s32)&u.sv[0]);
        u.sv[1].vx = work->unk18.vx + u.sv[0].vx;
        u.sv[1].vy = work->unk18.vy + u.sv[0].vy - 0x140;
        u.sv[1].vz = work->unk18.vz + u.sv[0].vz;
        env->unk2C->unk0[1].vx = u.sv[1].vx;
        env->unk2C->unk0[1].vy = u.sv[1].vy;
        env->unk2C->unk0[1].vz = u.sv[1].vz;
        ex->func_800CFCA8(&work->unk10, -ex->btlfx_getUnitRadius(0), (s32)&u.sv[0]);
        u.sv[1].vx = work->unk8.vx + u.sv[0].vx;
        u.sv[1].vy = u.sv[0].vy - 0x140;
        u.sv[1].vz = work->unk8.vz + u.sv[0].vz;
        env->unk2C->unk0[2].vx = u.sv[1].vx;
        env->unk2C->unk0[2].vy = u.sv[1].vy;
        env->unk2C->unk0[2].vz = u.sv[1].vz;
        file = ex->btlfx_getFileData(entry->unk8, 2);
        ex->func_800E9000((Unk800E9000 *)ex->btlfx_getFileData(entry->unk8, 6), &file, 1, &D_801E8E84);
        ex->Hi_SetSummonMotion(((Fx05eSummon *)env->unk4)->unk54->unk180, ((Fx05eSummon *)env->unk4)->unk60);
        work->unk34[2] = 0x1000;
        work->unk34[1] = 0x1000;
        work->unk34[0] = 0x1000;
        work->unk2C.vx = 0;
        work->unk2C.vz = 0;
        work->unk2C.vy = work->unk10.vy;
        ex->btlfx_initEffModels(D_801E9014, 1);
        work->unk28 = (s16)ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0);
    } else {
    state = work->unk0;
    frame = entry->unk0;
    k = 0;
    switch (state) {
    case 0:
        t = (frame << 12) / 24;
        if (frame == 0) {
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk18);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unk18);
            if (p != NULL) {
                p->unk14 = (s32)D_801E88D4;
                p->unk12 = 0x18;
                p->unkC = 0x3C;
                p->unk22 = 0x40;
            }
        }
        if (frame > 0) {
            u.pos.vx = work->unk18.vx;
            u.pos.vy = work->unk18.vy;
            u.pos.vz = work->unk18.vz;
            s = ((0x1000 - ex->math_sin(t >> 2)) * 0x514) >> 12;
            u.pos.vy += s;
            ex->Hi_SetEffModelSlice(work->unk28, s);
            ex->Hi_DrawSliceEffModel(&work->unk2C, &u.pos, (s32)work->unk34, work->unk28);
            ex->Hi_ModifyEffModelRGB(work->unk28, (((t >> 1) + 0x800) >> 5) - 0x80, (((t >> 1) + 0x800) >> 5) - 0x80,
                                     (((t >> 1) + 0x800) >> 5) - 0x80);
        }
        if (frame >= 24) {
            work->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    case 1:
        u.pos.vx = work->unk18.vx;
        u.pos.vy = work->unk18.vy;
        u.pos.vz = work->unk18.vz;
        ex->Hi_SetSummonMotFrame(((Fx05eSummon *)env->unk4)->unk60, 0);
        ex->Hi_DrawSummonModel(&work->unk2C, &u.pos, (s32)work->unk34, ((Fx05eSummon *)env->unk4)->unk60, state);
        if (frame >= 8) {
            work->unk0 = 2;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotFrame(((Fx05eSummon *)env->unk4)->unk60, 0);
        }
        break;
    case 2:
        u.pos.vx = work->unk18.vx;
        u.pos.vy = work->unk18.vy;
        u.pos.vz = work->unk18.vz;
        ex->Hi_DrawSummonModel(&work->unk2C, &u.pos, (s32)work->unk34, ((Fx05eSummon *)env->unk4)->unk60, 1);
        t = frame << 12;
        if (frame == 0) {
            k = -1;
        } else if (frame < 24) {
            k = t / 24;
        } else {
            k = 0x1000;
        }
        if (frame == 32) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
        }
        if (frame == 31) {
            i = ((work->unk10.vy & 0xFFF) - 0x400U > 0x800) != ((env->unk2C->unk50 & 0xFFF) - 0x400U > 0x800);
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E8860[i]), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
            }
        }
        if (frame == 33) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xDC, 0, 0x20);
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
            }
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x40);
            }
        }
        if (frame == 38) {
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk18);
            if (p != NULL) {
                p->unk14 = (s32)D_801E90E4;
                p->unk12 = 0x18;
                p->unkC = 0x44;
                p->unk22 = 0x40;
            }
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xB), frame);
        if (frame >= 41) {
            work->unk0 = 3;
            entry->unk0 = -1;
        }
        break;
    case 3:
        v = (frame << 9) >> 2;
        work->unk34[2] = work->unk34[0] = ((0x1000 - ex->math_sin(v)) << 12) >> 12;
        u.pos.vx = work->unk18.vx;
        u.pos.vy = work->unk18.vy;
        u.pos.vz = work->unk18.vz;
        ex->Hi_SetSummonMotFrame(((Fx05eSummon *)env->unk4)->unk60, 41 - frame);
        ex->Hi_DrawSummonModel(&work->unk2C, &u.pos, (s32)work->unk34, ((Fx05eSummon *)env->unk4)->unk60, 1);
        k = ex->math_cos(v);
        ex->Hi_ModifySummonModelRGB(((Fx05eSummon *)env->unk4)->unk60, (k >> 5) - 0x80, (k >> 5) - 0x80,
                                    (k >> 5) - 0x80);
        if (frame >= 8) {
            work->unk0 = state;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    if (k != 0) {
        func_801E7B74(((Fx05eSummon *)env->unk4)->unk60, 9, &D_801E8874, &a);
        func_801E7B74(((Fx05eSummon *)env->unk4)->unk60, 9, &D_801E887C, &b);
        func_801E7B74(((Fx05eSummon *)env->unk4)->unk60, 9, &D_801E8884, &c);
        if (k < 0) {
            func_801E7704(&a, &b, 0x3F8, (CVECTOR *)&D_801E8868, (CVECTOR *)&D_801E886C, 0, D_801E9744, 1);
            func_801E7704(&b, &c, 0x3F8, (CVECTOR *)&D_801E886C, (CVECTOR *)&D_801E8870, 0, D_801E9854, 1);
        } else {
            func_801E7704(&a, &b, 0x10, (CVECTOR *)&D_801E8868, (CVECTOR *)&D_801E886C, k, D_801E9744, 1);
            func_801E7704(&b, &c, 0x10, (CVECTOR *)&D_801E886C, (CVECTOR *)&D_801E8870, k, D_801E9854, 1);
            BTLFX_COPY_SVEC(&work->unk20, &a);
            sg = D_801E9744;
            for (i = 0; i < 17; i++) {
                sg[i].unk0.vy -= sg[i].unk0.vy >> 6;
                ((SVECTOR *)sg)[i * 2 + 1].vy -= ((SVECTOR *)sg)[i * 2 + 1].vy >> 4;
            }
            sg = D_801E9854;
            for (i = 0; i < 17; i++) {
                sg[i].unk0.vy -= sg[i].unk0.vy >> 4;
                ((SVECTOR *)sg)[i * 2 + 1].vy -= ((SVECTOR *)sg)[i * 2 + 1].vy >> 3;
            }
        }
    }
    work->unk4++;
    }
    return 0;
}
