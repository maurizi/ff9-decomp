/* Draft of fx_0fb_1 func_801ECBE0. Status: 80 aligned mismatches with registers, 32 ignoring them, all from
 * `it->unkA >> 4` / `>> 5`: the original loads lh + sra 4/5, ours folds the sign extension into lhu + sll 16 +
 * sra 20/21 (combine merges the shifts; a non-constant shift count gives srav instead). equiv.py: 42/42 agree.
 */
#include "fxmod/fx_0fb_1.h"

typedef struct {
    s16 unk0; /* kind */
    s16 unk2; /* state */
    s16 unk4; /* velocity */
    s16 unk6;
    s16 unk8;
    s16 unkA; /* speed */
    SVECTOR unkC; /* position */
    SVECTOR unk14; /* rotation */
    s16 unk1C; /* spin */
    s16 unk1E;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
} Fx0fb1Rock; /* 0x28 */

typedef struct {
    u8 pad0[0x4C];
    s32 unk4C; /* model */
    s32 unk50; /* model */
    u8 pad54[0x5C - 0x54];
    s32 unk5C; /* ground height */
    u8 pad60[0x6C - 0x60];
    SVECTOR unk6C; /* summon position */
    u8 pad74[0xC0 - 0x74];
    s32 unkC0; /* model */
} Fx0fb1Work80;

extern s32 D_801F0D38;

