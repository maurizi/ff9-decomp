/* Draft of fx_1fa fx_1fa_op81. Status: 13 aligned mismatches with registers, 7 ignoring them: (1) mode 1 loads
 * entry->unk8 before the D_801E80EC = 0 store; (2) the init loop's constant 8 is hoisted after a = i (original:
 * before); (3) in the ring loop the original reloads unkC for math_sin and computes unk10 - 2 in v0, then
 * 'move s1,v0' (looks like reload_cse turning a reload of unk10 into a move, i.e. cse1 did not reuse the value).
 * Fixed here: i - (j - 4) via k (keeps j - 4 in the loop), a -= 0x2A9 before i++ and rr/ang before j++ in the
 * increments (biv order). fx_1ee op81 is the same code (one constant differs, 0x60 -> 0x40). */
#include "fxmod/fx_1fa.h"

s32 fx_1fa_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1faRing *r;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 j;
    s32 n;
    s32 rad;
    s32 a;
    s32 ang;
    s32 rr;
    s32 k;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E80EC = 0;
        D_801E80DC = env;
        D_801E811C[0][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 3);
        D_801E811C[0][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x12);
        D_801E811C[0][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x12);
        D_801E811C[0][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x12);
        D_801E811C[1][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 8);
        D_801E811C[1][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xF);
        D_801E811C[1][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xF);
        D_801E811C[1][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xF);
        D_801E811C[2][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x15);
        D_801E811C[2][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10);
        D_801E811C[2][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10);
        D_801E811C[2][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10);
        D_801E811C[3][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x16);
        D_801E811C[3][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xD);
        D_801E811C[3][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xD);
        D_801E811C[3][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xD);
        D_801E811C[4][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x18);
        D_801E811C[4][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xE);
        D_801E811C[4][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xE);
        D_801E811C[4][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xE);
        D_801E811C[5][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x19);
        D_801E811C[5][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xB);
        D_801E811C[5][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xB);
        D_801E811C[5][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xB);
        D_801E811C[6][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A);
        D_801E811C[6][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xC);
        D_801E811C[6][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xC);
        D_801E811C[6][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xC);
        D_801E811C[7][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B);
        D_801E811C[7][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 9);
        D_801E811C[7][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 9);
        D_801E811C[7][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 9);
        D_801E811C[8][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C);
        D_801E811C[8][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xA);
        D_801E811C[8][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xA);
        D_801E811C[8][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xA);
        D_801E811C[9][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 1);
        D_801E811C[9][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 6);
        D_801E811C[9][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 6);
        D_801E811C[9][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 6);
        D_801E811C[10][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 4);
        D_801E811C[10][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 7);
        D_801E811C[10][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 7);
        D_801E811C[10][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 7);
        D_801E811C[11][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x11);
        D_801E811C[11][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 5);
        D_801E811C[11][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 5);
        D_801E811C[11][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, 5);
        for (i = 0, a = 0; i < 12; a -= 0x2A9, i++) {
            for (j = 0, rr = 0x10, ang = a; j < 4; rr += 0x10, ang += 0xC0, j++) {
                D_801E811C[i][j].unk10 = rr;
                D_801E811C[i][j].unkC = ang;
                k = j - 4;
                D_801E811C[i][j].unk0 = i - k;
                D_801E811C[i][j].unk4 = 8;
            }
        }
        D_801E80F4.vx = env->unk2C->unk0[3].vx;
        D_801E80F4.vy = env->unk2C->unk0[3].vy;
        D_801E80F4.vz = env->unk2C->unk0[3].vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            D_801E80EC = 0x20;
            break;
        case 8:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&D_801E80F4);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            break;
        case 0x18:
            for (i = 0; i < env->unkC->unk24; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E80FC[i]);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
            }
            break;
        }
        if (D_801E80EC != 0) {
            D_801E80EC--;
            env->unk28 = 0x200;
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 4; j++) {
                    r = &D_801E811C[i][j];
                    if (D_801E811C[i][j].unk0 != 0) {
                        D_801E811C[i][j].unk0--;
                    } else if (D_801E811C[i][j].unk4 != 0) {
                        do {
                            D_801E811C[i][j].unkC -= 0x40;
                            n = --D_801E811C[i][j].unk4 << 9;
                        } while (0);
                        rad = ((D_801E811C[i][j].unk10 -= 2) << 8) + 0x4000;
                        ex->btlfx_drawParticles(D_801E811C[i][j].unk8, &D_801E80F4, 0, rad, 0, 1, n,
                                          (ex->math_sin(D_801E811C[i][j].unkC) * D_801E811C[i][j].unk10) >> 12,
                                          (ex->math_cos(D_801E811C[i][j].unkC) * D_801E811C[i][j].unk10) >> 12);
                    }
                }
            }
            env->unk28 = 0;
        }
        return frame >= 0x60;
    }
    return 0;
}
