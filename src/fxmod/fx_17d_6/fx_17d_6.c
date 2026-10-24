#include "fxmod/fx_17d_6.h"

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR ang;
    CVECTOR col;
    SVECTOR rot;
    VECTOR vv;
    VECTOR vv2;
    SVECTOR cam;
    Pool6CEntry *p;
    s32 n1;
    s32 frame;
    s32 c;
    s32 i;
    s32 u;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E93B4 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801E93C4, 3);
        work->unk7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8B), 1);
        work->unk80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8D), 2);
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1A8, SUMMON(env)->unk60);
        work->unk70.vx = 0x100;
        work->unk70.vy = 0x780;
        work->unk58.vz = work->unk58.vy = -0x2000;
        work->unk64.vx = work->unk64.vy = work->unk64.vz = 0x800;
        work->unk70.vz = 0;
        work->unk58.vx = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0x400;
        work->unk44.vz = 0;
        work->unk38.vx = D_801E9360.vx;
        work->unk38.vy = D_801E9360.vy;
        work->unk38.vz = D_801E9360.vz;
        work->unk4C.vx = work->unk4C.vz = work->unk4C.vy = 0x1800;
        ((s32 *)&work->unk8)[0] = ((s32 *)&D_801E9360)[0];
        work->unk38.vy -= 0x800;
        ((s32 *)&work->unk8)[1] = ((s32 *)&D_801E9360)[1];
        work->unk8.vy -= 0x400;
        BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
        work->unk18.vx = work->unk58.vx;
        work->unk18.vy = work->unk58.vy;
        work->unk18.vz = work->unk58.vz - 0x200;
        work->unk18.vy -= 0x200;
        env->camera->points[2].vx = work->unk18.vx;
        env->camera->points[2].vy = work->unk18.vy;
        env->camera->points[2].vz = work->unk18.vz;
        work->unk20.vx = D_801E9360.vx;
        work->unk20.vy = D_801E9360.vy;
        work->unk20.vz = D_801E9360.vz;
        env->camera->points[3].vx = work->unk20.vx;
        env->camera->points[3].vy = work->unk20.vy;
        env->camera->points[3].vz = work->unk20.vz;
        work->unk58.vx += 0x19F;
        work->unk58.vy -= 0x200;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
        work->unk78 = p;
        if (p != NULL) {
            p->unk22 = 0x200;
        }
        D_801EB114 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            u = (frame << 12) / 66;
            if (frame < 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF - ((frame * 0xFF) >> 4),
                                         (0xFF - ((frame * 0xFF) >> 4)) * 2 / 3, (0xFF - ((frame * 0xFF) >> 4)) / 2);
            }
            if (frame < 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF - ((frame * 0xFF) >> 2),
                                         0xFF - ((frame * 0xFF) >> 2), 0xFF - ((frame * 0xFF) >> 2));
            }
            if (frame < 0x1D) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            }
            pos.vx = work->unk58.vx;
            pos.vy = work->unk58.vy;
            pos.vz = work->unk58.vz;
            pos.vy += (ex->math_sin(u >> 2) * 0x6A4) >> 12;
            ex->Hi_DrawSummonModel(&work->unk70, &pos, (s32)&work->unk64, SUMMON(env)->unk60, 0);
            ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, ex->btlfx_rand() & 0x1F, -0x20, -0x40);
            work->unk44.vy += 6;
            for (n1 = 0; n1 < 4; n1++) {
                sc.vx = sc.vy = sc.vz = (work->unk4C.vx * (n1 + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->unk44);
                ang.vy = ang.vy * (n1 + 2) / 3;
                if (n1 != 2) {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk38, (s32)&sc, work->unk7C);
                } else {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk38, (s32)&sc, work->unk80);
                }
            }
            ex->btlfx_drawScrollMove(0x3E8 - frame * 6, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[0], (u16 *)&work->unk28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[2], (u16 *)&work->unk30);
            BTLFX_COPY_SVEC(&D_801EB104, &work->unk28);
            BTLFX_COPY_SVEC(&D_801EB10C, &work->unk30);
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk30, NULL, ex->math_sin(u >> 2) >> 3,
                              0, 1, ex->math_sin(u >> 2));
            env->unk28 = 0;
            c = (((0x1000 - u) << 5) >> 12) + 0x89;
            col.r = col.g = col.b = c;
            vv.vy = vv.vx = 0x1080;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            vv.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 0, NULL);
            if (frame >= 0x42) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3D54((Pool6CEntry *)&work->unk78);
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk1AC, SUMMON(env)->unk60);
                pos.vx = work->unk58.vx;
                pos.vy = work->unk58.vy;
                pos.vz = work->unk58.vz;
                work->unk70.vx = 0;
                work->unk70.vy = 0x800;
                work->unk70.vz = 0;
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[0], (u16 *)&work->unk18);
                env->camera->points[2].vx = work->unk18.vx;
                env->camera->points[2].vy = work->unk18.vy;
                env->camera->points[2].vz = work->unk18.vz;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        case 1:
            u = (frame << 12) / 100;
            ex->Hi_DrawSummonModel(&work->unk70, (VECTOR *)&work->unk58, (s32)&work->unk64, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[0], (u16 *)&work->unk28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[2], (u16 *)&work->unk30);
            BTLFX_COPY_SVEC(&D_801EB104, &work->unk28);
            BTLFX_COPY_SVEC(&D_801EB10C, &work->unk30);
            ex->btlfx_lerpVec((s16 *)&work->unk28, (s16 *)&work->unk30, frame < 0x20 ? ex->math_sin(frame << 5) : 0x1000,
                              (s16 *)&work->unk8);
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk30, NULL,
                              (ex->math_sin(u >> 2) >> 1) + 0x400, 0, 1, 0xAAA);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk30, NULL,
                              (ex->math_sin(u >> 2) >> 2) + 0x80, 0, 1, 0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk30, NULL,
                              ex->math_cos(u >> 2) / 3 + 0x5DC, 0, 1, u);
            env->unk28 = 0;
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
            env->camera->points[2].vx = work->unk18.vx;
            env->camera->points[2].vy = work->unk18.vy;
            env->camera->points[2].vz = work->unk18.vz;
            if (frame == 0x18) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame == 0x20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9624;
                    p->unk12 = 0x28;
                    p->unkC = 0xAC;
                    p->unk22 = 0x80;
                }
            }
            if (frame == 0x32) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x54) {
                col.r = col.g = col.b = (((frame - 0x54) << 15) >> 12) + 0x20;
                vv2.vy = vv2.vx = (u >> 1) + 0x1000;
                vv2.vz = 0x1000;
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv2, &col, 1, NULL);
            }
            if (frame >= 0x64) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                D_801EB114 = 2;
                BTLFX_COPY_SVEC(&work->unk18, &D_801E9360);
                work->unk18.vy -= 0x200;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                BTLFX_COPY_SVEC(&work->unk10, &work->unk18);
                work->unk10.vy -= 0x400;
            }
            break;
        case 2:
            u = (frame << 12) / 18;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0, (0x40 - (frame << 6) / 18) >> 2,
                                     0x40 - (frame << 6) / 18);
            work->unk44.vy += 6;
            for (i = 0; i < 4; i++) {
                sc.vx = sc.vy = sc.vz = (work->unk4C.vx * (i + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->unk44);
                ang.vy = ang.vy * (i + 2) / 3;
                if (i != 2) {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk38, (s32)&sc, work->unk7C);
                    ex->Hi_ModifyEffModelRGB(work->unk7C, ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80);
                } else {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->unk38, (s32)&sc, work->unk80);
                }
            }
            env->unk28 = 0x9B;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk10, NULL, ex->math_sin(u >> 2) * 4,
                              0, 1, ex->math_sin(u >> 2));
            env->unk28 = 0;
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), &work->unk10, (u >> 6) + 8, u / 2);
            ex->btlfx_drawScrollMove(0x3E8 - frame * 6, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            col.r = col.g = col.b = ((u * 0x58) >> 12) + 0x84U;
            vv.vy = vv.vx = 0x1080;
            vv.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 0, NULL);
            if (frame >= 0x12) {
                work->unk0 = 3;
                entry->unk0 = -1;
                D_801EB114 = 0;
            }
            break;
        case 3:
            u = (frame << 12) / 14;
            ex->Hi_DrawSummonModel(&work->unk70, (VECTOR *)&work->unk58, (s32)&work->unk64, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk18);
            env->camera->points[2].vx = work->unk18.vx;
            env->camera->points[2].vy = work->unk18.vy;
            env->camera->points[2].vz = work->unk18.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[0], (u16 *)&work->unk28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[2], (u16 *)&work->unk30);
            BTLFX_COPY_SVEC(&D_801EB104, &work->unk28);
            BTLFX_COPY_SVEC(&D_801EB10C, &work->unk30);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801E9368[1], (u16 *)&work->unk28);
            BTLFX_COPY_SVEC(&D_801EB10C, &work->unk28);
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk28, NULL,
                              ex->math_sin(u >> 2) + 0x555, 0, 1, ex->math_sin(u >> 2) / 2 + 0x800);
            env->unk28 = 0;
            col.r = col.g = col.b = ((u << 7) >> 12) + 0x20;
            vv.vy = vv.vx = (u >> 3) + 0x1000;
            vv.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 1, NULL);
            if (frame >= 0xE) {
                work->unk0 = 4;
                cam.vx = D_801E9360.vx;
                cam.vy = D_801E9360.vy;
                cam.vz = D_801E9360.vz;
                cam.vy -= 0x300;
                env->camera->points[3].vx = cam.vx;
                env->camera->points[3].vy = cam.vy;
                env->camera->points[3].vz = cam.vz;
                D_801EB114 = 1;
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

void func_801E8B10(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801E93B4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    a = cnt;
    trail->unk0 = trail->unk2 = a - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < a; i++, pt++) {
        t = 0x1000 - (i << 12) / (a - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++ / 3;
        h = (ex->math_sin((t >> 1) >> 1) * h) >> 12;
        t >>= 1;
        h += ex->btlfx_lerp(h0, h1, ex->math_sin(t));
        ex->func_800D044C(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->unk28 = 0;
}

void func_801E8DBC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *path, Fx17d6Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E93B4);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E8B10(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801E8ED4(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 a;
    s32 c;
    s32 k;
    s32 i;
    Fx17d6Trail *tr;
    Fx17d6Trail *tr2;
    s32 u;

    if (mode != 1 && b->unk11 == 0) {
        t = (frame << 12) / 6;
        c = ex->math_cos(t >> 2);
        k = frame < 3;
        a = 0x1000 - t;
        i = b->unk10;
        u = b->unk15;
        tr = (Fx17d6Trail *)&D_801EB124[i];
        tr2 = (Fx17d6Trail *)&D_801EB474[i];
        b->unk14 += 3;
        func_801E8B10(entry, 8, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0xDC, b->unk14 * 16, 0xF0,
                      frame, c, a * u / 1500, 0);
        func_801E8DBC(entry, 7, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_6_op83(s32 mode, Fx17d6Work83 *work, Unk800F78A4 *env, Fx17d6Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 frame;
    Fx17d6Bolt *b;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E8ED4;
        init->unk4 = 6;
    } else if (mode == 1) {
        n = init->unk0;
        D_801E93B4 = env;
        work->unk0 = D_801EB584[0];
        p = D_801EB584[0];
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0xD2);
            *p++ = ex->btlfx_randCentered(0x1C3);
            *p++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        frame = inst->unk0;
        if (work->unk8 != 0) {
            return 0;
        }
        if (D_801EB114 == 0) {
            if (frame < 0x43 && (frame & 3)) {
                return 0;
            }
            work->unk4++;
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EB104)[0];
                ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EB104)[1];
                ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EB10C)[0];
                ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EB10C)[1];
                b->unk11 = 0;
                b->unk10 = work->unk4 % 6;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0xB;
                b->unk14 = ex->btlfx_rand();
            }
        }
        if (D_801EB114 == 1) {
            ex->btlfx_poolClear((Unk800DA1B8 *)inst->unk10);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_headerPtr);
