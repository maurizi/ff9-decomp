#include "fxmod/fx_188.h"

s32 func_801E7704(s32 mode, Fx188Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 t;

    if (mode == 1) {
        e->unk0 = 0;
        e->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&e->unkC);
        if (e->unk1C != NULL) {
            e->unk1C->unk22 = 0;
        }
        e->unk4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        e->unk8 = (ex->btlfx_rand() & 0x1F) + 0x20;
    } else if (e->unk0 == 0) {
        e->unk4 += 0xC;
        if (e->unk4 > 0x140) {
            e->unk4 = 0x140;
        }
        e->unk8 += 0x1D;
        if (e->unk8 > 0x22C) {
            e->unk8 = 0x22D;
        }
        t = 0x1000;
        if (frame < 5) {
            t = frame << 10;
        }
        t = (t * D_801E86CC) >> 12;
        ex->btlfx_aimAngles((s16 *)&e->unkC, (s16 *)&e->unk20, (s16 *)&ang);
        ex->btlfx_turnToward((s16 *)&ang, (s16 *)&e->unk14, e->unk8);
        ex->btlfx_pointAhead((s16 *)&e->unkC, (s16 *)&e->unk14, e->unk4, (s16 *)&e->unkC);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0xE), &e->unkC, 0, t + 0x800, frame, 1, -1, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 3), &e->unkC, 0, t + 0x800, frame, 2, -1, 0, 0);
        if (e->unk1C != NULL && e->unk1C->unk30 == -1) {
            e->unk1C = NULL;
        }
        if (D_801E86CC == 0) {
            if (e->unk1C != NULL) {
                e->unk1C->unk38 = e->unk1C->unk24->unk4.unk2;
            }
            e->unk0 = 1;
            return 1;
        }
    }
    return 0;
}

s32 fx_188_op80(s32 mode, Fx188Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx188Elem *e;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 s;
    s32 t;
    s32 n;

    if (mode == 0) {
        arg4->workSize = 0x20;
        arg4->elemSize = 0x28;
        arg4->elemFn = (s32)func_801E7704;
        arg4->poolCount = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk1C = 0;
        D_801E7E88 = env;
        BTLFX_COPY_SVEC(&work->unk4, &env->targets->centerX);
        work->unk4.vy -= 0x200;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        D_801E86CC = 0x1000;
        work->unk14.vy += 0x800;
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame & 1) {
                if (work->unk1C < 0xC) {
                    work->unk1C++;
                    e = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (e != NULL) {
                        BTLFX_COPY_SVEC(&e->unkC, &work->unkC);
                        e->unk20.vx = work->unk4.vx;
                        e->unk20.vz = work->unk4.vz;
                        e->unk20.vy = work->unk4.vy;
                        n = work->unk1C & 3;
                        e->unk14.vy = (s16)(work->unk14.vy - 0x300) + (n << 9);
                        n = work->unk1C >> 2;
                        e->unk14.vx = work->unk14.vx + n * 0x180;
                        e->unk14.vz = 0;
                    }
                }
            }
            if (frame >= 0x3E) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame << 9;
            if (frame < 8) {
                s = 0x1000;
                D_801E86CC = ex->math_cos(frame << 7);
            } else {
                s = ex->math_cos((frame - 8) << 7);
                t = 0x1000;
                D_801E86CC = 0;
            }
            s = s * 3 + 0x400;
            v.vx = 0x400;
            v.vz = 0;
            v.vy = 0;
            v.pad = 1;
            env->otZ = 0x20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->unk4, &v, s, 0, 1, t);
            env->otZ = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xF), frame);
            if (frame >= 0x10) {
                work->unk0 = 2;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E7E98;
                    p->unk12 = 0x19;
                    p->unk22 = 0x20;
                    p->unkC = 0x54;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            break;
        case 2:
            if (frame >= 0x20) {
                work->unk0 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_188/nonmatchings/fx_188", fx_188_headerPtr);
