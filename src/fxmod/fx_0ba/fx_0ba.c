#include "fxmod/fx_0ba.h"

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = D_801EC5F0;
    POLY_G4 *prim;
    BtlfxExports *ex;
    s32 i;

    w >>= 1;
    prim = (POLY_G4 *)env->unk24;
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++, prim++) {
        setlen(prim, 8);
        prim->code = 0x3A;
        if (!(i & 1)) {
            prim->r0 = prim->r2 = 0;
            prim->g0 = prim->g2 = 0;
            prim->b0 = prim->b2 = b;
            prim->r1 = prim->r3 = r;
            prim->g1 = prim->g3 = g;
            prim->b1 = prim->b3 = b;
            prim->x0 = prim->x2 = 0xA0 - w;
            prim->x1 = prim->x3 = 0xA0;
        } else {
            prim->r1 = prim->r3 = 0;
            prim->g1 = prim->g3 = 0;
            prim->b1 = prim->b3 = b;
            prim->r0 = prim->r2 = r;
            prim->g0 = prim->g2 = g;
            prim->b0 = prim->b2 = b;
            prim->x0 = prim->x2 = 0xA0;
            prim->x1 = prim->x3 = w + 0xA0;
        }
        prim->y0 = prim->y1 = (i >> 1) * 0x78;
        prim->y2 = prim->y3 = prim->y0 + 0x78;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
    }
}

