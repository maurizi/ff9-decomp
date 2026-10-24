#include "fxmod/fx_17d_4.h"

void func_801E7720(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);
    SVECTOR *v;

    D_801E987C.unk0 = D_801E987C.unk2 = 1;
    D_801E987C.unk1 = 0;
    v = D_801E987C.unk4;
    v[0].vx = a->vx;
    v[0].vy = a->vy;
    v[0].vz = a->vz;
    v[0].pad = arg6;
    v[1].vx = b->vx;
    v[1].vy = b->vy;
    v[1].vz = b->vz;
    v[1].pad = arg6;
    ex->btlfx_drawTrail(file, 0, NULL, arg3, 0x1000, arg4, arg5, (u8 *)&D_801E987C, 0);
}

void func_801E77DC(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag) {
    u8 *data;
    s32 k;
    s32 width;
    BtlfxExports *ex;
    SVECTOR last;
    SVECTOR p0;
    SVECTOR p1;
    s16 *pts;
    Fx17d4Line *tr;
    SVECTOR *dst;
    s32 j;
    s32 i;
    s32 cnt;
    s32 t;

    ex = BTLFX_EXPORTS(D_801E9890);
    k = 0;
    data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x8A);
    w = (w * 3) >> 1;
    do {
        for (j = 0; j < 18; j++) {
            cnt = *(u16 *)(data + ((j + k * 18) << 2));
            pts = (s16 *)(data + cnt);
            cnt = pts[1];
            tr = D_801E97B4[j];
            tr = (Fx17d4Line *)((u8 *)tr + D_801E9724[j * 4 + 2] * k);
            width = w * (D_801E9724[j * 4 + 3] + 1);
            tr->unk0 = tr->unk2 = cnt - 1;
            tr->unk1 = 0;
            pts += 2;
            dst = tr->unk4;
            for (i = 0; i < cnt; i++, dst++, pts += 3) {
                t = D_801E9724[j * 4 + 1];
                if (i == 0) {
                    last.vx = from->vx + pts[0] * 2;
                    last.vy = from->vy + pts[1] * 2;
                    last.vz = from->vz + pts[2] * 2;
                    last.pad = 0;
                }
                t += i;
                if (n >= t) {
                    dst->vx = from->vx + pts[0] * 2;
                    dst->vy = from->vy + pts[1] * 2;
                    dst->vz = from->vz + pts[2] * 2;
                    dst->pad = width;
                    BTLFX_COPY_SVEC(&last, dst);
                    if (t == n && flag) {
                        BTLFX_COPY_SVEC(&p0, dst);
                        BTLFX_COPY_SVEC(&p1, dst);
                        p1.vy -= 0x1000;
                        func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &p1, &p0, 0, 0x800, 1, 0x32);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E9724[j * 4]), 0, (s32 *)from, 0, 0x1000, a,
                                1, (u8 *)tr, fade);
        }
        k++;
    } while (k < 3);
}

void func_801E7B8C(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    s32 k = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);
    Fx17d4Pt *p;
    s32 i;
    s32 a;

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    for (; k < 3; k++) {
        p = (Fx17d4Pt *)ex->btlfx_getFileData(entry->unk8, D_801E9814[k]);
        p = (Fx17d4Pt *)((u8 *)p + 4);
        for (i = 0; i < 20; i++, p++) {
            if (i < n) {
                pos.vx = from->vx + p->x * 2;
                pos.vy = from->vy + p->y * 2;
                pos.vz = from->vz + p->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i & 7]), &pos, &rot, 0x3000,
                                        0, 1, ot, 0, 0);
            }
        }
    }
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 27; i++) {
            if (i <= n * 27 / 20) {
                a = ((i + k * 27) << 12) / 81;
                pos.vx = from->vx + ((ex->math_cos(a) * 1600) >> 12);
                pos.vy = from->vy;
                pos.vz = from->vz + ((ex->math_sin(a) * 1600) >> 12);
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i & 7]), &pos, &rot, 0x1800,
                                        0, 1, ot, 0, 0);
            }
        }
    }
}

