/* Draft of fx_05e op80: the fx_0ed op80 draft (identical body) with fx_05e's symbols and file ids. Status: 72
 * aligned mismatches, 0 ignoring registers. Case 0: the original has t0 in s0, the spawn pointer p in s1 and
 * &work->unk18 in s5 (here p s0, &work->unk18 s1, t0 s5); case 2: &work->unk8/&work->unk18 in s0 with p in s1.
 * gsim says the target needs t0 allocated before p (priority 2.13 vs 0.15 here) and &work->unk18 after
 * ex/work/frame, or t0 preferring s0 and &work->unk18 preferring s5. The previous 05e draft (209/13) is in the
 * history. Matching this also closes fx_0ed op80 (fxmod_propagate.py). */
#include "fxmod/fx_05e.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* unit position */
    SVECTOR unk10; /* unit rotation */
    SVECTOR unk18; /* model position */
    SVECTOR unk20;
    s32 unk28; /* model */
    SVECTOR unk2C; /* model rotation */
    VECTOR unk34; /* model scale */
} Fx0edW; /* 0x40 */

typedef struct {
    u8 pad0[0x54];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180;
    } *unk54;
    u8 pad58[0x60 - 0x58];
    s32 unk60; /* summon model */
} Fx0edS;


s32 fx_05e_op80(s32 mode, Fx0edW *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    u8 *file;
    s32 state;
    s32 frame;
    s32 amp;
    s32 t;
    s32 t0;
    s32 t3;
    s32 k;
    s32 i;
    Pool6CEntry *p;
    SVECTOR *sv;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8E74 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk10);
        work->unk10.vy &= 0xFFF;
        if ((u32)(work->unk10.vy - 0x400) > 0x800) {
            work->unk10.vy = 0;
        } else {
            work->unk10.vy = 0x800;
        }
        ex->func_800CFCA8(&work->unk10, ex->btlfx_getUnitRadius(0) + 0x78, (s32)&pos);
        work->unk18.vx = work->unk8.vx + ((SVECTOR *)&pos)->vx;
        work->unk18.vy = work->unk8.vy + ((SVECTOR *)&pos)->vy;
        work->unk18.vz = work->unk8.vz + ((SVECTOR *)&pos)->vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        BTLFX_COPY_SVEC(&((SVECTOR *)&pos)[1], &work->unk18);
        ((SVECTOR *)&pos)[1].vy -= 0x140;
        env->unk2C->unk0[0].vx = ((SVECTOR *)&pos)[1].vx;
        env->unk2C->unk0[0].vy = ((SVECTOR *)&pos)[1].vy;
        env->unk2C->unk0[0].vz = ((SVECTOR *)&pos)[1].vz;
        ex->func_800CFCA8(&work->unk10, -0xD2, (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->unk18.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = work->unk18.vy + ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->unk18.vz + ((SVECTOR *)&pos)->vz;
        env->unk2C->unk0[1].vx = ((SVECTOR *)&pos)[1].vx;
        env->unk2C->unk0[1].vy = ((SVECTOR *)&pos)[1].vy;
        env->unk2C->unk0[1].vz = ((SVECTOR *)&pos)[1].vz;
        ex->func_800CFCA8(&work->unk10, -ex->btlfx_getUnitRadius(0), (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->unk8.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->unk8.vz + ((SVECTOR *)&pos)->vz;
        env->unk2C->unk0[2].vx = ((SVECTOR *)&pos)[1].vx;
        env->unk2C->unk0[2].vy = ((SVECTOR *)&pos)[1].vy;
        env->unk2C->unk0[2].vz = ((SVECTOR *)&pos)[1].vz;
        file = (u8 *)ex->btlfx_getFileData(entry->unk8, 2);
        ex->func_800E9000((Unk800E9000 *)ex->btlfx_getFileData(entry->unk8, 6), (s32 *)&file, 1, &D_801E8E84);
        ex->Hi_SetSummonMotion(((Fx0edS *)env->unk4)->unk54->unk180, ((Fx0edS *)env->unk4)->unk60);
        work->unk34.vx = work->unk34.vy = work->unk34.vz = 0x1000;
        work->unk2C.vx = 0;
        work->unk2C.vz = 0;
        work->unk2C.vy = work->unk10.vy;
        ex->btlfx_initEffModels(D_801E9014, 1);
        work->unk28 = (s16)ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0);
    } else {
        state = work->unk0;
        frame = entry->unk0;
        amp = 0;
        switch (state) {
        case 0:
            t0 = (frame << 12) / 24;
            if (frame == 0) {
                do {
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk18);
                } while (0);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk14 = D_801E88D4;
                    p->unk12 = 0x18;
                    p->unkC = 0x3C;
                    p->unk22 = 0x40;
                }
            }
            if (frame > 0) {
                s32 h;

                pos.vx = work->unk18.vx;
                pos.vy = work->unk18.vy;
                pos.vz = work->unk18.vz;
                h = ((0x1000 - ex->math_sin(t0 >> 2)) * 0x514) >> 12;
                pos.vy += h;
                ex->Hi_SetEffModelSlice(work->unk28, h);
                ex->Hi_DrawSliceEffModel(&work->unk2C, &pos, (s32)&work->unk34, work->unk28);
                ex->Hi_ModifyEffModelRGB(work->unk28, (((t0 >> 1) + 0x800) >> 5) - 0x80, (((t0 >> 1) + 0x800) >> 5) - 0x80,
                                         (((t0 >> 1) + 0x800) >> 5) - 0x80);
            }
            if (frame >= 0x18) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            t0 = ((Fx0edS *)env->unk4)->unk60;
            ex->Hi_SetSummonMotFrame(t0, 0);
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)&work->unk34, ((Fx0edS *)env->unk4)->unk60, 1);
            if (frame >= 8) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotFrame(((Fx0edS *)env->unk4)->unk60, 0);
            }
            break;
        case 2:
            t = frame << 12;
            /* Dead store: only its sign word survives. */
            amp = t / 30;
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)&work->unk34, ((Fx0edS *)env->unk4)->unk60, 1);
            if (frame == 0) {
                amp = -1;
            } else if (frame < 0x18) {
                amp = t / 24;
            } else {
                amp = 0x1000;
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 0x1F) {
                s32 ang;

                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8,
                                                           *(D_801E8860 + (((ang = work->unk10.vy & 0xFFF) - 0x400U > 0x800) !=
                                                                      ((ang = env->unk2C->unk50 & 0xFFF) - 0x400U > 0x800)))),
                                      (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0x21) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xDC, 0, 0x20);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0x26) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk14 = D_801E90E4;
                    p->unk12 = 0x18;
                    p->unkC = 0x44;
                    p->unk22 = 0x40;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xB), frame);
            if (frame >= 0x29) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            t3 = (frame << 9) >> 2;
            work->unk34.vx = work->unk34.vz = ((0x1000 - ex->math_sin(t3)) * 0x1000) >> 12;
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            ex->Hi_SetSummonMotFrame(((Fx0edS *)env->unk4)->unk60, 0x29 - frame);
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)&work->unk34, ((Fx0edS *)env->unk4)->unk60, 1);
            amp = ex->math_cos(t3);
            ex->Hi_ModifySummonModelRGB(((Fx0edS *)env->unk4)->unk60, (amp >> 5) - 0x80, (amp >> 5) - 0x80,
                                        (amp >> 5) - 0x80);
            if (frame >= 8) {
                work->unk0 = 3;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (amp != 0) {
            func_801E7B74(((Fx0edS *)env->unk4)->unk60, 9, &D_801E8874, &a);
            func_801E7B74(((Fx0edS *)env->unk4)->unk60, 9, &D_801E887C, &b);
            func_801E7B74(((Fx0edS *)env->unk4)->unk60, 9, &D_801E8884, &c);
            if (amp < 0) {
                func_801E7704(&a, &b, 0x3F8, (CVECTOR *)&D_801E8868, (CVECTOR *)&D_801E886C, 0, D_801E9744, 1);
                func_801E7704(&b, &c, 0x3F8, (CVECTOR *)&D_801E886C, (CVECTOR *)&D_801E8870, 0, D_801E9854, 1);
            } else {
                func_801E7704(&a, &b, 0x10, (CVECTOR *)&D_801E8868, (CVECTOR *)&D_801E886C, amp, D_801E9744, 1);
                func_801E7704(&b, &c, 0x10, (CVECTOR *)&D_801E886C, (CVECTOR *)&D_801E8870, amp, D_801E9854, 1);
                BTLFX_COPY_SVEC(&work->unk20, &a);
                {
                    s32 o;
                    s32 p2;
                    SVECTOR *sv2;

                    sv = (SVECTOR *)D_801E9744;
                    for (i = 0, o = 8; i < 17; o += 16, i++) {
                        sv[i * 2].vy -= sv[i * 2].vy >> 6;
                        ((SVECTOR *)(o + (s32)sv))->vy -= ((SVECTOR *)(o + (s32)sv))->vy >> 4;
                    }
                    sv2 = (SVECTOR *)D_801E9854;
                    i = 0;
                    p2 = 8;
                    for (; i < 17; p2 += 16, i++) {
                        sv2[i * 2].vy -= sv2[i * 2].vy >> 4;
                        ((SVECTOR *)(p2 + (s32)sv2))->vy -= ((SVECTOR *)(p2 + (s32)sv2))->vy >> 3;
                    }
                }
            }
        }
        work->unk4++;
    }
    return 0;
}