s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR mpos;
    VECTOR scale;
    s32 tint;
    s32 c;
    s32 tint2;
    s32 fade;
    s32 bright;
    s32 k;
    s32 k_2;
    s32 k_1;
    Pool6CEntry *p;
    s16 *keys;
    s32 frame;
    s32 state;
    s32 t;
    s32 n;
    s32 key;

    if (mode == 0) {
        *arg4 = sizeof(Fx0baWork80);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC5F0 = env;
        ex->btlfx_initEffModels(D_801EC600, 6);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0);
        work->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0);
        work->unk48.vx = 0;
        work->unk60.vy = 0x800;
        work->unk54.vx = 0x1000;
        work->unk54.vy = 0x1000;
        work->unk54.vz = 0x1000;
        work->unk48.vy = -0x1770;
        work->unk48.vz = -0xA00;
        work->unk60.vx = 0;
        work->unk60.vz = 0;
        D_801ECAB0.vx = work->unk48.vx;
        D_801ECAB0.vy = work->unk48.vy;
        D_801ECAB0.vz = work->unk48.vz;
        work->unk68.vy = 0x600;
        work->unk68.vx = 0;
        work->unk68.vz = 0;
        work->unk70.vx = 0x6000;
        work->unk70.vy = 0x6000;
        work->unk70.vz = 0x6000;
        work->unk18.vx = work->unk48.vx;
        work->unk18.vy = work->unk48.vy;
        work->unk18.vz = work->unk48.vz;
        env->unk2C->unk0[3].vx = work->unk18.vx;
        env->unk2C->unk0[3].vy = work->unk18.vy;
        env->unk2C->unk0[3].vz = work->unk18.vz;
        env->unk2C->unk0[3].vy -= 0x190;
        D_801ECAB8.vz = 0;
        D_801ECAB8.vy = 0;
        D_801ECAB8.vx = 0;
        work->unk96 = 0;
        work->unk94 = 0;
        work->unk9A = 0;
        work->unk98 = 0;
    } else {
        bright = 0;
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            if (frame < 6) {
                bright = (frame << 12) / 6;
            } else {
                bright = 0x1000;
            }
            tint = 0;
            ex->func_800CFC2C(frame, 0xC, 4, -1, 0x18, &tint);
            ex->func_800CFC2C(frame, 0x14, 4, -1, 0x20, &tint);
            if (tint != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x100), 1, tint, tint, tint >> 1);
            }
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            if (frame >= 0x20) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk180, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            break;
        case 1:
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            t = (frame << 12) / 60 + 0x23;
            pos.vy += ex->math_sin(t * 2) >> 7;
            D_801ECAB0.vx = pos.vx;
            D_801ECAB0.vy = pos.vy;
            D_801ECAB0.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->unk60, &pos, (s32)&work->unk54, ((Fx0baSummon *)env->unk4)->unk60, 0);
            ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->unk4)->unk60, 0x33, (u16 *)&work->unk18);
            work->unk18.vy += 0x40;
            env->unk2C->unk0[2].vx = work->unk18.vx;
            env->unk2C->unk0[2].vy = work->unk18.vy;
            c = 0;
            env->unk2C->unk0[2].vz = work->unk18.vz;
            ex->func_800CFC2C(frame, 2, 7, -1, 0x555, &c);
            ex->func_800CFC2C(frame, 0xC, 2, 0, 0x1800, &c);
            ex->func_800CFC2C(frame, 0xE, 0xA, -1, 0x1800, &c);
            ex->func_800CFC2C(frame, 0x18, 2, 0, 0x2000, &c);
            ex->func_800CFC2C(frame, 0x1A, 0xF, -1, 0x2000, &c);
            c -= ex->math_cos(t >> 2) - 0x1000;
            ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->unk4)->unk60, (c >> 5) - 0x80, (c >> 5) - 0x80,
                                        (c >> 5) - 0x80);
            tint2 = 0;
            ex->func_800CFC2C(frame, 0, 6, -1, 0xBE, &tint2);
            ex->func_800CFC2C(frame, 0xC, 6, -1, 0x80, &tint2);
            ex->func_800CFC2C(frame, 0x18, 0xA, -1, 0xF0, &tint2);
            bright = 0x1000;
            if (tint2 != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xC8), 1, tint2, tint2, tint2 / 3);
            }
            if (frame == 0xC) {
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 1);
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 2);
            }
            if (frame == 0x18) {
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 1);
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 2);
            }
            if (frame == 0x3B) {
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk184, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            if (frame >= 0x3C) {
                if (frame == 0x3C) {
                    work->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 1);
                    work->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 1);
                    work->unk10.vx = work->unk48.vx;
                    work->unk10.vy = work->unk48.vy;
                    work->unk10.vz = work->unk48.vz;
                    work->unk10.vy += 0x2F3;
                    work->unk10.vz += 0x400;
                    D_801ECAB8.vx = work->unk10.vx;
                    D_801ECAB8.vy = work->unk10.vy;
                    D_801ECAB8.vz = work->unk10.vz;
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x86), (s32)&work->unk10);
                    if (p != NULL) {
                        p->unk22 = 0x37;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x84), (s32)&work->unk10);
                    if (p != NULL) {
                        p->unk22 = 0x37;
                    }
                    D_801EC550 = 0;
                }
                bright = 0;
                ex->func_800CFC2C(frame, 0x3C, 0x23, 0, 0x1000, &fade);
                keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x85);
                n = frame - 0x3C;
                if (n >= 0x1E) {
                    n = 0x1D;
                }
                keys += n * 3 + 2;
                key = *keys;
                scale.vz = 0x1000;
                scale.vy = 0x1000;
                scale.vx = 0x1000;
                mpos.vx = work->unk48.vx;
                mpos.vy = work->unk48.vy;
                mpos.vz = work->unk48.vz;
                mpos.vy += 0x2F3;
                mpos.vz += 0x400;
                ex->Hi_DrawMorphEffModel(&work->unk68, &mpos, (s32)&scale, work->unk84, work->unk88, key);
                env->unk28 = 0x100;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x27), &work->unk10, NULL, fade * 3 + 0x1000,
                                  0, 1, ex->math_cos(fade >> 2));
                env->unk28 = 0;
                k_1 = ((fade * 3 / 4 + 0x400) >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->unk4)->unk60, k_1, k_1, k_1);
                ex->func_800E6624(10000 - work->unk4 * 7, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->func_800E6624(10000 - work->unk4 * 7, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            } else {
                ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->func_800E6624(work->unk4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            }
            if (frame >= 0x5F) {
                work->unk0 = 2;
                entry->unk0 = -1;
                D_801ECAB8.vz = 0;
                D_801ECAB8.vy = 0;
                D_801ECAB8.vx = 0;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->unk4)->unk54->unk188, ((Fx0baSummon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->unk4)->unk60, 0);
                work->unk18.vx = work->unk48.vx;
                work->unk18.vy = work->unk48.vy;
                work->unk18.vz = work->unk48.vz;
                work->unk18.vy -= 0x127;
                work->unk18.vz += 0x80;
                env->unk2C->unk0[2].vx = work->unk18.vx;
                env->unk2C->unk0[2].vy = work->unk18.vy;
                env->unk2C->unk0[2].vz = work->unk18.vz;
                work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 1);
                work->unk90 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 1);
            }
            break;
        case 2:
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            D_801ECAB0.vx = pos.vx;
            D_801ECAB0.vy = pos.vy;
            D_801ECAB0.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->unk60, &pos, (s32)&work->unk54, ((Fx0baSummon *)env->unk4)->unk60, 0);
            work->unk48.vy -= 0x1000;
            ex->Hi_DrawEffModel(&work->unk68, (VECTOR *)&work->unk48, (s32)(VECTOR *)&work->unk70, work->unk80);
            work->unk48.vy += 0x1000;
            if (frame == 0x14) {
                ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->unk4)->unk60, 0x2F, (u16 *)&mpos);
                work->unk28.vx = ((SVECTOR *)&mpos)->vx;
                work->unk28.vy = ((SVECTOR *)&mpos)->vy;
                work->unk28.vz = ((SVECTOR *)&mpos)->vz;
                work->unk40.vx = 0x1DA;
                work->unk40.vy = 0;
                work->unk40.vz = 0;
                work->unk34.vz = 0x1000;
                work->unk34.vy = 0x1000;
                work->unk34.vx = 0x1000;
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->unk4)->unk60, 0);
            }
            if (frame >= 0x14) {
                work->unk28.vy += 0xAE;
                work->unk28.vz += 0x96;
                ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk8C);
                ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk90);
                k_2 = ((((frame - 0x16) * 0x180) + 0x400) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk90, k_2, k_2, k_2);
                ex->func_800E6624(work->unk4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
            }
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->func_800E6624(work->unk4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            if (frame >= 0x1E) {
                work->unk0 = state;
                D_801EC54C = 1;
                return 1;
            }
            break;
        }
        if (bright != 0) {
            ex->Hi_DrawEffModel(&work->unk68, (VECTOR *)&work->unk48, (s32)(VECTOR *)&work->unk70, work->unk7C);
            k = (bright >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk7C, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}

void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR pos;
    SVECTOR ang;
    SVECTOR *v;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->unk4;
    buf->unk0 = buf->unk2 = 24;
    buf->unk1 = 0;
    for (i = 0; i < 25; i++, v++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += 0x20 + ((ex->math_sin(t >> 1) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + ex->btlfx_randCentered(0x100);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0, NULL, a7, 0x1000, a8, 1, (u8 *)buf, 0);
}

s32 fx_0ba_op81(s32 mode, Fx0baPWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 base;
    s32 lt;
    s32 type;
    s32 e;
    s32 f;
    s32 g;
    Fx0baPTrail17 *ta;
    Fx0baPTrail17 *tb;
    SVECTOR *pb;
    s16 *s1p;
    s16 *s2p;
    s32 pad;
    s16 *q1;
    s32 k2;
    VECTOR pos2;
    VECTOR scale2;
    SVECTOR rot2;
    s32 arr60[3];
    s32 arr70[3];
    s32 arr80[3];
    s32 arr90[3];
    Fx0baPKey12 *key;
    Fx0baPTrail13 *tc;
    Fx0baPKeys *keys;
    Pool6CEntry *p;
    SVECTOR *pt;
    s32 frame;
    s32 k;
    s32 cw;
    s32 f5;
    s32 q9;
    s32 q10;
    s32 ch;
    s32 n2;
    s32 v2;
    s32 n3;
    s32 j2;
    s32 i2;
    SVECTOR *pt2;
    s16 *q2;
    s32 c1;
    s32 c2;
    s32 c3;
    s32 c4;
    s32 c5;
    s32 c5b;
    s32 c6k;
    s32 c7;
    s32 c8;
    s32 kk;
    s32 c9;
    s32 c10;
    s32 c11;
    s32 c12;
    s32 c13;
    s32 j;
    s32 n;
    s32 i;
    s32 t;
    s32 v;
    s32 h;
    s32 f2;
    s32 m;
    s32 c6;
    s32 d7;
    s32 spE8;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC5F0 = env;
        ex->btlfx_initEffModels(D_801ECB90, 5);
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        work->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x26), 0);
        work->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0);
        work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 0);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 1);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        FX0BAP_W(work->unk18, 0) = FX0BAP_W(work->unk8, 0);
        FX0BAP_W(work->unk18, 1) = FX0BAP_W(work->unk8, 1);
        work->unk18.vy -= 0x1EA;
        env->unk2C->unk0[3].vx = work->unk18.vx;
        env->unk2C->unk0[3].vy = work->unk18.vy;
        env->unk2C->unk0[3].vz = work->unk18.vz;
    } else {
        alpha = 0;
        base = 0;
        lt = 0;
        type = 0;
        e = 0;
        frame = entry->unk0;
        d7 = c6 = m = 0;
        f = 0;
        arr60[0] = arr60[1] = arr60[2] = 0;
        g = 0;
        switch (work->unk0) {
        case 0: {
            key = (Fx0baPKey12 *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A) + (frame * 12 + 4));
            work->unk48[0] = key->x + work->unk8.vx;
            work->unk48[1] = key->y + work->unk8.vy;
            work->unk48[2] = key->z + work->unk8.vz;
            work->unk60.vx = key->rx;
            work->unk60.vy = key->ry;
            work->unk60.vz = key->rz;
            work->unk54[0] = 0x2000;
            work->unk54[1] = 0x2000;
            work->unk54[2] = 0x2000;
            if (frame >= 7) {
                SVECTOR rot;

                ex->func_800CFC2C(frame, 7, 8, 0, 0x1000, &spE8);
                env->unk28 = -0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame * 0x18;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x21), &work->unk8, &rot,
                                  ex->math_sin(spE8 >> 2), 0, 1, ex->math_cos(spE8 >> 2));
                env->unk28 = 0;
                c6 = ex->math_cos(spE8 >> 2) >> 1;
                d7 = ex->math_sin(spE8 >> 2);
                e = ex->math_sin(spE8 >> 2) >> 2;
            } else {
                c6 = (ex->math_cos((frame << 10) / 7) >> 1) + 0x800;
            }
            if (frame >= 15) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } break;
        case 1:
            alpha = 0x1000;
            t = (frame << 12) / 9;
            base = ((0x1000 - ex->math_cos(t >> 2)) << 5) >> 12;
            type = (t >> 3) + 0xE00;
            if (frame < 4) {
                c1 = (ex->math_cos(frame << 8) * 0x46) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, c1, c1, c1);
            }
            if (frame >= 5) {
                c2 = ex->math_sin((frame - 5) << 8) << 7;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c2 >> 14, c2 >> 12);
            }
            if (frame == 3) {
                ex->func_800E60A4(1, 6);
            }
            if (frame >= 9) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
                ex->func_800E60A4(-1, 2);
                work->unk6C = NULL;
            }
            break;
        case 2:
            t = (frame << 12) / 51;
            if (frame == 0) {
                ex->func_800E60A4(-1, 1);
            }
            if (frame == 2) {
                ex->func_800E60A4(0, 3);
            }
            if (frame < 4) {
                c3 = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c3 >> 2, c3, c3);
            }
            if (frame >= 16 && frame < 22) {
                if (frame == 16) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0);
                }
                c4 = 0xFF - (frame - 16) * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c4 >> 2, c4);
            }
            f = 0x1000;
            if (frame < 16) {
                g = (ex->math_cos(frame << 6) >> 3) + 0x12C;
            } else {
                g = 0x12C;
            }
            if (frame < 20) {
                m = ex->math_cos((frame << 10) / 20);
            } else {
                m = ((frame - 20) << 11) / 31;
                if (frame & 1) {
                    m += 0x800;
                } else {
                    m += 0x400;
                }
                if (ex->btlfx_rand() & 1) {
                    m += 0x400;
                }
            }
            if (frame < 24) {
                n2 = (frame << 12) / 24;
                if (frame < 4) {
                    v = ex->math_sin((frame << 10) >> 2) * 5 / 2;
                } else {
                    v = (frame << 7) + 0x2600;
                }
                scale2.vx = scale2.vy = scale2.vz = v;
                rot2.vz = 0;
                rot2.vx = 0;
                rot2.vy = frame * 16;
                pos2.vx = work->unk8.vx;
                pos2.vy = work->unk8.vy;
                pos2.vz = work->unk8.vz;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->unk70);
                c5 = ((0x1000 - n2) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk70, c5, c5, c5);
                if (frame < 16) {
                    ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->unk74);
                    c5b = ((0xC00 - (frame << 8)) >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->unk74, c5b, c5b, c5b);
                }
            }
            if (frame > 0) {
                SVECTOR rot;

                keys = (Fx0baPKeys *)ex->btlfx_getFileData(entry->unk8, 7);
                c6k = frame + 9;
                if (c6k >= 36) {
                    h = *(s16 *)((u8 *)keys + (36 * 6 + 4)) * 2 / 3 + ((frame - 27) << 6);
                } else {
                    keys = (Fx0baPKeys *)((u8 *)keys + (c6k * 6 + 4));
                    h = keys->unk0 * 2 / 3;
                }
                FX0BAP_W(work->unk10, 0) = FX0BAP_W(work->unk8, 0);
                FX0BAP_W(work->unk10, 1) = FX0BAP_W(work->unk8, 1);
                if (frame < 16) {
                    work->unk10.vy -= (ex->math_sin((frame << 10) >> 4) * 0x56E) >> 12;
                } else {
                    work->unk10.vy -= 0x56E;
                }
                FX0BAP_W(D_801EC554, 0) = FX0BAP_W(work->unk10, 0);
                FX0BAP_W(D_801EC554, 1) = FX0BAP_W(work->unk10, 1);
                if (frame < 24) {
                    D_801EC55C = 0x1000;
                } else if (frame < 32) {
                    D_801EC55C = (h * 0x44C) >> 12;
                } else {
                    D_801EC55C = 0xB86;
                }
                env->unk28 = 0x140;
                if (frame >= 15) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), &work->unk10, NULL, h * 5 / 2,
                                            frame & 1, 1, ex->math_cos(t >> 2), 0, 0);
                }
                rot.vx = rot.vy = 0x200;
                rot.vz = frame * 0x2C;
                rot.pad = 0;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), &work->unk10, &rot,
                                  ex->btlfx_randCentered(0x1000) + 0x4000, 0, 1, 0x1000);
                env->unk28 = 0;
            }
            if (frame >= 20) {
                c7 = ((frame - 20) << 12) / 31;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk10, NULL, (c7 >> 2) + 0x2800, 0, 1,
                                  ex->math_sin(c7 >> 1));
            }
            if (frame == 20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk10);
                work->unk6C = p;
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            if (frame == 14) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk8);
                work->unk68 = p;
                if (p != NULL) {
                    p->unk22 = -0x20;
                }
            }
            if (frame < 8) {
                c8 = frame << 9;
                alpha = 0x1000 - c8;
                base = 0x24;
                c8 >>= 2;
                lt = ex->math_sin(c8);
                type = 0x1000 - c8;
            }
            {
                for (kk = 0; kk < 3; kk++) {
                    f2 = frame - kk * 3;
                    if (frame >= kk * 3 && f2 < 22) {
                        if (f2 < 6) {
                            arr60[kk] = 0x1000;
                            arr70[kk] = f2 * 2;
                            arr80[kk] = (f2 << 12) / 22;
                            arr90[kk] = 0x1000;
                        } else {
                            v2 = 0x1000 - ((f2 - 6) << 8);
                            arr60[kk] = v2;
                            arr70[kk] = 11;
                            arr80[kk] = (f2 << 12) / 22;
                            arr90[kk] = v2;
                        }
                    }
                }
                ch = 2;
                cw = 4;
            }
            if (frame == 22) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x20);
            }
            if (frame >= 23 && frame < 26) {
                c9 = 0x80 - ((frame - 23) << 6);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c9, c9, c9 >> 3);
            }
            if (frame == 24) {
                ex->func_800E60A4(-1, 0);
            }
            if (frame == 25) {
                ex->func_800E60A4(0, 5);
            }
            if (frame == 29) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x64, 0xFF);
            }
            if (frame == 41) {
                ex->func_800E60A4(1, 8);
            }
            if (frame == 33) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x40, 0x40, 0);
            }
            if (frame == 34) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0x40, 0x40);
            }
            if (frame >= 41) {
                c10 = (frame - 41) * 0xFF / 10;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c10 >> 2, c10 >> 1, c10);
            }
            if (frame >= 43) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                sc.vy = sc.vx = 0x1020;
                rot.vz = rot.vy = rot.vx = 0;
                sc.vz = 0x1000;
                col.r = col.g = col.b = ((frame - 39) << 5) / 12;
                ex->func_800E6764(0x200, 0xA0, 0x78, 0x50, 0x3C, cw, cw, ch, ch, &rot, &sc, &col, 1, 0);
            }
            if (work->unk6C != NULL) {
                work->unk6C->unk1C = (t >> 1) + 0x1000;
                if (work->unk6C != NULL) {
                    work->unk6C->unk20 = (0x1000 - (t >> 2)) * 3 / 4;
                }
            }
            if (frame >= 51) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->func_800E60A4(-1, 6);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2C), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            break;
        case 3: {
            t = (frame << 12) / 20;
            if (frame < 12) {
                f5 = 0xFF - (frame << 5) / 12;
                func_801E7704(env->unk20 + 2, 1, D_801EC560[frame], 0xC8 - frame * 0xC8 / 12, 0xC8 - frame * 0xC8 / 12,
                              f5);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0xFF, 0xFF);
            }
            if (work->unk68 != NULL && frame < 10) {
                q10 = (frame << 12) / 10;
                if (work->unk68->unk30 == -1) {
                    work->unk68 = NULL;
                }
                if (work->unk68 != NULL) {
                    c11 = 0x1000 - q10;
                    work->unk68->unk20 = c11;
                    if (work->unk68 != NULL) {
                        work->unk68->unk1C = c11;
                    }
                }
                if (frame == 9) {
                    ex->func_800E3D54(work->unk68);
                }
            }
            if (work->unk6C != NULL && frame < 8) {
                q9 = frame << 9;
                if (work->unk6C->unk30 == -1) {
                    work->unk6C = NULL;
                }
                if (work->unk6C != NULL) {
                    work->unk6C->unk1C = 0x1000 - q9;
                }
                if (frame == 7) {
                    ex->func_800E3D54(work->unk6C);
                }
            }
            if (frame < 14) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                col.r = col.g = col.b = 0x89;
                sc.vz = sc.vy = sc.vx = 0x1000;
                rot.vz = rot.vy = rot.vx = 0;
                ex->func_800E6764(2, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
            }
            key = (Fx0baPKey12 *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19) + (frame * 12 + 4));
            work->unk48[0] = key->x + work->unk8.vx;
            work->unk48[1] = key->y + work->unk8.vy - 0x2EE;
            work->unk48[2] = key->z + work->unk8.vz;
            work->unk54[0] = key->rx * 2;
            work->unk54[1] = key->ry * 2;
            work->unk54[2] = key->rz * 2;
            work->unk60.vx = work->unk60.vy = work->unk60.vz = 0;
            if (frame >= 20) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        } break;
        }
        {
            VECTOR pos;
            VECTOR sc;
            SVECTOR a;
            SVECTOR b;

            pos.vx = work->unk48[0];
            pos.vy = work->unk48[1];
            pos.vz = work->unk48[2];
            pos.vy += ex->math_cos(work->unk4 * 0x70) >> 6;
            pos.vy += ex->math_sin(work->unk4 << 7) >> 6;
            ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)work->unk54, work->unk78);
            if (c6 != 0) {
                sc.vx = work->unk54[0];
                sc.vy = work->unk54[1];
                sc.vz = work->unk54[2];
                sc.vx += d7;
                sc.vy += e;
                sc.vz += d7;
                ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)&sc, work->unk7C);
                c12 = (c6 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk7C, c12, c12, c12);
                ex->func_800E6624(work->unk4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
            }
            if (m != 0) {
                ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)work->unk54, work->unk80);
                c13 = (((m * 2) - 0x1000) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk80, c13, c13, c13);
            }
            if (f != 0) {
                n3 = work->unk4 * 0x29E;
                a.vx = pos.vx;
                a.vy = pos.vy;
                a.vz = pos.vz;
                FX0BAP_W(b, 0) = FX0BAP_W(a, 0);
                FX0BAP_W(b, 1) = FX0BAP_W(a, 1);
                work->unk60.vx -= 0x400;
                ex->func_800CFD78((s16 *)&a, (s16 *)&work->unk60, 0x5DC, (s16 *)&a);
                work->unk60.vx += 0x400;
                func_801E8778(entry, (Fx0baStrip *)&D_801ECAC0, &a, &b, g, n3, 0xF0, work->unk4 & 7, f, 0x40);
            }
        }
        env->unk28 = 0x1EA;
        if (alpha != 0) {
            for (k = 0; k < 3; k++) {
                pad = 0x168;
                s1p = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EC578[k]) + 2;
                s2p = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EC584[k]) + 2;
                if (k != 0) {
                    pad = 0x80;
                    if (k == 1) {
                        pad = 0x58;
                    }
                }
                ta = &D_801ECF80[k * 2];
                pt = ta->unk4;
                ta->unk0 = ta->unk2 = 0x10;
                ta->unk1 = 0;
                tb = &D_801ECF80[k * 2 + 1];
                tb->unk0 = tb->unk2 = 0x10;
                tb->unk1 = 0;
                pb = tb->unk4;
                for (j = n = 0; j < 34; j++, n++) {
                    if (j == 17) {
                        pt = pb;
                        n--;
                    }
                    i = base - n;
                    if (i < 0) {
                        i = 0;
                    }
                    i *= 3;
                    pt->vx = work->unk8.vx + ex->btlfx_lerp(s1p[i], s2p[i], lt);
                    pt->vy = work->unk8.vy + ex->btlfx_lerp(s1p[i + 1], s2p[i + 1], lt);
                    pt->vz = work->unk8.vz + ex->btlfx_lerp(s1p[i + 2], s2p[i + 2], lt);
                    pt->pad = pad;
                    pt++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), 0, NULL, (work->unk4 + k) & 7, type, alpha,
                                    1, (u8 *)ta, 2);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0, NULL, (work->unk4 + k) & 7, type,
                                    alpha, 1, (u8 *)tb, 2);
            }
        }
        for (k2 = 0; k2 < 3; k2++) {
            if (arr60[k2] != 0) {
                q1 = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EC590[k2]) + 8;
                q2 = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EC59C[k2]);
                q2 += 8;
                tc = &D_801ED2D0[k2];
                tc->unk0 = tc->unk2 = 0xC;
                tc->unk1 = 0;
                pt2 = tc->unk4;
                for (j2 = 0; j2 < 13; j2++) {
                    i2 = arr70[k2] - j2;
                    if (i2 < 0) {
                        i2 = 0;
                    }
                    i2 *= 3;
                    pt2->vx = work->unk8.vx + ex->btlfx_lerp(q1[i2], q2[i2], arr80[k2]) * 2;
                    pt2->vy = work->unk8.vy + ex->btlfx_lerp(q1[i2 + 1], q2[i2 + 1], arr80[k2]) * 3 / 2;
                    pt2->vz = work->unk8.vz + ex->btlfx_lerp(q1[i2 + 2], q2[i2 + 2], arr80[k2]) * 2;
                    pt2->pad = 0xC8;
                    pt2++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0, (s32 *)&work->unk8,
                                    (work->unk4 + k2) & 7, arr90[k2], arr60[k2], 1, (u8 *)tc, 1);
            }
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