void func_801E7EB4(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *d = D_801E981C;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9890);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + d->vx * 2;
        pos.vy = from->vy + d->vy * 2;
        pos.vz = from->vz + d->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, D_801E97FC[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        d++;
        i++;
    } while (i < 3);
}

INCLUDE_RODATA("asm/us/fx_17d_4/nonmatchings/fx_17d_4", fx_17d_4_headerPtr);

/* Matching notes: scalar temporaries are split per case and per use (names are role-search leftovers: _cN = case,
 * the rest found by build/s/roles.py), since sharing a variable across cases makes it a call-crossing global and
 * changes the allocation; which roles share a variable was searched. Case 6: func_801E7B8C's third argument (f3)
 * is its own variable, o = 0x20 is set inside the frame < 0x1E branch, x and f3 are set first in their branches
 * and x is declared before flag/f3 (spill slot order 0x78/0x7C/0x80). Case 4's
 * f-update reuses one variable for 0x1000 - cos and f >> 12; case 5's offsets go through k5z/n5z and are copied
 * a -> b; mode 1's copy order and other statement orders were searched (build/s/permhc.py, bperm.py, winperm.py). */
s32 fx_17d_4_op80(s32 mode, Fx17d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 q6b;
    s32 g5;
    Pool6CEntry *p_c1;
    s32 w13_c2;
    s32 r_c2;
    s32 t_c3;
    s32 x_c3;
    s32 i_c3;
    s32 q_c3;
    Pool6CEntry *p_c4;
    s32 t_c5;
    s32 q4_c6;
    s32 w13_c6;
    s32 r_c6;
    s32 q_c6;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    Fx17d4Vec3 pos;
    SVECTOR rot;
    SVECTOR s;
    SVECTOR b;
    SVECTOR a;
    SVECTOR rot2;
    SVECTOR b2;
    SVECTOR a2;
    s32 c_1;
    s32 c_5;
    s32 c_6;
    s32 frame;
    s32 k;
    s32 n;
    s32 m;
    s32 f;
    s32 g;
    s32 x6n;
    s32 flag;
    s32 f3;
    s32 k5z;
    s32 n5z;
    s32 n5y;
    s32 k5x;
    s32 fk;
    s32 s6a;
    s32 c6a;
    s32 k5;
    s32 w3;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E9890 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EB65C, 5);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        work->unk6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 1);
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x84), 1);
        work->unk74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 0);
        work->unk78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x86), 1);
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk19C, SUMMON(env)->unk60);
        work->unk48.vx = 0;
        work->unk48.vy = -0x400;
        work->unk48.vz = 0;
        work->unk3C.vx = work->unk3C.vz = 0x400;
        work->unk28.vy = work->unk48.vy;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk28.vx = work->unk48.vx;
        work->unk28.vz = work->unk48.vz;
        work->unk3C.vy = 0x1000;
        work->unk28.vy += 0x800;
        work->unk20.vx = work->unk48.vx;
        work->unk20.vy = work->unk48.vy;
        work->unk20.vz = work->unk48.vz;
        env->camera->points[3].vx = work->unk20.vx;
        env->camera->points[3].vy = work->unk20.vy;
        env->camera->points[3].vz = work->unk20.vz;
        D_801EB25C = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            r_c2 = (frame << 12) / 20;
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            work->unk48.vy += ex->math_cos(r_c2 >> 2) >> 4;
            work->unk54.vx = work->unk54.vy = work->unk54.vz = ex->math_cos(r_c2 >> 2) + 0x800;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelAbr(work->unk68, 1);
            ex->Hi_ModifyEffModelRGB(work->unk68, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->Hi_ModifyEffModelAbr(work->unk68, 2);
            ex->Hi_ModifyEffModelRGB(work->unk68, 0, 0, 0);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy += 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0x800;
            rot.vy = frame * 24;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            ex->btlfx_drawScrollMove(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame >= 0x14) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3C58();
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1A0, SUMMON(env)->unk60);
                work->unk34.vx = 0;
                work->unk34.vy = 0;
                work->unk34.vz = 0;
                work->unk3C.vx = work->unk3C.vz = 0x800;
                work->unk3C.vy = 0x1000;
                work->unk28.vx = work->unk48.vx;
                work->unk28.vy = work->unk48.vy;
                work->unk28.vz = work->unk48.vz;
                work->unk20.vx = work->unk48.vx;
                work->unk20.vy = work->unk48.vy;
                work->unk20.vz = work->unk48.vz;
                work->unk28.vy -= 0x400;
                work->unk20.vy -= 0x8FC;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            break;
        case 1:
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            work->unk48.vy -= 0xC;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            ex->btlfx_drawScrollMove(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame >= 0xA) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk10.vy = work->unk48.vy;
                work->unk10.vx = work->unk48.vx;
                work->unk10.vz = work->unk48.vz;
                work->unk10.vy -= 0x400;
                p_c1 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
                if (p_c1 != NULL) {
                    p_c1->unk14 = (s32)D_801E989C;
                    p_c1->unk12 = 0x30;
                    p_c1->unkC = 0x8C;
                    p_c1->unk22 = 0x20;
                }
            }
            break;
        case 2:
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            t_c5 = (frame << 12) / 20;
            m = t_c5 >> 2;
            work->unk48.vy -= (ex->math_cos(m) * 10) >> 12;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk68);
            n = t_c5 / 2 + 0x800;
            sc.vy = ex->math_sin(m) / 2 + 0x800;
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            for (r_c2 = 0, w13_c2 = 0x20; r_c2 < 4; r_c2++, w13_c2 += 0x10) {
                s32 y;

                rot.vx = 0;
                rot.vz = 0;
                sc.vx = sc.vz = (n * (r_c2 + 2)) / 6;
                y = frame * w13_c2;
                rot.vy = y;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk78);
                ex->Hi_ModifyEffModelAbr(work->unk78, 1);
                ex->Hi_ModifyEffModelRGB(work->unk78, (ex->math_sin(t_c5 >> 2) >> 5) - 0x80, (ex->math_sin(t_c5 >> 2) >> 5) - 0x80,
                                         (ex->math_sin(t_c5 >> 2) >> 5) - 0x80);
                y = 0;
            }
            ex->btlfx_drawScrollMove(frame * 24, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame > 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, (frame - 0x10) * 0x3C, (frame - 0x10) * 0x3C,
                                         (frame - 0x10) * 0x3C);
            }
            if (frame >= 0x14) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1A0, SUMMON(env)->unk60);
                ex->func_800E3C58();
                work->unk3C.vx = work->unk3C.vz = 0x1800;
                work->unk34.vx = 0;
                work->unk34.vy = 0;
                work->unk34.vz = 0;
                work->unk3C.vy = 0xAAA;
                work->unk28.vy = work->unk48.vy;
                work->unk28.vx = work->unk48.vx;
                work->unk28.vz = work->unk48.vz;
                work->unk28.vy += 0x800;
            }
            break;
        case 3:
            x_c3 = frame << 6;
            if (frame < 0x20) {
                work->unk60.vy = (ex->math_sin((frame << 10) >> 5) - 0x1000) >> 3;
            }
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk70);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            pos.vx = work->unk48.vx;
            pos.vy = work->unk48.vy;
            pos.vz = work->unk48.vz;
            pos.vy -= 0x1000;
            sc.vy = 0x2000;
            rot.vx = 0x800;
            rot.vy = frame * 4;
            rot.vz = 0;
            sc.vx = sc.vz = x_c3 / 3 + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(x_c3 >> 2) >> 5) - 0x80, (ex->math_cos(x_c3 >> 2) >> 5) - 0x80,
                                     (ex->math_cos(x_c3 >> 2) >> 5) - 0x80);
            if (frame < 0x28) {
                q_c3 = (frame << 12) / 40;
                sc.vy = q_c3 / 3 + 0x1000;
                w3 = q_c3 + 0x1000;
                pos.vx = work->unk48.vx;
                pos.vy = work->unk48.vy;
                q6b = ((0x1000 - q_c3) >> 5) - 0x80;
                pos.vz = work->unk48.vz;
                for (i_c3 = 0, t_c3 = 0x20; i_c3 < 3; i_c3++, t_c3 += 0x10) {
                    s32 y;

                    rot.vx = 0;
                    rot.vz = 0;
                    sc.vx = sc.vz = (w3 * (i_c3 + 2)) / 5;
                    sc.vy = sc.vy * 2 / 3;
                    y = frame * t_c3;
                    rot.vy = y;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk78);
                    ex->Hi_ModifyEffModelAbr(work->unk78, 1);
                    ex->Hi_ModifyEffModelRGB(work->unk78, q6b, q6b, q6b);
                    y = 0;
                }
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 3, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            if (frame < 0xC) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xF0 - frame * 0x14, 0xF0 - frame * 0x14,
                                         0xF0 - frame * 0x14);
            }
            work->unk20.vy += 8;
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            if (frame >= 0x40) {
                work->unk0 = 4;
                ex->func_800E3C58();
                entry->unk0 = -1;
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk20);
                work->unk20.vx += 0x40;
                work->unk20.vy -= 0x5A;
                work->unk20.vz -= 0x40;
                work->unk10.vx = work->unk20.vx;
                work->unk10.vy = work->unk20.vy;
                work->unk10.vz = work->unk20.vz;
                env->camera->points[2].vx = work->unk20.vx;
                env->camera->points[2].vy = work->unk20.vy;
                env->camera->points[2].vz = work->unk20.vz;
                work->unk28.vy += 0x1000;
            }
            break;
        case 4:
            r_c6 = (frame << 12) / 20;
            work->unk54.vx = work->unk54.vy = work->unk54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->unk60, (VECTOR *)&work->unk48, (s32)&work->unk54, SUMMON(env)->unk60, 0);
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk28, (s32)&work->unk3C, work->unk70);
            pos.vx = work->unk28.vx;
            pos.vy = work->unk28.vy;
            pos.vz = work->unk28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->unk74);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk20);
            work->unk20.vx += 0x40;
            work->unk20.vy -= 0x5A;
            work->unk20.vz -= 0x40;
            work->unk10.vx = work->unk20.vx;
            work->unk10.vy = work->unk20.vy;
            work->unk10.vz = work->unk20.vz;
            if (frame >= 0xA) {
                fk = 0x1000 - ex->math_cos(((frame - 0xA) << 10) / 10);
                f = fk * 0x258;
                fk = f >> 12;
                work->unk20.vy += f >> 13;
                work->unk20.vz -= fk;
            }
            env->camera->points[2].vx = work->unk20.vx;
            env->camera->points[2].vy = work->unk20.vy;
            env->camera->points[2].vz = work->unk20.vz;
            if (frame == 0) {
                p_c4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
                if (p_c4 != NULL) {
                    p_c4->unk22 = 0x8C;
                }
            }
            env->unk28 = 0x10;
            w13_c6 = 0x1000;
            if (frame < 8) {
                w13_c6 = frame << 9;
            }
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unk10, NULL, 0x1000, 0, 1, w13_c6);
            if (frame >= 6) {
                c_5 = ((frame - 6) << 12) / 14;
            } else {
                c_5 = 0;
            }
            BTLFX_COPY_SVEC(&s, &work->unk10);
            s.vy += ex->math_sin(r_c6 >> 2) >> 3;
            s.vz -= 0x400;
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &s, &work->unk10, frame, c_5, 1, 0x18);
            env->unk28 = 0;
            if (frame >= 0x14) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk8.vx = D_801E971C.vx;
                work->unk8.vy = D_801E971C.vy;
                work->unk8.vz = D_801E971C.vz;
                work->unk10.vy = work->unk8.vy;
                work->unk10.vx = work->unk8.vx;
                work->unk10.vz = work->unk8.vz;
                env->camera->points[3].vx = work->unk8.vx;
                env->camera->points[3].vy = work->unk8.vy;
                env->camera->points[3].vz = work->unk8.vz;
            }
            break;
        case 5:
            env->unk28 = 0x10;
            t_c5 = (frame << 12) / 24;
            c_1 = ex->math_cos(t_c5 >> 1) >> 1;
            g5 = ex->math_cos(t_c5 >> 2) >> 1;
            k5z = (ex->math_cos(c_1) * g5) >> 12;
            n5z = (ex->math_sin(c_1) * g5) >> 12;
            a.vx = work->unk8.vx + k5z;
            a.vy = work->unk8.vy;
            a.vz = work->unk8.vz + n5z;
            b.vx = a.vx;
            b.vy = work->unk8.vy - 0x1000;
            b.vz = a.vz;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &a, NULL, 0xAAA, 0, 1, t_c5);
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &a, &b, frame, t_c5, 1, 0x20);
            env->unk28 = 0;
            if (frame >= 0x18) {
                work->unk0 = 6;
                entry->unk0 = -1;
            }
            break;
        case 6:
            a2.vx = work->unk8.vx;
            a2.vy = work->unk8.vy;
            a2.vz = work->unk8.vz;
            b2.vx = a2.vx;
            b2.vy = work->unk8.vy - 0x1000;
            b2.vz = a2.vz;
            env->unk28 = 0x10;
            r_c2 = (frame << 12) / 51;
            c_6 = 0x1000 - r_c2;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &a2, NULL, 0xAAA, 0, 1, c_6);
            func_801E7720((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &a2, &b2, frame, c_6, 1,
                          (r_c2 >> 7) + 0x20);
            env->unk28 = 0;
            rot2.vx = 0x400;
            rot2.vz = 0;
            rot2.vy = 0;
            rot2.pad = 1;
            if (frame < 0x10) {
                q4_c6 = (frame << 12) >> 6;
                s6a = ex->math_sin(q4_c6) * 5;
                c6a = ex->math_cos(q4_c6);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &a2, &rot2, s6a, 0, 1, c6a);
            }
            if (frame >= 0x1E) {
                k = (frame - 0x1E) << 9;
                if (k > 0x1000) {
                    k = 0x1000;
                }
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &a2, &rot2, 0x5000, 0, 1, k);
            }
            flag = 0;
            if (frame < 0x26) {
                if (frame < 0x1E) {
                    k5x = 0x20;
                    x6n = frame;
                    k5 = 0x1000;
                    g = 0;
                    flag = 1;
                    f3 = 0;
                    q_c6 = 0;
                    k = 0;
                } else {
                    n5y = (frame - 0x1E) << 9;
                    x6n = 0x1D;
                    k5x = 0x27;
                    k5 = 0x1000;
                    g = 0;
                    f3 = (frame - 0x1E) * 3;
                    q_c6 = 0x1000;
                    if (n5y >= 0x800) {
                        k = (n5y - 0x800) * 2;
                    } else {
                        k = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, ((0x1000 - n5y) * 0x64) >> 12, 0, 0);
                }
            } else {
                g = 1;
                x6n = 0x1D;
                n5y = ((frame - 0x26) << 12) / 13;
                k5x = (n5y >> 3) + 0x40;
                k5 = 0x1000 - n5y * 2 / 3;
                f3 = 0x15;
                k = k5;
                q_c6 = k;
            }
            func_801E77DC(entry, &D_801E971C, x6n, k5x, k5, g, flag);
            if (q_c6 != 0) {
                func_801E7B8C(entry, &D_801E971C, f3, q_c6);
                func_801E7EB4(entry, &D_801E971C, k);
            }
            if (frame >= 0x33) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}
