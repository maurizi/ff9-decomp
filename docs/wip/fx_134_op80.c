/* Pass 4: 743 aligned mismatches, frame size matches. Variables split per case (build/s/split17.py) and at each
 * top-level re-assignment (build/s/phase.py) and at nested assignments
 * whose value stays in their block (build/s/webs.py); case 3's first angle is t >> 2 of t = (frame << 12) / 24 in a
 * block-local variable (a3x): with it the case-3 block has eight call-crossing locals, the 0x1000 constant lands
 * in s7 and is kicked out by work (which then gets s7), so ex gets fp as in the original (local-alloc doubles
 * REG_LIVE_LENGTH of work for its REG_EQUIV, so ex is allocated first); case 7 writes t >> 2 inline after the if;
 * case 0's k as an if/else-if chain. Earlier header follows. */
/* Draft of fx_134 fx_134_op80 (first draft from m2c; same family as fx_0e0 op80, whose draft gave the work layout).
 * Status: 2359 aligned mismatches, 747 ignoring registers, 523 ignoring registers and stack offsets (of ~3600
 * instructions; was 2398/796); frame 0x150 vs 0x148 (one spill slot too many: case 6 spills &work->unk24, which
 * the original keeps in s6; the original has ex in fp).
 * Stack layout notes from the original: locals 0x38..0x107, spills 0x108 (frame), 0x10C (t), 0x110 (c, tail
 * alpha), 0x114 (s, tail scale), 0x118/0x11C (x/y offsets). Cases 0-2 and the tail reuse 0x38 (wrapped blocks),
 * case 3 uses 0x38..0x8F and case 4 0x90..0xC7 without being freed in between, cases 5, 6 and 10 reuse 0xC8.
 * The camera copies work->unk18 = work->unk8 are word copies (BTLFX_COPY_SVEC), and so is case 0's
 * work->unk8 = D_801EC860. The m2c temporaries a/b hurt: the original computes (t >> 7) + 0xA0, (t >> 6) + ...,
 * (t >> 8) + ... and 0x1000 - t inside the expressions (after the getFileData calls; an assigned multi-use
 * variable lets sched1 hoist it across calls). Keeping a = D_801EC880[i] + 0x300 in case 1 is better than
 * inlining it. */
#include "fxmod/fx_134.h"

extern SVECTOR D_801EC860; /* caster position */
extern CVECTOR D_801EC878[];
extern s16 D_801EC880[4];
extern CVECTOR D_801EC888[];
extern CVECTOR D_801EC894[2];
extern Fx134Bolt D_801EC8F4[];
extern BtlModel D_801ECB64[];
extern u8 D_801ED1A4[];

