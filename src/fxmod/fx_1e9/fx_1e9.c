#include "fxmod/fx_1e9.h"

s32 fx_1e9_op80(s32 mode, Fx1e9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 b;
    s32 frame;
    s32 t0;
    s32 t2;
    s32 a0;
    s32 a1;
    s32 a2;
    s32 a;
    s32 c;
    s32 n;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E8564 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk20 = 0;
        work->unk4 = 0;
        D_801E8518 = 0;
        ex->btlfx_initEffModels(D_801E8574, 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xB7, 0x3D80, 0, 0);
    } else {
    b = 0;
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        t0 = frame << 8;
        a0 = (ex->math_sin(t0 >> 2) * 0x10FF) >> 12;
        env->unk28 = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a0, 0, 1, t0);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, t0);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a0, 0, 1, t0);
        scale.vx = scale.vz = 0x174;
        scale.vy = ex->math_sin(t0 >> 2) / 6;
        b = t0;
        if (frame >= 0x10) {
            work->unk0 = 1;
            entry->unk0 = -1;
            env->unk0->pad10[0] = 0;
            ex->btlfx_fadeUnitById(0x10, 0);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            work->unk1C = 0;
        }
        break;
    case 1:
        n = ++work->unk1C;
        t0 = ((n % 10) << 12) / 10;
        a1 = (ex->math_sin(t0) >> 4) + 0x10FF;
        env->unk28 = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a1, 0, 1, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, 0x1000);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a1, 0, 1, 0x1000);
        scale.vx = scale.vz = 0x174;
        scale.vy = (ex->math_sin(t0) >> 7) + 0x2AA;
        b = 0x1000;
        if (work->unk8 != NULL && work->unk8->unk30 == -1) {
            work->unk8 = NULL;
        }
        if (frame != 0 && t0 == 0 && env->unk0->pad10[0] != 0) {
            work->unk0 = 2;
            entry->unk0 = -1;
            env->camera->unk52 = 0;
            ex->btlfx_fadeUnitById(0x10, 0x80);
            ex->btlfx_stopEmitter(work->unk8);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unkC);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
        }
        break;
    case 2:
        t0 = (frame << 12) / 47;
        c = 0x1000 - t0;
        env->unk28 = 0x7C;
        a2 = t0 + 0x10FF;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk14, NULL, a2, 0, 1, c);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, 0x1000, 0, 1, c);
        env->unk28 = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, a2, 0, 1, c);
        if (frame < 0x10) {
            t2 = (frame << 8) >> 2;
            b = ex->math_cos(t2);
            scale.vx = scale.vz = (ex->math_sin(t2) >> 3) + 0x174;
            scale.vy = ex->math_cos(t2) / 6;
        }
        if (frame == 3) {
            ex->func_800E8554(0, 0x10);
            work->unk20 = 0;
        }
        if (frame >= 0x2F) {
            D_801E8518 = 1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    if (b != 0) {
        pos.vx = work->unkC.vx;
        pos.vy = work->unkC.vy;
        pos.vz = work->unkC.vz;
        rot.vx = 0;
        rot.vy = work->unk4 * 32;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        a = (b >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk24, a, a, a);
        scale.vy >>= 1;
        scale.vx = scale.vz = scale.vx * 3 / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
        ex->Hi_ModifyEffModelRGB(work->unk24, a, a, a);
    }
    ex->btlfx_drawScrollMove(work->unk4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
    work->unk4++;
    }
    return 0;
}

s32 func_801E7FCC(s32 mode, Fx1e9Spark *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 a;

    if (mode == 1) {
        e->unk12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        a = 0x1000 - (frame << 9);
        pt = e->unk18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x14;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)e, 0, (SVECTOR *)zero, pt);
        if (e->unk16 == 0) {
            TRAIL(e)->unk0 = TRAIL(e)->unk2 = 8;
            TRAIL(e)->unk1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, NULL, 5, 0x1000, a, 1, (u8 *)e->unk18 - 4, 0);
        }
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

void func_801E810C(void) {
    D_801E88A4 = 0;
}

void func_801E8118(Fx1e9Inst *inst, SVECTOR *origin, s32 h, SVECTOR *from, SVECTOR *to) {
    Fx1e9Trail *tr = &D_801E8644[D_801E88A4];
    SVECTOR *pt = &tr->unk4[8];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8564);
    SVECTOR v;
    Fx1e9Spark *e;
    s32 i;
    s32 t;

    tr->unk0 = tr->unk2 = 8;
    tr->unk1 = 0;
    for (i = 0; i < 9; i++, pt--) {
        t = 0x1000 - (i << 9);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += ex->btlfx_randCentered(0x20);
        v.vy += ex->btlfx_randCentered(0x20);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&v, h + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 5), (s16 *)pt);
        e = ex->btlfx_poolSpawn(inst->unk10);
        if (e != NULL) {
            e->unk14 = D_801E88A4;
            e->unk16 = 8 - i;
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
    D_801E88A4++;
    D_801E88A4 %= 8;
}

s32 fx_1e9_op82(s32 mode, Fx1e9Work82 *work, Unk800F78A4 *env, Fx1e9Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;

    if (mode == 0) {
        init->unk0 = 0x10;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E7FCC;
        init->unk4 = 0x48;
    } else if (mode == 1) {
        D_801E8564 = env;
        func_801E810C();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801E8518 == 0) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->unk8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            func_801E8118(inst, &work->unk0, 0x80, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801E8518 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1e9/nonmatchings/fx_1e9", fx_1e9_headerPtr);
