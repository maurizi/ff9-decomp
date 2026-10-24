#include "fxmod/fx_059.h"

s32 func_801E7704(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx059Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode != 1) {
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, D_801E7D50[work->unkC]), pos, 0, 0x1000, frame, 1,
                          0x1000 - (frame << 12) / 12, 0, 0);
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

s32 fx_059_op80(s32 mode, Fx059Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx059Rot rot;
    SVECTOR pos;
    s32 frame;
    s32 r;
    SVECTOR *p;
    void (*fn)(SVECTOR *);

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        s32 n = *arg4;
        work->unk0 = 0;
        work->unk4 = -0x20;
        work->unkC = n - 1;
        work->unk8 = (n << 10) - 0x400;
        D_801E7DB0 = env;
        work->unk10 = (ex->btlfx_rand() & 0x1F) + 0x100;
        work->unk18 = 0;
        work->unk14 = (Unk800DA1B8 *)ex->btlfx_allocTaskMem(entry, ex->btlfx_poolSize(8, 12));
        ex->btlfx_poolInit(work->unk14, entry, 8, 12, (s32)func_801E7704);
        return 0;
    } else {
        frame = entry->frame;
        rot.unk4 = work->unk18;
        work->unk18 += 0x10;
        fn = ex->btlfx_pushViewRotation;
        rot.unk0 = 0;
        fn((SVECTOR *)&rot);
        switch (work->unk0) {
        case 0:
            r = 0xA0 - frame * 0xA0 / 6;
            pos.vx = (ex->math_cos(work->unk8) * r) >> 12;
            pos.vy = (ex->math_sin(work->unk8) * r) >> 12;
            pos.vz = work->unk4;
            work->unk4 += 0xC8;
            work->unk8 += work->unk10;
            p = ex->btlfx_poolSpawn((s16 *)work->unk14);
            if (p != NULL) {
                ((s32 *)p)[0] = ((s32 *)&pos)[0];
                ((s32 *)p)[1] = ((s32 *)&pos)[1];
            }
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, D_801E7D38[work->unkC]), &pos, 0, 0x1000, 0,
                              1, -1, 0, 0);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame >= 7) {
                ex->btlfx_popViewMatrix();
                return 1;
            }
            break;
        }
        if (work->unk14 != NULL) {
            ex->btlfx_poolUpdate(work->unk14);
        }
        ex->btlfx_popViewMatrix();
    }
    return 0;
}

s32 fx_059_op81(s32 mode, Fx059Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7DB0 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        s32 frame = entry->frame;
        if (work->unk0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

s32 fx_059_op82(s32 mode, Fx059Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7DB0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        s32 frame = entry->frame;
        if (work->unk0 == 0 && frame >= 0x18) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_059/nonmatchings/fx_059", fx_059_headerPtr);