s32 fx_0ba_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 *keys;
    SVECTOR v;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EC5F0 = env;
    } else {
        frame = entry->unk0;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2E);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2F);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave) {
    Unk800F78A4 *env = D_801EC5F0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 n;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    n = (size - 4) / 8;
    v = buf->unk4;
    buf->unk0 = buf->unk2 = n - 1;
    buf->unk1 = 0;
    for (i = 0; i < n; i++, v++) {
        t = 0x1000 - (i << 12) / (n - 1);
        t += *tab++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        if (wave != 0) {
            v->vy -= (ex->math_sin(t >> 1) * wave) >> 12;
        }
        v->pad = pad;
        s += step + *tab++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, a12, 0x1000, a13, 1, (u8 *)buf, a2);
    env->unk28 = 0;
}

void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad) {
    SVECTOR *v;
    BtlfxExports *ex;
    SVECTOR to;

    v = path->unk4;
    v += i;
    ex = BTLFX_EXPORTS(D_801EC5F0);
    ex->btlfx_lerpVec((s16 *)(v + 1), (s16 *)v, 0x4000, (s16 *)&to);
    func_801EA850(entry, file, a2, buf, size, tab, v, &to, 0, 0x20, ex->btlfx_rand(), 0x33, a12, a13, pad, 0);
}

