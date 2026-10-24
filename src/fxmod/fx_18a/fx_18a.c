#include "fxmod/fx_18a.h"

s32 fx_18a_op80(s32 mode, Fx18aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    VECTOR pos;
    CVECTOR col;
    SVECTOR ang;
    VECTOR scale;
    SVECTOR rot2;
    s32 t;
    s32 c;
    s32 state;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 a;
    s32 f;
    s32 g1;
    s32 g2;
    s32 g3;
    s32 g4;
    s32 g;
    s32 x1;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E8A24 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk20 = 0;
        work->unk4 = 0;
        D_801E89D8 = 0;
        ex->btlfx_initEffModels(D_801E8A34, 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xB7, 0x3D80, 0, 0);
    } else {
        c = 0;
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            t = frame << 8;
            n2 = t >> 2;
            n1 = (ex->math_sin(n2) * 0x1400) >> 12;
            env->unk28 = 0x100;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, n1, 0, 1, t);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, n1, 0, 1, t);
            sc.vx = sc.vz = 0x200;
            sc.vy = ex->math_sin(n2) / 5;
            c = t;
            if (frame >= 0x10) {
                work->unk0 = 1;
                entry->unk0 = -1;
                work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk14);
                if (work->unk8 != NULL) {
                    work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                work->unk1C = 0;
            }
            break;
        case 1:
            t = ((++work->unk1C % 10) << 12) / 10;
            a = (ex->math_sin(t) >> 4) + 0x1400;
            env->unk28 = 0x100;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, a, 0, 1, 0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, 0x1000);
            env->unk28 = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, a, 0, 1, 0x1000);
            sc.vx = sc.vz = 0x200;
            sc.vy = (ex->math_sin(t) >> 7) + 0x333;
            c = 0x1000;
            if (work->unk8 != NULL && work->unk8->unk30 == -1) {
                work->unk8 = NULL;
            }
            if (frame >= 0xB && t == 0) {
                work->unk0 = 2;
                entry->unk0 = -1;
                env->camera->unk52 = 0;
                ex->btlfx_fadeUnitById(0x10, -1);
                ex->btlfx_fadeUnitById(0, -2);
                work->unkC.vz += 0x64;
                ex->btlfx_setUnitPos(0, (s16 *)&work->unkC);
                ex->func_800E3D54(work->unk8);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0x10, 0x20);
                }
            }
            break;
        case 2:
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            t = (frame << 12) / 180;
            col.r = col.g = col.b = (((0x1000 - t) << 6) >> 12) - 0x74;
            if (frame < 0x8C) {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(t >> 2) >> 3) + 0x1000;
            } else {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(((frame - 0x8C) << 10) / 40) >> 2) + 0x1000;
            }
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &ang, &scale, &col, 0, 0);
            if (frame >= 0x8C) {
                rot2.vx = 0x400;
                rot2.pad = 1;
                rot2.vz = 0;
                rot2.vy = 0;
                env->unk28 = 0x20;
                g1 = ((frame - 0x8C) << 12) / 40;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g1 >> 2) * 5, 0, 1, ex->math_cos(g1 >> 2));
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g1 >> 2) * 2, 0, 1, ex->math_cos(g1 >> 2));
                rot2.vx += 0x200;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g1 >> 2) * 3, 0, 1, ex->math_cos(g1 >> 2));
                rot2.vx -= 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk14, &rot2,
                                  ex->math_sin(g1 >> 2) * 3, 0, 1, ex->math_cos(g1 >> 2));
                env->unk28 = 0;
            }
            f = 0x1000 - t;
            env->unk28 = 0x100;
            x1 = t + 0x1400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, x1, 0, 1, f);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL, 0x1000, 0, 1, f);
            if (frame < 6) {
                g2 = (frame << 10) / 6;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk14, NULL,
                                  ex->math_sin(g2) * 2, 0, 1, ex->math_cos(g2));
            }
            env->unk28 = 0;
            if (frame == 0x1E) {
                ex->func_800E83EC();
            }
            if (frame < 4) {
                g3 = ex->math_cos(frame << 8) * 0xFF;
                f = g3 >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, f, g3 >> 14, g3 >> 13);
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, &rot, x1, 0, 1, f);
            if (frame < 0x16) {
                g4 = (frame << 12) / 22 >> 2;
                c = ex->math_cos(g4);
                sc.vx = sc.vz = (ex->math_sin(g4) >> 2) + 0x174;
                sc.vy = ex->math_cos(g4) / 4;
            }
            if (frame >= 0xB4) {
                D_801E89D8 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (c != 0) {
            pos.vx = work->unkC.vx;
            pos.vy = work->unkC.vy;
            pos.vz = work->unkC.vz;
            rot.vx = 0;
            rot.vy = work->unk4 * 32;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            sc.vy >>= 1;
            sc.vx = sc.vz = sc.vx * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk24);
            ex->Hi_ModifyEffModelRGB(work->unk24, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        }
        ex->func_800E6624(work->unk4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
        work->unk4++;
    }
    return 0;
}

s32 func_801E844C(s32 mode, Fx18aSpark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 12) / 12;
        pt = e->unk18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x1A;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->unk16 == 0) {
            TRAIL(e)->unk0 = TRAIL(e)->unk2 = 0xE;
            TRAIL(e)->unk1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->unk18 - 4, 0);
        }
        if (frame >= 0xC) {
            return 1;
        }
    }
    return 0;
}

void func_801E85A4(void) {
    D_801E90D4 = 0;
}

void func_801E85B0(Fx18aInst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx18aTrail *tr = &D_801E8B04[D_801E90D4];
    SVECTOR *pt = &tr->unk4[14];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8A24);
    SVECTOR v;
    Fx18aSpark *e;
    s32 i;
    s32 t;

    tr->unk0 = tr->unk2 = 0xE;
    tr->unk1 = 0;
    for (i = 0; i < 15; i++, pt--) {
        t = 0x1000 - (i << 12) / 14;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 3), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->unk10);
        if (e != NULL) {
            e->unk14 = D_801E90D4;
            e->unk16 = 0xE - i;
            e->unk18 = pt;
            e->unk0 = pt->vx << 12;
            e->unk4 = pt->vy << 12;
            e->unk8 = pt->vz << 12;
            e->unkC = 0;
            e->unkE = 0;
            e->unk10 = 0;
            e->unkC = ((pt->vx - origin->vx) >> 4) + ex->btlfx_randCentered(0x20);
            e->unkE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0x20);
            e->unk10 = ((pt->vz - origin->vz) >> 4) + ex->btlfx_randCentered(0x20);
        }
    }
    D_801E90D4 = (D_801E90D4 + 1) % 12;
}

s32 fx_18a_op82(s32 mode, Fx18aWork82 *work, Unk800F78A4 *env, Fx18aInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    s32 frame;
    s32 c;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E844C;
        init->unk4 = 0xB4;
    } else if (mode == 1) {
        D_801E8A24 = env;
        func_801E85A4();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        frame = inst->unk0;
        c = frame < 0xE6;
        if (c) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->unk8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            func_801E85B0(inst, &work->unk0, frame + 0x40, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (!c) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18a/nonmatchings/fx_18a", fx_18a_headerPtr);
