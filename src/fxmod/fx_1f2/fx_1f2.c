#include "fxmod/fx_1f2.h"

void func_801E7704(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot) {
    Unk800F78A4 *env = D_801EA260;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 c = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), arg4, c, c, c);
}

s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA260);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

/* Matching levers: variables shared between distant blocks become long global pseudos that take callee-saved
 * registers, so most blocks use their own copies of i, j, tr, sv, alpha, lo, md and tint. Trail point stores go
 * through an SVECTOR pointer (sv->vx ... sv->pad; sv++) so loop.c combines the vy/vz/pad stores onto one giv.
 * The unk4C block: the GTE path's header pointer pt is assigned in both paths, so it is no giv (find_givs wants
 * a single set): it stays off + base, with sv = pt + 4, as in the original. The loop's insn count decides
 * move_movables' threshold test (threshold * savings * lifetime >= insn_count, threshold 29 - 3 per move,
 * insn_count doubled for a register already moved by an inner loop); the GTE x offset written j * 0x60 (a giv
 * whose multiply is deleted after loop.c) and one drawTrail call in the pad path keep 0x1000 and the pad's 0x80
 * in the loop. The second sparks loop passes &D_801EA690[i] (not tr_sp2) to drawTrail, which puts the
 * hoisted D_801EA690 in the tr register and copies it to the base register. */
#define FX1F2_DRAW(m) ex->Hi_DrawEffModel(&(m).unk8, &(m).unk10, (s32)&(m).unk20, (m).unk0)