s32 fx_134_op80(s32 mode, Fx134Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    s32 a0b;
    s32 a_c4_w1;
    s32 k_c4_w2;
    s32 k_c4_w3;
    s32 a1b;
    s32 a_c3_p2;
    s32 k_c3_p2;
    s32 i_c3_p2;
    s32 a_c4_p2;
    s32 a_c5_p2;
    s32 a_c5_p3;
    s32 k_c5_p2;
    s32 i_c5_p2;
    s32 k_c9_p2;
    Fx134Orb *o_c1;
    Fx134Orb *o_c4;
    Fx134Orb *o_c8;
    s32 a3x;
    s32 d1;
    s32 a_c0;
    s32 b_c0;
    s32 k_c0;
    s32 a_c1;
    s32 k_c1;
    s32 i_c1;
    s32 n_c1;
    s32 a_c2;
    s32 b_c2;
    s32 i_c2;
    s32 a_c3;
    s32 k_c3;
    s32 i_c3;
    s32 n_c3;
    s32 a_c4;
    s32 b_c4;
    s32 k_c4;
    s32 i_c4;
    s32 n_c4;
    s32 a_c5;
    s32 k_c5;
    s32 i_c5;
    s32 n_c5;
    s32 b_c6;
    s32 k_c6;
    s32 i_c6;
    s32 n_c6;
    s32 a1_c6;
    s32 a_c7;
    s32 k_c7;
    s32 k_c8;
    s32 i_c8;
    s32 k_c9;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx134Orb *o;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 c;
    s32 s;
    s32 xo;
    s32 yo;
    s32 i;
    s32 a;
    s32 b;
    s32 k;
    s32 n;
    s32 a1;

    if (mode == 0) {
        init->unk0 = 0x84;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E8658;
        init->unk4 = 0x31;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC8E4 = env;
        ex->btlfx_initEffModels(D_801ECB64, 8);
        work->unk5C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x98, 0x3D80, 0, 0);
        work->unk60 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x118, 0xFFFF, 0x40,
                                                 0);
        work->unk64 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), 0x118, 0xFFFF,
                                                 0x40, 0);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x37, 0x3D07, 0,
                                                 0);
        work->unk6C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0x37, 0x3D07, 0,
                                                 0);
        work->unk74 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0xBC, 0x3E00, 0x80,
                                                 0);
        work->unk8.vx = D_801EC860.vx;
        work->unk8.vy = D_801EC860.vy - 0x3000;
        work->unk8.vz = D_801EC860.vz;
        work->unk7C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801ED1A4;
            work->unk7C->unk12 = 0x40;
            work->unk7C->unkC = 0xCC;
        }
    } else {
        c = 0;
        s = 0;
        xo = 0;
        yo = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            switch (frame) {
            default: {
                SVECTOR v;

                t = (frame << 12) / 40;
                if (frame < 0x10) {
                    BTLFX_COPY_SVEC(&work->unk18, &D_801EC860);
                    a_c0 = frame << 6;
                    work->unk18.vy -= ex->math_sin(a_c0) * 3;
                    env->unk2C->unk0[3].vx = work->unk18.vx;
                    env->unk2C->unk0[3].vy = work->unk18.vy;
                    env->unk2C->unk0[3].vz = work->unk18.vz;
                    b_c0 = (ex->math_sin(a_c0) >> 3) - 0x200;
                } else {
                    b_c0 = 0;
                }
                v.vx = ex->btlfx_randCentered(0x44C) + 0x400;
                v.vy = 0;
                v.vz = 0;
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
                if (frame < 8) {
                    k_c0 = 0;
                } else if (frame < 0xC) {
                    k_c0 = (frame - 8) << 10;
                } else {
                    k_c0 = 0x1000;
                }
                if (k_c0 != 0) {
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), a0b = (t >> 7) + 0xA0, b_c0, 0, 1, k_c0);
                    k_c0 += ex->btlfx_randCentered(0x18);
                    if (k_c0 < 0) {
                        k_c0 = 0;
                    }
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), a0b, b_c0, 0, 1, k_c0);
                    ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), a0b, b_c0 + 0x60, 0, 1, k_c0);
                }
                if (frame == 0x18) {
                    work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
                }
                if (frame >= 8) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk8, NULL, 0x1800, frame, 2, -1);
                }
                if (frame >= 0x28) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                    ex->func_800E3D54(work->unk7C);
                    ex->func_800E3D54(work->unk80);
                    BTLFX_COPY_SVEC(&work->unk8, &D_801EC860);
                    work->unk30.vx = 0x1800;
                    work->unk30.vy = 0x1800;
                    work->unk30.vz = 0x1800;
                    work->unk3C.vx = 0x3C0;
                    work->unk8.vy -= 0x3000;
                    work->unk3C.vy = 0x100;
                    work->unk3C.vz = 0x180;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vz = work->unk8.vz;
                    ex->func_800E63E4(1, 1);
                }
                break;
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR ang;

                t = frame << 6;
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
                d1 = 0x1000 - t;
                work->unk18.vy += 0x400 + (d1 >> 1);
                s = d1 >> 4;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk60);
                a_c1 = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk60, (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), work->unk4);
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), (t >> 6) + 0xA0, (t >> 9) + 0x78,
                                  0, 1, -1);
                func_801E7748(0xFFD, (t >> 5) + 0x78, (t >> 8) + 0xA9, 0x100, 0x40, 0x80, 0x98, 0x3DC0, 0, 0xC0, 0xFF);
                ex->func_800D0B18((s32)(env->unk20 + 8), (t >> 8) + 0x78, 1, 2, 0x78, 1, D_801EC878, 0, 0, 0, 0);
                c = 0x93;
                yo = 0;
                xo = 0;
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), (frame << 6) / 40 + 0xBE, 0x73, 0,
                                  0xFF, -1);
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), (t >> 6) + 0x20, 0x40, 0, 0xFF, -1);
                n_c1 = (frame << 7) + 0x2000;
                o_c1 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o_c1 != NULL) {
                    i_c1 = frame & 3;
                    k_c1 = frame * 0x140 + (i_c1 << 5) - 0x20;
                    k_c1 = k_c1 % 3072 + 0x600;
                    a1b = D_801EC880[i_c1] + 0x300;
                    o_c1->unkC.vx = work->unk8.vx + ((ex->math_cos(k_c1) * n_c1) >> 12);
                    o_c1->unkC.vz = work->unk8.vz + ((ex->math_sin(k_c1) * n_c1) >> 12);
                    o_c1->unkC.vy = work->unk8.vy + ((((frame + 4) % 6) << 11) - 0x1400);
                    ex->btlfx_aimAngles((s16 *)&o_c1->unkC, (s16 *)&work->unk8, (s16 *)&ang);
                    ex->func_800CFCA8(&ang, a1b, (s32)&o_c1->unk4);
                    o_c1->unk0 = frame & 1;
                    o_c1->unk4.pad = i_c1;
                }
                D_801EF97C = (t >> 8) + 0x116;
                if (frame == 0x3E) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x46, 0x46, 0x46);
                }
                if (frame == 0x3F) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0x80);
                }
                if (frame >= 0x40) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                    D_801EF980 = 1;
                    ex->func_800E3C58();
                    work->unk3C.vx = 0x400;
                    work->unk3C.vy = 0;
                    work->unk3C.vz = 0;
                    ex->func_800E63E4(1, 1);
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                SVECTOR rot;
                Fx134Vec3 pos;
                Fx134Vec3 scale;

                t = (frame << 12) / 6;
                if (frame == 0) {
                    ex->btlfx_poolClear(entry->unk10);
                }
                c = 0x97;
                s = 0x10;
                yo = 0;
                xo = 0;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xC0, 0xC0, 0xC0);
                }
                if (frame == 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x60, 0x60, 0x60);
                }
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vy += 4;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xC), work->unk4);
                ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x120, 0x58, 0, 1, -1);
                ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x78, 0xA0, 0, 1, -1);
                ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 0x78, 0x46, 0, 1, -1);
                a_c2 = (ex->math_sin(t >> 2) >> 3) + 0x1300;
                scale.vz = 0x1000;
                b_c2 = (((t >> 1) + 0x800) >> 5) - 0x80;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy;
                pos.vz = work->unk24.vz;
                pos.vy += 0xC00;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame << 5;
                for (i_c2 = 0; i_c2 < 4; i_c2++) {
                    scale.vx = scale.vy = (a_c2 * ((ex->math_sin((i_c2 << 11) / 3) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, b_c2, b_c2, b_c2);
                    rot.vz += 0x400;
                    pos.vy -= 0x800;
                }
                if (frame >= 6) {
                    work->unk0 = 3;
                    entry->unk0 = -1;
                    work->unk50.vx = 0x400;
                    work->unk44.vx = 0x3000;
                    work->unk44.vy = 0x3000;
                    work->unk50.vy = 0;
                    work->unk50.vz = 0;
                    work->unk44.vz = 0x1800;
                }
                break;
            }
            }
            break;
        case 3: {
            Fx134Vec3 pos;
            Fx134Vec3 scale;
            SVECTOR rot;
            Fx134Vec3 pos2;
            Fx134Vec3 scale2;
            SVECTOR rot2;

            t = (frame << 12) / 24;
            a3x = t >> 2;
            k_c3 = ex->math_cos(a3x);
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            c = 0x97;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            s = (k_c3 >> 4) + 0x20;
            work->unk3C.vy += 4;
            n_c3 = t >> 7;
            work->unk3C.vz += n_c3;
            yo = 0;
            xo = 0;
            work->unk24.vy += (((0x1000 - ex->math_cos(a3x)) << 9) >> 12) + (ex->math_cos(a3x) >> 6) * ((frame & 1) * 2 - 1);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, -0x2B, -0x2B, -0x2B);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = t + 0x2000;
            work->unk44.vz = 0x1800;
            work->unk50.vz += ex->math_sin(a3x) >> 7;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a3x) >> 6) - 0x80, (ex->math_sin(a3x) >> 6) - 0x80,
                                     (ex->math_sin(a3x) >> 6) - 0x80);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a3x) >> 5) - 0x80, (ex->math_sin(a3x) >> 5) - 0x80,
                                     (ex->math_sin(a3x) >> 5) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, n_c3 - 0x80, n_c3 - 0x80, n_c3 - 0x80);
            scale.vx = scale.vy = ex->math_sin(a3x) * 2 + 0x400;
            scale.vz = 0x1000;
            pos.vx = work->unk24.vx;
            a_c3 = t >> 1;
            pos.vy = work->unk24.vy;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 7;
            pos.vz = work->unk24.vz;
            for (i_c3 = 0; i_c3 < 4; i_c3++) {
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a_c3) >> 5) - 0x80, (ex->math_sin(a_c3) >> 5) - 0x80,
                                         (ex->math_sin(a_c3) >> 5) - 0x80);
                pos.vy += 0x40;
                rot.vz += 0x200;
                scale.vx = scale.vy = scale.vx * 2 / 3;
            }
            a_c3_p2 = t >> 2;
            k_c3_p2 = ex->math_sin(a_c3_p2);
            scale2.vz = (t >> 1) + 0x1000;
            pos2.vx = work->unk24.vx;
            pos2.vy = work->unk24.vy + 0xC00;
            rot2.vx = -0x400;
            rot2.vy = 0;
            rot2.vz = 0xC0 - (frame << 6);
            pos2.vz = work->unk24.vz;
            for (i_c3_p2 = 0; i_c3_p2 < 4; i_c3_p2++) {
                scale2.vx = scale2.vy = ((k_c3_p2 * 3 / 2 + 0x1500) * ((ex->math_sin((i_c3_p2 << 10) / 3) >> 1) + 0x800)) >> 12;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_cos(a_c3_p2) >> 5) - 0x80, (ex->math_cos(a_c3_p2) >> 5) - 0x80,
                                         (ex->math_cos(a_c3_p2) >> 5) - 0x80);
                rot2.vz += 0x400;
                pos2.vy -= 0x800;
            }
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x120, 0x58, 0, 1, -1);
            ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x78, 0xA0, 0, 1, -1);
            ex->func_800D1BC4(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 0x78, 0x46, 0, 1, -1);
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy = work->unk8.vy;
                work->unk24.vy -= 0x800;
                work->unk30.vx = 0x36D;
                work->unk30.vy = 0x36D;
                work->unk30.vz = 0x36D;
                work->unk10.vx = work->unk24.vx;
                work->unk10.vz = work->unk24.vz;
                work->unk10.vy = work->unk24.vy;
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk10);
                work->unk80 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk10);
            }
            break;
        }
        case 4: {
            SVECTOR rot;
            Fx134Vec3 scale;
            Fx134Vec3 pos;
            SVECTOR pos3;
            SVECTOR rot3;

            t = (frame << 12) / 40;
            if (frame > 0x10) {
                a_c4 = ((frame - 0x10) << 10) / 24;
                c = (ex->math_sin(a_c4) >> 6) + 0x8D;
                s = a_c4 + 0x10;
            } else {
                c = 0x8D;
                s = 0x10;
            }
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            yo = 0;
            work->unk3C.vz += 0x20;
            work->unk24.vy += 0x60 + ex->btlfx_randCentered(8);
            work->unk10.vz = work->unk24.vz;
            work->unk10.vy = work->unk24.vy;
            work->unk10.vx = work->unk24.vx;
            work->unk10.vy += 0x578;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, 0, 0, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x492;
            work->unk44.vz = 0xAAA;
            work->unk24.vy -= 0x578;
            work->unk50.vz += 0x18;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk44.vx = work->unk44.vy = 0x924;
            work->unk44.vz = 0x1000;
            work->unk50.vz -= 0x200;
            work->unk24.vy -= 0x60E;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            a_c4_w1 = t >> 2;
            ex->Hi_ModifyEffModelRGB(work->unk6C, (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80);
            xo = 0;
            work->unk24.vy += 0xB86;
            if (frame > 0x1C) {
                b_c4 = frame - 0x1C;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy + 0x6A4;
                rot.vx = 0;
                rot.vy = frame << 6;
                rot.vz = 0;
                pos.vz = work->unk24.vz;
                k_c4 = (b_c4 << 12) / 48;
                scale.vx = scale.vz = ((ex->math_sin(k_c4) * 7) >> 3) + 0x578;
                scale.vy = ex->math_sin(k_c4) * 2 / 3 + 0x400;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                k_c4_w2 = (b_c4 << 12) / 24;
                ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(k_c4_w2) >> 5) - 0x80, (ex->math_sin(k_c4_w2) >> 5) - 0x80,
                                         (ex->math_sin(k_c4_w2) >> 5) - 0x80);
            }
            env->unk28 = 0x100;
            k_c4_w3 = ex->math_sin(a_c4_w1);
            pos3.vx = work->unk24.vx;
            pos3.vy = work->unk24.vy;
            rot3.pad = 0;
            rot3.vy = 0;
            rot3.vx = 0;
            rot3.vz = frame << 5;
            pos3.vz = work->unk24.vz;
            for (i_c4 = 0; i_c4 < 0x10; i_c4++) {
                pos3.vy -= 0x19F;
                ex->btlfx_rand();
                func_801E7B38(&pos3, &rot3, 0x600, k_c4_w3, 0x1A, 0x3D02, (i_c4 & 3) << 6, (i_c4 >> 2) << 6, 0x40, 0x40, 1);
            }
            env->unk28 = 0;
            n_c4 = frame % 5;
            if (n_c4 == 2) {
                o_c4 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o_c4 != NULL) {
                    o_c4->unkC.vx = work->unk24.vx;
                    o_c4->unkC.vz = work->unk24.vz;
                    o_c4->unk14.vx = -0x400;
                    o_c4->unk14.vy = 0;
                    o_c4->unk0 = n_c4;
                    o_c4->unk14.vz = frame * 0x154;
                    o_c4->unkC.vy = work->unk24.vy + 0x400;
                    o_c4->unk4.pad = t;
                }
            }
            a_c4_p2 = t >> 6;
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xA0 - a_c4_p2, 0x58, 0, 1, -1);
            ex->func_800D0B18((s32)(env->unk20 + 0x3E8), 0x28, 1, 3, 0x64, 1, D_801EC888, 0, 0, 0, 0);
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0xDC - t / 55, 0x78, 0, 1, -1);
            ex->func_800D1BC4(0xFFE, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0x28 - a_c4_p2, 0x28, 0, 1, -1);
            work->unk58 += 4 + ((t * 10) >> 12);
            func_801E7940(0, work->unk58 % 320, 0xA0 - (t >> 8), 0x80, 0x80, 0x80, 0x9C, 0x3E00, 0x80, 0, 1, 0);
            func_801E7940(0xFFF, (work->unk58 / 320) * 0xA0 - (work->unk58 >> 1), (t >> 9) + 0x8C, 0x80, 0x80, 0x80, 0x9C,
                          0x3E00, 0x80, 0, 0, 1);
            if (frame >= 0x28) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->btlfx_poolClear(entry->unk10);
                ex->func_800E3C58();
                work->unk24.vy = work->unk8.vy;
                work->unk30.vx = 0x400;
                work->unk30.vy = 0x400;
                work->unk30.vz = 0x400;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy -= 0x280;
                ex->func_800E63E4(0, 1);
            }
            break;
        }
        case 5:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                SVECTOR rot2;
                Fx134Bolt *bolt;

                t = (frame << 12) / 23;
                c = (frame << 12) / 2944 + 0x8D;
                s = (t >> 4) + 0x20;
                ex->func_800E6624(frame * 0x14, 0x1C0, 0x180, 0x18, 0x60, 0x320, 0x100);
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vz += 0x18;
                work->unk24.vy += 0x20;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                k_c5 = (t >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk64, k_c5, k_c5, k_c5);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x800;
                work->unk44.vz = 0xAAA;
                work->unk24.vy -= 0x578;
                work->unk50.vz += 0x18;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk44.vx = work->unk44.vy = 0x6DB;
                work->unk44.vz = 0x1000;
                work->unk50.vz += 0x2AA;
                work->unk24.vy -= 0x578;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                a_c5 = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_sin(a_c5) >> 5) - 0x80, (ex->math_sin(a_c5) >> 5) - 0x80,
                                         (ex->math_sin(a_c5) >> 5) - 0x80);
                work->unk24.vy += 0xAF0;
                yo = 0;
                xo = 0;
                scale.vz = 0x555;
                scale.vx = scale.vy = ex->math_sin(a_c5) * 2 / 3 + 0x100;
                pos.vx = work->unk24.vx;
                a_c5_p2 = t >> 1;
                pos.vy = work->unk24.vy + 0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame << 7;
                pos.vz = work->unk24.vz;
                for (i_c5 = 0; i_c5 < 3; i_c5++) {
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a_c5_p2) >> 5) - 0x80, (ex->math_sin(a_c5_p2) >> 5) - 0x80,
                                             (ex->math_sin(a_c5_p2) >> 5) - 0x80);
                    scale.vx = scale.vy = (scale.vx * 3) >> 2;
                    rot.vz += 0x300;
                    pos.vy += 0x20;
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy + 0x400;
                from.vz = work->unk24.vz;
                k_c5_p2 = ex->btlfx_rand();
                for (i_c5_p2 = 0, bolt = D_801EC8F4; i_c5_p2 < 3; i_c5_p2++, bolt++) {
                    n_c5 = ex->btlfx_randCentered(0x400);
                    to.vx = work->unk8.vx + (ex->math_cos(k_c5_p2) >> 1);
                    to.vz = work->unk8.vz + (ex->math_sin(k_c5_p2) >> 1);
                    func_801E8460(entry, bolt, &from, &to, 0x100, 0x200, k_c5_p2, 0xC0, 0, n_c5 + 0xC00, (ex->btlfx_rand() & 0xF) + 0xF);
                    k_c5_p2 += 0x554 + (ex->btlfx_rand() & 0xFF);
                }
                rot2.vx = -0x400;
                rot2.vy = 0;
                a_c5_p3 = t >> 2;
                rot2.vz = ex->math_sin(a_c5_p3) >> 3;
                func_801E7E48(&work->unk8, &rot2, (ex->math_sin(a_c5_p3) >> 3) + 0x1000, 0x40, 0x17, 0x3D00, 0, 0x80, a_c5_p3, 1);
                if (frame >= 0x17) {
                    work->unk0 = 6;
                    entry->unk0 = -1;
                    D_801EC860.vz += 0x400;
                    work->unk8.vx = D_801EC860.vx;
                    work->unk8.vz = D_801EC860.vz;
                    D_801EC860.vz -= 0x400;
                    work->unk30.vx = 0x2800;
                    work->unk30.vy = 0x2800;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vz = work->unk8.vz;
                    work->unk24.vz -= 0x1C00;
                    work->unk24.vy -= 0x3800;
                    work->unk30.vz = 0x1800;
                    work->unk10.vx = work->unk24.vx;
                    work->unk10.vy = work->unk24.vy;
                    work->unk10.vz = work->unk24.vz;
                    ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk3C);
                    work->unk50.vx = work->unk3C.vx;
                    work->unk50.vz = work->unk3C.vz;
                    ex->func_800E63E4(1, 1);
                    work->unk3C.vz += 0x320;
                }
                break;
            }
            }
            break;
        case 6:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                Fx134Bolt *bolt;

                t = (frame << 12) / 56;
                c = ((t << 5) >> 12) + 0x8D;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, (t * 0x58) >> 12, 0, 0);
                s = 0x80;
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

                a1_c6 = t >> 2;
                yo = 0;
                work->unk18.vy = work->unk18.vy - 0x400 - (ex->math_sin(a1_c6) >> 1);
                xo = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
                work->unk3C.vz += 8;
                work->unk24.vz += 0x4D;
                work->unk24.vy += 0x9B;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x3800;
                work->unk44.vz = 0x2000;
                work->unk24.vy -= 0x578;
                work->unk50.vz += 0x14;
                work->unk24.vz += 0x2BC;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80);
                work->unk50.vz -= 0x200;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80);
                work->unk44.vz = 0x3000;
                work->unk44.vx = work->unk44.vy = 0x2000;
                work->unk50.vz += 0x200;
                work->unk24.vz -= 0x2BC;
                work->unk24.vz -= 0x9C4;
                work->unk24.vy += 0x578;
                work->unk24.vy -= 0x1388;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                ex->Hi_ModifyEffModelRGB(work->unk6C, -0x2B, -0x2B, -0x2B);
                work->unk24.vy += 0x1388;
                work->unk24.vz += 0x9C4;
                if (frame > 0x28) {
                    b_c6 = (frame - 0x28) << 8;
                    k_c6 = ex->math_sin(b_c6 >> 2) * 5 / 2 + 0x400;
                    scale.vz = b_c6 + 0x1800;
                    scale.vx = scale.vy = k_c6;
                    rot.vx = -0x400;
                    rot.vy = 0;
                    rot.vz = frame * 0x8D;
                    pos.vx = D_801EC860.vx;
                    pos.vy = D_801EC860.vy - 0x100;
                    pos.vz = D_801EC860.vz;
                    for (i_c6 = 0; i_c6 < 1; i_c6++) {
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                        ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(b_c6 >> 2) >> 5) - 0x80,
                                                 (ex->math_sin(b_c6 >> 2) >> 5) - 0x80, (ex->math_sin(b_c6 >> 2) >> 5) - 0x80);
                        scale.vx = scale.vy = (scale.vx * 3) >> 2;
                        rot.vz += 0x400;
                        pos.vy += 0x20;
                    }
                    scale.vx = scale.vz = ex->math_sin(b_c6 >> 2) * 5 / 2 + 0x1000;
                    scale.vy = (((frame - 0x28) * 0x300 << 8) >> 12) + 0x800;
                    rot.vx = 0;
                    rot.vz = 0;
                    rot.vy = frame * 0x8D;
                    pos.vx = D_801EC860.vx;
                    pos.vy = D_801EC860.vy;
                    pos.vz = D_801EC860.vz;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                    ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(b_c6 >> 1) >> 5) - 0x80,
                                             (ex->math_sin(b_c6 >> 1) >> 5) - 0x80, (ex->math_sin(b_c6 >> 1) >> 5) - 0x80);
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy + 0x1838;
                from.vz = work->unk24.vz + 0xC1C;
                for (i_c6 = 0, bolt = D_801EC8F4; i_c6 < 2; i_c6++, bolt++) {
                    k_c6 = ex->btlfx_rand();
                    n_c6 = ex->btlfx_randCentered(0x400);
                    ((s32 *)&to)[0] = work->unk24.vx;
                    ((s32 *)&to)[1] = work->unk24.vy;
                    ex->func_800D044C(&from, (s32)&to, 0x1000, (XY32 *)&work->unk3C, 0x3000, k_c6);
                    func_801E8460(entry, bolt, &from, &to, 0x100, 0x200, k_c6, 0xC0, 0, n_c6 + 0xC00, (ex->btlfx_rand() & 0x1F) + 0x58);
                }
                if (frame >= 0x38) {
                    work->unk0 = 7;
                    entry->unk0 = -1;
                    work->unk8.vx = work->unk10.vx = work->unk24.vx;
                    work->unk8.vy = work->unk10.vy = work->unk24.vy;
                    work->unk8.vz = work->unk10.vz = work->unk24.vz;
                }
                break;
            }
            }
            break;
        case 7:
            c = 0xAD;
            yo = 0;
            xo = 0;
            t = frame << 8;
            a_c7 = t >> 2;
            s = a_c7 + 0x80;
            D_801EC894[1].r = (ex->math_sin(a_c7) * 0xFF) >> 12;
            D_801EC894[1].g = (ex->math_sin(a_c7) * 0xFF) >> 12;
            D_801EC894[1].b = ex->math_sin(a_c7) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x58, 0, 0);
            ex->func_800D0B18((s32)(env->unk20 + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, D_801EC894, 0, 0, 0, 0);
            if (frame > 8) {
                k_c7 = (frame - 8) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, k_c7, k_c7, k_c7);
            }
            work->unk24.vy = work->unk8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x31), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x578;
            work->unk50.vz += 8;
            work->unk24.vz += 0x2BC;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk44.vz = 0x3000;
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vz -= 0x2BC;
            work->unk24.vz -= 0x9C4;
            work->unk24.vy += 0x578;
            work->unk24.vy -= 0x1388;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk24.vy += 0x1388;
            work->unk24.vz += 0x9C4;
            env->unk28 = 0x898;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &D_801EC860, NULL, t * 3 + 0x1000, frame, 1,
                              ex->math_sin(t >> 2));
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &D_801EC860, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->unk28 = 0;
            if (frame >= 0x10) {
                work->unk0 = 8;
                entry->unk0 = -1;
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vx -= 0x2800;
                work->unk10.vy += 0xB86;
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk10);
            }
            break;
        case 8:
            c = 0xBC;
            yo = 0;
            xo = 0;
            t = frame << 7;
            s = 0x200;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                k_c8 = ((frame - 0x1A) << 7) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k_c8, k_c8, k_c8);
            }
            if (frame >= 0xA) {
                for (i_c8 = 0; i_c8 < 3; i_c8++) {
                    o_c8 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (o_c8 != NULL) {
                        k_c8 = ex->btlfx_randCentered(0x200);
                        o_c8->unkC.vy = 0x6A4 - ((0x1000 - t) >> 1);
                        o_c8->unkC.vx = work->unk18.vx - ((t >> 4) + 0x1800) + k_c8;
                        o_c8->unkC.vz = work->unk18.vz + ex->btlfx_randCentered(0x1C00);
                        o_c8->unk4.vx = ex->btlfx_randCentered(0x100);
                        o_c8->unk4.vy = -0x140 - (ex->btlfx_rand() & 0xFF);
                        o_c8->unk4.vz = ex->btlfx_randCentered(0x100);
                        o_c8->unk14.vx = ex->btlfx_rand();
                        o_c8->unk14.vy = ex->btlfx_rand();
                        o_c8->unk14.vz = ex->btlfx_rand();
                        o_c8->unk0 = 3;
                        o_c8->unk4.pad = t / 5 + (ex->btlfx_rand() & 0x3FF) + 0x80;
                    }
                }
            }
            if (frame == 1) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk10);
            }
            if (frame == 4) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                work->unk0 = 0xA;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->unk10);
                ex->func_800E3C58();
                work->unk18.vx = 0;
                work->unk18.vy = -0x200;
                work->unk18.vz = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
            }
            break;
        case 10:
            switch (frame) {
            default: {
                CVECTOR col;
                SVECTOR rot;
                Fx134Vec3 scale;

                t = frame << 7;
                k_c9 = 0xF5 - ((frame * 0x7A80) >> 12);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k_c9, k_c9, k_c9);
                rot.vx = 0;
                rot.vy = 0;
                k_c9_p2 = 0xA0 - ((frame << 14) >> 12);
                rot.vz = 0x20;
                scale.vx = 0x1100;
                scale.vy = 0x1100;
                scale.vz = 0x1000;
                col.r = col.g = col.b = k_c9_p2;
                ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 1, NULL);
                if (frame >= 0x20) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        if (c != 0) {
            CVECTOR col;
            SVECTOR rot;
            Fx134Vec3 scale;

            col.r = col.g = col.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vx = scale.vy = s + 0x1000;
            scale.vz = 0x1000;
            ex->func_800E6764(1, xo + 0xA0, yo + 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 0, NULL);
        }
        if ((u32)(work->unk0 - 3) < 5) {
            ex->func_800E6624((0x1F4 - work->unk4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->unk4++;
    }
    return 0;
}
