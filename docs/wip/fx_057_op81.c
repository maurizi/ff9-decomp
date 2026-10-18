/* Draft of fx_057 op81. Status: ~518 aligned mismatches, register allocation: the original keeps the constant 1 in
 * fp (CSE of the mode test) and spills entry instead; here entry gets fp and the constant is rematerialised. In
 * case 2 the original does not hoist q * 2 out of the first loop. */
#include "fxmod/fx_057.h"

s32 fx_057_op81(s32 mode, Fx057Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR ang;
    SVECTOR out;
    SVECTOR v;
    SVECTOR out2;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 i;
    s32 j;
    s32 r;
    s32 step;
    s32 n;
    s32 cs;
    s32 q;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8EA4 = env;
        work->unk8 = ex->func_800E7CC4(0, 0);
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = work->unk8 + 0x40;
        }
    } else {
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        t = (frame << 12) / 7;
        a = t >> 2;
        s = ex->math_sin(a);
        env->unk28 = work->unk8 + 0x40;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unkC, NULL, 0x13000, frame, 1, s);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), &work->unkC, NULL, (t >> 1) + 0x3000, 0, 1,
                          s);
        env->unk28 = 0;
        ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unkC, work->unk4, ex->math_sin(a));
        if (frame >= 7) {
            work->unk0 = 1;
            entry->unk0 = -1;
            work->unk28 =
                ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x27), 0xB9, 0x3D80, 0x80, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0xFF);
        }
        break;
    case 1:
        d.vx = 0;
        d.vy = 0;
        d.vz = 0x400;
        d.pad = 0;
        t = (frame << 12) / 7;
        env->unk28 = work->unk8 + 0x40;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unkC, &d, 0x3000, frame, 1, 0x1000);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unkC, NULL, t + 0x13000, frame, 1,
                          0x1000);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), &work->unkC, NULL, (t >> 1) + 0x3800, 0, 1,
                          0x1000);
        env->unk28 = 0;
        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x80);
        ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unkC, work->unk4, 0x1000);
        if (frame >= 7) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
            work->unk0 = 2;
            entry->unk0 = -1;
            ((s32 *)&work->unk14)[0] = ((s32 *)&work->unkC)[0] = ((s32 *)&D_801EA714)[0];
            ((s32 *)&work->unk14)[1] = ((s32 *)&work->unkC)[1] = ((s32 *)&D_801EA714)[1];
            work->unk1C.vx = work->unkC.vx;
            work->unk1C.vy = work->unkC.vy;
            work->unk1C.vz = work->unkC.vz;
            work->unk14.vy -= 0xC00;
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x40;
            }
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x40;
            }
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unkC);
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x80;
            }
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2A), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = work->unk8 + 0x46;
            }
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x29), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x46;
            }
        }
        break;
    case 2:
        t = frame << 7;
        env->unk28 = 0;
        if (frame < 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, (0xFF - frame * 0xFF / 6) >> 1,
                                     0xFF - frame * 0xFF / 6, 0xFF - frame * 0xFF / 6);
        }
        if (frame == 7) {
            p = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2B), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = 0x46;
            }
        }
        r = 0x80;
        i = 0;
        cs = ex->math_cos(t >> 2);
        step = ((t >> 7) + 4) * 3;
        ang.vx = work->unk4 << 7;
        n = (frame + 7) & 0xF;
        ang.vy = work->unk4 << 5;
        q = (t / 40 + 10) * 2;
        do {
            ex->func_800CFD78((s16 *)&work->unkC, (s16 *)&ang, r, (s16 *)&out);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &out, NULL, 0x1000, n, 1,
                              (cs * ex->math_sin((i << 11) >> 7)) >> 12, 0, 0);
            i++;
            r += step;
            ang.vx += q * 2;
            ang.vy += q;
        } while (i < 0x2E);
        a = t >> 2;
        cs = ex->math_cos(a);
        t = ex->math_sin(a) * 0x92E / 4096 + 0x200;
        v.vx = 0;
        v.vy = 0;
        v.vz = 0;
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                v.vx = work->unk4 * 0x18 + 0x200;
            } else {
                v.vx = -0x200 - work->unk4 * 0x18;
            }
            s = work->unk4 * 0x29 + (j << 7);
            for (i = 0; i < 0x10; i++) {
                s += 0x100;
                ex->func_800D044C(&work->unkC, (s32)&out2, 0, (XY32 *)&v, t, s);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), &out2, NULL, 0x1800, frame & 0xF, 1,
                                  cs, 0, 0);
            }
        }
        if (frame >= 0x20) {
            work->unk0 = 3;
            entry->unk0 = -1;
        }
        break;
    case 3:
        if (frame >= 0xF) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->unk4++;
    }
    return 0;
}