s32 func_801EAC08(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR ang;
    Fx0baTrail *trail;
    Fx0baStrip4 *strip;
    s32 t;
    s32 c;
    s32 k;
    s32 a;
    s32 q;
    s32 amp;
    s32 t2;
    Fx0baTrail *trail2;
    Fx0baStrip4 *strip2;
    s32 c2;
    s32 r;
    s32 m;
    s32 w; /* block-local, so 0x1000 - t2 and w >> 11 are tied to it */

    if (mode != 1) {
        switch (p->unk11) {
        case 0:
        case 1:
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            trail = &D_801ED420[p->unk10];
            strip = &D_801EE5A0[p->unk10];
            if (p->unk11 == 0) {
                p->unk8.vx = D_801EC554.vx;
                p->unk8.vy = D_801EC554.vy;
                p->unk8.vz = D_801EC554.vz;
                func_801EA850(entry, 0x1C, 0, (Fx0baStrip *)trail, 0x8C, p->unk18, &p->unk0, &p->unk8, 0, 0x180,
                              frame * 0x97, 0x140, frame, c, ((0x1000 - t) * p->unk15) >> 12,
                              ((((frame << 10) + t) << 7) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    k = 2;
                } else if (frame < 4) {
                    k = 3;
                } else {
                    k = 0;
                }
                a = ((0x1000 - t) * p->unk15) >> 12;
                if (frame == 0) {
                    a = (a * 3) >> 1;
                }
                func_801EA850(entry, 2, 0, (Fx0baStrip *)trail, 0x8C, p->unk18 + frame * k, &p->unk0, &p->unk8, 0,
                              0x180, frame * 0xC0, 0x168, frame, c, a, 0);
                func_801EAAF0(entry, 3, 0, (Fx0baStrip *)trail, (Fx0baStrip *)strip, 0x24, p->unk18, p->unk12, frame, c,
                              a >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
            break;
        case 2:
            t2 = frame << 9;
            c2 = ex->math_cos(t2 >> 2);
            trail2 = &D_801ED420[p->unk10];
            strip2 = &D_801EE5A0[p->unk10];
            if (frame < 3 && D_801EC550 != 0) {
                q = ((frame + 1) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->unk28 = 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x29), &p->unk0, NULL, (q >> 2) + 0x2000, 0, 1,
                                  0x1000 - q * 2 / 3);
                env->unk28 = 0;
            }
            if ((u32)(frame - 3) < 3 && D_801EC550 != 0) {
                q = ((frame - 2) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->unk28 = 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x29), &p->unk8, NULL, (q >> 2) + 0x2000, 0, 1,
                                  0x1000 - q * 2 / 3);
                env->unk28 = 0;
            }
            if (frame < 2) {
                ex->btlfx_lerpVec((s16 *)&p->unk0, (s16 *)&p->unk8, (frame + 1) << 11, (s16 *)&v);
                amp = p->unk15 >> 1;
            } else {
                BTLFX_COPY_SVEC(&v, &p->unk8);
                amp = p->unk15;
            }
            r = (frame < 3) * 2;
            m = frame * r;
            /* No code (only m * 4 is used), but m is then set twice, so sched1 does not delay the multiply to its
             * use and the product stays in a caller-saved register. */
            m &= 0x3FFFFFFF;
            func_801EA850(entry, 2, 0, (Fx0baStrip *)trail2, 0x8C, p->unk18 + m, &p->unk0, &v, 0,
                          0x227, frame * 0xC0, 0x168, frame, c2, (w = (0x1000 - t2) * amp) / 1500, 0);
            func_801EAAF0(entry, 3, 0, (Fx0baStrip *)trail2, (Fx0baStrip *)strip2, 0x24, p->unk18, p->unk12, frame, c2,
                          w >> 11);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 func_801EB1CC(SVECTOR *v) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR *p = D_801ED424[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;
    v->vx = p->vx;
    v->vy = p->vy;
    v->vz = p->vz;
    v->pad = 1;
    return 0;
}

s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    s32 frame;
    Fx0baBolt *p;
    Fx0baPt *pt;
    s32 *tab;
    s32 i;
    s32 r;
    s32 ang;
    s32 amp;
    s32 dist;
    Fx0baBolt *q;
    s32 len;
    s32 j;
    s32 k;
    s32 n;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0baWork83);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0baBolt);
        ((EffInit *)arg4)->unk14 = (s32)func_801EAC08;
        ((EffInit *)arg4)->unk4 = 0x20;
    } else if (mode == 1) {
        n = *arg4;
        D_801EC5F0 = env;
        work->unk0 = D_801EEA20;
        tab = work->unk0;
        for (i = 0; i < 64; i++) {
            *tab++ = ex->btlfx_randCentered(0xD2);
            *tab++ = ex->btlfx_randCentered(0x1C3);
            *tab++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk8) {
        case 0:
            BTLFX_COPY_SVEC(&base, &D_801EC554);
            if (frame < 0x14) {
                work->unk4++;
                BTLFX_COPY_SVEC(&a, &base);
                a.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (p != NULL) {
                    r = ex->btlfx_randCentered(0x200) + 0x7D0;
                    ang = ((work->unk4 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x200);
                    a.vx += (ex->math_cos(ang) * r) >> 12;
                    a.vz += (ex->math_sin(ang) * r) >> 12;
                    BTLFX_COPY_SVEC(&p->unk0, &a);
                    BTLFX_COPY_SVEC(&p->unk8, &b);
                    p->unk11 = 0;
                    p->unk10 = work->unk4 % 32;
                    p->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    p->unk12 = ex->btlfx_rand() % 15;
                    p->unk13 = ex->btlfx_rand() % 15;
                    p->unk15 = 0x3E;
                    p->unk14 = 0;
                }
                work->unk4++;
                BTLFX_COPY_SVEC(&a, &base);
                a.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                if (frame >= 0xB) {
                    p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (p != NULL) {
                        r = ex->btlfx_randCentered(0x100) + 0x3E8;
                        ang = ((work->unk4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                        a.vx += (ex->math_cos(ang) * r) >> 12;
                        a.vz += (ex->math_sin(ang) * r) >> 12;
                        BTLFX_COPY_SVEC(&p->unk0, &a);
                        BTLFX_COPY_SVEC(&p->unk8, &b);
                        p->unk11 = 0;
                        p->unk10 = work->unk4 % 32;
                        p->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                        p->unk12 = ex->btlfx_rand() % 15;
                        p->unk13 = ex->btlfx_rand() % 15;
                        p->unk15 = 0x32;
                        p->unk14 = 0;
                    }
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            BTLFX_COPY_SVEC(&a, &D_801EC554);
            if (frame < 0x1D) {
                amp = (D_801EC55C * 100) / 2048 + 0x3C;
                if (amp >= 0x100) {
                    amp = 0xFF;
                }
                for (j = 0; j < 2; j++) {
                    work->unk4++;
                    BTLFX_COPY_SVEC(&c, &a);
                    BTLFX_COPY_SVEC(&d, &a);
                    q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (q != NULL) {
                        len = D_801EC55C;
                        ex->btlfx_randRotation(&b);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&b, 0x180, (s16 *)&c);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&b, len + 0x180, (s16 *)&d);
                        BTLFX_COPY_SVEC(&q->unk0, &c);
                        BTLFX_COPY_SVEC(&q->unk8, &d);
                        q->unk11 = 1;
                        q->unk10 = work->unk4 % 32;
                        q->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                        q->unk12 = ex->btlfx_rand() % 15;
                        q->unk13 = ex->btlfx_rand() % 15;
                        q->unk15 = amp;
                        q->unk14 = 0;
                    }
                }
                if (frame >= 0xF) {
                    for (j = 0; j < 2; j++) {
                        work->unk4++;
                        BTLFX_COPY_SVEC(&c, &a);
                        BTLFX_COPY_SVEC(&d, &a);
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (q != NULL) {
                            len = D_801EC55C;
                            ex->btlfx_randRotation(&b);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&b, 0x180, (s16 *)&c);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&b, len + 0x180, (s16 *)&d);
                            BTLFX_COPY_SVEC(&q->unk0, &c);
                            BTLFX_COPY_SVEC(&q->unk8, &d);
                            q->unk11 = 1;
                            q->unk10 = work->unk4 % 32;
                            q->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                            q->unk12 = ex->btlfx_rand() % 15;
                            q->unk13 = ex->btlfx_rand() % 15;
                            q->unk15 = amp - 0x32;
                            q->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x1D) {
                return 1;
            }
            break;
        case 2:
            if ((D_801EC550 != 0 || !(frame & 1)) && D_801EC54C == 0) {
                do {
                    k = ex->btlfx_rand() % 10;
                    pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23) + 4) + k;
                    b.vx = -pt->vx + D_801ECAB0.vx;
                    b.vy = pt->vy + D_801ECAB0.vy;
                    b.vz = -pt->vz + D_801ECAB0.vz;
                    if ((D_801ECAB8.vx != 0 || D_801ECAB8.vy != 0 || D_801ECAB8.vz != 0) && (frame & 1)) {
                        BTLFX_COPY_SVEC(&c, &D_801ECAB8);
                        b.vz >>= 1;
                        b.vx >>= 1;
                        b.vy += 0x400;
                    } else {
                        k = ex->btlfx_rand() % 10;
                        pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24) + 4) + k;
                        c.vx = -pt->vx + D_801ECAB0.vx;
                        c.vy = pt->vy + D_801ECAB0.vy;
                        c.vz = -pt->vz + D_801ECAB0.vz;
                    }
                    dist = (b.vx - c.vx) * (b.vx - c.vx) + (b.vy - c.vy) * (b.vy - c.vy) +
                           (b.vz - c.vz) * (b.vz - c.vz);
                } while ((u32)(dist - 0x1000000) > 0x4000FFF);
                work->unk4++;
                p = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (p != NULL) {
                    BTLFX_COPY_SVEC(&p->unk0, &b);
                    BTLFX_COPY_SVEC(&p->unk8, &c);
                    p->unk11 = 2;
                    p->unk10 = work->unk4 % 32;
                    p->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    p->unk12 = ex->btlfx_rand() % 15;
                    p->unk13 = ex->btlfx_rand() % 15;
                    p->unk15 = ex->btlfx_rand() % 70 + 0x37;
                    p->unk14 = 0;
                }
            }
            if (D_801EC54C != 0) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    s32 c;
    u8 *q;
    s32 (*fn)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *);

    if (mode == 1) {
        p->unk12 = ex->btlfx_randCentered(0x20) + 0x1080;
    } else {
        c = 0x1000 - (frame << 12) / 5;
        q = p->unk18;
        if (frame != 0) {
            c >>= 1;
        }
        ((s16 *)q)[3] = (0x16 - frame * 4) >> 1;
        fn = ex->func_800E0014;
        bias[0] = 0;
        bias[1] = 0;
        fn((Pool6CEntry *)p, 0, (SVECTOR *)bias, (SVECTOR *)q);
        if (p->unk16 == 0) {
            p->unk18[-0x10] = p->unk18[-0xE] = 0x10;
            p->unk18[-0xF] = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x32), 0, NULL, frame, 0x1000, c, 1,
                              p->unk18 - 4, 0);
        }
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

void func_801EC124(void) {
    D_801EEFDC = 0;
}

void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to) {
    Fx0baTrail *trail = &D_801EED20[D_801EEFDC];
    SVECTOR *p = &trail->unk4[16];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR v;
    Fx0baPart *part;
    s32 i;
    s32 t;

    trail->unk0 = trail->unk2 = 16;
    trail->unk1 = 0;
    for (i = 0; i < 17; i++, p--) {
        t = 0x1000 - (i << 8);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += 0x20 + ex->btlfx_randCentered(0x10);
        v.vy += 0x46 + ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)center, (s16 *)&v, r + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 6),
                          (s16 *)p);
        part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (part != NULL) {
            part->unk14 = D_801EEFDC;
            part->unk16 = 0x10 - i;
            part->unk18 = (u8 *)p;
            part->unk0 = p->vx << 12;
            part->unk4 = p->vy << 12;
            part->unk8 = p->vz << 12;
            part->unkC = 0;
            part->unkE = 0;
            part->unk10 = 0;
            part->unkC = ((p->vx - center->vx) >> 5) + ex->btlfx_randCentered(0xE);
            part->unkE = ((p->vy - center->vy) >> 5) + ex->btlfx_randCentered(0x18);
            part->unk10 = ((p->vz - center->vz) >> 5) + ex->btlfx_randCentered(0xE);
        }
    }
    D_801EEFDC = (D_801EEFDC + 1) % 5;
}

s32 fx_0ba_op85(s32 mode, Fx0baWork85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    SVECTOR c;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0baWork85);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0baPart);
        ((EffInit *)arg4)->unk14 = (s32)func_801EBFB8;
        ((EffInit *)arg4)->unk4 = 0x55;
    } else if (mode == 1) {
        D_801EC5F0 = env;
        func_801EC124();
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801EC54C == 0) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0xFF);
            work->unk8.vy += 0x800 + (ex->btlfx_rand() & 0x1FF);
            BTLFX_COPY_SVEC(&c, &D_801ECAB0);
            c.vy -= 0x140;
            func_801EC130(entry, &c, 0x100, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801EC54C != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_headerPtr);
