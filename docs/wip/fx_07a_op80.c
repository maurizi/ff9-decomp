/* Draft of fx_07a fx_07a_op80 (types in include/fxmod/fx_07a.h). Status: 3 lines (build/w/nd.py): (1) the
 * point-loop preheader sets a2 = tr before a3 = tr + 10 in the original (giv init order; permuting the loop body
 * statements didn't help); (2) "if (r < 0) r = -r" before the mult: the original leaves a nop in the bgez delay
 * slot, ours copies the target's mult into it (reorg). abs() puts the test above the math_sin call; dead loop notes
 * around the if change nothing. Fixed: q = tr copy before the point loop, passed to btlfx_drawTrail (the
 * original's s0 copy); "j < n - 1" in the jitter loop (permuter). */
#include "fxmod/fx_07a.h"

s32 fx_07a_op80(s32 mode, Fx07aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR out;
    VECTOR cur;
    VECTOR step;
    Fx07aStrip *tr;
    Fx07aStrip *q;
    Fx07aPt *pt;
    s32 frame;
    s32 i;
    s32 j;
    s32 n = 11;
    s32 w;
    s32 r;
    s32 a;
    s32 b;
    s32 s;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x108;
    } else if (mode == 1) {
        D_801E94F4 = env;
        ex->btlfx_initEffModels(D_801E9934, 3);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            D_801E9BA4[i].unk10C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E9BA4[i], 0);
        }
        work->unk94.vx = D_801E9B94.vx = env->unkC->unk38;
        work->unk94.vy = D_801E9B94.vz = env->unkC->unk3C;
        work->unk94.vz = D_801E9B94.vy = -0x500;
        work->unk40[0].unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x137,
                                                         0xFFFF, 0, 0);
        work->unk40[0].unk4.vx = 0x800;
        work->unk40[0].unk4.vy = 0;
        work->unk40[0].unk4.vz = 0;
        work->unk40[0].unkC.vx = 0x5C;
        work->unk40[0].unkC.vy = 0x5C;
        work->unk40[0].unkC.vz = 0x5C;
        work->unk40[1].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD));
        work->unk40[1].unk4.vx = 0;
        work->unk40[1].unk4.vy = 0;
        work->unk40[1].unk4.vz = 0;
        work->unk40[1].unkC.vx = 0x50;
        work->unk40[1].unkC.vy = 0x50;
        work->unk40[1].unkC.vz = 0x50;
        work->unk40[2].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE));
        work->unk40[2].unk4.vx = 0;
        work->unk40[2].unk4.vy = 0;
        work->unk40[2].unk4.vz = 0;
        work->unk40[2].unkC.vx = 0x66;
        work->unk40[2].unkC.vy = 0x66;
        work->unk40[2].unkC.vz = 0x66;
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unkA4[i]);
            work->unkE4[i] = ex->btlfx_rand() & 7;
        }
        D_801E94F8 = 0;
        D_801E9500 = 0;
        D_801E94FC = 0;
        D_801E9504 = 0;
        D_801E9514[4] = 0;
        D_801E9514[3] = 0;
        D_801E9514[2] = 0;
        D_801E9514[1] = 0;
        D_801E9514[0] = 0;
    } else {
        frame = entry->unk0;
        ex->func_800DC818(0x10, 0x17, (u16 *)&D_801E9B8C);
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x1C0, 0x17E, 0x70, 0x70, D_801E9534, D_801E9B94.vx, D_801E9B94.vz, 0x1208);
            break;
        case 1:
            work->unk0[9] = 1;
            work->unk0[1] = 1;
            work->unk0[0] = 0x24;
            work->unk0[2] = 8;
            work->unk0[4] = 0x10;
            break;
        case 8:
            work->unk0[10] = 1;
            work->unk0[11] = 8;
            work->unk0[12] = 0x20;
            work->unk0[6] = 0x1C;
            break;
        case 12:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&D_801E9B94);
            break;
        case 24:
            work->unk0[13] = 0x10;
            work->unk0[5] = 0x10;
            break;
        case 36:
            work->unk0[7] = 4;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9B94);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 40:
            work->unk0[10] = 0;
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 0xFF, 0xFF, 0xFF, 0xFF);
            work->unk0[1] = 0;
            break;
        }
        pos.vx = D_801E9B94.vx;
        pos.vy = 0;
        pos.vz = D_801E9B94.vz;
        if (work->unk0[11] != 0) {
            work->unk0[11]--;
            D_801E9508 = func_801E78E4(0, 8, work->unk0[11], 0, 0x4A00);
            D_801E94F8 = func_801E78E4(0, 8, work->unk0[11], 0, 0x5A00);
            D_801E950C = func_801E78E4(0, 8, work->unk0[11], 0, 0xB20);
            D_801E9500 = func_801E78E4(0, 8, work->unk0[11], 0, 0x340);
        }
        if (work->unk0[12] != 0) {
            work->unk0[12]--;
            D_801E9514[1] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0x1000);
            D_801E9514[2] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xEAB);
            D_801E9514[3] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xD56);
            D_801E9514[4] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xC00);
            D_801E94FC = func_801E78E4(1, 0x20, work->unk0[12], 0, 0x340);
        }
        if (work->unk0[13] != 0) {
            work->unk0[13]--;
            D_801E9508 = func_801E78E4(0, 0x10, work->unk0[12], 0x4A00, 0);
            D_801E950C = func_801E78E4(0, 0x10, work->unk0[13], 0xB20, 0);
            D_801E94F8 = func_801E78E4(0, 0x10, work->unk0[12], 0x5A00, 0);
        }
        if (work->unk0[10] != 0) {
            func_801E8A88(&pos, 0xFFF, 0x117, -1, 0xFF);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            func_801E77A8(0, 8, work->unk0[2], &D_801E9B8C, &D_801E9B94, &out);
            work->unk94.vx = out.vx;
            work->unk94.vy = out.vy;
            work->unk94.vz = out.vz;
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x5C, 0x2A2);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x50, 0x243);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x66, 0x299);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x2A2, 0);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x243, 0);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x299, 0);
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                if (0x1C - work->unk0[6] >= work->unkE4[i]) {
                    pos.vx = work->unkA4[i].vx;
                    pos.vz = work->unkA4[i].vz;
                    pos.vy = func_801E78E4(2, 0x1C - work->unkE4[i], work->unk0[6], work->unkA4[i].vy,
                                           work->unkA4[i].vy - 0x200);
                    ex->btlfx_setUnitPos(i, (s16 *)&pos);
                }
            }
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                if (work->unk0[7] != 0) {
                    pos.vx = work->unkA4[i].vx;
                    pos.vz = work->unkA4[i].vz;
                    pos.vy = func_801E78E4(2, 4, work->unk0[7], work->unkA4[i].vy - 0x200, work->unkA4[i].vy);
                } else {
                    ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (work->unk0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&D_801E9B94, (s16 *)&env->unk2C->unk54, (s16 *)&work->unk40[1].unk4);
            work->unk40[0].unk4.vx = work->unk40[1].unk4.vx + 0x800;
            work->unk40[0].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vx = work->unk40[1].unk4.vx + 0x400;
            work->unk40[1].unk4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->unk40[1].unk4, &work->unk94, (s32)&work->unk40[1].unkC, work->unk40[1].unk0);
            ex->Hi_DrawEffModel(&work->unk40[2].unk4, &work->unk94, (s32)&work->unk40[2].unkC, work->unk40[2].unk0);
            ex->Hi_DrawEffModel(&work->unk40[0].unk4, &work->unk94, (s32)&work->unk40[0].unkC, work->unk40[0].unk0);
            ex->Hi_ModifyEffModelAbr(work->unk40[0].unk0, 1);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < 5; i++) {
                tr = &D_801E9BA4[i];
                if (tr->unk10C > 0) {
                    tr->unk10C--;
                    tr->unk0 = tr->unk2 = n - 1;
                    tr->unk1 = 0;
                    cur.vx = D_801E9B8C.vx << 12;
                    cur.vy = D_801E9B8C.vy << 12;
                    cur.vz = D_801E9B8C.vz << 12;
                    step.vx = ((work->unk94.vx + tr->unk5C - D_801E9B8C.vx) << 12) / n;
                    step.vy = ((work->unk94.vy + tr->unk60 - D_801E9B8C.vy) << 12) / n;
                    step.vz = ((work->unk94.vz + tr->unk64 - D_801E9B8C.vz) << 12) / n;
                    q = tr;
                    for (j = 0, pt = tr->unk4; j < n; j++, pt++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->unk6C[j] + (cur.vy >> 12);
                        pt->unk6 = (u16)tr->unk10C * 4;
                        cur.vx += step.vx;
                        cur.vy += step.vy;
                        cur.vz += step.vz;
                        tr->unk6C[j] += tr->unkBC[j];
                    }
                    if (tr->unk10C < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, frame, 0x1000,
                                            tr->unk10C << 10, 1, (u8 *)q, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, frame, 0x1000,
                                            0x1000, 1, (u8 *)q, 0);
                    }
                } else if (tr->unk10C < 0) {
                    tr->unk10C++;
                } else if (work->unk0[0] >= 5) {
                    tr->unk10C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    a = ex->btlfx_rand() & 0xFFF;
                    b = ex->btlfx_rand() & 0xFFF;
                    r = (ex->math_cos(a) * (work->unk40[0].unkC.vx + 0x80)) >> 12;
                    tr->unk60 = r;
                    s = ex->math_sin(b);
                    if (r < 0) {
                        r = -r;
                    }
                    tr->unk5C = (s * r) >> 12;
                    tr->unk64 = (ex->math_cos(b) * r) >> 12;
                    tr->unkBC[0] = 0;
                    tr->unk6C[0] = 0;
                    for (; j < n - 1; j++) {
                        w = j * 8 + 0x10;
                        tr->unk6C[j] = ex->btlfx_randRange(-w * 2, w * 2);
                        tr->unkBC[j] = ex->btlfx_randRange(-w, w);
                    }
                    tr->unkBC[j] = 0;
                    tr->unk6C[j] = 0;
                }
            }
        }
        if (work->unk0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 8), work->unk0[9]);
            work->unk0[9]++;
        }
        return frame >= 0x46;
    }
    return 0;
}
