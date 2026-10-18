/* Draft of fx_057 op81 (near-twin of fx_081 op81). Status: 22 aligned mismatches, 0 ignoring registers: only the
 * registers of case 2's second-loop variables differ (original: a in s0 and j in s2; here swapped).
 * Fixed with the fx_081 op81 draft's levers: literal 1 arguments (the original's constant 1 in fp is the mode test's
 * CSE), one t for cases 0-2 (t = frame << 7 in case 2), s = sin(t >> 2) with the last call recomputing it, case 1
 * sets env->unk28 before t, the case 1 copy block as BTLFX_COPY_SVEC(unkC, D_801EA714) then the unk1C fields then
 * BTLFX_COPY_SVEC(unk14, unkC) and unk14.vy -= 0xC00, a separate c for the second loop's cosine, and the second
 * loop's angle in a (a = t >> 2 first) with a += 0x100 in the for increment. First loop: (frame + 7) & 0xF inline. */
#include "fxmod/fx_057.h"

s32 fx_057_op81(s32 mode, Fx057Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR ang;
    SVECTOR out;
    SVECTOR v;
    SVECTOR out2;
    Pool6CEntry *p;
    s32 t1;
    s32 t2;
    s32 t4;
    s32 s_1;
    s32 s_2;
    s32 frame;
    s32 t;
    s32 a;
    s32 s;
    s32 i;
    s32 j;
    s32 r;
    s32 step;
    s32 cs;
    s32 q;
    s32 c;
    s32 h;

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
        s = ex->math_sin(t >> 2);
        env->unk28 = work->unk8 + 0x40;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unkC, NULL, 0x13000, frame, 1, s);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x22), &work->unkC, NULL, (t >> 1) + 0x3000, 0, 1,
                          s);
        env->unk28 = 0;
        ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unkC, work->unk4, ex->math_sin(t >> 2));
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
        env->unk28 = work->unk8 + 0x40;
        t = (frame << 12) / 7;
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
            BTLFX_COPY_SVEC(&work->unkC, &D_801EA714);
            work->unk1C.vx = work->unkC.vx;
            work->unk1C.vy = work->unkC.vy;
            work->unk1C.vz = work->unkC.vz;
            BTLFX_COPY_SVEC(&work->unk14, &work->unkC);
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
        cs = ex->math_cos(t >> 2);
        r = 0x80;
        i = 0;
        step = ((t >> 7) + 4) * 3;
        ang.vx = work->unk4 * 128;
        ang.vy = work->unk4 * 32;
        q = (t / 40 + 10) * 2;
        do {
            ex->func_800CFD78((s16 *)&work->unkC, (s16 *)&ang, r, (s16 *)&out);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &out, NULL, 0x1000, (frame + 7) & 0xF, 1,
                              (cs * ex->math_sin((i << 11) >> 7)) >> 12, 0, 0);
            i++;
            r += step;
            ang.vx += q * 2;
            ang.vy += q;
        } while (i < 0x2E);
        a = t >> 2;
        c = ex->math_cos(a);
        t4 = ex->math_sin(a) * 0x92E / 4096 + 0x200;
        v.vx = 0;
        v.vy = 0;
        v.vz = 0;
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                v.vx = work->unk4 * 0x18 + 0x200;
            } else {
                v.vx = -0x200 - work->unk4 * 0x18;
            }
            a = work->unk4 * 0x29 + (j << 7);
            for (i = 0; i < 0x10; i++, a += 0x100) {
                ex->func_800D044C(&work->unkC, (s32)&out2, 0, (XY32 *)&v, t4, a);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), &out2, NULL, 0x1800, frame & 0xF, 1,
                                  c, 0, 0);
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