s32 fx_1f2_op80(s32 mode, Fx1f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    MATRIX m;
    SVECTOR in[2];
    VECTOR out[2];
    VECTOR step;
    s32 flag;
    s32 flag2;
    s32 frame;
    Pool6CEntry *p;
    Fx1f2Trail *tr;
    Fx1f2Ring *rg;
    Fx1f2Model *md;
    SVECTOR *sv;
    s32 i;
    s32 j;
    s32 a;
    s32 r;
    s32 c;
    s32 lo;
    s32 hi;
    s32 alpha;
    s32 n;
    s32 tint;
    s32 j_m1s;
    s32 lo_m1s;
    s32 a_m1s;
    s32 lo_r0;
    s32 lo_md;
    Fx1f2Model *md_r28;
    Fx1f2Trail *tr_sp;
    s32 lo_sp;
    Fx1f2Trail *tr_sp2;
    SVECTOR *sv_sp2;
    s32 alpha_u4c;
    s32 j_u4ce;
    s32 alpha_u4cn;
    s32 j_u58;
    s32 alpha_u58;
    s32 a_u58;
    SVECTOR *sv_u58;
    s32 n_u58;
    s32 k8;
    s32 fd;
    Fx1f2Model *mm;
    VECTOR *q;
    VECTOR *q2;

    if (mode == 0) {
        *arg4 = 0x128;
    } else if (mode == 1) {
        env->camera->points[1].vx = 0;
        env->camera->points[1].vz = -0x4000;
        env->camera->points[1].vy = -0x6000;
        D_801EA260 = env;
        ex->btlfx_initEffModels(D_801ED090, 0x20);
        for (i = 0; i < 0x20; i++) {
            ((s32 *)work)[i] = 0;
            ((s32 *)work)[i + 0x20] = 0;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&D_801EA670[i]);
            D_801EA670[i].vy = 0;
        }
        ex->btlfx_getUnitBonePos(0x10, 5, (u16 *)&work->unk110[0]);
        ex->btlfx_getUnitBonePos(0x10, 9, (u16 *)&work->unk110[1]);
        work->unk120.vx = work->unk110[0].vx;
        work->unk120.vz = work->unk110[0].vz - 0x200;
        work->unk120.vy = (work->unk110[0].vy + work->unk110[1].vy) >> 1;
        D_801EA650.vx = 0;
        D_801EA650.vy = 0;
        D_801EA650.vz = 0;
        D_801EA660.vx = 0;
        D_801EA660.vy = 0;
        D_801EA660.vz = 0;
        for (i = 0; i < 0x10; i++) {
            D_801EA690[i].unk4C = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801EA690[i], 0);
        }
        for (i = 0; i < 8; i++) {
            D_801EA690[i].unk4C = ex->btlfx_rand() & 0xF;
            for (j_m1s = 0; j_m1s < 0x10;) {
                if (i & 1) {
                    a_m1s = j_m1s << 8;
                    c = 1;
                    r = (ex->btlfx_rand() & 0xFF) + 0x17E;
                } else {
                    a_m1s = 0x1000 - (j_m1s << 8);
                    c = 0;
                    r = (ex->btlfx_rand() & 0xFF) + 0x200;
                }
                lo_m1s = (ex->math_sin(a_m1s) * r) >> 12;
                hi = ex->math_cos(a_m1s) * r;
                D_801EA690[i].unk50[j_m1s].vx = work->unk110[c].vx - lo_m1s;
                D_801EA690[i].unk50[j_m1s].vz = work->unk110[c].vz - (hi >> 12);
                D_801EA690[i].unk50[j_m1s].vy = work->unk110[c].vy + (ex->btlfx_rand() & 0xFF) - 0x80;
                D_801EA690[i].unk50[j_m1s].pad = j_m1s + 1;
                j_m1s++;
            }
            D_801EA690[i].unk50[15].pad = 0;
        }
        D_801EA270.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x84), 0xFF);
        setVector(&D_801EA270.unk8, 0, 0, 0);
        setVector(&D_801EA270.unk10, 0, 0, 0);
        setVector(&D_801EA270.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA2A0.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 0xFF);
        setVector(&D_801EA2A0.unk8, 0, 0, 0);
        setVector(&D_801EA2A0.unk10, 0, 0, 0);
        setVector(&D_801EA2A0.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA2D0.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x89), 1);
        setVector(&D_801EA2D0.unk8, 0, 0, 0);
        setVector(&D_801EA2D0.unk10, 0, 0, 0);
        setVector(&D_801EA2D0.unk20, 0xD00, 0xD00, 0xD00);
        D_801EA3E0.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x88), 1);
        setVector(&D_801EA3E0.unk8, 0, 0, 0);
        setVector(&D_801EA3E0.unk10, 0, 0, 0);
        setVector(&D_801EA3E0.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA410[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 0xFF);
        D_801EA410[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8A), 0xFF);
        D_801EA410[0].unk4 = 0xC00;
        D_801EA410[1].unk4 = 0xC00;
        setVector(&D_801EA410[0].unk8, 0, 0, 0);
        setVector(&D_801EA410[1].unk8, 0, 0, 0x800);
        setVector(&D_801EA410[0].unk10, 0, 0, 0);
        setVector(&D_801EA410[1].unk10, 0, 0, 0);
        setVector(&D_801EA410[0].unk20, 0xC00, 0xC00, 0xC00);
        setVector(&D_801EA410[1].unk20, 0xC00, 0xC00, 0xC00);
        D_801EA470.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8C), 1);
        setVector(&D_801EA470.unk8, 0, 0, 0);
        setVector(&D_801EA470.unk10, 0, 0, 0);
        setVector(&D_801EA470.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA4A0[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        D_801EA4A0[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        setVector(&D_801EA4A0[0].unk8, 0, 0, 0);
        setVector(&D_801EA4A0[1].unk8, 0, 0, 0x800);
        setVector(&D_801EA4A0[0].unk10, 0, 0, 0);
        setVector(&D_801EA4A0[1].unk10, 0, 0, 0);
        setVector(&D_801EA4A0[0].unk20, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EA4A0[1].unk20, 0x1000, 0x1000, 0x1000);
        D_801EA530[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        D_801EA530[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        D_801EA530[2].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        D_801EA530[3].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        setVector(&D_801EA530[0].unk8, 0, 0, 0);
        setVector(&D_801EA530[1].unk8, 0, 0, 0x800);
        setVector(&D_801EA530[2].unk8, 0, 0, 0);
        setVector(&D_801EA530[3].unk8, 0, 0, 0x800);
        setVector(&D_801EA530[0].unk10, 0, 0, 0);
        setVector(&D_801EA530[1].unk10, 0, 0, 0);
        setVector(&D_801EA530[2].unk10, 0, 0, 0);
        setVector(&D_801EA530[3].unk10, 0, 0, 0);
        setVector(&D_801EA530[0].unk20, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EA530[1].unk20, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EA530[2].unk20, 0x1000, 0x1000, 0x1000);
        setVector(&D_801EA530[3].unk20, 0x1000, 0x1000, 0x1000);
        D_801EA500.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8E), 1);
        setVector(&D_801EA500.unk8, 0, 0, 0);
        setVector(&D_801EA500.unk10, 0, 0, 0);
        setVector(&D_801EA500.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA5F0.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xF), 1);
        setVector(&D_801EA5F0.unk8, 0, 0, 0);
        setVector(&D_801EA5F0.unk10, 0, 0, 0);
        setVector(&D_801EA5F0.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA620.unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 2);
        setVector(&D_801EA620.unk8, 0, 0, 0);
        setVector(&D_801EA620.unk10, work->unk120.vx, work->unk120.vy, work->unk120.vz);
        setVector(&D_801EA620.unk20, 0x1000, 0x1000, 0x1000);
        D_801EA300[0].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 0xFF);
        D_801EA300[1].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 0xFF);
        D_801EA300[2].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8B), 0xFF);
        D_801EA300[3].unk0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 0xFF);
        for (i = 0; i < 4; i++) {
            D_801EA300[i].unk4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
            D_801EA300[i].unk8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 1);
            D_801EA300[i].unkC = 0xC00;
            setVector(&D_801EA300[i].unk10, 0, i * 0x400 + 0x800, 0);
            setVector(&D_801EA300[i].unk18, 0, 0, 0);
            setVector(&D_801EA300[i].unk28, 0x1000, 0x1000, 0x1000);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk2C = 0x20;
            work->unk4C = 0x5F;
            p = work->unk84 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x2D:
            work->unk54 = 0x32;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk110[0]);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk110[1]);
            p = work->unk80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk120);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x4F:
            work->unkC = 0x10;
            break;
        case 0x5F:
            work->unk4C = 0;
            for (i = 0; i < 0x10; i++) {
                D_801EA690[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EA690[i], 0);
            }
            work->unk20 = 0x40;
            work->unk0 = 1;
            break;
        case 0x8F:
            work->unk8 = 0x50;
            work->unk44 = 0x40;
            break;
        case 0x9F:
            work->unk3C = 0x40;
            work->unk24 = 0x50;
            work->unk100.vy = -0x4C8;
            work->unk100.vx = 0;
            work->unk100.vz = 0;
            work->unk108.vx = 0;
            work->unk108.vy = 0x4C8;
            work->unk108.vz = 0;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk100);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk108);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0xCF:
            work->unk28 = 0x80;
            work->unk34 = 0x80;
            work->unk38 = 0x200;
            break;
        case 0xFF:
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801EA650);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            break;
        case 0x11F:
            p = work->unk88 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&D_801EA660);
            if (p != NULL) {
                p->unk22 = 0x400;
            }
            p = work->unk8C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&D_801EA660);
            if (p != NULL) {
                p->unk22 = 0x200;
            }
            work->unk4 = 0x80;
            break;
        case 0x13F:
            work->unk14 = 0x10;
            break;
        case 0x14F:
            work->unk2C = 0x40;
            D_801EA650.vx = 0;
            D_801EA650.vy = -0x4FD;
            D_801EA650.vz = 0;
            D_801EA600.vx = 0;
            D_801EA600.vy = -0x4FD;
            D_801EA600.vz = 0;
            ex->btlfx_moveEmitterTo((s32)work->unk88, (s32)&D_801EA650);
            ex->btlfx_moveEmitterTo((s32)work->unk8C, (s32)&D_801EA650);
            work->unk18 = 0x10;
            work->unk50 = 0x68;
            work->unk24 = 0;
            work->unk0 = 0;
            work->unk8 = 0;
            work->unk44 = 0;
            work->unk3C = 0;
            work->unk4 = 0;
            work->unk20 = 0x40;
            work->unk34 = 0x50;
            break;
        case 0x18F:
            work->unk14 = 0x10;
            for (i = 0; i < 0x10; i++) {
                D_801EBB90[i].unk4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&D_801EBB90[i], 0);
            }
            break;
        case 0x19F:
            work->unk18 = 0x10;
            ex->btlfx_stopEmitter(work->unk8C);
            ex->btlfx_stopEmitter(work->unk88);
            break;
        case 0x1A7:
            work->unk58 = 0x18;
            break;
        case 0x1BF:
            return 1;
        }
        ex->btlfx_aimAngles((s16 *)&D_801EA650, (s16 *)&env->camera->eye, (s16 *)&D_801EA658);
        if (work->unk0 != 0) {
            D_801EA2A0.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA2A0.unk8.vy = D_801EA658.vy;
            FX1F2_DRAW(D_801EA270);
            FX1F2_DRAW(D_801EA2A0);
            FX1F2_DRAW(D_801EA2D0);
            FX1F2_DRAW(D_801EA3E0);
            for (i = 0; i < 4; i++) {
                rg = &D_801EA300[i];
                rg->unk18.vx = (ex->math_sin(rg->unk10.vy) * rg->unkC) >> 12;
                lo_r0 = ex->math_cos(rg->unk10.vy) * rg->unkC;
                rg->unk18.vy = 0;
                rg->unk18.vz = lo_r0 >> 12;
                if (work->unk48 == 0) {
                    ex->Hi_DrawEffModel(&D_801EA300[i].unk10, &D_801EA300[i].unk18, (s32)&D_801EA300[i].unk28, rg->unk8);
                    if (work->unk44 != 0) {
                        if (--work->unk44 == 0) {
                            work->unk48 = 1;
                        }
                        tint = (work->unk44 - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->unk8, tint, tint, tint);
                        rg->unkC -= work->unk44 >> 4;
                    }
                }
                rg->unk18.vx += ex->btlfx_randRange(-8, 8);
                rg->unk18.vy += ex->btlfx_randRange(-8, 8);
                if (work->unk40 == 0) {
                    ex->Hi_DrawEffModel(&D_801EA300[i].unk10, &D_801EA300[i].unk18, (s32)&D_801EA300[i].unk28, rg->unk4);
                    if (work->unk3C != 0) {
                        if (--work->unk3C == 0) {
                            work->unk40 = 1;
                        }
                        tint = (work->unk3C - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->unk4, tint, tint, tint);
                    }
                }
                ex->Hi_DrawEffModel(&D_801EA300[i].unk10, &D_801EA300[i].unk18, (s32)&D_801EA300[i].unk28, rg->unk0);
            }
            for (i = 0; i < 2; i++) {
                md = &D_801EA410[i];
                lo_md = ex->math_cos(md->unk8.vz) * md->unk4;
                md->unk10.vx = 0;
                md->unk10.vz = 0;
                md->unk10.vy = lo_md >> 12;
                ex->Hi_DrawEffModel(&D_801EA410[i].unk8, &D_801EA410[i].unk10, (s32)&D_801EA410[i].unk20, md->unk0);
                md->unk4 -= 4;
            }
            D_801EA270.unk8.vy++;
            D_801EA2D0.unk8.vy--;
            ex->btlfx_drawScrollMove(0x7530 - frame * 3, 0x340, 0x180, 0x10, 0x80, 0x350, 0x180);
            ex->btlfx_drawScrollMove(0x7530 - frame * 4, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unk4 != 0) {
            work->unk4--;
            D_801EA470.unk20.vx += 8;
            D_801EA470.unk20.vy += 8;
            D_801EA470.unk20.vz += 8;
            FX1F2_DRAW(D_801EA470);
            if (work->unk4 > 0x60) {
                tint = (0x60 - work->unk4) * 2;
                ex->Hi_ModifyEffModelRGB(D_801EA470.unk0, tint, tint, tint);
            }
        }
        if (work->unk24 != 0) {
            work->unk24--;
            D_801EA500.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA500.unk8.vy = D_801EA658.vy;
            FX1F2_DRAW(D_801EA500);
            if (work->unk24 > 0x30) {
                tint = (0x30 - work->unk24) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA500.unk0, tint, tint, tint);
            }
            if (work->unk24 < 0x20) {
                tint = (work->unk24 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA500.unk0, tint, tint, tint);
            }
        }
        if (work->unk8 != 0) {
            work->unk8--;
            D_801EA4A0[0].unk8.vx = 0x900;
            D_801EA4A0[1].unk8.vx = 0x700;
            D_801EA4A0[1].unk8.vy = D_801EA658.vy;
            D_801EA4A0[0].unk8.vy = D_801EA658.vy;
            for (i = 0; i < 2; i++) {
                ex->Hi_DrawEffModel(&D_801EA4A0[i].unk8, &D_801EA4A0[i].unk10, (s32)&D_801EA4A0[i].unk20, D_801EA4A0[i].unk0);
                md = &D_801EA4A0[i];
                if (work->unk8 > 0x30) {
                    tint = (0x30 - work->unk8) * 8;
                    ex->Hi_ModifyEffModelRGB(md->unk0, tint, tint, tint);
                }
                if (work->unk8 < 0x20) {
                    tint = (work->unk8 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md->unk0, tint, tint, tint);
                }
            }
        }
        if (work->unk28 != 0) {
            work->unk28--;
            for (i = 0; i < 4; i++) {
                md_r28 = &D_801EA530[i];
                md_r28->unk8.vx = D_801EA658.vx + 0x800;
                md_r28->unk8.vy = D_801EA658.vy;
                ex->Hi_DrawEffModel(&D_801EA530[i].unk8, &D_801EA530[i].unk10, (s32)&D_801EA530[i].unk20, md_r28->unk0);
                if (work->unk28 > 0x70) {
                    tint = (0x70 - work->unk28) * 0x10;
                    ex->Hi_ModifyEffModelRGB(md_r28->unk0, tint, tint, tint);
                }
                if (work->unk28 < 0x20) {
                    tint = (work->unk28 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md_r28->unk0, tint, tint, tint);
                }
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x320, 0x180, 0x20, 0x80, 0x360, 0x180);
        }
        for (i = 0, md_r28 = D_801EA530; i < 4; i++) {
            md_r28[i].unk8.vz += work->unk2C;
        }
        if (work->unk50 != 0) {
            work->unk50--;
            D_801EA5F0.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA5F0.unk8.vy = D_801EA658.vy;
            D_801EA5F0.unk8.vz += 0x40;
            FX1F2_DRAW(D_801EA5F0);
            if (work->unk50 > 0x70) {
                tint = (0x40 - work->unk50) * 0x10;
                ex->Hi_ModifyEffModelRGB(D_801EA5F0.unk0, tint, tint, tint);
            }
            if (work->unk50 < 0x20) {
                tint = (work->unk50 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(D_801EA5F0.unk0, tint, tint, tint);
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unk54 != 0) {
            work->unk54--;
            D_801EA620.unk8.vx = D_801EA658.vx + 0x800;
            D_801EA620.unk8.vy = D_801EA658.vy;
            D_801EA620.unk8.vz += 0x40;
            FX1F2_DRAW(D_801EA620);
            if (work->unk54 > 0x2A) {
                tint = (0x2A - work->unk54) << 5;
                ex->Hi_ModifyEffModelRGB(D_801EA620.unk0, tint, tint, tint);
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->unkC != 0) {
            work->unkC--;
            tint = func_801E7794(1, 0x20, work->unkC, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 2, tint, tint, tint);
        }
        if (work->unk10 != 0) {
            work->unk10--;
            tint = func_801E7794(2, 0x10, work->unk10, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 2, tint, tint, tint);
        }
        if (work->unk14 != 0) {
            work->unk14--;
            tint = func_801E7794(1, 0x10, work->unk14, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 8), 1, tint, tint, tint);
        }
        if (work->unk18 != 0) {
            work->unk18--;
            tint = func_801E7794(0, 0x10, work->unk18, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 8), 1, tint, tint, tint);
        }
        if (work->unk20 != 0) {
            s32 k;

            work->unk20--;
            k = work->unk20;
            scale.vz = scale.vy = scale.vx = k * 3 + 0x1000;
            col.r = col.g = col.b = k * 2;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
        if (work->unk34 != 0) {
            work->unk34--;
            for (i = 0; i < 0x10; i++) {
                tr_sp = &D_801EA690[i];
                if (tr_sp->unk4C == 0 && work->unk34 >= 0xD) {
                    ex->func_800316D8(&D_801EA568, &m);
                    m.t[0] = D_801EA650.vx;
                    m.t[1] = D_801EA650.vy;
                    m.t[2] = D_801EA650.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    for (j = 0; j < 2; j++) {
                        if (i & 1) {
                            in[j].vx = ex->btlfx_randRange(0xC00 - j * 0x2400, 0x1800 - j * 0x2400);
                        } else {
                            in[j].vx = ex->btlfx_randRange(j * 0x2400 - 0x1800, j * 0x2400 - 0xC00);
                        }
                        in[j].vy = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        in[j].vz = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        gte_ldv0(&in[j]);
                        gte_rtv0tr();
                        gte_stlvnl(&out[j]);
                        gte_stflg(&flag);
                    }
                    tr_sp->unk4C = (ex->btlfx_rand() & 3) + 8;
                    step.vx = (out[0].vx - out[1].vx) / 9;
                    step.vy = (out[0].vy - out[1].vy) / 9;
                    step.vz = (out[0].vz - out[1].vz) / 9;
                    for (j = 0; j < 9; j++) {
                        D_801EA690[i].unk50[j].vx = out[0].vx;
                        D_801EA690[i].unk50[j].vy = out[0].vy;
                        D_801EA690[i].unk50[j].vz = out[0].vz;
                        D_801EA690[i].unkD0[j].vx = ex->btlfx_randRange(-0x80, 0x80);
                        D_801EA690[i].unkD0[j].vy = ex->btlfx_randRange(-0x80, 0x80);
                        D_801EA690[i].unkD0[j].vz = ex->btlfx_randRange(-0x80, 0x80);
                        out[0].vx -= step.vx;
                        out[0].vy -= step.vy;
                        out[0].vz -= step.vz;
                    }
                }
            }
            env->unk28 = 0x80;
            for (i = 0; i < 0x10; i++) {
                tr_sp2 = &D_801EA690[i];
                if (tr_sp2->unk4C != 0) {
                    if (--tr_sp2->unk4C < 8) {
                        s32 n = 9;

                        tr_sp2->unk0 = tr_sp2->unk2 = 8;
                        tr_sp2->unk1 = 0;
                        sv_sp2 = tr_sp2->unk4;
                        for (j = 0; j < n; j++) {
                            D_801EA690[i].unk50[j].vx += D_801EA690[i].unkD0[j].vx;
                            D_801EA690[i].unk50[j].vy += D_801EA690[i].unkD0[j].vy;
                            D_801EA690[i].unk50[j].vz += D_801EA690[i].unkD0[j].vz;
                            sv_sp2->vx = D_801EA690[i].unk50[j].vx;
                            sv_sp2->vy = D_801EA690[i].unk50[j].vy;
                            sv_sp2->vz = D_801EA690[i].unk50[j].vz;
                            sv_sp2->pad = work->unk38;
                            sv_sp2++;
                        }
                        alpha = tr_sp2->unk4C << 9;
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha, 1,
                                            (u8 *)&D_801EA690[i], 0);
                    }
                }
            }
            env->unk28 = 0;
        }
        if (work->unk4C != 0) {
            work->unk4C--;
            for (i = 0; i < 0x10; i++) {
                s32 n;
                s32 dz;
                s32 dx;
                Fx1f2Trail *pt;

                if (i >= 8) {
                    if (D_801EA690[i].unk4C != 0) {
                        if (--D_801EA690[i].unk4C < 8) {
                            if (i & 1) {
                                ex->btlfx_getUnitBoneMatrix(0x10, 5, (BtlBone *)&m);
                                m.t[0] = work->unk110[0].vx;
                                m.t[1] = work->unk110[0].vy;
                                m.t[2] = work->unk110[0].vz;
                                dz = 0x200;
                            } else {
                                ex->btlfx_getUnitBoneMatrix(0x10, 9, (BtlBone *)&m);
                                m.t[0] = work->unk110[1].vx;
                                m.t[1] = work->unk110[1].vy;
                                m.t[2] = work->unk110[1].vz;
                                dz = 0x180;
                            }
                            gte_SetRotMatrix(&m);
                            gte_SetTransMatrix(&m);
                            n = 9;
                            pt = &D_801EA690[i];
                            pt->unk0 = pt->unk2 = 8;
                            pt->unk1 = 0;
                            sv = pt->unk4;
                            for (j_u4ce = 0; j_u4ce < n; j_u4ce++) {
                                ((SVECTOR *)out)->vx = D_801EA690[i].unk50[j_u4ce].vx + j_u4ce * 0x60 - 0x180;
                                ((SVECTOR *)out)->vz = D_801EA690[i].unk50[j_u4ce].vz + dz;
                                ((SVECTOR *)out)->vy = D_801EA690[i].unk50[j_u4ce].vy;
                                gte_ldv0((SVECTOR *)out);
                                gte_rtv0tr();
                                gte_stlvnl((VECTOR *)in);
                                gte_stflg(&flag2);
                                sv->vx = ((VECTOR *)in)->vx;
                                sv->vy = ((VECTOR *)in)->vy;
                                sv->vz = ((VECTOR *)in)->vz;
                                sv->pad = 0x40;
                                sv++;
                                D_801EA690[i].unk50[j_u4ce].vx += D_801EA690[i].unkD0[j_u4ce].vx;
                                D_801EA690[i].unk50[j_u4ce].vy += D_801EA690[i].unkD0[j_u4ce].vy;
                                D_801EA690[i].unk50[j_u4ce].vz += D_801EA690[i].unkD0[j_u4ce].vz;
                            }
                            alpha_u4c = 0x800;
                            if (i & 1) {
                                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xC), 0, NULL, frame, 0x1000, alpha_u4c, 1, (u8 *)&D_801EA690[i], 0);
                            } else {
                                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha_u4c, 1, (u8 *)&D_801EA690[i], 0);
                            }
                        }
                    } else {
                        D_801EA690[i].unk4C = (ex->btlfx_rand() & 3) + 8;
                        for (j_u4ce = 0; j_u4ce < 9; j_u4ce++) {
                            D_801EA690[i].unk50[j_u4ce].vx = 0;
                            D_801EA690[i].unk50[j_u4ce].vy = 0;
                            D_801EA690[i].unk50[j_u4ce].vz = 0;
                            D_801EA690[i].unkD0[j_u4ce].vx = (ex->btlfx_rand() & 0xF) - 8;
                            D_801EA690[i].unkD0[j_u4ce].vy = (ex->btlfx_rand() & 0x1F) - 0x10;
                            D_801EA690[i].unkD0[j_u4ce].vz = (ex->btlfx_rand() & 0x1F) - 0x10;
                        }
                    }
                } else {
                    Fx1f2Trail *tp = &D_801EA690[i];

                    n = 9;
                    sv = tp->unk4;
                    a = D_801EA690[i].unk4C;
                    D_801EA690[i].unk4C = D_801EA690[i].unk50[a].pad;
                    pt = &D_801EA690[i];
                    pt->unk0 = pt->unk2 = 8;
                    pt->unk1 = 0;
                    for (j_u4ce = 0; j_u4ce < n; j_u4ce++) {
                        sv->vx = D_801EA690[i].unk50[a].vx;
                        sv->vy = D_801EA690[i].unk50[a].vy;
                        sv->vz = D_801EA690[i].unk50[a].vz;
                        sv->pad = 0x80;
                        sv++;
                        a = D_801EA690[i].unk50[a].pad;
                    }
                    env->unk28 = 0x80;
                    alpha_u4cn = 0xFFF;
                    if (i & 1) {
                        fd = ex->btlfx_getFileData(entry->file, 0xC);
                    } else {
                        fd = ex->btlfx_getFileData(entry->file, 6);
                    }
                    ex->btlfx_drawTrail((u8 *)fd, 0, NULL, frame, 0x1000, alpha_u4cn, 1, (u8 *)&D_801EA690[i], 0);
                    env->unk28 = 0;
                }
            }
        }
        if (work->unk58 != 0) {
            work->unk58--;
            env->unk28 = 0x40;
            k8 = 8;
            for (i = 0; i < env->targets->targetCount; i++) {
                tr = &D_801EBB90[i];
                if (tr->unk4C != 0) {
                    if (--tr->unk4C < 8) {
                        n_u58 = 9;
                        sv_u58 = tr->unk4;
                        tr->unk0 = tr->unk2 = k8;
                        tr->unk1 = 0;
                        for (j_u58 = 0; j_u58 < n_u58; j_u58++) {
                            tr->unk50[j_u58].vx += tr->unkD0[j_u58].vx;
                            tr->unk50[j_u58].vy += tr->unkD0[j_u58].vy;
                            tr->unk50[j_u58].vz += tr->unkD0[j_u58].vz;
                            sv_u58->vx = tr->unk50[j_u58].vx;
                            sv_u58->vy = tr->unk50[j_u58].vy;
                            sv_u58->vz = tr->unk50[j_u58].vz;
                            sv_u58->pad = 0x80;
                            sv_u58++;
                        }
                        alpha_u58 = 0x1000;
                        if (work->unk58 < 8) {
                            alpha_u58 = work->unk58 << 9;
                        }
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha_u58, 1,
                                            (u8 *)tr, 0);
                    }
                } else {
                    tr->unk4C = k8;
                    ((SVECTOR *)out)[1].vx = D_801EA670[i].vx + ex->btlfx_randRange(-0x40, 0x40);
                    ((SVECTOR *)out)[1].vz = D_801EA670[i].vz + ex->btlfx_randRange(-0x40, 0x40);
                    for (j_u58 = 0; j_u58 < 9; j_u58++) {
                        a_u58 = j_u58 + 1;
                        tr->unk50[j_u58].vx = ((SVECTOR *)out)[1].vx + ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->unk50[j_u58].vz = ((SVECTOR *)out)[1].vz + ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->unk50[j_u58].vy = ex->btlfx_randRange(-a_u58 * 4, 0) - (j_u58 << 6);
                        tr->unkD0[j_u58].vx = ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->unkD0[j_u58].vz = ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->unkD0[j_u58].vy = -ex->btlfx_rand() & 7;
                    }
                }
            }
            env->unk28 = 0;
        }
        D_801EA3E0.unk8.vy += 3;
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f2/nonmatchings/fx_1f2", fx_1f2_headerPtr);