s32 func_801ECBE0(s32 mode, Fx0fb1Rock *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0fb1Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 state;
    s32 c;
    s32 k;
    s32 a;
    Fx0fb1Rock *q;

    if (mode == 1) {
        it->unk2 = 0;
        it->unk14.vx = ex->btlfx_rand();
        it->unk14.vy = ex->btlfx_rand();
        it->unk14.vz = 0;
        it->unk1C = ex->btlfx_rand() % 128 + 0x80;
        it->unk1E = 0xC0 - ex->btlfx_rand() % 32;
        it->unk24 = NULL;
        it->unk20 = NULL;
    } else {
    if (it->unk0 != 4) {
        it->unk14.vx += it->unk1C;
        it->unk14.vy += it->unk1E;
        BTLFX_COPY_SVEC(&rot, &work->unk6C);
        rot.pad = 1;
    }
    switch (it->unk0) {
    case 0:
    case 3: {
        SVECTOR v;

        state = it->unk2;
        switch (state) {
        case 0:
            if (D_801F0D38 == 0) {
                it->unkC.vz -= it->unkA >> 4;
                it->unkC.vy += it->unkA >> 5;
            }
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vz = scale.vy = scale.vx = 0x199;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, work->unk4C);
            if (D_801F0D34 != 0x1000) {
                c = (D_801F0D34 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk4C, c, c, c);
            }
            if (it->unk0 == 0) {
                if (pos.vz < work->unk5C + 0x20) {
                    it->unk2 = 1;
                    ((s16 *)it)[-1] = -1;
                    a = ex->btlfx_rand();
                    for (k = 0; k < 3; k++, a += 0x555) {
                        q = ex->btlfx_poolSpawn(entry->unk10);
                        if (q != NULL) {
                            q->unk0 = 2;
                            BTLFX_COPY_SVEC(&q->unkC, &it->unkC);
                            q->unkC.vx += ex->btlfx_randCentered(0x100);
                            q->unkC.vy += ex->btlfx_randCentered(0x100);
                            q->unk1C = ex->btlfx_randCentered(0x84);
                            q->unk1E = ex->btlfx_randCentered(0x84);
                            ex->func_800D0580((s16 *)&v, (XY32 *)&work->unk6C, a);
                            ex->func_800CFCA8(&v, it->unkA >> 5, (s32)&q->unk4);
                        }
                    }
                    it->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&it->unkC);
                    if (it->unk20 != NULL) {
                        it->unk20->unk22 = 0x20;
                    }
                    it->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&it->unkC);
                    if (it->unk20 != NULL) {
                        it->unk20->unk22 = 0x20;
                    }
                }
            } else {
                gte_SetRotMatrix(env->unk14);
                gte_SetTransMatrix(env->unk14);
                gte_ldv0(&pos);
                gte_rtps();
                gte_stsxy(&v);
                if (v.vx >= 0xF1 || v.vy >= 0x141) {
                    return 1;
                }
                break;
            }
            break;
        case 1:
            if (it->unk20 != NULL) {
                it->unk20->unk5C.vx = work->unk6C.vx;
                it->unk20->unk5C.vy = work->unk6C.vy;
                it->unk20->unk5C.vz = work->unk6C.vz;
            }
            env->unk28 = 0x40;
            if (frame < 10) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), &it->unkC, NULL, 0x1000, frame, state,
                                  ex->math_cos((frame << 10) / 10));
            }
            env->unk28 = 0x10;
            if (frame < 8) {
                VECTOR scale2;
                VECTOR pos2;
                s32 t;

                if (frame < 3) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &it->unkC, &rot, 0x1800, frame,
                                      state, ex->math_cos((frame << 10) / 3));
                }
                pos2.vx = it->unkC.vx;
                pos2.vy = it->unkC.vy;
                t = (frame << 10) >> 3;
                pos2.vz = it->unkC.vz;
                scale2.vx = scale2.vy = scale2.vz = ex->math_sin(t) + 0x800;
                ex->Hi_DrawEffModel(&rot, &pos2, (s32)&scale2, work->unkC0);
                ex->Hi_ModifyEffModelRGB(work->unkC0, (ex->math_cos(t) >> 5) - 0x80, (ex->math_cos(t) >> 5) - 0x80,
                                         (ex->math_cos(t) >> 5) - 0x80);
            }
            env->unk28 = 0;
            if (it->unk20->unk30 == -1) {
                return 1;
            }
            break;
        default:
            return 0;
        }
        break;
    }
    case 1:
        state = it->unk2;
        switch (state) {
        case 0:
            if (D_801F0D38 == 0) {
                it->unkC.vz -= it->unkA >> 4;
                it->unkC.vy += it->unkA >> 5;
            }
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vz = scale.vy = scale.vx = 0xCC;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, work->unk50);
            if (D_801F0D34 != 0x1000) {
                c = (D_801F0D34 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk50, c, c, c);
            }
            if (work->unk5C > pos.vz) {
                it->unk2 = 1;
                ((s16 *)it)[-1] = -1;
                it->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&it->unkC);
                if (it->unk20 != NULL) {
                    it->unk20->unk22 = 0x20;
                }
                it->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&it->unkC);
                if (it->unk20 != NULL) {
                    it->unk20->unk22 = 0x20;
                }
            }
            break;
        case 1:
            if (it->unk20 != NULL) {
                it->unk20->unk5C.vx = work->unk6C.vx;
                it->unk20->unk5C.vy = work->unk6C.vy;
                it->unk20->unk5C.vz = work->unk6C.vz;
            }
            env->unk28 = 0x40;
            if (frame < 6) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), &it->unkC, NULL, 0x800, frame, state,
                                  ex->math_cos((frame << 10) / 6));
            }
            env->unk28 = 0xC;
            if (frame < 8) {
                VECTOR scale2;
                VECTOR pos2;
                s32 t;

                if (frame < 3) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &it->unkC, &rot, 0x1000, frame,
                                      state, ex->math_cos((frame << 10) / 3));
                }
                pos2.vx = it->unkC.vx;
                pos2.vy = it->unkC.vy;
                t = (frame << 10) >> 3;
                pos2.vz = it->unkC.vz;
                scale2.vx = scale2.vy = scale2.vz = (ex->math_sin(t) >> 1) + 0x555;
                ex->Hi_DrawEffModel(&rot, &pos2, (s32)&scale2, work->unkC0);
                ex->Hi_ModifyEffModelRGB(work->unkC0, (ex->math_cos(t) >> 5) - 0x80, (ex->math_cos(t) >> 5) - 0x80,
                                         (ex->math_cos(t) >> 5) - 0x80);
            }
            env->unk28 = 0;
            if (it->unk20->unk30 == -1) {
                return 1;
            }
            break;
        default:
            return 0;
        }
        break;
    case 2:
        it->unkC.vz += 0x32;
        it->unkC.vy -= 0x19;
        it->unkC.vx += it->unk4;
        it->unkC.vy += it->unk6;
        it->unkC.vz += it->unk8;
        pos.vx = it->unkC.vx;
        pos.vy = it->unkC.vy;
        pos.vz = it->unkC.vz;
        scale.vz = scale.vy = scale.vx = 0xCC;
        ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, work->unk50);
        if (D_801F0D34 != 0x1000) {
            c = (D_801F0D34 >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk50, c, c, c);
        }
        if (frame >= 0xD) {
            return 1;
        }
        break;
    case 4:
        it->unkC.vx = (s16)(it->unkC.vx - 0x80) - it->unkA;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &it->unkC, NULL, (it->unk4 * 3) >> 1, 0,
                                1, -1, 0, 0);
        if (frame >= 0x11) {
            return 1;
        }
        break;
    }
    }
    return 0;
}
